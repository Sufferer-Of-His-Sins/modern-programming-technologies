#include <chrono>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <cctype>

#include "Image.h"

namespace fs = std::filesystem;

// =====================================================================
// Пакетная обработка изображений.
//
// См. задание:
//   раздел 2 — что должна делать программа
//   раздел 3 — многопоточность: что и чем защищать
//
// Класс Image (Image.h / Image.cpp) предоставлен и менять его нельзя.
// Всё, что нужно реализовать, отмечено TODO.
// =====================================================================

struct Task {
    fs::path inputPath;
    fs::path outputPath;
};

enum class Operation {
    Grayscale = 1,
    Invert = 2,
    GrayscaleInvert = 3
};

std::string operationToString(Operation op) {
    switch (op) {
        case Operation::Grayscale: return "Grayscale";
        case Operation::Invert: return "Invert";
        case Operation::GrayscaleInvert: return "Grayscale + Invert";
        default: return "Unknown";
    }
}

bool isSupportedImage(const fs::path& p) {
    if (!p.has_extension()) return false;
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return ext == ".png" || ext == ".jpg" || ext == ".jpeg" ||
           ext == ".bmp" || ext == ".tga";
}

std::vector<fs::path> collectImages(const fs::path& dir) {
    std::vector<fs::path> images;
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && isSupportedImage(entry.path())) {
            images.push_back(entry.path());
        }
    }
    std::sort(images.begin(), images.end());
    return images;
}

fs::path makeOutputPath(const fs::path& inputPath, const fs::path& outputDir) {
    std::string filename = inputPath.filename().string();
    return outputDir / (filename + ".processed.png");
}

bool processOneImage(const Task& task, Operation op) {
    Image img;
    if (!img.load(task.inputPath.string())) {
        return false;
    }
    switch (op) {
        case Operation::Grayscale:
            img.grayscale();
            break;
        case Operation::Invert:
            img.invert();
            break;
        case Operation::GrayscaleInvert:
            img.grayscale();
            img.invert();
            break;
    }
    return img.save(task.outputPath.string());
}

void sequentialProcess(const std::vector<Task>& tasks, Operation op,
                       int& success, int& failed) {
    for (const auto& task : tasks) {
        bool ok = processOneImage(task, op);
        if (ok) {
            ++success;
            std::cout << "Completed: " << task.inputPath.filename().string() << std::endl;
        } else {
            ++failed;
            std::cout << "Failed: " << task.inputPath.filename().string() << std::endl;
        }
    }
}

void workerThread(int threadId,std::queue<Task>& taskQueue,std::mutex& queueMutex,
                  std::mutex& statisticsMutex,
                  std::mutex& outputMutex,
                  Operation op,
                  int& success,
                  int& failed) {
    while (true) {
        Task task;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (taskQueue.empty()) {
                return;
            }
            task = taskQueue.front();
            taskQueue.pop();
        }

        bool ok = processOneImage(task, op);
        {
            std::lock_guard<std::mutex> lock(outputMutex);
            if (ok) {
                std::cout << "[Thread " << threadId << "] Completed: "<< task.inputPath.filename().string() << std::endl;
            } else {
                std::cout << "[Thread " << threadId << "] Failed: "<< task.inputPath.filename().string() << std::endl;
            }
        }
        {
            std::lock_guard<std::mutex> lock(statisticsMutex);
            if (ok) {
                ++success;
            } else {
                ++failed;
            }
        }
    }
}

void parallelProcess(std::queue<Task>& taskQueue, Operation op, int numThreads,
                     int& success, int& failed) {
    std::mutex queueMutex;
    std::mutex statisticsMutex;
    std::mutex outputMutex;

    std::vector<std::thread> workers;
    workers.reserve(static_cast<std::size_t>(numThreads));

    for (int i = 1; i <= numThreads; ++i) {
        workers.emplace_back(workerThread, i,
                             std::ref(taskQueue),
                             std::ref(queueMutex),
                             std::ref(statisticsMutex),
                             std::ref(outputMutex),
                             op,
                             std::ref(success),
                             std::ref(failed));
    }

    for (auto& t : workers) {
        t.join();
    }
}

int main() {
    std::cout << "========================================\n";
    std::string inputDirStr;
    std::cout << "Enter input directory:  ";
    std::getline(std::cin, inputDirStr);
    if (inputDirStr.empty()) {
        std::cerr << "Error: input directory cannot be empty.\n";
        return 1;
    }
    fs::path inputDir(inputDirStr);

    if (!fs::exists(inputDir) || !fs::is_directory(inputDir)) {
        std::cerr << "Error: input directory does not exist or is not a directory.\n";
        return 1;
    }

    std::string outputDirStr;
    std::cout << "Enter output directory: ";
    std::getline(std::cin, outputDirStr);
    if (outputDirStr.empty()) {
        std::cerr << "Error: output directory cannot be empty.\n";
        return 1;
    }
    fs::path outputDir(outputDirStr);
    std::error_code ec;
    fs::create_directories(outputDir, ec);
    if (ec) {
        std::cerr << "Error: cannot create output directory: " << ec.message() << "\n";
        return 1;
    }

    std::cout << "\nScanning input directory...\n";
    std::vector<fs::path> images = collectImages(inputDir);
    std::cout << "Found " << images.size() << " supported image files.\n";

    if (images.empty()) {
        std::cout << "No images to process. Exiting.\n";
        return 0;
    }

    std::cout << "\nSelect processing operation:\n";
    std::cout << "  1. Grayscale\n";
    std::cout << "  2. Invert\n";
    std::cout << "  3. Grayscale + Invert\n";
    std::cout << "Enter operation number: ";
    std::string opStr;
    std::getline(std::cin, opStr);
    int opNum = 0;
    try {
        opNum = std::stoi(opStr);
    } catch (...) {
        std::cerr << "Error: invalid operation number.\n";
        return 1;
    }
    if (opNum < 1 || opNum > 3) {
        std::cerr << "Error: operation number must be 1, 2 or 3.\n";
        return 1;
    }
    Operation op = static_cast<Operation>(opNum);

    std::cout << "\nSelect processing mode:\n";
    std::cout << "  1. Sequential processing\n";
    std::cout << "  2. Parallel processing\n";
    std::cout << "Enter mode: ";
    std::string modeStr;
    std::getline(std::cin, modeStr);
    int mode = 0;
    try {
        mode = std::stoi(modeStr);
    } catch (...) {
        std::cerr << "Error: invalid mode.\n";
        return 1;
    }
    if (mode != 1 && mode != 2) {
        std::cerr << "Error: mode must be 1 or 2.\n";
        return 1;
    }

    int numThreads = 1;
    if (mode == 2) {
        std::cout << "\nEnter number of worker threads (> 0): ";
        std::string threadsStr;
        std::getline(std::cin, threadsStr);
        try {
            numThreads = std::stoi(threadsStr);
        } catch (...) {
            std::cerr << "Error: invalid number of threads.\n";
            return 1;
        }
        if (numThreads <= 0) {
            std::cerr << "Error: number of threads must be > 0.\n";
            return 1;
        }
    }

    std::vector<Task> tasks;
    tasks.reserve(images.size());
    for (const auto& imgPath : images) {
        tasks.push_back({imgPath, makeOutputPath(imgPath, outputDir)});
    }

    int success = 0;
    int failed = 0;

    auto start = std::chrono::steady_clock::now();

    if (mode == 1) {
        sequentialProcess(tasks, op, success, failed);
    } else {
        std::queue<Task> taskQueue;
        for (const auto& t : tasks) {
            taskQueue.push(t);
        }
        parallelProcess(taskQueue, op, numThreads, success, failed);
    }

    auto end = std::chrono::steady_clock::now();
    double seconds = std::chrono::duration<double>(end - start).count();

    std::cout << "========================================\n";
    std::cout << "Mode:                    " << (mode == 1 ? "Sequential" : "Parallel") << "\n";
    std::cout << "Operation:               " << operationToString(op) << "\n";
    if (mode == 2) {
        std::cout << "Worker threads:          " << numThreads << "\n";
    }
    std::cout << "Total images:            " << images.size() << "\n";
    std::cout << "Successfully processed:  " << success << "\n";
    std::cout << "Failed:                  " << failed << "\n";
    std::cout << "Processing time:         " << seconds << " seconds\n";
    std::cout << "Results directory:       " << outputDir.string() << "\n";
    std::cout << "========================================\n";

    return 0;
}