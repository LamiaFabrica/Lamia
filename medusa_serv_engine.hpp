#pragma once

/**
 * MedusaServ - Native C++ Web Server Engine
 * =========================================
 * 
 * Yorkshire Champion Gold Standard implementation replacing NGINX functionality
 * with advanced features for dual-stack IPv4/IPv6, SSL/TLS, and port forwarding.
 * 
 * Features:
 * - Virtual Host management with SSL termination
 * - Dual-stack IPv4/IPv6 support with automatic DNS resolution
 * - Advanced proxy pass with load balancing
 * - Hot reload configuration without downtime
 * - Integrated with RoyaleGate security architecture
 * 
 * Author: Claude Code (Yorkshire Champion)
 * Purpose: Ground-up NGINX replacement for Medusa ecosystem
 */

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <optional>
#include <functional>
#include <regex>

// Network includes
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

// SSL/TLS includes  
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>

// Medusa core includes
#include "../security/medusa_security_engine.hpp"
#include "../database/medusa_triforce_connector.hpp"

namespace medusa::core {

    // Forward declarations
    class VirtualHost;
    class ProxyTarget;
    class SSLContext;
    class RequestRouter;

    /**
     * Network address structure supporting both IPv4 and IPv6
     */
    struct NetworkAddress {
        enum class Type { IPv4, IPv6, HOSTNAME };
        
        Type address_type;
        std::string address;
        uint16_t port;
        bool is_ssl;
        
        // Dual-stack resolution results
        std::vector<sockaddr_storage> resolved_addresses;
        std::chrono::system_clock::time_point last_resolved;
        
        NetworkAddress(const std::string& addr, uint16_t p, bool ssl = false)
            : address(addr), port(p), is_ssl(ssl), last_resolved{} {
            determine_address_type();
        }
        
        void determine_address_type();
        bool resolve_addresses();
        bool needs_resolution() const;
    };

    /**
     * SSL/TLS Context Management
     */
    class SSLContext {
    private:
        SSL_CTX* ssl_context_;
        std::string cert_file_;
        std::string key_file_;
        std::string ca_file_;
        std::vector<std::string> supported_protocols_;
        
    public:
        SSLContext(const std::string& cert_file, const std::string& key_file, 
                  const std::string& ca_file = "");
        ~SSLContext();
        
        bool initialize();
        SSL* create_ssl_connection(int socket_fd);
        bool load_certificate_chain(const std::string& cert_file);
        bool load_private_key(const std::string& key_file);
        bool verify_certificate_key_pair();
        
        // Advanced SSL features
        bool set_cipher_suites(const std::vector<std::string>& suites);
        bool enable_perfect_forward_secrecy();
        bool set_supported_protocols(const std::vector<std::string>& protocols);
        
        SSL_CTX* get_context() const { return ssl_context_; }
    };

    /**
     * Proxy Target Configuration
     */
    class ProxyTarget {
    public:
        enum class LoadBalanceMethod {
            ROUND_ROBIN,
            LEAST_CONNECTIONS,
            WEIGHTED_ROUND_ROBIN,
            IP_HASH,
            HEALTH_CHECK_BASED
        };
        
        struct TargetServer {
            NetworkAddress address;
            uint32_t weight;
            std::atomic<uint32_t> current_connections;
            std::atomic<bool> is_healthy;
            std::chrono::system_clock::time_point last_health_check;
            double response_time_avg;
            
            TargetServer(const NetworkAddress& addr, uint32_t w = 1)
                : address(addr), weight(w), current_connections(0), 
                  is_healthy(true), response_time_avg(0.0) {}
        };
        
    private:
        std::string proxy_name_;
        std::vector<std::unique_ptr<TargetServer>> targets_;
        LoadBalanceMethod balance_method_;
        std::atomic<size_t> round_robin_index_;
        std::mutex targets_mutex_;
        
        // Health checking
        std::thread health_check_thread_;
        std::atomic<bool> health_check_running_;
        std::chrono::seconds health_check_interval_;
        
    public:
        ProxyTarget(const std::string& name, LoadBalanceMethod method = LoadBalanceMethod::ROUND_ROBIN);
        ~ProxyTarget();
        
        void add_target(const NetworkAddress& address, uint32_t weight = 1);
        void remove_target(const std::string& address, uint16_t port);
        TargetServer* select_target();
        
        void start_health_monitoring();
        void stop_health_monitoring();
        
        // Configuration
        void set_health_check_interval(std::chrono::seconds interval) {
            health_check_interval_ = interval;
        }
        
        // Statistics
        std::vector<std::pair<std::string, uint32_t>> get_connection_stats() const;
        double get_average_response_time() const;
        
    private:
        void health_check_worker();
        bool check_target_health(TargetServer& target);
    };

    /**
     * Virtual Host Configuration
     */
    class VirtualHost {
    public:
        struct Location {
            std::string path_pattern;
            std::regex compiled_pattern;
            std::string document_root;
            std::unique_ptr<ProxyTarget> proxy_target;
            std::unordered_map<std::string, std::string> headers;
            bool requires_ssl;
            bool requires_auth;
            std::vector<std::string> allowed_methods;
            
            Location(const std::string& pattern) 
                : path_pattern(pattern), compiled_pattern(pattern), 
                  requires_ssl(false), requires_auth(false) {}
        };
        
    private:
        std::string server_name_;
        std::vector<std::string> server_aliases_;
        NetworkAddress listen_address_;
        std::string document_root_;
        std::unique_ptr<SSLContext> ssl_context_;
        std::vector<std::unique_ptr<Location>> locations_;
        std::unordered_map<std::string, std::string> custom_headers_;
        
        // Security integration
        std::shared_ptr<medusa::security::SecurityEngine> security_engine_;
        
    public:
        VirtualHost(const std::string& server_name, const NetworkAddress& listen_addr);
        ~VirtualHost() = default;
        
        // Configuration methods
        void add_server_alias(const std::string& alias);
        void set_document_root(const std::string& root);
        bool configure_ssl(const std::string& cert_file, const std::string& key_file,
                          const std::string& ca_file = "");
        
        // Location management
        Location* add_location(const std::string& path_pattern);
        Location* find_matching_location(const std::string& request_path) const;
        
        // Proxy configuration
        bool configure_proxy_pass(const std::string& location_pattern, 
                                 const std::vector<NetworkAddress>& targets,
                                 ProxyTarget::LoadBalanceMethod method = ProxyTarget::LoadBalanceMethod::ROUND_ROBIN);
        
        // Security integration
        void set_security_engine(std::shared_ptr<medusa::security::SecurityEngine> engine) {
            security_engine_ = engine;
        }
        
        // Getters
        const std::string& get_server_name() const { return server_name_; }
        const NetworkAddress& get_listen_address() const { return listen_address_; }
        bool has_ssl() const { return ssl_context_ != nullptr; }
        SSLContext* get_ssl_context() const { return ssl_context_.get(); }
        
        // Request matching
        bool matches_request(const std::string& host, const std::string& path) const;
    };

    /**
     * HTTP Request/Response structures
     */
    struct HttpRequest {
        std::string method;
        std::string path;
        std::string query_string;
        std::string http_version;
        std::unordered_map<std::string, std::string> headers;
        std::string body;
        NetworkAddress client_address;
        bool is_ssl;
        
        // Medusa-specific fields
        std::string session_token;
        std::string user_agent_fingerprint;
        std::chrono::system_clock::time_point timestamp;
    };

    struct HttpResponse {
        uint16_t status_code;
        std::string status_message;
        std::unordered_map<std::string, std::string> headers;
        std::string body;
        bool connection_close;
        
        HttpResponse(uint16_t code = 200, const std::string& message = "OK") 
            : status_code(code), status_message(message), connection_close(false) {}
    };

    /**
     * Connection Management
     */
    class Connection {
    private:
        int socket_fd_;
        SSL* ssl_connection_;
        NetworkAddress client_address_;
        NetworkAddress server_address_;
        std::chrono::system_clock::time_point created_at_;
        std::chrono::system_clock::time_point last_activity_;
        std::atomic<bool> is_active_;
        
        // Buffer management
        std::vector<uint8_t> read_buffer_;
        std::vector<uint8_t> write_buffer_;
        size_t read_position_;
        size_t write_position_;
        
    public:
        Connection(int socket_fd, const NetworkAddress& client_addr, 
                  const NetworkAddress& server_addr);
        ~Connection();
        
        bool setup_ssl(SSLContext* ssl_context);
        
        // I/O operations
        ssize_t read_data(void* buffer, size_t size);
        ssize_t write_data(const void* buffer, size_t size);
        bool read_http_request(HttpRequest& request);
        bool write_http_response(const HttpResponse& response);
        
        // Connection management
        void close();
        bool is_alive() const;
        bool should_keep_alive() const;
        void update_activity();
        
        // Getters
        int get_socket() const { return socket_fd_; }
        const NetworkAddress& get_client_address() const { return client_address_; }
        bool is_ssl_connection() const { return ssl_connection_ != nullptr; }
        std::chrono::system_clock::time_point get_last_activity() const { return last_activity_; }
        
    private:
        bool parse_request_line(const std::string& line, HttpRequest& request);
        bool parse_headers(const std::vector<std::string>& header_lines, HttpRequest& request);
    };

    /**
     * Main MedusaServ Engine
     */
    class MedusaServEngine {
    private:
        // Server configuration
        std::string server_name_;
        std::string server_version_;
        std::vector<std::unique_ptr<VirtualHost>> virtual_hosts_;
        std::unordered_map<std::string, std::shared_ptr<SSLContext>> ssl_contexts_;
        
        // Network management
        std::vector<int> listening_sockets_;
        std::vector<std::thread> worker_threads_;
        std::atomic<bool> server_running_;
        size_t worker_thread_count_;
        
        // Connection management
        std::vector<std::unique_ptr<Connection>> active_connections_;
        std::mutex connections_mutex_;
        std::thread connection_cleanup_thread_;
        
        // Security and database integration
        std::shared_ptr<medusa::security::SecurityEngine> security_engine_;
        std::shared_ptr<medusa::database::TriforceConnector> database_connector_;
        
        // Performance monitoring
        std::atomic<uint64_t> requests_served_;
        std::atomic<uint64_t> bytes_transferred_;
        std::chrono::system_clock::time_point server_start_time_;
        
        // Configuration hot-reload
        std::string config_file_path_;
        std::chrono::system_clock::time_point config_last_modified_;
        std::thread config_watcher_thread_;
        
    public:
        MedusaServEngine(const std::string& config_file = "/home/medusa/medusaserv.conf");
        ~MedusaServEngine();
        
        // Core server operations
        bool initialize();
        bool start();
        void stop();
        bool is_running() const { return server_running_.load(); }
        
        // Configuration management
        bool load_configuration(const std::string& config_file);
        bool reload_configuration();
        VirtualHost* add_virtual_host(const std::string& server_name, 
                                     const NetworkAddress& listen_addr);
        
        // SSL management
        bool add_ssl_context(const std::string& name, const std::string& cert_file,
                           const std::string& key_file, const std::string& ca_file = "");
        
        // Integration setup
        void set_security_engine(std::shared_ptr<medusa::security::SecurityEngine> engine) {
            security_engine_ = engine;
        }
        
        void set_database_connector(std::shared_ptr<medusa::database::TriforceConnector> connector) {
            database_connector_ = connector;
        }
        
        // Statistics and monitoring
        struct ServerStats {
            uint64_t requests_served;
            uint64_t bytes_transferred;
            uint64_t active_connections;
            std::chrono::seconds uptime;
            double requests_per_second;
            double bytes_per_second;
        };
        
        ServerStats get_server_stats() const;
        void log_request(const HttpRequest& request, const HttpResponse& response,
                        std::chrono::milliseconds processing_time);
        
    private:
        // Server initialization
        bool create_listening_sockets();
        bool setup_ssl_contexts();
        void start_worker_threads();
        
        // Request processing
        void worker_thread_main(size_t thread_id);
        void handle_connection(std::unique_ptr<Connection> connection);
        HttpResponse process_request(const HttpRequest& request);
        HttpResponse serve_static_file(const VirtualHost::Location& location, 
                                     const std::string& file_path);
        HttpResponse proxy_request(const VirtualHost::Location& location, 
                                 const HttpRequest& request);
        
        // Security integration
        bool validate_request_security(const HttpRequest& request);
        void log_security_event(const HttpRequest& request, const std::string& event_type);
        
        // Configuration management
        void config_watcher_main();
        bool parse_configuration_file(const std::string& config_file);
        
        // Connection cleanup
        void connection_cleanup_main();
        void cleanup_inactive_connections();
        
        // Utility methods
        VirtualHost* find_virtual_host(const std::string& host, const std::string& path) const;
        std::string generate_server_signature() const;
        void setup_default_headers(HttpResponse& response) const;
    };

} // namespace medusa::core