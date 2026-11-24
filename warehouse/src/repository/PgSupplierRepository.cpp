#include "PgSupplierRepository.hpp"
#include "db/Connection.hpp"

namespace repository {

PgSupplierRepository::PgSupplierRepository(std::shared_ptr<pqxx::connection> conn): conn_(std::move(conn)) {}

std::vector<domain::Supplier> PgSupplierRepository::findAll() {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec("SELECT id, name, contact, phone FROM suppliers ORDER BY name");
        txn.commit();

        std::vector<domain::Supplier> items;
        items.reserve(result.size());
        for (const auto& row : result) {
            items.emplace_back(
                row["id"].as<int>(),
                row["name"].as<std::string>(),
                row["contact"].is_null() ? "" : row["contact"].as<std::string>(),
                row["phone"].is_null() ? "" : row["phone"].as<std::string>()
            );
        }
        return items;
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка получения поставщиков: ") + e.what());
    }
}

std::optional<domain::Supplier> PgSupplierRepository::findById(int id) {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec_params(
            "SELECT id, name, contact, phone FROM suppliers WHERE id = $1", id);
        txn.commit();

        if (result.empty()) return std::nullopt;

        const auto& row = result[0];
        return domain::Supplier{
            row["id"].as<int>(),
            row["name"].as<std::string>(),
            row["contact"].is_null() ? "" : row["contact"].as<std::string>(),
            row["phone"].is_null() ? "" : row["phone"].as<std::string>()
        };
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка поиска поставщика: ") + e.what());
    }
}

int PgSupplierRepository::create(const domain::Supplier& supplier) {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec_params(
            "INSERT INTO suppliers (name, contact, phone) VALUES ($1, $2, $3) RETURNING id",
            supplier.name, supplier.contact, supplier.phone);
        txn.commit();
        return result[0]["id"].as<int>();
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка создания поставщика: ") + e.what());
    }
}

bool PgSupplierRepository::update(const domain::Supplier& supplier) {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec_params(
            "UPDATE suppliers SET name = $1, contact = $2, phone = $3 WHERE id = $4",
            supplier.name, supplier.contact, supplier.phone, supplier.id);
        txn.commit();
        return result.affected_rows() > 0;
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка обновления поставщика: ") + e.what());
    }
}

bool PgSupplierRepository::remove(int id) {
    try {
        pqxx::work txn(*conn_);
        txn.exec_params("UPDATE products SET supplier_id = NULL WHERE supplier_id = $1", id);
        auto result = txn.exec_params("DELETE FROM suppliers WHERE id = $1", id);
        txn.commit();
        return result.affected_rows() > 0;
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка удаления поставщика: ") + e.what());
    }
}

}