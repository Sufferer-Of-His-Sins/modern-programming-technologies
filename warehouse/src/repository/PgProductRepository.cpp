#include "PgProductRepository.hpp"
#include "db/Connection.hpp"

namespace repository {

PgProductRepository::PgProductRepository(std::shared_ptr<pqxx::connection> conn)
    : conn_(std::move(conn)) {}

domain::Product PgProductRepository::rowToProduct(const pqxx::row& row) const {
    domain::Product p;
    p.id = row["id"].as<int>();
    p.name = row["name"].as<std::string>();
    p.price = row["price"].as<double>();
    p.quantity = row["quantity"].as<int>();
    p.category_id = row["category_id"].as<int>();

    if (!row["supplier_id"].is_null()) {
        p.supplier_id = row["supplier_id"].as<int>();
    }

    try {
        if (!row["category_name"].is_null())
            p.category_name = row["category_name"].as<std::string>();
    } catch (...) {}

    try {
        if (!row["supplier_name"].is_null())
            p.supplier_name = row["supplier_name"].as<std::string>();
    } catch (...) {}

    return p;
}

std::vector<domain::Product> PgProductRepository::findAll() {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec(R"(
            SELECT p.id, p.name, p.price, p.quantity, p.category_id, p.supplier_id,
                   c.name AS category_name,
                   s.name AS supplier_name
            FROM products p
            JOIN categories c ON c.id = p.category_id
            LEFT JOIN suppliers s ON s.id = p.supplier_id
            ORDER BY p.name
        )");
        txn.commit();

        std::vector<domain::Product> items;
        items.reserve(result.size());
        for (const auto& row : result) {
            items.push_back(rowToProduct(row));
        }
        return items;
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка получения товаров: ") + e.what());
    }
}

std::optional<domain::Product> PgProductRepository::findById(int id) {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec_params(R"(
            SELECT p.id, p.name, p.price, p.quantity, p.category_id, p.supplier_id,
                   c.name AS category_name,
                   s.name AS supplier_name
            FROM products p
            JOIN categories c ON c.id = p.category_id
            LEFT JOIN suppliers s ON s.id = p.supplier_id
            WHERE p.id = $1
        )", id);
        txn.commit();

        if (result.empty()) return std::nullopt;
        return rowToProduct(result[0]);
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка поиска товара: ") + e.what());
    }
}

std::vector<domain::Product> PgProductRepository::findByCategory(int category_id) {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec_params(R"(
            SELECT p.id, p.name, p.price, p.quantity, p.category_id, p.supplier_id,
                   c.name AS category_name,
                   s.name AS supplier_name
            FROM products p
            JOIN categories c ON c.id = p.category_id
            LEFT JOIN suppliers s ON s.id = p.supplier_id
            WHERE p.category_id = $1
            ORDER BY p.name
        )", category_id);
        txn.commit();

        std::vector<domain::Product> items;
        items.reserve(result.size());
        for (const auto& row : result) {
            items.push_back(rowToProduct(row));
        }
        return items;
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка фильтрации товаров: ") + e.what());
    }
}

std::vector<domain::Product> PgProductRepository::findLowStock(int threshold) {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec_params(R"(
            SELECT p.id, p.name, p.price, p.quantity, p.category_id, p.supplier_id,
                   c.name AS category_name,
                   s.name AS supplier_name
            FROM products p
            JOIN categories c ON c.id = p.category_id
            LEFT JOIN suppliers s ON s.id = p.supplier_id
            WHERE p.quantity <= $1
            ORDER BY p.quantity ASC, p.name
        )", threshold);
        txn.commit();

        std::vector<domain::Product> items;
        items.reserve(result.size());
        for (const auto& row : result) {
            items.push_back(rowToProduct(row));
        }
        return items;
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка отчёта по остаткам: ") + e.what());
    }
}

int PgProductRepository::create(const domain::Product& product) {
    try {
        pqxx::work txn(*conn_);
        pqxx::result result;

        if (product.supplier_id.has_value()) {
            result = txn.exec_params(
                "INSERT INTO products (name, price, quantity, category_id, supplier_id) "
                "VALUES ($1, $2, $3, $4, $5) RETURNING id",
                product.name, product.price, product.quantity,
                product.category_id, *product.supplier_id);
        } else {
            result = txn.exec_params(
                "INSERT INTO products (name, price, quantity, category_id, supplier_id) "
                "VALUES ($1, $2, $3, $4, NULL) RETURNING id",
                product.name, product.price, product.quantity, product.category_id);
        }
        txn.commit();
        return result[0]["id"].as<int>();
    } catch (const pqxx::foreign_key_violation&) {
        throw db::DatabaseError("Указана несуществующая категория или поставщик");
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка создания товара: ") + e.what());
    }
}

bool PgProductRepository::update(const domain::Product& product) {
    try {
        pqxx::work txn(*conn_);
        pqxx::result result;

        if (product.supplier_id.has_value()) {
            result = txn.exec_params(
                "UPDATE products SET name = $1, price = $2, quantity = $3, "
                "category_id = $4, supplier_id = $5 WHERE id = $6",
                product.name, product.price, product.quantity,
                product.category_id, *product.supplier_id, product.id);
        } else {
            result = txn.exec_params(
                "UPDATE products SET name = $1, price = $2, quantity = $3, "
                "category_id = $4, supplier_id = NULL WHERE id = $5",
                product.name, product.price, product.quantity,
                product.category_id, product.id);
        }
        txn.commit();
        return result.affected_rows() > 0;
    } catch (const pqxx::foreign_key_violation&) {
        throw db::DatabaseError("Указана несуществующая категория или поставщик");
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка обновления товара: ") + e.what());
    }
}

bool PgProductRepository::remove(int id) {
    try {
        pqxx::work txn(*conn_);
        auto result = txn.exec_params("DELETE FROM products WHERE id = $1", id);
        txn.commit();
        return result.affected_rows() > 0;
    } catch (const std::exception& e) {
        throw db::DatabaseError(std::string("Ошибка удаления товара: ") + e.what());
    }
}

}