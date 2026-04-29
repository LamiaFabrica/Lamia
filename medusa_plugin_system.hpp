/*
 * MEDUSA MODULAR PLUGIN SYSTEM
 * Yorkshire Champion extensible .so/.dll plugin architecture
 * Maximum extensibility for back-office++ ecosystem
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

#ifdef _WIN32
    #include <windows.h>
    #define PLUGIN_EXPORT __declspec(dllexport)
    #define PLUGIN_HANDLE HMODULE
#else
    #include <dlfcn.h>
    #define PLUGIN_EXPORT __attribute__((visibility("default")))
    #define PLUGIN_HANDLE void*
#endif

namespace MedusaServer {

// Plugin API Version
constexpr int MEDUSA_PLUGIN_API_VERSION = 1;

// Plugin Types
enum class PluginType {
    AUTHENTICATION = 1,
    DATABASE = 2,
    THEME = 3,
    SECURITY = 4,
    ANALYTICS = 5,
    COMMUNICATION = 6,
    INTEGRATION = 7,
    UTILITY = 8,
    CUSTOM = 99
};

// Plugin Status
enum class PluginStatus {
    UNLOADED,
    LOADING,
    LOADED,
    ACTIVE,
    SUSPENDED,
    ERROR,
    UNLOADING
};

// Plugin Metadata
struct PluginMetadata {
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::string license;
    PluginType type;
    int api_version;
    std::vector<std::string> dependencies;
    std::map<std::string, std::string> configuration;
    std::chrono::system_clock::time_point build_date;
};

// Plugin Interface - All plugins must implement this
class IPlugin {
public:
    virtual ~IPlugin() = default;
    
    // Lifecycle methods
    virtual bool initialize(const std::map<std::string, std::string>& config) = 0;
    virtual bool start() = 0;
    virtual bool stop() = 0;
    virtual void cleanup() = 0;
    
    // Plugin information
    virtual PluginMetadata getMetadata() const = 0;
    virtual std::string getVersion() const = 0;
    virtual int getAPIVersion() const = 0;
    
    // Health and status
    virtual bool isHealthy() const = 0;
    virtual std::string getHealthStatus() const = 0;
    virtual std::map<std::string, std::string> getMetrics() const = 0;
    
    // Configuration
    virtual bool configure(const std::map<std::string, std::string>& config) = 0;
    virtual std::map<std::string, std::string> getConfiguration() const = 0;
    
    // Event handling
    virtual void onEvent(const std::string& event_name, const std::map<std::string, std::string>& data) = 0;
};

// Plugin Factory Function Type
typedef IPlugin* (*CreatePluginFunc)();
typedef void (*DestroyPluginFunc)(IPlugin*);

// Plugin Instance
class PluginInstance {
private:
    std::string plugin_path_;
    PLUGIN_HANDLE handle_;
    std::unique_ptr<IPlugin> plugin_;
    PluginStatus status_;
    PluginMetadata metadata_;
    CreatePluginFunc create_func_;
    DestroyPluginFunc destroy_func_;
    std::chrono::system_clock::time_point load_time_;
    std::map<std::string, double> performance_metrics_;
    
public:
    PluginInstance(const std::string& plugin_path);
    ~PluginInstance();
    
    bool load();
    bool unload();
    bool reload();
    
    IPlugin* getPlugin() { return plugin_.get(); }
    const IPlugin* getPlugin() const { return plugin_.get(); }
    
    PluginStatus getStatus() const { return status_; }
    PluginMetadata getMetadata() const { return metadata_; }
    std::string getPath() const { return plugin_path_; }
    
    // Performance tracking
    void recordMetric(const std::string& metric_name, double value);
    std::map<std::string, double> getPerformanceMetrics() const;
    
private:
    bool loadLibrary();
    bool unloadLibrary();
    bool validatePlugin();
};

// Plugin Manager - Central plugin management system
class PluginManager {
private:
    std::map<std::string, std::unique_ptr<PluginInstance>> plugins_;
    std::vector<std::string> plugin_directories_;
    std::map<PluginType, std::vector<std::string>> plugins_by_type_;
    std::mutex plugins_mutex_;
    std::atomic<bool> auto_discovery_enabled_;
    std::thread discovery_thread_;
    std::atomic<bool> running_;
    
    // Event system
    std::map<std::string, std::vector<std::function<void(const std::map<std::string, std::string>&)>>> event_handlers_;
    std::mutex events_mutex_;
    
    // Configuration
    std::map<std::string, std::map<std::string, std::string>> plugin_configs_;
    
public:
    PluginManager();
    ~PluginManager();
    
    // Lifecycle
    bool initialize(const std::vector<std::string>& plugin_dirs);
    void shutdown();
    
    // Plugin management
    bool loadPlugin(const std::string& plugin_path);
    bool unloadPlugin(const std::string& plugin_name);
    bool reloadPlugin(const std::string& plugin_name);
    bool loadAllPlugins();
    void unloadAllPlugins();
    
    // Plugin discovery
    void enableAutoDiscovery(bool enable);
    std::vector<std::string> discoverPlugins();
    void scanDirectory(const std::string& directory);
    
    // Plugin access
    IPlugin* getPlugin(const std::string& plugin_name);
    std::vector<std::string> getLoadedPlugins() const;
    std::vector<std::string> getPluginsByType(PluginType type) const;
    PluginMetadata getPluginMetadata(const std::string& plugin_name) const;
    
    // Configuration
    bool configurePlugin(const std::string& plugin_name, const std::map<std::string, std::string>& config);
    std::map<std::string, std::string> getPluginConfig(const std::string& plugin_name) const;
    bool savePluginConfigs(const std::string& config_file);
    bool loadPluginConfigs(const std::string& config_file);
    
    // Event system
    void registerEventHandler(const std::string& event_name, 
                            std::function<void(const std::map<std::string, std::string>&)> handler);
    void unregisterEventHandler(const std::string& event_name);
    void triggerEvent(const std::string& event_name, const std::map<std::string, std::string>& data);
    
    // Health monitoring
    std::map<std::string, bool> getPluginHealth() const;
    std::map<std::string, std::string> getPluginStatuses() const;
    
    // Performance monitoring
    std::map<std::string, std::map<std::string, double>> getAllPerformanceMetrics() const;
    
private:
    void discoveryLoop();
    bool isValidPluginFile(const std::string& file_path);
    std::string generatePluginName(const std::string& plugin_path);
    void notifyPluginsOfEvent(const std::string& event_name, const std::map<std::string, std::string>& data);
};

// Base Plugin Class - Helper for plugin implementation
class BasePlugin : public IPlugin {
protected:
    PluginMetadata metadata_;
    std::map<std::string, std::string> config_;
    std::atomic<bool> running_;
    std::atomic<bool> healthy_;
    std::map<std::string, double> metrics_;
    std::mutex metrics_mutex_;
    
public:
    BasePlugin(const std::string& name, const std::string& version, PluginType type);
    virtual ~BasePlugin() = default;
    
    // IPlugin implementation
    virtual bool initialize(const std::map<std::string, std::string>& config) override;
    virtual bool start() override;
    virtual bool stop() override;
    virtual void cleanup() override;
    
    virtual PluginMetadata getMetadata() const override { return metadata_; }
    virtual std::string getVersion() const override { return metadata_.version; }
    virtual int getAPIVersion() const override { return MEDUSA_PLUGIN_API_VERSION; }
    
    virtual bool isHealthy() const override { return healthy_; }
    virtual std::string getHealthStatus() const override;
    virtual std::map<std::string, std::string> getMetrics() const override;
    
    virtual bool configure(const std::map<std::string, std::string>& config) override;
    virtual std::map<std::string, std::string> getConfiguration() const override { return config_; }
    
    virtual void onEvent(const std::string& event_name, const std::map<std::string, std::string>& data) override;
    
protected:
    // Helper methods for derived plugins
    void setHealthy(bool healthy) { healthy_ = healthy; }
    void updateMetric(const std::string& name, double value);
    std::string getConfigValue(const std::string& key, const std::string& default_value = "") const;
    bool hasConfigValue(const std::string& key) const;
};

// Plugin Development Macros
#define MEDUSA_PLUGIN_EXPORT extern "C" {

#define MEDUSA_PLUGIN_CREATE(PluginClass) \
    PLUGIN_EXPORT IPlugin* createPlugin() { \
        return new PluginClass(); \
    }

#define MEDUSA_PLUGIN_DESTROY \
    PLUGIN_EXPORT void destroyPlugin(IPlugin* plugin) { \
        delete plugin; \
    }

#define MEDUSA_PLUGIN_EXPORT_END }

// Standard Medusa Plugins

// Authentication Plugin Interface
class IAuthenticationPlugin : public IPlugin {
public:
    virtual bool authenticate(const std::string& username, const std::string& password) = 0;
    virtual std::string generateToken(const std::string& user_id) = 0;
    virtual bool validateToken(const std::string& token) = 0;
    virtual bool revokeToken(const std::string& token) = 0;
};

// Database Plugin Interface
class IDatabasePlugin : public IPlugin {
public:
    virtual bool connect(const std::string& connection_string) = 0;
    virtual bool disconnect() = 0;
    virtual std::vector<std::map<std::string, std::string>> query(const std::string& sql) = 0;
    virtual bool execute(const std::string& sql) = 0;
    virtual bool beginTransaction() = 0;
    virtual bool commitTransaction() = 0;
    virtual bool rollbackTransaction() = 0;
};

// Theme Plugin Interface
class IThemePlugin : public IPlugin {
public:
    virtual std::string getThemeName() const = 0;
    virtual std::map<std::string, std::string> getColorScheme() const = 0;
    virtual std::string generateCSS() = 0;
    virtual bool applyTheme() = 0;
};

// Security Plugin Interface
class ISecurityPlugin : public IPlugin {
public:
    virtual bool scanForVulnerabilities() = 0;
    virtual std::vector<std::string> getVulnerabilities() const = 0;
    virtual bool fixVulnerability(const std::string& vuln_id) = 0;
    virtual std::string encryptData(const std::string& data) = 0;
    virtual std::string decryptData(const std::string& encrypted_data) = 0;
};

// Plugin Registry - Global plugin registration
class PluginRegistry {
private:
    static PluginRegistry* instance_;
    std::map<std::string, std::function<IPlugin*()>> factories_;
    std::mutex registry_mutex_;
    
    PluginRegistry() = default;
    
public:
    static PluginRegistry* getInstance();
    
    void registerPlugin(const std::string& name, std::function<IPlugin*()> factory);
    IPlugin* createPlugin(const std::string& name);
    std::vector<std::string> getRegisteredPlugins() const;
    
    ~PluginRegistry() = default;
};

// Helper function for plugin registration
template<typename T>
void registerPluginType(const std::string& name) {
    PluginRegistry::getInstance()->registerPlugin(name, []() { return new T(); });
}

} // namespace MedusaServer

// Example Plugin Implementation Template
/*
#include "medusa_plugin_system.hpp"

class ExamplePlugin : public MedusaServer::BasePlugin {
public:
    ExamplePlugin() : BasePlugin("ExamplePlugin", "1.0.0", MedusaServer::PluginType::UTILITY) {
        metadata_.author = "Yorkshire Champion";
        metadata_.description = "Example plugin for Medusa system";
    }
    
    bool initialize(const std::map<std::string, std::string>& config) override {
        BasePlugin::initialize(config);
        // Custom initialization
        return true;
    }
    
    bool start() override {
        running_ = true;
        healthy_ = true;
        return true;
    }
    
    bool stop() override {
        running_ = false;
        return true;
    }
};

MEDUSA_PLUGIN_EXPORT
MEDUSA_PLUGIN_CREATE(ExamplePlugin)
MEDUSA_PLUGIN_DESTROY
MEDUSA_PLUGIN_EXPORT_END
*/