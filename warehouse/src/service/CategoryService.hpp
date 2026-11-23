#pragma once

#include "domain/Category.hpp"
#include "repository/ICategoryRepository.hpp"
#include <vector>
#include <optional>
#include <stdexcept>

namespace service {

class CategoryService {
public:
    explicit CategoryService(repository::ICategoryRepositoryPtr repo);

    std::vector<domain::Category> getAll() const;
    std::optional<domain::Category> getById(int id) const;
    int create(const std::string& name, const std::string& description);
    void update(int id, const std::string& name, const std::string& description);
    void remove(int id);

private:
    repository::ICategoryRepositoryPtr repo_;

    void validateName(const std::string& name) const;
};

}