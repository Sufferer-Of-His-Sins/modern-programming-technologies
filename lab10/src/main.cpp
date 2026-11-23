#include <algorithm>
#include <atomic>
#include <cctype>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace fs = std::filesystem;

struct Options {
    bool recursive = false;        // -r
    bool ignoreCase = false;       // -i
    bool invert = false;           // -v
    bool lineNumber = false;       // -n
    bool forceFilename = false;    // -H
    bool noFilename = false;       // -h
    bool countOnly = false;        // -c
    bool filesWithMatches = false; // -l
    bool quiet = false;            // -q
    bool wordMatch = false;        // -w
    bool lineMatch = false;        // -x
    int  numThreads = 0;           // -j
    std::string pattern;           
    std::vector<std::string> paths;
};
struct Stats {
    std::atomic<int> filesProcessed{0};
    std::atomic<int> matches{0};
    std::atomic<int> errors{0};
};
struct FileResult {
    int matches = 0;
    bool failed = false;
    std::vector<std::string> output;
};



static char toLower(char c) {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}
static bool isWordChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}
static size_t findPattern(const std::string& text, const std::string& pattern, size_t from, bool ignoreCase) {
    if (!ignoreCase) return text.find(pattern, from);

    auto it = std::search(text.begin() + from, text.end(),pattern.begin(), pattern.end(), [](char a, char b) { return toLower(a) == b; });
    return it == text.end() ? std::string::npos : static_cast<size_t>(it - text.begin());
}
static bool containsWholeWord(const std::string& line, const std::string& pattern,bool ignoreCase) {
    if (pattern.empty()) return true;

    for (size_t pos = 0; pos < line.size(); ) {
        size_t found = findPattern(line, pattern, pos, ignoreCase);
        if (found == std::string::npos) return false;

        size_t end = found + pattern.size();
        bool leftOk  = (found == 0) || !isWordChar(line[found - 1]);
        bool rightOk = (end >= line.size()) || !isWordChar(line[end]);
        if (leftOk && rightOk) return true;

        pos = found + 1;
    }
    return false;
}


static bool equalsPattern(const std::string& line, const Options& opt) {
    if (!opt.ignoreCase) return line == opt.pattern;
    return line.size() == opt.pattern.size() && std::equal(line.begin(), line.end(), opt.pattern.begin(),[](char a, char b) { return toLower(a) == b; });
}
static bool lineMatches(const std::string& line, const Options& opt) {
    bool match;
    if (opt.lineMatch) match = equalsPattern(line, opt);
    else if (opt.wordMatch) match = containsWholeWord(line, opt.pattern, opt.ignoreCase);
    else match = findPattern(line, opt.pattern, 0, opt.ignoreCase) != std::string::npos;

    return opt.invert ? !match : match;
}
static void collectFiles(const fs::path& p, bool recursive, std::vector<fs::path>& out, std::ostream& err) {
    std::error_code ec;

    if (!fs::exists(p, ec)) {
        err << "pgrep: " << p.string() << ": No such file or directory\n";
        return;
    }

    if (fs::is_directory(p, ec)) {
        if (!recursive) {
            err << "pgrep: " << p.string() << ": Is a directory\n";
            return;
        }

        fs::recursive_directory_iterator it(p, fs::directory_options::skip_permission_denied, ec);
        const fs::recursive_directory_iterator end;

        if (ec) {
            err << "pgrep: " << p.string() << ": " << ec.message() << "\n";
            return;
        }

        for (; it != end; it.increment(ec)) {
            if (ec) {
                err << "pgrep: " << p.string() << ": " << ec.message() << "\n";
                break;
            }
            if (it->is_symlink(ec)) continue;
            if (it->is_regular_file(ec)) out.push_back(it->path());
        }
    } else if (fs::is_regular_file(p, ec)) {
        out.push_back(p);
    } else {
        err << "pgrep: " << p.string() << ": Not a regular file\n";
    }
}


static FileResult processFile(const fs::path& path, const Options& opt, bool printFilename) {
    FileResult result;

    std::ifstream in(path, std::ios::binary);
    if (!in) {
        result.failed = true;
        return result;
    }

    const bool printLines = !opt.countOnly && !opt.filesWithMatches && !opt.quiet;
    const bool stopAtFirst = !opt.countOnly && (opt.filesWithMatches || opt.quiet);
    const std::string prefix = printFilename ? path.string() + ':' : std::string();

    std::string line;
    int lineNo = 0;

    while (std::getline(in, line)) {
        ++lineNo;
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (!lineMatches(line, opt)) continue;
        ++result.matches;

        if (printLines) {
            std::string text = prefix;
            if (opt.lineNumber) text += std::to_string(lineNo) + ':';
            text += line;
            result.output.push_back(std::move(text));
        }
        if (stopAtFirst) break;
    }

    if (!opt.quiet) {
        if (opt.countOnly) result.output.push_back(prefix + std::to_string(result.matches));
        if (opt.filesWithMatches && result.matches > 0) result.output.push_back(path.string());
    }

    return result;
}
static void worker(const std::vector<fs::path>& files, std::atomic<size_t>& nextIndex, std::mutex& outputMutex, const Options& opt, bool printFilename, Stats& stats) {
    while (true) {
        size_t i = nextIndex.fetch_add(1);
        if (i >= files.size()) return;

        const fs::path& path = files[i];
        FileResult result = processFile(path, opt, printFilename);

        {
            std::lock_guard<std::mutex> lock(outputMutex);
            if (result.failed) std::cerr << "pgrep: " << path.string() << ": Cannot open file\n";
            for (const auto& s : result.output) std::cout << s << '\n';
        }

        ++stats.filesProcessed;
        if (result.failed) ++stats.errors;
        else stats.matches += result.matches;
    }
}
static bool parseThreadCount(const std::string& str, int& out) {
    try {
        out = std::stoi(str);
    } catch (...) {
        out = 0;
    }
    if (out < 1) {
        std::cerr << "pgrep: invalid number of threads\n";
        return false;
    }
    return true;
}


static bool parseArgs(int argc, char* argv[], Options& opt) {
    bool endOfOptions = false;
    bool hasPattern = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (!endOfOptions && arg == "--") {
            endOfOptions = true;
            continue;
        }

        if (!endOfOptions && arg.size() > 1 && arg[0] == '-') {
            for (size_t j = 1; j < arg.size(); ++j) {
                switch (arg[j]) {
                    case 'r': opt.recursive = true; break;
                    case 'i': opt.ignoreCase = true; break;
                    case 'v': opt.invert = true; break;
                    case 'n': opt.lineNumber = true; break;
                    case 'H': opt.forceFilename = true; break;
                    case 'h': opt.noFilename = true; break;
                    case 'c': opt.countOnly = true; break;
                    case 'l': opt.filesWithMatches = true; break;
                    case 'q': opt.quiet = true; break;
                    case 'w': opt.wordMatch = true; break;
                    case 'x': opt.lineMatch = true; break;
                    case 'j': {
                        std::string numStr;
                        if (j + 1 < arg.size()) {
                            numStr = arg.substr(j + 1);
                            j = arg.size();
                        } else if (i + 1 < argc) {
                            numStr = argv[++i];
                        } else {
                            std::cerr << "pgrep: option requires an argument -- 'j'\n";
                            return false;
                        }
                        if (!parseThreadCount(numStr, opt.numThreads)) return false;
                        break;
                    }
                    default:
                        std::cerr << "pgrep: invalid option '" << arg[j] << "'\n";
                        return false;
                }
            }
            continue;
        }

        if (!hasPattern) {
            opt.pattern = arg;
            hasPattern = true;
        } else {
            opt.paths.push_back(arg);
        }
    }

    if (!hasPattern) {
        std::cerr << "Usage: pgrep [OPTIONS] PATTERN [PATH]\n";
        return false;
    }

    if (opt.paths.empty()) opt.paths.push_back(".");

    if (opt.numThreads == 0) opt.numThreads = std::max(1, static_cast<int>(std::thread::hardware_concurrency()));

    if (opt.ignoreCase)
        std::transform(opt.pattern.begin(), opt.pattern.end(), opt.pattern.begin(), toLower);

    return true;
}

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);

    Options opt;
    if (!parseArgs(argc, argv, opt)) return 2;

    std::vector<fs::path> files;
    for (const auto& p : opt.paths)
        collectFiles(p, opt.recursive, files, std::cerr);

    bool printFilename;
    if (opt.forceFilename)   printFilename = true;
    else if (opt.noFilename) printFilename = false;
    else printFilename = opt.paths.size() > 1 || opt.recursive || files.size() > 1;

    Stats stats;
    std::atomic<size_t> nextIndex{0};
    std::mutex outputMutex;

    const auto start = std::chrono::steady_clock::now();
    const size_t threadCount =std::max<size_t>(1, std::min(static_cast<size_t>(opt.numThreads), files.size()));

    auto run = [&] { worker(files, nextIndex, outputMutex, opt, printFilename, stats); };

    std::vector<std::thread> pool;
    pool.reserve(threadCount - 1);
    for (size_t t = 1; t < threadCount; ++t) pool.emplace_back(run);
    run();
    for (auto& t : pool) t.join();

    const double seconds =std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();

    if (!opt.quiet) {
        std::cerr << "Processed: " << stats.filesProcessed
                  << " files, " << stats.matches
                  << " matches, " << stats.errors
                  << " errors, " << seconds << " s\n";
    }

    if (stats.errors > 0)  return 2;
    if (stats.matches > 0) return 0;
    return 1;
}