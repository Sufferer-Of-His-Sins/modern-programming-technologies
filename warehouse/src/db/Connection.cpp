#include "Connection.hpp"
#include <cstdlib>
#include <sstream>

namespace db {

namespace {

std::string get_env(const char* name, const char* default_value = "") {
    const char* val = std::getenv(name);
    return val ? val : default_value;
}

} 

std::unique_ptr<pqxx::connection> Connection::create() {
    std::ostringstream oss;
    oss << "host="     << get_env("PGHOST", "localhost")
        << " port="    << get_env("PGPORT", "5432")
        << " dbname="  << get_env("PGDATABASE", "warehouse")
        << " user="    << get_env("PGUSER", "postgres")
        << " password="<< get_env("PGPASSWORD", "");

    return create(oss.str());
}

std::unique_ptr<pqxx::connection> Connection::create(const std::string& conninfo) {
    try {
        auto conn = std::make_unique<pqxx::connection>(conninfo);
        if (!conn->is_open()) {
            throw DatabaseError("Не удалось открыть соединение с PostgreSQL");
        }
        return conn;
    } catch (const pqxx::broken_connection& e) {
        throw DatabaseError(std::string("Ошибка подключения к PostgreSQL: ") + e.what());
    } catch (const std::exception& e) {
        throw DatabaseError(std::string("Ошибка создания соединения: ") + e.what());
    }
}

}