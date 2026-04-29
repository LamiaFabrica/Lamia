#ifndef MEDUSA_APPLICATION_HPP
#define MEDUSA_APPLICATION_HPP

#include "security_backbone.hpp"
#include "http_server.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <json/json.h>

namespace medusa {

// Forward declarations
class RTSEngine;
class MarketplaceEngine;
class ICEwallFirewall;
class UserManager;
class MenuSystem;
class WatcherSystem;
class AICommandSystem;
class DashboardManager;
class StorefrontEngine;
class AuctionHouseEngine;

/**
 * Master Medusa Application - Coordinates all subsystems
 * Integrates RTS, Marketplace, Security, User Management, etc.
 */
class MedusaApplication {
private:
    // Core Security & HTTP
    std::unique_ptr<security::SecurityBackbone> security_backbone;
    std::unique_ptr<http::GoldStandardHttpServer> http_server;
    
    // Major System Components
    std::unique_ptr<RTSEngine> rts_engine;                  // RoylePython Texture System
    std::unique_ptr<MarketplaceEngine> marketplace_engine;  // 3D Model Marketplace
    std::unique_ptr<ICEwallFirewall> icewall_firewall;     // Advanced Security Firewall
    std::unique_ptr<UserManager> user_manager;              // User CRUD & Permissions
    std::unique_ptr<MenuSystem> menu_system;                // Dynamic Menu System
    std::unique_ptr<WatcherSystem> watcher_system;          // Monitoring & Alerts
    std::unique_ptr<AICommandSystem> ai_command_system;     // AI-Powered Commands
    
    // Dashboard & Commerce Components
    std::unique_ptr<DashboardManager> dashboard_manager;    // All Dashboard Components
    std::unique_ptr<StorefrontEngine> storefront_engine;   // E-commerce Storefront
    std::unique_ptr<AuctionHouseEngine> auction_engine;    // Auction House System
    
    // Application State
    bool initialized;
    bool running;
    std::string config_path;
    Json::Value app_config;
    
    // Route Handlers
    std::unordered_map<std::string, std::function<http::HttpResponse(const http::HttpRequest&)>> route_handlers;
    
    // System Integration
    void setup_route_handlers();
    void initialize_subsystems();
    void start_background_services();
    void setup_cross_system_communication();
    
public:
    MedusaApplication();
    ~MedusaApplication();
    
    // Core Application Lifecycle
    bool initialize(const std::string& config_file);
    bool start();
    void stop();
    bool is_running() const { return running; }
    
    // Configuration Management
    bool load_config(const std::string& config_file);
    Json::Value get_config() const { return app_config; }
    
    // System Access (for internal communication)
    RTSEngine* get_rts_engine() const { return rts_engine.get(); }
    MarketplaceEngine* get_marketplace_engine() const { return marketplace_engine.get(); }
    ICEwallFirewall* get_icewall_firewall() const { return icewall_firewall.get(); }
    UserManager* get_user_manager() const { return user_manager.get(); }
    MenuSystem* get_menu_system() const { return menu_system.get(); }
    WatcherSystem* get_watcher_system() const { return watcher_system.get(); }
    AICommandSystem* get_ai_command_system() const { return ai_command_system.get(); }
    DashboardManager* get_dashboard_manager() const { return dashboard_manager.get(); }
    StorefrontEngine* get_storefront_engine() const { return storefront_engine.get(); }
    AuctionHouseEngine* get_auction_engine() const { return auction_engine.get(); }
    
    // HTTP Route Registration
    void register_route(const std::string& method, const std::string& path, 
                       std::function<http::HttpResponse(const http::HttpRequest&)> handler);
    
    // Health & Status
    struct SystemStatus {
        bool security_backbone_status;
        bool rts_engine_status;
        bool marketplace_status;
        bool icewall_status;
        bool user_manager_status;
        bool menu_system_status;
        bool watcher_status;
        bool ai_command_status;
        bool dashboard_status;
        bool storefront_status;
        bool auction_status;
        std::string overall_status;
        std::vector<std::string> error_messages;
    };
    
    SystemStatus get_system_status() const;
    std::string get_status_json() const;
    
    // Performance Metrics
    struct PerformanceMetrics {
        std::chrono::milliseconds startup_time;
        std::chrono::milliseconds average_response_time;
        size_t total_requests_processed;
        size_t active_users;
        size_t total_registered_users;
        size_t marketplace_listings;
        size_t active_auctions;
        double cpu_usage;
        size_t memory_usage;
        size_t rts_models_generated;
        size_t security_threats_blocked;
    };
    
    PerformanceMetrics get_performance_metrics() const;
    std::string get_metrics_json() const;
    
    // Event System for Cross-Component Communication
    enum class SystemEvent {
        USER_CREATED,
        USER_UPDATED,
        USER_DELETED,
        MENU_UPDATED,
        MARKETPLACE_LISTING_CREATED,
        AUCTION_STARTED,
        AUCTION_ENDED,
        RTS_MODEL_GENERATED,
        SECURITY_THREAT_DETECTED,
        WATCHER_ALERT_TRIGGERED,
        AI_COMMAND_EXECUTED
    };
    
    void emit_event(SystemEvent event, const Json::Value& data);
    void subscribe_to_event(SystemEvent event, std::function<void(const Json::Value&)> callback);
    
    // Error Handling & Recovery
    void handle_system_error(const std::string& component, const std::string& error);
    bool attempt_system_recovery(const std::string& component);
    
    // Configuration Hot-Reload
    void reload_configuration();
    void update_component_config(const std::string& component, const Json::Value& new_config);
};

} // namespace medusa

#endif // MEDUSA_APPLICATION_HPP