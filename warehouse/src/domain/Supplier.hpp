#pragma once

#include <string>

namespace domain {

struct Supplier {
    int id = 0;
    std::string name;
    std::string contact;
    std::string phone;

    Supplier() = default;

    Supplier(int id, std::string name, std::string contact, std::string phone)
        : id(id), name(std::move(name)), contact(std::move(contact)), phone(std::move(phone)) {}
};

}