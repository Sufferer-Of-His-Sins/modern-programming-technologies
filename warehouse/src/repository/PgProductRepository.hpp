#pragma once

#include "IProductRepository.hpp"
#include <pqxx/pqxx>
#include <memory>

namespace repository {

class PgProductRepository : public IProductRepository {
public:
    explicit PgProductRepository(std::shared_ptr<pqxx::connection> conn);

    std::vector<domain::Product> findAll() override;
    std::optional<domain::Product> findById(int id) override;
    std::vector<domain::Product> findByCategory(int category_id) override;
    std::vector<domain::Product> findLowStock(int threshold) override;
    int create(const domain::Product& product) override;
    bool update(const domain::Product& product) override;
    bool remove(int id) override;

private:
    std::shared_ptr<pqxx::connection> conn_;

    domain::Product rowToProduct(const pqxx::row& row) const;
};

}