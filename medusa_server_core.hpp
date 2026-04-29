/*
 * MEDUSA-SERVER CORE - UBUNTU 24.04 UNIFIED ARCHITECTURE
 * Combines both C++ projects with complete integration
 */

#pragma once

#include <string>
#include <memory>
#include <atomic>
#include <thread>
#include <vector>
#include <map>
#include <mutex>
#include <functional>

// Forward declarations for integrated components
namespace MedusaServer {
    class HTTPServer;
    class AuthenticationSystem;
    class DatabaseManager;
    class ThemeManager;
    class WebInterface;
    class DashboardManager;
    class LibraryLoader;
}

namespace MedusaServer {

struct ServerConfig {
    std::string config_file_path = "/home/medusa/conf/server_config.json";
    std::string credentials_password = "";
    int server_port = 3001;
    bool debug_mode = false;
    bool enable_ssl = true;
    std::string ssl_cert_path = "/home/medusa/ssl/medusa.crt";
    std::string ssl_key_path = "/home/medusa/ssl/medusa.key";
    
    // Database configuration
    std::string db_host = "172.236.23.54";
    int db_port = 5432;
    std::string db_name = "medusa_rts";
    
    // Theme configuration
    std::string theme_name = "yorkshire_material";
    bool dark_mode = true;
    
    // Performance settings
    int worker_threads = std::thread::hardware_concurrency();
    int max_connections = 1000;
    size_t request_timeout_ms = 30000;
};

struct ServerStats {
    std::atomic<uint64_t> requests_handled{0};
    std::atomic<uint64_t> active_connections{0};
    std::atomic<uint64_t> auth_attempts{0};
    std::atomic<uint64_t> auth_successes{0};
    std::atomic<uint64_t> database_queries{0};
    std::chrono::system_clock::time_point start_time;
    
    double uptime_seconds() const {
        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start_time);
        return duration.count();
    }
};

class MedusaServerCore {
private:
    ServerConfig config_;
    ServerStats stats_;
    std::atomic<bool> running_{false};
    std::atomic<bool> shutdown_requested_{false};
    
    // Core components (will be initialized with .so libraries)
    std::unique_ptr<HTTPServer> http_server_;
    std::unique_ptr<AuthenticationSystem> auth_system_;
    std::unique_ptr<DatabaseManager> database_manager_;
    std::unique_ptr<ThemeManager> theme_manager_;
    std::unique_ptr<WebInterface> web_interface_;
    std::unique_ptr<DashboardManager> dashboard_manager_;
    std::unique_ptr<LibraryLoader> library_loader_;
    
    // Threading
    std::vector<std::thread> worker_threads_;
    std::mutex server_mutex_;
    
public:
    explicit MedusaServerCore(const ServerConfig& config);
    ~MedusaServerCore();
    
    // Server lifecycle
    bool initialize();
    bool start();
    void run();
    void shutdown();
    
    // Status and monitoring
    bool isRunning() const { return running_.load(); }
    const ServerStats& getStats() const { return stats_; }
    std::string getStatusJson() const;
    
    // Component access
    HTTPServer* getHttpServer() { return http_server_.get(); }
    AuthenticationSystem* getAuthSystem() { return auth_system_.get(); }
    DatabaseManager* getDatabaseManager() { return database_manager_.get(); }
    ThemeManager* getThemeManager() { return theme_manager_.get(); }
    WebInterface* getWebInterface() { return web_interface_.get(); }
    DashboardManager* getDashboardManager() { return dashboard_manager_.get(); }

private:
    // Initialization methods
    bool initializeLibraries();
    bool initializeHTTPServer();
    bool initializeAuthentication();
    bool initializeDatabase();
    bool initializeTheme();
    bool initializeWebInterface();
    bool initializeDashboard();
    
    // Routing setup
    void setupRoutes();
    void setupStaticRoutes();
    void setupAPIRoutes();
    void setupAuthRoutes();
    void setupDashboardRoutes();
    void setupAdminRoutes();
    
    // Request handlers
    std::string handleHealthCheck();
    std::string handleServerStatus();
    std::string handleDashboardData();
    std::string handleThemeData();
    std::string handleUserProfile(const std::string& session_token);
    
    // Utility methods
    void logMessage(const std::string& level, const std::string& message);
    void logError(const std::string& error);
    void logInfo(const std::string& info);
    void logDebug(const std::string& debug);
};

} // namespace MedusaServer