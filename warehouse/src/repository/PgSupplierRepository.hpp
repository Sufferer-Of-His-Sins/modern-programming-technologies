#pragma once

#include "ISupplierRepository.hpp"
#include <pqxx/pqxx>
#include <memory>

namespace repository {

class PgSupplierRepository : public ISupplierRepository {
public:
    explicit PgSupplierRepository(std::shared_ptr<pqxx::connection> conn);

    std::vector<domain::Supplier> findAll() override;
    std::optional<domain::Supplier> findById(int id) override;
    int create(const domain::Supplier& supplier) override;
    bool update(const domain::Supplier& supplier) override;
    bool remove(int id) override;

private:
    std::shared_ptr<pqxx::connection> conn_;
};

}