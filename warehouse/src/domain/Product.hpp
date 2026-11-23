#pragma once

#include <string>
#include <optional>

namespace domain {

struct Product {
    int id = 0;
    std::string name;
    double price = 0.0;
    int quantity = 0;
    int category_id = 0;
    std::optional<int> supplier_id;

    std::string category_name;
    std::string supplier_name;

    Product() = default;

    Product(int id, std::string name, double price, int quantity,
            int category_id, std::optional<int> supplier_id = std::nullopt)
        : id(id)
        , name(std::move(name))
        , price(price)
        , quantity(quantity)
        , category_id(category_id)
        , supplier_id(supplier_id) {}
};

}