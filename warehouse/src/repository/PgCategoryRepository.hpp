#pragma once

#include "ICategoryRepository.hpp"
#include "db/Connection.hpp"
#include <pqxx/pqxx>
#include <memory>

namespace repository {

class PgCategoryRepository : public ICategoryRepository {
public:
    explicit PgCategoryRepository(std::shared_ptr<pqxx::connection> conn);

    std::vector<domain::Category> findAll() override;
    std::optional<domain::Category> findById(int id) override;
    int create(const domain::Category& category) override;
    bool update(const domain::Category& category) override;
    bool remove(int id) override;

private:
    std::shared_ptr<pqxx::connection> conn_;
};

} 