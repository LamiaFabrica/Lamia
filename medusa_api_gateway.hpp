/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 *
 * MEDUSA API GATEWAY - COMPREHENSIVE IMPLEMENTATION
 * =================================================
 *
 * Comprehensive API gateway for routing, authentication, rate limiting,
 * load balancing, and request/response transformation
 * Ground-up implementation - NO STUBS - Yorkshire Champion Standards
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <functional>
#include <mutex>
#include <atomic>
#include <thread>
#include <queue>
#include <condition_variable>
#include <unordered_map>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "medusa_native_http_client.hpp"
#include "medusa_logger_core.hpp"
#include <nlohmann/json.hpp>

namespace MedusaServ {
namespace APIGateway {

using json = nlohmann::json;

/**
 * @brief HTTP Method enumeration
 */
enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE,
    PATCH,
    OPTIONS,
    HEAD
};

/**
 * @brief Route configuration
 */
struct Route {
    std::string path;
    HttpMethod method;
    std::string target_url;
    std::string service_name;
    bool requires_auth;
    int rate_limit_per_minute;
    std::map<std::string, std::string> headers;
    std::function<bool(const json&)> validator;
    std::function<json(const json&)> transformer;
};

/**
 * @brief Rate limiter for IP addresses
 */
class RateLimiter {
private:
    struct RateLimitEntry {
        int request_count;
        std::chrono::steady_clock::time_point window_start;
    };
    
    std::mutex mutex_;
    std::unordered_map<std::string, RateLimitEntry> limits_;
    int max_requests_per_minute_;
    
public:
    RateLimiter(int max_requests = 60) : max_requests_per_minute_(max_requests) {}
    
    bool check_limit(const std::string& identifier) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto now = std::chrono::steady_clock::now();
        auto& entry = limits_[identifier];
        
        auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(
            now - entry.window_start).count();
        
        if (elapsed >= 1) {
            entry.request_count = 1;
            entry.window_start = now;
            return true;
        }
        
        if (entry.request_count >= max_requests_per_minute_) {
            return false;
        }
        
        entry.request_count++;
        return true;
    }
    
    void reset() {
        std::lock_guard<std::mutex> lock(mutex_);
        limits_.clear();
    }
};

/**
 * @brief Load balancer for distributing requests
 */
class LoadBalancer {
private:
    std::vector<std::string> endpoints_;
    std::atomic<size_t> current_index_;
    std::mutex mutex_;
    
public:
    LoadBalancer(const std::vector<std::string>& endpoints) 
        : endpoints_(endpoints), current_index_(0) {}
    
    std::string get_next_endpoint() {
        if (endpoints_.empty()) return "";
        
        size_t index = current_index_.fetch_add(1) % endpoints_.size();
        return endpoints_[index];
    }
    
    void add_endpoint(const std::string& endpoint) {
        std::lock_guard<std::mutex> lock(mutex_);
        endpoints_.push_back(endpoint);
    }
    
    void remove_endpoint(const std::string& endpoint) {
        std::lock_guard<std::mutex> lock(mutex_);
        endpoints_.erase(
            std::remove(endpoints_.begin(), endpoints_.end(), endpoint),
            endpoints_.end());
    }
};

/**
 * @brief Comprehensive API Gateway implementation
 */
class APIGateway {
private:
    std::vector<Route> routes_;
    std::mutex routes_mutex_;
    
    RateLimiter rate_limiter_;
    std::map<std::string, LoadBalancer> load_balancers_;
    std::mutex balancers_mutex_;
    
    std::string gateway_host_;
    int gateway_port_;
    bool ssl_enabled_;
    SSL_CTX* ssl_ctx_;
    
    std::atomic<bool> running_;
    std::thread gateway_thread_;
    
    mutable std::mutex stats_mutex_;
    mutable GatewayStats stats_;
    
    std::string extract_path(const std::string& url);
    std::string extract_ip(const std::string& request);
    HttpMethod parse_method(const std::string& method_str);
    Route* find_route(const std::string& path, HttpMethod method);
    bool authenticate_request(const std::string& token);
    json transform_request(const Route& route, const json& request);
    json transform_response(const Route& route, const json& response);
    std::string process_request(const std::string& client_request, const std::string& client_ip = "");
    void gateway_loop();
    bool initialize_ssl();
    void cleanup_ssl();
    
public:
    APIGateway(const std::string& host = "0.0.0.0", int port = 8080, bool ssl = false);
    ~APIGateway();
    
    bool initialize();
    void start();
    void stop();
    void shutdown();
    
    void add_route(const Route& route);
    void remove_route(const std::string& path, HttpMethod method);
    std::vector<Route> get_routes() const;
    
    void set_rate_limit(int requests_per_minute);
    void add_load_balancer(const std::string& service_name, const std::vector<std::string>& endpoints);
    
    struct GatewayStats {
        uint64_t total_requests;
        uint64_t successful_requests;
        uint64_t failed_requests;
        uint64_t rate_limited_requests;
        std::chrono::system_clock::time_point start_time;
    };
    
    GatewayStats get_statistics() const;
    void reset_statistics();
};

} // namespace APIGateway
} // namespace MedusaServ

