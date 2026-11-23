#pragma once

#include "domain/Category.hpp"
#include <vector>
#include <optional>
#include <memory>

namespace repository {

class ICategoryRepository {
public:
    virtual ~ICategoryRepository() = default;

    virtual std::vector<domain::Category> findAll() = 0;
    virtual std::optional<domain::Category> findById(int id) = 0;
    virtual int create(const domain::Category& category) = 0;
    virtual bool update(const domain::Category& category) = 0;
    virtual bool remove(int id) = 0;
};

using ICategoryRepositoryPtr = std::shared_ptr<ICategoryRepository>;

}