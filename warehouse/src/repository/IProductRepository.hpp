#pragma once

#include "domain/Product.hpp"
#include <vector>
#include <optional>
#include <memory>

namespace repository {

class IProductRepository {
public:
    virtual ~IProductRepository() = default;

    virtual std::vector<domain::Product> findAll() = 0;
    virtual std::optional<domain::Product> findById(int id) = 0;
    virtual std::vector<domain::Product> findByCategory(int category_id) = 0;
    virtual std::vector<domain::Product> findLowStock(int threshold) = 0;
    virtual int create(const domain::Product& product) = 0;
    virtual bool update(const domain::Product& product) = 0;
    virtual bool remove(int id) = 0;
};

using IProductRepositoryPtr = std::shared_ptr<IProductRepository>;

}