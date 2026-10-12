#include "db.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cctype>

namespace {

bool looksLikePhone(const std::string& s) {
    if (s.empty()) return false;
    int digits = 0;
    for (char c : s) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::isdigit(uc)) { digits++; continue; }
        if (c == '+' || c == '-' || c == ' ' || c == '(' || c == ')') continue;
        return false;
    }
    return digits >= 10;
}

const char* ORDER_COLUMNS =
    "id, order_code, "
    "COALESCE(phone, ''), "
    "COALESCE(parcel_article, ''), "
    "COALESCE(cell, ''), "
    "status, "
    "to_char(created_at, 'YYYY-MM-DD HH24:MI')";

Order rowToOrder(const pqxx::row& r) {
    Order o;
    o.id = r[0].as<int>();
    o.code = r[1].as<std::string>();
    o.phone = r[2].as<std::string>();
    o.article = r[3].as<std::string>();
    o.cell = r[4].as<std::string>();
    o.status = r[5].as<std::string>();
    o.created_at = r[6].as<std::string>();
    return o;
}

int countByStatus(pqxx::work& txn, const std::string& status) {
    pqxx::result r = txn.exec_params("SELECT COUNT(*) FROM orders WHERE status = $1", status);
    return r[0][0].as<int>();
}

}

Database::Database(const std::string& conn_str): conn_(conn_str)
{
    if (!conn_.is_open())
        throw std::runtime_error("Не удалось подключиться к PostgreSQL");
}

std::vector<Order> Database::listOrders(const std::string& filter) {
    pqxx::work txn(conn_);
    std::string sql = std::string("SELECT ") + ORDER_COLUMNS + " FROM orders";

    if (!filter.empty()) {
        std::string pattern = txn.quote("%" + filter + "%");
        sql += " WHERE order_code ILIKE " + pattern +
               " OR phone ILIKE " + pattern +
               " OR parcel_article ILIKE " + pattern;
    }
    sql += " ORDER BY created_at DESC, id DESC";

    pqxx::result res = txn.exec(sql);

    std::vector<Order> orders;
    for (const auto& row : res)
        orders.push_back(rowToOrder(row));
    return orders;
}

std::string Database::addOrder(const std::string& contact,const std::string& cell) {
    pqxx::work txn(conn_);

    std::string phone;
    std::string article;
    if (looksLikePhone(contact))
        phone = contact;
    else
        article = contact;

    pqxx::result res = txn.exec_params(
        "INSERT INTO orders (order_code, phone, parcel_article, cell) "
        "VALUES ('TMP', NULLIF($1::text, ''), NULLIF($2::text, ''), NULLIF($3::text, '')) "
        "RETURNING id",phone, article, cell);
    int id = res[0][0].as<int>();

    std::ostringstream code_stream;
    code_stream << "PVZ-" << std::setw(6) << std::setfill('0') << id;
    std::string code = code_stream.str();

    txn.exec_params("UPDATE orders SET order_code = $1 WHERE id = $2", code, id);

    txn.commit();
    return code;
}

bool Database::issueOrder(const std::string& code) {
    pqxx::work txn(conn_);

    pqxx::result res = txn.exec_params("SELECT status FROM orders WHERE order_code = $1", code);
    if (res.empty() || res[0][0].as<std::string>() != "ready")
        return false;

    txn.exec_params(
        "UPDATE orders SET status = 'issued', issued_at = NOW() "
        "WHERE order_code = $1", code);

    txn.commit();
    return true;
}

bool Database::cancelOrder(const std::string& code) {
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec_params("SELECT status FROM orders WHERE order_code = $1", code);
    if (res.empty() || res[0][0].as<std::string>() != "ready")
        return false;

    txn.exec_params("UPDATE orders SET status = 'cancelled' WHERE order_code = $1", code);

    txn.commit();
    return true;
}

Report Database::buildReport() {
    pqxx::work txn(conn_);

    Report rep;
    rep.ready_count = countByStatus(txn, "ready");
    rep.issued_count = countByStatus(txn, "issued");
    rep.cancelled_count = countByStatus(txn, "cancelled");
    return rep;
}

std::vector<Order> Database::listByStatus(const std::string& status) {
    pqxx::work txn(conn_);

    std::string sql = std::string("SELECT ") + ORDER_COLUMNS + " FROM orders";
    if (!status.empty())
        sql += " WHERE status = " + txn.quote(status);
    sql += " ORDER BY created_at DESC, id DESC";

    pqxx::result res = txn.exec(sql);

    std::vector<Order> orders;
    for (const auto& row : res)
        orders.push_back(rowToOrder(row));
    return orders;
}

int Database::countClosedOrders() {
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec("SELECT COUNT(*) FROM orders WHERE status IN ('issued', 'cancelled')");
    return res[0][0].as<int>();
}

int Database::deleteClosedOrders() {
    pqxx::work txn(conn_);

    pqxx::result res = txn.exec("DELETE FROM orders WHERE status IN ('issued', 'cancelled')");
    int deleted = static_cast<int>(res.affected_rows());

    txn.commit();
    return deleted;
}