/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * MIT LICENSE WITH MEDUSASERV RESTRICTIONS
 * 
 * This software is licensed under the MIT License with additional restrictions
 * prohibiting use by Kickstarter, WebHostingTalk, and any entity that discriminates
 * against disabled individuals until they implement proper protections and
 * reasonable adjustments under UK Law.
 * 
 * See LICENSE.md for full license terms.
 * See STANDING_AGAINST_DISABILITY_DISCRIMINATION.md for our commitment.
 * 
 * Yorkshire Champion Standards - Standing Against Discrimination
 */

/**
 * MEDUSASERV PLUGIN MANAGER (.PLG) SYSTEM v1.0.0
 * ===============================================
 * Revolutionary modular web server plugin architecture
 * Ground Up plugin system with Apache/Nginx/Tomcat compatibility
 * © 2025 The Medusa Initiative | Yorkshire Champion Standards
 */

#ifndef MEDUSASERV_PLUGIN_MANAGER_HPP
#define MEDUSASERV_PLUGIN_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <dlfcn.h>
#include <atomic>
#include <mutex>

namespace MedusaServ {
namespace PluginSystem {

// Plugin states
enum class PluginState {
    UNLOADED = 0,
    LOADED = 1,
    INITIALIZED = 2,
    ACTIVE = 3,
    ERROR = 4,
    DISABLED = 5
};

// Plugin types for compatibility
enum class PluginType {
    APACHE_COMPATIBILITY = 1,
    NGINX_COMPATIBILITY = 2,
    TOMCAT_COMPATIBILITY = 3,
    ICEWALL_SECURITY = 4,
    VHOST_MANAGER = 5,
    CALLBACK_MANAGER = 6,
    IPA_INTEGRATION = 7,
    CUSTOM_PLUGIN = 8
};

// Plugin function signatures
typedef int (*PluginInitFunc)();
typedef int (*PluginStartFunc)();
typedef int (*PluginStopFunc)();
typedef int (*PluginCleanupFunc)();
typedef const char* (*PluginVersionFunc)();
typedef const char* (*PluginNameFunc)();

// Plugin information structure
struct PluginInfo {
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::string filename;
    PluginType type;
    PluginState state;
    void* handle;
    
    // Function pointers
    PluginInitFunc init_func;
    PluginStartFunc start_func;
    PluginStopFunc stop_func;
    PluginCleanupFunc cleanup_func;
    PluginVersionFunc version_func;
    PluginNameFunc name_func;
    
    PluginInfo() : state(PluginState::UNLOADED), handle(nullptr),
                  init_func(nullptr), start_func(nullptr),
                  stop_func(nullptr), cleanup_func(nullptr),
                  version_func(nullptr), name_func(nullptr) {}
};

// Plugin Manager class
class PluginManager {
private:
    static std::unique_ptr<PluginManager> instance_;
    static std::mutex instance_mutex_;
    
    std::unordered_map<std::string, std::unique_ptr<PluginInfo>> plugins_;
    std::vector<std::string> plugin_directories_;
    std::mutex plugins_mutex_;
    std::atomic<bool> initialized_;
    
    PluginManager();

public:
    ~PluginManager();
    
    // Singleton access
    static PluginManager& getInstance();
    
    // Core plugin management
    bool initialize();
    bool shutdown();
    
    // Plugin loading/unloading
    bool loadPlugin(const std::string& filename);
    bool unloadPlugin(const std::string& plugin_name);
    bool reloadPlugin(const std::string& plugin_name);
    
    // Plugin control
    bool initializePlugin(const std::string& plugin_name);
    bool startPlugin(const std::string& plugin_name);
    bool stopPlugin(const std::string& plugin_name);
    
    // Plugin discovery
    bool scanPluginDirectory(const std::string& directory);
    bool autoLoadPlugins();
    std::vector<std::string> getAvailablePlugins();
    
    // Plugin information
    PluginInfo* getPluginInfo(const std::string& plugin_name);
    std::vector<PluginInfo*> getPluginsByType(PluginType type);
    std::vector<PluginInfo*> getActivePlugins();
    
    // Compatibility functions
    bool loadApacheCompatibilityModule();
    bool loadNginxCompatibilityModule();
    bool loadTomcatCompatibilityModule();
    
    // Manager system plugins
    bool loadIcewallSecurityManager();
    bool loadVirtualHostManager();
    bool loadCallbackManager();
    bool loadIPAIntegration();
    
    // Plugin directories management
    bool addPluginDirectory(const std::string& directory);
    void removePluginDirectory(const std::string& directory);
    std::vector<std::string> getPluginDirectories() const;
    
    // Statistics and monitoring
    size_t getLoadedPluginCount() const;
    size_t getActivePluginCount() const;
    void printPluginStatus() const;
    
private:
    // Internal helper functions
    bool validatePlugin(void* handle);
    bool loadPluginFunctions(PluginInfo* plugin);
    std::string getPluginPath(const std::string& filename);
    bool isPluginFile(const std::string& filename);
};

// Plugin API macros for plugin developers
#define MEDUSASERV_PLUGIN_EXPORT extern "C"
#define MEDUSASERV_PLUGIN_INIT(func) MEDUSASERV_PLUGIN_EXPORT int medusaserv_plugin_init() { return func(); }
#define MEDUSASERV_PLUGIN_START(func) MEDUSASERV_PLUGIN_EXPORT int medusaserv_plugin_start() { return func(); }
#define MEDUSASERV_PLUGIN_STOP(func) MEDUSASERV_PLUGIN_EXPORT int medusaserv_plugin_stop() { return func(); }
#define MEDUSASERV_PLUGIN_CLEANUP(func) MEDUSASERV_PLUGIN_EXPORT int medusaserv_plugin_cleanup() { return func(); }
#define MEDUSASERV_PLUGIN_VERSION(ver) MEDUSASERV_PLUGIN_EXPORT const char* medusaserv_plugin_version() { return ver; }
#define MEDUSASERV_PLUGIN_NAME(name) MEDUSASERV_PLUGIN_EXPORT const char* medusaserv_plugin_name() { return name; }

} // namespace PluginSystem
} // namespace MedusaServ

#endif // MEDUSASERV_PLUGIN_MANAGER_HPP