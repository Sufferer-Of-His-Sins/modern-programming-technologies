#include "SupplierService.hpp"

namespace service {

SupplierService::SupplierService(repository::ISupplierRepositoryPtr repo)
    : repo_(std::move(repo)) {}

std::vector<domain::Supplier> SupplierService::getAll() const {
    return repo_->findAll();
}

std::optional<domain::Supplier> SupplierService::getById(int id) const {
    return repo_->findById(id);
}

int SupplierService::create(const std::string& name, const std::string& contact, const std::string& phone) {
    validateName(name);
    domain::Supplier s{0, name, contact, phone};
    return repo_->create(s);
}

void SupplierService::update(int id, const std::string& name,
                             const std::string& contact, const std::string& phone) {
    validateName(name);
    if (!repo_->findById(id)) {
        throw std::runtime_error("Поставщик не найден");
    }
    domain::Supplier s{id, name, contact, phone};
    if (!repo_->update(s)) {
        throw std::runtime_error("Не удалось обновить поставщика");
    }
}

void SupplierService::remove(int id) {
    if (!repo_->remove(id)) {
        throw std::runtime_error("Поставщик не найден");
    }
}

void SupplierService::validateName(const std::string& name) const {
    if (name.empty() || name.size() > 150) {
        throw std::invalid_argument("Название поставщика должно быть от 1 до 150 символов");
    }
}

} 