#pragma once

#include "domain/Supplier.hpp"
#include "repository/ISupplierRepository.hpp"
#include <vector>
#include <optional>
#include <stdexcept>

namespace service {

class SupplierService {
public:
    explicit SupplierService(repository::ISupplierRepositoryPtr repo);

    std::vector<domain::Supplier> getAll() const;
    std::optional<domain::Supplier> getById(int id) const;
    int create(const std::string& name, const std::string& contact, const std::string& phone);
    void update(int id, const std::string& name, const std::string& contact, const std::string& phone);
    void remove(int id);

private:
    repository::ISupplierRepositoryPtr repo_;

    void validateName(const std::string& name) const;
};

}