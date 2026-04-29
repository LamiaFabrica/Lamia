/*
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * MEDUSA REPOSITORY MANAGER v0.3.0c - Yorkshire Champion Standards
 * ================================================================
 * 
 * Native C++ repository management system for library check-out/check-in
 * and MedusaServ registration with Triforce database architecture.
 * 
 * Ground-up C++ implementation - NO PHP MEEGER LANGUAGES!
 * Military-Grade Security with Yorkshire Champion certification.
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <chrono>
#include <functional>
#include <mutex>
#include <atomic>
#include <queue>
#include <thread>
#include <future>

// PostgreSQL C++ driver
#include <pqxx/pqxx>

// JSON support
#include <nlohmann/json.hpp>

// Encryption and security
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

namespace MedusaRepository {

    // Forward declarations
    class DatabaseConnection;
    class TriforceManager;
    class RepositoryManager;
    class LibraryCheckoutSystem;
    class MedusaServRegistry;

    // Enumerations
    enum class LibraryType {
        CORE,
        EXTENSION,
        SECURITY,
        DATABASE,
        NETWORK,
        CUSTOM
    };

    enum class SecurityLevel {
        STANDARD,
        ENHANCED,
        MILITARY_GRADE,
        YORKSHIRE_CHAMPION
    };

    enum class InstallationType {
        DEVELOPMENT,
        PRODUCTION,
        TESTING,
        ENTERPRISE
    };

    enum class CheckoutStatus {
        CHECKED_OUT,
        CHECKED_IN,
        OVERDUE,
        LOST,
        FORCE_RETURNED
    };

    enum class TriforceNode {
        WISDOM,    // Repository Master - .54
        POWER,     // Performance Engine - .155
        COURAGE    // Security Fortress - .28
    };

    // Data structures
    struct MedusaServRegistration {
        std::string registration_id;
        std::string user_name;
        std::string user_email;
        std::string organization;
        std::string medusaserv_version;
        InstallationType installation_type;
        std::string server_ip;
        std::string server_domain;
        std::string license_type;
        std::chrono::system_clock::time_point registration_date;
        std::chrono::system_clock::time_point last_checkin;
        std::string status;
        std::string api_key;
        std::string verification_token;
        bool verified;
        nlohmann::json metadata;
    };

    struct RepositoryLibrary {
        std::string library_id;
        std::string library_name;
        LibraryType library_type;
        std::string version;
        std::string file_path;
        size_t file_size;
        std::string file_hash;
        std::string description;
        std::vector<std::string> dependencies;
        std::string compile_flags;
        std::string link_flags;
        std::vector<std::string> headers;
        std::vector<std::string> source_files;
        nlohmann::json compatibility;
        bool yorkshire_certified;
        SecurityLevel security_level;
        std::chrono::system_clock::time_point upload_date;
        std::string uploader_id;
        int download_count;
        double rating;
        std::string status;
        nlohmann::json metadata;
    };

    struct LibraryCheckout {
        std::string checkout_id;
        std::string library_id;
        std::string registration_id;
        std::string checkout_type;
        std::chrono::system_clock::time_point checkout_date;
        std::chrono::system_clock::time_point expected_checkin;
        std::chrono::system_clock::time_point actual_checkin;
        CheckoutStatus status;
        std::string checkout_reason;
        std::vector<std::string> modifications_made;
        std::string checkin_notes;
        std::string ip_address;
        std::string user_agent;
    };

    struct TriforceNodeInfo {
        std::string node_id;
        std::string node_name;
        TriforceNode node_type;
        std::string node_status;
        std::string connection_string;
        std::chrono::system_clock::time_point last_sync;
        std::string sync_status;
        double health_score;
        nlohmann::json metadata;
    };

    // Database Connection Manager
    class DatabaseConnection {
    private:
        std::unique_ptr<pqxx::connection> conn;
        std::string connection_string;
        std::mutex conn_mutex;
        std::atomic<bool> connected;
        
    public:
        DatabaseConnection(const std::string& host, const std::string& port, 
                          const std::string& dbname, const std::string& user, 
                          const std::string& password);
        ~DatabaseConnection();
        
        bool connect();
        void disconnect();
        bool is_connected() const { return connected.load(); }
        
        // Execute queries with automatic retry
        pqxx::result execute_query(const std::string& query);
        pqxx::result execute_prepared(const std::string& statement_name, 
                                     const std::vector<std::string>& params);
        
        // Transaction support
        std::unique_ptr<pqxx::work> begin_transaction();
        void commit_transaction(std::unique_ptr<pqxx::work>& txn);
        void rollback_transaction(std::unique_ptr<pqxx::work>& txn);
        
        // Connection health
        bool test_connection();
        void reconnect_if_needed();
    };

    // Triforce Database Architecture Manager
    class TriforceManager {
    private:
        std::unordered_map<TriforceNode, std::unique_ptr<DatabaseConnection>> nodes;
        std::mutex sync_mutex;
        std::atomic<bool> sync_in_progress;
        std::thread sync_thread;
        std::queue<std::function<void()>> sync_queue;
        
        // Node configuration
        struct NodeConfig {
            std::string host;
            std::string port;
            std::string dbname;
            std::string user;
            std::string password;
        };
        
        std::unordered_map<TriforceNode, NodeConfig> node_configs;
        
    public:
        TriforceManager();
        ~TriforceManager();
        
        // Node management
        bool initialize_nodes();
        bool add_node(TriforceNode node_type, const NodeConfig& config);
        bool remove_node(TriforceNode node_type);
        
        // Database operations
        DatabaseConnection* get_node(TriforceNode node_type);
        DatabaseConnection* get_primary_node(); // Wisdom node
        
        // Synchronization
        void start_sync_engine();
        void stop_sync_engine();
        bool synchronize_nodes();
        bool synchronize_table(const std::string& table_name);
        
        // Health monitoring
        std::vector<TriforceNodeInfo> get_node_health();
        double calculate_overall_health();
        bool is_cluster_healthy();
        
        // Failover support
        bool failover_to_node(TriforceNode target_node);
        bool restore_primary_node();
    };

    // MedusaServ Registration System
    class MedusaServRegistry {
    private:
        std::shared_ptr<TriforceManager> triforce;
        std::mutex registry_mutex;
        std::unordered_map<std::string, MedusaServRegistration> active_registrations;
        
        // API key generation
        std::string generate_api_key();
        std::string generate_verification_token();
        std::string hash_password(const std::string& password);
        
    public:
        MedusaServRegistry(std::shared_ptr<TriforceManager> triforce_mgr);
        ~MedusaServRegistry();
        
        // Registration management
        std::string register_medusaserv(const std::string& user_name,
                                       const std::string& user_email,
                                       const std::string& organization,
                                       const std::string& medusaserv_version,
                                       InstallationType installation_type,
                                       const std::string& server_ip);
        
        bool verify_registration(const std::string& registration_id, 
                               const std::string& verification_token);
        bool update_registration(const std::string& registration_id, 
                               const MedusaServRegistration& updated_info);
        bool deactivate_registration(const std::string& registration_id);
        
        // Authentication
        bool authenticate_api_key(const std::string& api_key);
        MedusaServRegistration get_registration_by_api_key(const std::string& api_key);
        MedusaServRegistration get_registration_by_id(const std::string& registration_id);
        
        // Statistics
        std::vector<MedusaServRegistration> get_active_registrations();
        int get_registration_count_by_type(InstallationType type);
        std::vector<std::string> get_recent_registrations(int days = 7);
        
        // Maintenance
        void update_last_checkin(const std::string& registration_id);
        void cleanup_expired_registrations();
        void send_notifications();
    };

    // Library Checkout System
    class LibraryCheckoutSystem {
    private:
        std::shared_ptr<TriforceManager> triforce;
        std::shared_ptr<MedusaServRegistry> registry;
        std::mutex checkout_mutex;
        std::unordered_map<std::string, LibraryCheckout> active_checkouts;
        
        // Checkout validation
        bool validate_checkout_request(const std::string& library_id,
                                     const std::string& registration_id);
        std::string generate_checkout_id();
        
    public:
        LibraryCheckoutSystem(std::shared_ptr<TriforceManager> triforce_mgr,
                             std::shared_ptr<MedusaServRegistry> registry_mgr);
        ~LibraryCheckoutSystem();
        
        // Checkout operations
        std::string checkout_library(const std::string& library_id,
                                   const std::string& registration_id,
                                   const std::string& checkout_type,
                                   const std::string& reason,
                                   int expected_return_hours = 24);
        
        bool checkin_library(const std::string& checkout_id,
                           const std::vector<std::string>& modifications_made,
                           const std::string& checkin_notes);
        
        bool force_checkin(const std::string& checkout_id, 
                         const std::string& admin_reason);
        
        // Checkout management
        std::vector<LibraryCheckout> get_active_checkouts();
        std::vector<LibraryCheckout> get_overdue_checkouts();
        std::vector<LibraryCheckout> get_checkout_history(const std::string& registration_id);
        
        LibraryCheckout get_checkout_by_id(const std::string& checkout_id);
        bool extend_checkout(const std::string& checkout_id, int additional_hours);
        
        // Monitoring and alerts
        void check_overdue_items();
        void send_reminder_notifications();
        void generate_checkout_reports();
        
        // Statistics
        int get_total_active_checkouts();
        int get_checkouts_by_user(const std::string& registration_id);
        std::vector<std::pair<std::string, int>> get_popular_libraries();
    };

    // Main Repository Manager
    class RepositoryManager {
    private:
        std::shared_ptr<TriforceManager> triforce;
        std::shared_ptr<MedusaServRegistry> registry;
        std::shared_ptr<LibraryCheckoutSystem> checkout_system;
        
        std::mutex library_mutex;
        std::unordered_map<std::string, RepositoryLibrary> libraries;
        
        // Library management
        std::string generate_library_id();
        std::string calculate_file_hash(const std::string& file_path);
        bool validate_library_upload(const RepositoryLibrary& library);
        
    public:
        RepositoryManager();
        ~RepositoryManager();
        
        // Initialization
        bool initialize();
        bool setup_database_schema();
        void shutdown();
        
        // Library management
        std::string add_library(const RepositoryLibrary& library);
        bool update_library(const std::string& library_id, 
                          const RepositoryLibrary& updated_library);
        bool remove_library(const std::string& library_id);
        
        RepositoryLibrary get_library(const std::string& library_id);
        std::vector<RepositoryLibrary> search_libraries(const std::string& query);
        std::vector<RepositoryLibrary> get_libraries_by_type(LibraryType type);
        std::vector<RepositoryLibrary> get_yorkshire_certified_libraries();
        
        // Version management
        bool create_library_version(const std::string& library_id,
                                  const std::string& new_version,
                                  const std::string& change_summary);
        std::vector<std::string> get_library_versions(const std::string& library_id);
        bool promote_library_version(const std::string& library_id,
                                   const std::string& version);
        
        // Access control
        bool has_library_access(const std::string& registration_id,
                              const std::string& library_id);
        bool grant_library_access(const std::string& registration_id,
                                const std::string& library_id);
        bool revoke_library_access(const std::string& registration_id,
                                 const std::string& library_id);
        
        // Statistics and monitoring
        nlohmann::json get_repository_statistics();
        nlohmann::json get_health_report();
        void generate_usage_reports();
        
        // Component access
        std::shared_ptr<TriforceManager> get_triforce_manager() { return triforce; }
        std::shared_ptr<MedusaServRegistry> get_registry() { return registry; }
        std::shared_ptr<LibraryCheckoutSystem> get_checkout_system() { return checkout_system; }
    };

    // Utility functions
    namespace Utils {
        std::string enum_to_string(LibraryType type);
        std::string enum_to_string(SecurityLevel level);
        std::string enum_to_string(InstallationType type);
        std::string enum_to_string(CheckoutStatus status);
        std::string enum_to_string(TriforceNode node);
        
        LibraryType string_to_library_type(const std::string& str);
        SecurityLevel string_to_security_level(const std::string& str);
        InstallationType string_to_installation_type(const std::string& str);
        CheckoutStatus string_to_checkout_status(const std::string& str);
        TriforceNode string_to_triforce_node(const std::string& str);
        
        std::string generate_uuid();
        std::string current_timestamp_iso();
        std::string encrypt_data(const std::string& data, const std::string& key);
        std::string decrypt_data(const std::string& encrypted_data, const std::string& key);
        
        bool validate_email(const std::string& email);
        bool validate_ip_address(const std::string& ip);
        bool validate_version_string(const std::string& version);
        
        // Yorkshire Champion validation
        bool validate_yorkshire_standards(const RepositoryLibrary& library);
        bool verify_military_grade_security(const std::string& library_path);
        double calculate_yorkshire_score(const RepositoryLibrary& library);
    }

} // namespace MedusaRepository