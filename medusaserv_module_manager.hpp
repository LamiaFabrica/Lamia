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
 * MEDUSASERV MODULE MANAGER (.MOD) SYSTEM v1.0.0
 * ===============================================
 * Revolutionary modular web server module architecture
 * Ground Up module system with dynamic loading capabilities
 * © 2025 The Medusa Initiative | Yorkshire Champion Standards
 */

#ifndef MEDUSASERV_MODULE_MANAGER_HPP
#define MEDUSASERV_MODULE_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <atomic>
#include <mutex>
#include <chrono>

namespace MedusaServ {
namespace ModuleSystem {

// Module states
enum class ModuleState {
    UNLOADED = 0,
    LOADED = 1,
    CONFIGURED = 2,
    ACTIVE = 3,
    SUSPENDED = 4,
    ERROR = 5,
    UPDATING = 6
};

// Module priority levels
enum class ModulePriority {
    CRITICAL = 1,      // Core system modules
    HIGH = 2,          // Security and routing modules
    NORMAL = 3,        // Standard functionality modules
    LOW = 4,           // Optional feature modules
    BACKGROUND = 5     // Background processing modules
};

// Module types
enum class ModuleType {
    CORE_ENGINE = 1,
    HTTP_HANDLER = 2,
    SECURITY_MODULE = 3,
    VHOST_MODULE = 4,
    CALLBACK_MODULE = 5,
    AUTHENTICATION = 6,
    COMPRESSION = 7,
    CACHE_ENGINE = 8,
    LOGGING_MODULE = 9,
    MONITORING = 10,
    CUSTOM_MODULE = 99
};

// Forward declarations
class ModuleManager;
class ModuleInterface;

// Module function signatures
typedef int (*ModuleInitFunc)(ModuleInterface* interface);
typedef int (*ModuleStartFunc)();
typedef int (*ModuleStopFunc)();
typedef int (*ModuleReloadFunc)();
typedef int (*ModuleCleanupFunc)();
typedef const char* (*ModuleInfoFunc)();

// Module interface for communication between modules and server
class ModuleInterface {
public:
    virtual ~ModuleInterface() = default;
    
    // Server information
    virtual std::string getServerVersion() const = 0;
    virtual std::string getServerConfig(const std::string& key) const = 0;
    virtual bool setServerConfig(const std::string& key, const std::string& value) = 0;
    
    // Logging functions
    virtual void logInfo(const std::string& message) = 0;
    virtual void logWarning(const std::string& message) = 0;
    virtual void logError(const std::string& message) = 0;
    
    // Module communication
    virtual bool sendMessageToModule(const std::string& module_name, const std::string& message) = 0;
    virtual bool broadcastMessage(const std::string& message) = 0;
    
    // Resource management
    virtual bool registerResource(const std::string& name, void* resource) = 0;
    virtual void* getResource(const std::string& name) = 0;
    virtual bool unregisterResource(const std::string& name) = 0;
};

// Module information structure
struct ModuleInfo {
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::string config_file;
    ModuleType type;
    ModulePriority priority;
    ModuleState state;
    
    // Dependencies
    std::vector<std::string> dependencies;
    std::vector<std::string> optional_dependencies;
    
    // Statistics
    std::chrono::steady_clock::time_point load_time;
    std::chrono::steady_clock::time_point last_activity;
    size_t requests_handled;
    size_t errors_count;
    
    // Function pointers
    ModuleInitFunc init_func;
    ModuleStartFunc start_func;
    ModuleStopFunc stop_func;
    ModuleReloadFunc reload_func;
    ModuleCleanupFunc cleanup_func;
    ModuleInfoFunc info_func;
    
    // Configuration
    std::unordered_map<std::string, std::string> config_values;
    
    ModuleInfo() : type(ModuleType::CUSTOM_MODULE), priority(ModulePriority::NORMAL),
                  state(ModuleState::UNLOADED), requests_handled(0), errors_count(0),
                  init_func(nullptr), start_func(nullptr), stop_func(nullptr),
                  reload_func(nullptr), cleanup_func(nullptr), info_func(nullptr) {}
};

// Module Manager class
class ModuleManager : public ModuleInterface {
private:
    static std::unique_ptr<ModuleManager> instance_;
    static std::mutex instance_mutex_;
    
    std::unordered_map<std::string, std::unique_ptr<ModuleInfo>> modules_;
    std::vector<std::string> module_directories_;
    std::unordered_map<std::string, void*> shared_resources_;
    std::mutex modules_mutex_;
    std::mutex resources_mutex_;
    std::atomic<bool> initialized_;
    
    // Server configuration
    std::unordered_map<std::string, std::string> server_config_;
    std::mutex config_mutex_;
    
    ModuleManager();

public:
    ~ModuleManager();
    
    // Singleton access
    static ModuleManager& getInstance();
    
    // Core module management
    bool initialize();
    bool shutdown();
    
    // Module lifecycle
    bool loadModule(const std::string& module_file);
    bool unloadModule(const std::string& module_name);
    bool reloadModule(const std::string& module_name);
    bool configureModule(const std::string& module_name, const std::string& config_file);
    
    // Module control
    bool startModule(const std::string& module_name);
    bool stopModule(const std::string& module_name);
    bool suspendModule(const std::string& module_name);
    bool resumeModule(const std::string& module_name);
    
    // Module discovery and loading
    bool scanModuleDirectory(const std::string& directory);
    bool autoLoadModules();
    bool loadModulesByPriority();
    
    // Dependency management
    bool resolveDependencies(const std::string& module_name);
    std::vector<std::string> getModuleDependencies(const std::string& module_name);
    bool isDependencyMet(const std::string& dependency);
    
    // Module information
    ModuleInfo* getModuleInfo(const std::string& module_name);
    std::vector<ModuleInfo*> getModulesByType(ModuleType type);
    std::vector<ModuleInfo*> getModulesByPriority(ModulePriority priority);
    std::vector<ModuleInfo*> getActiveModules();
    
    // Configuration management
    bool loadModuleConfiguration(const std::string& module_name, const std::string& config_file);
    bool saveModuleConfiguration(const std::string& module_name, const std::string& config_file);
    std::string getModuleConfig(const std::string& module_name, const std::string& key);
    bool setModuleConfig(const std::string& module_name, const std::string& key, const std::string& value);
    
    // Monitoring and statistics
    void updateModuleActivity(const std::string& module_name);
    void incrementModuleRequests(const std::string& module_name);
    void incrementModuleErrors(const std::string& module_name);
    void printModuleStatistics() const;
    
    // Module directories
    bool addModuleDirectory(const std::string& directory);
    void removeModuleDirectory(const std::string& directory);
    std::vector<std::string> getModuleDirectories() const;
    
    // Statistics
    size_t getLoadedModuleCount() const;
    size_t getActiveModuleCount() const;
    void printModuleStatus() const;
    
    // ModuleInterface implementation
    std::string getServerVersion() const override;
    std::string getServerConfig(const std::string& key) const override;
    bool setServerConfig(const std::string& key, const std::string& value) override;
    
    void logInfo(const std::string& message) override;
    void logWarning(const std::string& message) override;
    void logError(const std::string& message) override;
    
    bool sendMessageToModule(const std::string& module_name, const std::string& message) override;
    bool broadcastMessage(const std::string& message) override;
    
    bool registerResource(const std::string& name, void* resource) override;
    void* getResource(const std::string& name) override;
    bool unregisterResource(const std::string& name) override;

private:
    // Internal helper functions
    bool validateModule(const std::string& module_file);
    bool loadModuleFunctions(ModuleInfo* module, void* handle);
    std::string getModulePath(const std::string& module_file);
    bool isModuleFile(const std::string& filename);
    void sortModulesByPriority(std::vector<ModuleInfo*>& modules);
    bool checkCircularDependencies(const std::string& module_name, std::vector<std::string>& path);
};

// Module API macros for module developers
#define MEDUSASERV_MODULE_EXPORT extern "C"
#define MEDUSASERV_MODULE_INIT(func) MEDUSASERV_MODULE_EXPORT int medusaserv_module_init(MedusaServ::ModuleSystem::ModuleInterface* interface) { return func(interface); }
#define MEDUSASERV_MODULE_START(func) MEDUSASERV_MODULE_EXPORT int medusaserv_module_start() { return func(); }
#define MEDUSASERV_MODULE_STOP(func) MEDUSASERV_MODULE_EXPORT int medusaserv_module_stop() { return func(); }
#define MEDUSASERV_MODULE_RELOAD(func) MEDUSASERV_MODULE_EXPORT int medusaserv_module_reload() { return func(); }
#define MEDUSASERV_MODULE_CLEANUP(func) MEDUSASERV_MODULE_EXPORT int medusaserv_module_cleanup() { return func(); }
#define MEDUSASERV_MODULE_INFO(info) MEDUSASERV_MODULE_EXPORT const char* medusaserv_module_info() { return info; }

} // namespace ModuleSystem
} // namespace MedusaServ

#endif // MEDUSASERV_MODULE_MANAGER_HPP