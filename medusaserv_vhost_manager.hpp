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
 * MEDUSASERV ENCRYPTED VIRTUAL HOST MANAGER v1.0.0
 * =================================================
 * Web-based WYSIWYG Virtual Host Configuration Manager
 * Ground Up encrypted vhost system with professional interface
 * © 2025 The Medusa Initiative | Yorkshire Champion Standards
 */

#ifndef MEDUSASERV_VHOST_MANAGER_HPP
#define MEDUSASERV_VHOST_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <atomic>
#include <mutex>
#include <chrono>

namespace MedusaServ {
namespace VHostSystem {

// SSL/TLS Configuration
struct SSLConfig {
    bool enabled;
    std::string certificate_path;
    std::string private_key_path;
    std::string ca_certificate_path;
    std::string cipher_suite;
    std::string protocols; // TLS 1.2, 1.3, etc.
    bool force_https;
    int hsts_max_age;
    
    SSLConfig() : enabled(false), force_https(false), hsts_max_age(31536000) {}
};

// Virtual Host Configuration
struct VirtualHostConfig {
    std::string name;
    std::string server_name;
    std::vector<std::string> server_aliases;
    std::string document_root;
    int port;
    bool enabled;
    
    // SSL Configuration
    SSLConfig ssl_config;
    
    // Directory Configuration
    std::string index_files;
    bool directory_listing;
    std::string error_pages_path;
    
    // Access Control
    std::vector<std::string> allowed_ips;
    std::vector<std::string> denied_ips;
    bool require_authentication;
    std::string auth_realm;
    
    // Rewrite Rules (Apache/Nginx compatibility)
    std::vector<std::string> rewrite_rules;
    
    // Custom Headers
    std::unordered_map<std::string, std::string> custom_headers;
    
    // Performance Settings
    bool compression_enabled;
    std::string compression_types;
    int max_connections;
    int timeout_seconds;
    
    // Logging
    std::string access_log_path;
    std::string error_log_path;
    std::string log_format;
    
    // Statistics
    std::chrono::steady_clock::time_point created_time;
    std::chrono::steady_clock::time_point last_modified;
    size_t requests_served;
    size_t bytes_transferred;
    
    VirtualHostConfig() : port(80), enabled(true), directory_listing(false),
                         require_authentication(false), compression_enabled(true),
                         max_connections(1000), timeout_seconds(300),
                         requests_served(0), bytes_transferred(0) {}
};

// VHost Manager class
class VHostManager {
private:
    static std::unique_ptr<VHostManager> instance_;
    static std::mutex instance_mutex_;
    
    std::unordered_map<std::string, std::unique_ptr<VirtualHostConfig>> virtual_hosts_;
    std::mutex vhosts_mutex_;
    std::atomic<bool> initialized_;
    
    std::string config_directory_;
    std::string templates_directory_;
    std::string wysiwyg_port_;
    
    VHostManager();

public:
    ~VHostManager();
    
    // Singleton access
    static VHostManager& getInstance();
    
    // Core management
    bool initialize(const std::string& config_dir = "./executable_processes/vhosts/");
    bool shutdown();
    
    // Virtual Host Management
    bool createVirtualHost(const std::string& name, const VirtualHostConfig& config);
    bool updateVirtualHost(const std::string& name, const VirtualHostConfig& config);
    bool deleteVirtualHost(const std::string& name);
    bool enableVirtualHost(const std::string& name);
    bool disableVirtualHost(const std::string& name);
    
    // Configuration Management
    VirtualHostConfig* getVirtualHostConfig(const std::string& name);
    std::vector<std::string> getVirtualHostNames() const;
    std::vector<VirtualHostConfig*> getEnabledVirtualHosts() const;
    std::vector<VirtualHostConfig*> getAllVirtualHosts() const;
    
    // File Operations
    bool loadVirtualHostsFromDirectory();
    bool saveVirtualHostToFile(const std::string& name);
    bool loadVirtualHostFromFile(const std::string& filename);
    bool exportVirtualHostsConfig(const std::string& format = "json");
    
    // SSL/TLS Management
    bool configureSLS(const std::string& vhost_name, const SSLConfig& ssl_config);
    bool generateSelfSignedCertificate(const std::string& vhost_name, const std::string& domain);
    bool validateSSLConfiguration(const std::string& vhost_name);
    
    // Security Features
    bool addAllowedIP(const std::string& vhost_name, const std::string& ip_address);
    bool addDeniedIP(const std::string& vhost_name, const std::string& ip_address);
    bool removeIPFromList(const std::string& vhost_name, const std::string& ip_address);
    bool isIPAllowed(const std::string& vhost_name, const std::string& ip_address);
    
    // Rewrite Rules (Apache/Nginx compatibility)
    bool addRewriteRule(const std::string& vhost_name, const std::string& rule);
    bool removeRewriteRule(const std::string& vhost_name, const std::string& rule);
    std::vector<std::string> getRewriteRules(const std::string& vhost_name);
    
    // WYSIWYG Web Interface
    bool startWYSIWYGInterface(int port = 8080);
    bool stopWYSIWYGInterface();
    std::string generateWYSIWYGConfigPage(const std::string& vhost_name);
    std::string processWYSIWYGConfigUpdate(const std::string& vhost_name, const std::string& json_data);
    
    // Template Management
    bool createVirtualHostTemplate(const std::string& name, const VirtualHostConfig& config);
    bool applyTemplate(const std::string& vhost_name, const std::string& template_name);
    std::vector<std::string> getAvailableTemplates() const;
    
    // Statistics and Monitoring
    void updateVirtualHostStats(const std::string& vhost_name, size_t bytes_transferred);
    void printVirtualHostStatistics() const;
    std::string getVirtualHostStatisticsJSON() const;
    
    // Compatibility Functions
    bool exportApacheConfig(const std::string& output_file);
    bool exportNginxConfig(const std::string& output_file);
    bool importApacheConfig(const std::string& config_file);
    bool importNginxConfig(const std::string& config_file);
    
private:
    // Internal helper functions
    std::string generateVHostConfigFile(const VirtualHostConfig& config);
    bool parseVHostConfigFile(const std::string& content, VirtualHostConfig& config);
    std::string encryptConfigData(const std::string& data);
    std::string decryptConfigData(const std::string& encrypted_data);
    bool validateVirtualHostConfig(const VirtualHostConfig& config);
    std::string generateUniqueVHostID(const std::string& name);
    void initializeDefaultTemplates();
};

// Web Interface Handler for WYSIWYG
class WYSIWYGHandler {
public:
    static std::string handleConfigRequest(const std::string& vhost_name);
    static std::string handleUpdateRequest(const std::string& vhost_name, const std::string& data);
    static std::string handleListRequest();
    static std::string handleDeleteRequest(const std::string& vhost_name);
    static std::string handleTemplateRequest(const std::string& template_name);
    
private:
    static std::string generateHTML(const std::string& title, const std::string& content);
    static std::string generateConfigForm(const VirtualHostConfig& config);
    static std::string generateJavaScript();
    static std::string generateCSS();
};

} // namespace VHostSystem
} // namespace MedusaServ

#endif // MEDUSASERV_VHOST_MANAGER_HPP