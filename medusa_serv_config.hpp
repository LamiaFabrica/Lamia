#pragma once

/**
 * MedusaServ Configuration Management
 * ==================================
 * 
 * Yorkshire Champion configuration system supporting:
 * - Virtual Host configuration with SSL
 * - Dual-stack IPv4/IPv6 networking
 * - Advanced proxy pass configurations
 * - Hot-reload without downtime
 * - Integration with SSL certificates and DNS
 * 
 * Configuration Format: JSON-based for easy management
 */

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>
#include <filesystem>
#include <json/json.h>

namespace medusa::core {

    /**
     * SSL Certificate Configuration
     */
    struct SSLCertificateConfig {
        std::string certificate_file;
        std::string private_key_file;
        std::string ca_bundle_file;
        std::vector<std::string> supported_protocols;
        std::vector<std::string> cipher_suites;
        bool enable_ocsp_stapling;
        bool enable_perfect_forward_secrecy;
        
        SSLCertificateConfig() 
            : enable_ocsp_stapling(false), enable_perfect_forward_secrecy(true) {}
    };

    /**
     * Proxy Target Configuration
     */
    struct ProxyTargetConfig {
        std::string address;
        uint16_t port;
        uint32_t weight;
        bool is_backup;
        std::chrono::seconds health_check_interval;
        std::string health_check_path;
        
        ProxyTargetConfig()
            : port(80), weight(1), is_backup(false), 
              health_check_interval(30), health_check_path("/") {}
    };

    /**
     * Location Block Configuration
     */
    struct LocationConfig {
        std::string path_pattern;
        std::string document_root;
        std::vector<std::string> index_files;
        std::vector<std::string> allowed_methods;
        std::unordered_map<std::string, std::string> custom_headers;
        
        // Proxy configuration
        std::vector<ProxyTargetConfig> proxy_targets;
        std::string load_balance_method;
        bool enable_proxy_buffering;
        std::chrono::seconds proxy_timeout;
        
        // Security settings
        bool requires_ssl;
        bool requires_authentication;
        std::vector<std::string> allowed_origins;
        
        LocationConfig()
            : enable_proxy_buffering(true), proxy_timeout(30),
              requires_ssl(false), requires_authentication(false) {}
    };

    /**
     * Virtual Host Configuration
     */
    struct VirtualHostConfig {
        std::string server_name;
        std::vector<std::string> server_aliases;
        std::string listen_address;
        uint16_t listen_port;
        bool enable_ipv6;
        
        std::string document_root;
        std::vector<std::string> index_files;
        std::string access_log_file;
        std::string error_log_file;
        
        // SSL configuration
        std::optional<SSLCertificateConfig> ssl_config;
        bool force_https;
        
        // Location configurations
        std::vector<LocationConfig> locations;
        
        // Custom headers
        std::unordered_map<std::string, std::string> global_headers;
        
        VirtualHostConfig()
            : listen_address("0.0.0.0"), listen_port(80), enable_ipv6(true),
              force_https(false) {}
    };

    /**
     * Global Server Configuration
     */
    struct MedusaServConfig {
        // Server basics
        std::string server_name;
        std::string server_version;
        std::string pid_file;
        std::string log_directory;
        
        // Network settings
        size_t worker_processes;
        size_t worker_connections;
        std::chrono::seconds keepalive_timeout;
        size_t client_max_body_size;
        
        // Security settings
        bool hide_server_tokens;
        std::vector<std::string> allowed_methods;
        std::string rate_limit_zone;
        
        // SSL/TLS settings
        std::string ssl_protocols;
        std::string ssl_ciphers;
        bool ssl_prefer_server_ciphers;
        std::string ssl_dhparam;
        
        // Performance settings
        bool enable_gzip;
        std::vector<std::string> gzip_types;
        size_t gzip_min_length;
        
        // Virtual hosts
        std::vector<VirtualHostConfig> virtual_hosts;
        
        // Integration settings
        std::string triforce_database_url;
        std::string security_engine_config;
        bool enable_icewall_integration;
        
        MedusaServConfig()
            : server_name("MedusaServ"), server_version("1.0.0"),
              worker_processes(0), worker_connections(1024),
              keepalive_timeout(65), client_max_body_size(1024 * 1024), // 1MB
              hide_server_tokens(true), ssl_prefer_server_ciphers(true),
              enable_gzip(true), gzip_min_length(1000),
              enable_icewall_integration(true) {}
    };

    /**
     * Configuration Parser and Manager
     */
    class ConfigurationManager {
    private:
        std::string config_file_path_;
        MedusaServConfig current_config_;
        std::filesystem::file_time_type last_modified_;
        
    public:
        ConfigurationManager(const std::string& config_file = "");
        
        // Configuration loading
        bool load_configuration(const std::string& config_file = "");
        bool reload_if_changed();
        bool validate_configuration() const;
        
        // Configuration access
        const MedusaServConfig& get_config() const { return current_config_; }
        MedusaServConfig& get_mutable_config() { return current_config_; }
        
        // Configuration modification
        void add_virtual_host(const VirtualHostConfig& vhost);
        void remove_virtual_host(const std::string& server_name);
        VirtualHostConfig* find_virtual_host(const std::string& server_name);
        
        // SSL certificate management
        bool add_ssl_certificate(const std::string& server_name, 
                                const SSLCertificateConfig& ssl_config);
        bool remove_ssl_certificate(const std::string& server_name);
        
        // Configuration generation
        bool generate_default_config(const std::string& output_file);
        bool save_configuration(const std::string& output_file = "");
        
        // Configuration validation
        struct ValidationResult {
            bool is_valid;
            std::vector<std::string> errors;
            std::vector<std::string> warnings;
        };
        
        ValidationResult validate_detailed() const;
        
    private:
        // JSON parsing methods
        bool parse_json_config(const Json::Value& root);
        bool parse_virtual_host(const Json::Value& vhost_json, VirtualHostConfig& vhost_config);
        bool parse_location(const Json::Value& location_json, LocationConfig& location_config);
        bool parse_ssl_config(const Json::Value& ssl_json, SSLCertificateConfig& ssl_config);
        bool parse_proxy_targets(const Json::Value& proxy_json, std::vector<ProxyTargetConfig>& targets);
        
        // JSON generation methods
        Json::Value generate_json_config() const;
        Json::Value generate_virtual_host_json(const VirtualHostConfig& vhost) const;
        Json::Value generate_location_json(const LocationConfig& location) const;
        Json::Value generate_ssl_json(const SSLCertificateConfig& ssl_config) const;
        
        // Validation helpers
        bool validate_network_address(const std::string& address) const;
        bool validate_file_path(const std::string& path, bool must_exist = true) const;
        bool validate_ssl_certificate(const SSLCertificateConfig& ssl_config) const;
        bool check_port_availability(const std::string& address, uint16_t port) const;
    };

    /**
     * Default Configuration Generator
     */
    class DefaultConfigGenerator {
    public:
        static MedusaServConfig generate_basic_config();
        static MedusaServConfig generate_ssl_config(const std::string& domain,
                                                   const std::string& cert_file,
                                                   const std::string& key_file);
        static MedusaServConfig generate_proxy_config(const std::string& domain,
                                                     const std::vector<std::string>& backend_servers);
        static MedusaServConfig generate_dual_stack_config(const std::string& domain);
        
        // Predefined virtual host templates
        static VirtualHostConfig create_static_site_vhost(const std::string& domain,
                                                         const std::string& document_root);
        static VirtualHostConfig create_proxy_vhost(const std::string& domain,
                                                   const std::vector<std::string>& backends);
        static VirtualHostConfig create_ssl_vhost(const std::string& domain,
                                                 const std::string& cert_file,
                                                 const std::string& key_file);
        
        // Location templates
        static LocationConfig create_static_location(const std::string& path,
                                                    const std::string& document_root);
        static LocationConfig create_proxy_location(const std::string& path,
                                                   const std::vector<std::string>& backends);
        static LocationConfig create_api_location(const std::string& path,
                                                 const std::string& backend_server);
    };

    /**
     * Configuration Hot-Reload Manager
     */
    class HotReloadManager {
    private:
        ConfigurationManager& config_manager_;
        std::function<void(const MedusaServConfig&)> reload_callback_;
        std::thread watcher_thread_;
        std::atomic<bool> is_watching_;
        std::chrono::seconds check_interval_;
        
    public:
        HotReloadManager(ConfigurationManager& config_manager,
                        std::function<void(const MedusaServConfig&)> callback);
        ~HotReloadManager();
        
        void start_watching();
        void stop_watching();
        bool is_watching() const { return is_watching_.load(); }
        
        void set_check_interval(std::chrono::seconds interval) {
            check_interval_ = interval;
        }
        
    private:
        void watch_config_file();
    };

} // namespace medusa::core