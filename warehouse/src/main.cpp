#include "db/Connection.hpp"
#include "repository/PgCategoryRepository.hpp"
#include "repository/PgProductRepository.hpp"
#include "repository/PgSupplierRepository.hpp"
#include "service/CategoryService.hpp"
#include "service/ProductService.hpp"
#include "service/SupplierService.hpp"
#include "service/LowStockMonitor.hpp"
#include "ui/MainWindow.hpp"

#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <iostream>
#include <memory>
#include <string>

namespace {

struct MonitorMessage {
    ui::MainWindow* window;
    std::string title;
};

void applyMonitorMessage(void* data) {
    std::unique_ptr<MonitorMessage> msg(static_cast<MonitorMessage*>(data));
    msg->window->copy_label(msg->title.c_str());
}

}

int main(int argc, char** argv) {
    try {
        auto sharedConn = std::shared_ptr<pqxx::connection>(db::Connection::create());

        auto categoryRepo = std::make_shared<repository::PgCategoryRepository>(sharedConn);
        auto productRepo  = std::make_shared<repository::PgProductRepository>(sharedConn);
        auto supplierRepo = std::make_shared<repository::PgSupplierRepository>(sharedConn);

        auto categoryService = std::make_shared<service::CategoryService>(categoryRepo);
        auto productService  = std::make_shared<service::ProductService>(productRepo, categoryRepo);
        auto supplierService = std::make_shared<service::SupplierService>(supplierRepo);

        auto workerConn = std::shared_ptr<pqxx::connection>(db::Connection::create());
        auto workerCategoryRepo = std::make_shared<repository::PgCategoryRepository>(workerConn);
        auto workerProductRepo  = std::make_shared<repository::PgProductRepository>(workerConn);
        auto workerProductService =
            std::make_shared<service::ProductService>(workerProductRepo, workerCategoryRepo);

        Fl::lock();

        ui::MainWindow window(900, 600, productService, categoryService, supplierService);
        window.show(argc, argv);

        ui::MainWindow* windowPtr = &window;

        service::LowStockMonitor monitor(
            workerProductService,10,
            std::chrono::seconds(30),
            [windowPtr](std::vector<domain::Product> items) {
                auto* msg = new MonitorMessage{
                    windowPtr,
                    "Склад - товаров с низким остатком: " + std::to_string(items.size())};
                Fl::awake(applyMonitorMessage, msg);
            },
            [](const std::string& error) {
                std::cerr << "Монитор остатков: " << error << std::endl;
            });

        monitor.start();

        int result = Fl::run();

        monitor.stop();
        return result;

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