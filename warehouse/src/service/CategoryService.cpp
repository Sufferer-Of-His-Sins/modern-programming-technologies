#include "CategoryService.hpp"

namespace service {

CategoryService::CategoryService(repository::ICategoryRepositoryPtr repo): repo_(std::move(repo)) {}

std::vector<domain::Category> CategoryService::getAll() const {
    return repo_->findAll();
}

std::optional<domain::Category> CategoryService::getById(int id) const {
    return repo_->findById(id);
}

int CategoryService::create(const std::string& name, const std::string& description) {
    validateName(name);
    domain::Category cat{0, name, description};
    return repo_->create(cat);
}

void CategoryService::update(int id, const std::string& name, const std::string& description) {
    validateName(name);
    if (!repo_->findById(id)) {
        throw std::runtime_error("Категория не найдена");
    }
    domain::Category cat{id, name, description};
    if (!repo_->update(cat)) {
        throw std::runtime_error("Не удалось обновить категорию");
    }
}

void CategoryService::remove(int id) {
    if (!repo_->remove(id)) {
        throw std::runtime_error("Категория не найдена");
    }
}

void CategoryService::validateName(const std::string& name) const {
    if (name.empty() || name.size() > 100) {
        throw std::invalid_argument("Название категории должно быть от 1 до 100 символов");
    }
}

} 