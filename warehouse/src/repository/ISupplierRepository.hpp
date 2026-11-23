#pragma once

#include "domain/Supplier.hpp"
#include <vector>
#include <optional>
#include <memory>

namespace repository {

class ISupplierRepository {
public:
    virtual ~ISupplierRepository() = default;

    virtual std::vector<domain::Supplier> findAll() = 0;
    virtual std::optional<domain::Supplier> findById(int id) = 0;
    virtual int create(const domain::Supplier& supplier) = 0;
    virtual bool update(const domain::Supplier& supplier) = 0;
    virtual bool remove(int id) = 0;
};

using ISupplierRepositoryPtr = std::shared_ptr<ISupplierRepository>;

} 