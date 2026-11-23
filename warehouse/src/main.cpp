#include "db/Connection.hpp"
#include "repository/PgCategoryRepository.hpp"
#include "repository/PgProductRepository.hpp"
#include "repository/PgSupplierRepository.hpp"
#include "service/CategoryService.hpp"
#include "service/ProductService.hpp"
#include "service/SupplierService.hpp"
#include "ui/MainWindow.hpp"

#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <iostream>
#include <memory>

int main(int argc, char** argv) {
    try {
        auto conn = db::Connection::create();
        auto sharedConn = std::shared_ptr<pqxx::connection>(std::move(conn));

        auto categoryRepo = std::make_shared<repository::PgCategoryRepository>(sharedConn);
        auto productRepo  = std::make_shared<repository::PgProductRepository>(sharedConn);
        auto supplierRepo = std::make_shared<repository::PgSupplierRepository>(sharedConn);


        auto categoryService = std::make_shared<service::CategoryService>(categoryRepo);
        auto productService  = std::make_shared<service::ProductService>(productRepo, categoryRepo);
        auto supplierService = std::make_shared<service::SupplierService>(supplierRepo);

        ui::MainWindow window(900, 600, productService, categoryService, supplierService);
        window.show(argc, argv);

        return Fl::run();

    } catch (const db::DatabaseError& e) {
        fl_alert("Ошибка базы данных:\n%s\n\n"
                 "Проверьте переменные окружения PGHOST, PGPORT, PGDATABASE, PGUSER, PGPASSWORD\n"
                 "и что PostgreSQL запущен.", e.what());
        std::cerr << "Database error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        fl_alert("Критическая ошибка:\n%s", e.what());
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}