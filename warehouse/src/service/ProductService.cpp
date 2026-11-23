#include "ProductService.hpp"

namespace service {

ProductService::ProductService(repository::IProductRepositoryPtr productRepo,
                               repository::ICategoryRepositoryPtr categoryRepo)
    : productRepo_(std::move(productRepo))
    , categoryRepo_(std::move(categoryRepo)) {}

std::vector<domain::Product> ProductService::getAll() const {
    return productRepo_->findAll();
}

std::optional<domain::Product> ProductService::getById(int id) const {
    return productRepo_->findById(id);
}

std::vector<domain::Product> ProductService::getByCategory(int category_id) const {
    return productRepo_->findByCategory(category_id);
}

std::vector<domain::Product> ProductService::getLowStock(int threshold) const {
    if (threshold < 0) {
        throw std::invalid_argument("Порог остатка не может быть отрицательным");
    }
    return productRepo_->findLowStock(threshold);
}

int ProductService::create(const std::string& name, double price, int quantity,
                           int category_id, std::optional<int> supplier_id) {
    validate(name, price, quantity, category_id);
    domain::Product p{0, name, price, quantity, category_id, supplier_id};
    return productRepo_->create(p);
}

void ProductService::update(int id, const std::string& name, double price, int quantity,
                            int category_id, std::optional<int> supplier_id) {
    validate(name, price, quantity, category_id);
    if (!productRepo_->findById(id)) {
        throw std::runtime_error("Товар не найден");
    }
    domain::Product p{id, name, price, quantity, category_id, supplier_id};
    if (!productRepo_->update(p)) {
        throw std::runtime_error("Не удалось обновить товар");
    }
}

void ProductService::remove(int id) {
    if (!productRepo_->remove(id)) {
        throw std::runtime_error("Товар не найден");
    }
}

void ProductService::validate(const std::string& name, double price, int quantity, int category_id) const {
    if (name.empty() || name.size() > 200) {
        throw std::invalid_argument("Название товара должно быть от 1 до 200 символов");
    }
    if (price < 0) {
        throw std::invalid_argument("Цена не может быть отрицательной");
    }
    if (quantity < 0) {
        throw std::invalid_argument("Количество не может быть отрицательным");
    }
    if (!categoryRepo_->findById(category_id)) {
        throw std::invalid_argument("Указанная категория не существует");
    }
}

} // namespace service