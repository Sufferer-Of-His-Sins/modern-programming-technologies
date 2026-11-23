#include "LowStockMonitor.hpp"

namespace service {

LowStockMonitor::LowStockMonitor(std::shared_ptr<ProductService> productService,
                                 int threshold,
                                 std::chrono::seconds interval,
                                 ResultCallback onResult,
                                 ErrorCallback onError)
    : productService_(std::move(productService))
    , threshold_(threshold)
    , interval_(interval)
    , onResult_(std::move(onResult))
    , onError_(std::move(onError)) {}

LowStockMonitor::~LowStockMonitor() {
    stop();
}

void LowStockMonitor::start() {
    if (thread_.joinable()) return;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stopRequested_ = false;
    }
    thread_ = std::thread(&LowStockMonitor::run, this);
}

void LowStockMonitor::stop() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stopRequested_ = true;
    }
    cv_.notify_all();
    if (thread_.joinable()) {
        thread_.join();
    }
}

void LowStockMonitor::run() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (!stopRequested_) {
        lock.unlock();

        try {
            auto items = productService_->getLowStock(threshold_);
            if (onResult_) onResult_(std::move(items));
        } catch (const std::exception& e) {
            if (onError_) onError_(e.what());
        }

        lock.lock();
        cv_.wait_for(lock, interval_, [this] { return stopRequested_; });
    }
}

}