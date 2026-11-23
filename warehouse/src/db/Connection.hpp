#pragma once

#include <pqxx/pqxx>
#include <memory>
#include <string>
#include <stdexcept>

namespace db {

class Connection {
public:
    static std::unique_ptr<pqxx::connection> create();
    static std::unique_ptr<pqxx::connection> create(const std::string& conninfo);
};

class DatabaseError : public std::runtime_error {
public:
    explicit DatabaseError(const std::string& message)
        : std::runtime_error(message) {}
};

}