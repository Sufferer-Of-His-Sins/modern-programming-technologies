#pragma once

#include "domain/Product.hpp"
#include "repository/IProductRepository.hpp"
#include "repository/ICategoryRepository.hpp"
#include <vector>
#include <optional>
#include <stdexcept>

namespace service {

class ProductService {
public:
    ProductService(repository::IProductRepositoryPtr productRepo,
                   repository::ICategoryRepositoryPtr categoryRepo);

    std::vector<domain::Product> getAll() const;
    std::optional<domain::Product> getById(int id) const;
    std::vector<domain::Product> getByCategory(int category_id) const;
    std::vector<domain::Product> getLowStock(int threshold = 10) const;

    int create(const std::string& name, double price, int quantity,
               int category_id, std::optional<int> supplier_id = std::nullopt);

    void update(int id, const std::string& name, double price, int quantity,
                int category_id, std::optional<int> supplier_id = std::nullopt);

    void remove(int id);

private:
    repository::IProductRepositoryPtr productRepo_;
    repository::ICategoryRepositoryPtr categoryRepo_;

    void validate(const std::string& name, double price, int quantity, int category_id) const;
};

}