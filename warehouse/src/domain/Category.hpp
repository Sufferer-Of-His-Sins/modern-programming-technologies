#pragma once

#include <string>

namespace domain {

struct Category {
    int id = 0;
    std::string name;
    std::string description;

    Category() = default;

    Category(int id, std::string name, std::string description): id(id), name(std::move(name)), description(std::move(description)) {}
};

}