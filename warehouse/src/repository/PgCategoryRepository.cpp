#include "PgCategoryRepository.hpp"
#include "db/Connection.hpp"

namespace repository {

PgCategoryRepository::PgCategoryRepository(std::shared_ptr<pqxx::connection> conn): conn_(std::move(conn)) {}

std::vector<domain::Category> PgCategoryRepository::findAll() {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec("SELECT id, name, description FROM categories ORDER BY name");
        txn.commit();

        std::vector<domain::Category> items;
        items.reserve(result.size());
        for (const auto& row : result) {
            items.emplace_back(
                row["id"].as<int>(),
                row["name"].as<std::string>(),
                row["description"].is_null() ? "" : row["description"].as<std::string>()
            );
        }
        return items;
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка получения категорий: ") + e.what());
    }
}

std::optional<domain::Category> PgCategoryRepository::findById(int id) {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec_params(
            "SELECT id, name, description FROM categories WHERE id = $1", id);
        txn.commit();

        if (result.empty()) return std::nullopt;

        const auto& row = result[0];
        return domain::Category{
            row["id"].as<int>(),
            row["name"].as<std::string>(),
            row["description"].is_null() ? "" : row["description"].as<std::string>()
        };
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка поиска категории: ") + e.what());
    }
}

int PgCategoryRepository::create(const domain::Category& category) {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec_params(
            "INSERT INTO categories (name, description) VALUES ($1, $2) RETURNING id",
            category.name, category.description);
        txn.commit();
        return result[0]["id"].as<int>();
    } catch (const pqxx::unique_violation&) {
        throw db::DatabaseError("Категория с таким названием уже существует");
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка создания категории: ") + e.what());
    }
}

bool PgCategoryRepository::update(const domain::Category& category) {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec_params(
            "UPDATE categories SET name = $1, description = $2 WHERE id = $3",
            category.name, category.description, category.id);
        txn.commit();
        return result.affected_rows() > 0;
    } catch (const pqxx::unique_violation&) {
        throw db::DatabaseError("Категория с таким названием уже существует");
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка обновления категории: ") + e.what());
    }
}

bool PgCategoryRepository::remove(int id) {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec_params("DELETE FROM categories WHERE id = $1", id);
        txn.commit();
        return result.affected_rows() > 0;
    } catch (const pqxx::foreign_key_violation&) {
        throw db::DatabaseError("Нельзя удалить категорию: есть связанные товары");
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка удаления категории: ") + e.what());
    }
}

} 