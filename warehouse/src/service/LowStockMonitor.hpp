#pragma once

#include "domain/Product.hpp"
#include "service/ProductService.hpp"
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace service {

class LowStockMonitor {
public:
    using ResultCallback = std::function<void(std::vector<domain::Product>)>;
    using ErrorCallback  = std::function<void(const std::string&)>;

    LowStockMonitor(std::shared_ptr<ProductService> productService,
                    int threshold,
                    std::chrono::seconds interval,
                    ResultCallback onResult,
                    ErrorCallback onError);

    ~LowStockMonitor();

    LowStockMonitor(const LowStockMonitor&) = delete;
    LowStockMonitor& operator=(const LowStockMonitor&) = delete;

    void start();
    void stop();

private:
    void run();

    std::shared_ptr<ProductService> productService_;
    int threshold_;
    std::chrono::seconds interval_;
    ResultCallback onResult_;
    ErrorCallback onError_;

    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stopRequested_ = false;
};

}