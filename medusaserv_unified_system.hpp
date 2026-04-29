#pragma once

/**
 * MEDUSASERV UNIFIED SYSTEM v0.2.7
 * ================================
 * 
 * The BASE MEDUSASERV - Ties up ALL loose ends!
 * 
 * Integrates:
 * ✅ MedusaServ Engine (NGINX replacement)
 * ✅ Synciecorn Engine (Bidirectional sync)
 * ✅ RoyaleGate API (Intelligent routing)
 * ✅ Version Manager (Hash verification + NAS backup)
 * ✅ Triforce Connector (Database foundation)
 * ✅ ICEWALL Integration (Security monitoring)
 * ✅ Settings Permission System (Hierarchical access)
 * ✅ Encrypted Vault (Secure credentials)
 * 
 * Yorkshire Champion Gold Standard - Complete Server Solution
 * 
 * Author: Claude Code (Yorkshire Champion)
 * Purpose: Unified server system replacing NGINX + Next.js + all external dependencies
 */

#include "medusa_serv_engine.hpp"
#include "medusa_version_manager.hpp"
#include "../sync/synciecorn_engine.hpp"
#include "../routing/roylegate_api_engine.hpp"
#include "../database/medusa_triforce_connector.hpp"
#include "../security/medusa_encrypted_vault.hpp"
#include "../settings/settings_permission_manager.hpp"
#include "../build/build_push_pipeline.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <atomic>
#include <thread>
#include <chrono>

namespace medusa::core {

    /**
     * NAS Backup Integration with Hash Verification
     */
    struct NASBackupConfig {
        std::string nas_hostname = "192.168.1.168";
        std::string nas_username = "medusa";
        std::string nas_password;  // From encrypted vault
        std::string nas_backup_path = "/volume1/medusa_backups";
        std::string nas_protocol = "smb";  // smb, nfs, ftp
        
        // Hash verification settings
        bool verify_hashes = true;
        std::string hash_algorithm = "sha256";
        bool store_hash_manifest = true;
        std::string manifest_file = "backup_manifest.json";
        
        // Backup retention
        size_t max_backups_per_target = 10;
        std::chrono::hours backup_retention_hours{720}; // 30 days
        bool compress_backups = true;
        
        NASBackupConfig() = default;
    };

    /**
     * Hash Verification System
     */
    class HashVerificationSystem {
    private:
        std::string hash_algorithm_;
        
    public:
        HashVerificationSystem(const std::string& algorithm = "sha256") 
            : hash_algorithm_(algorithm) {}
        
        struct HashResult {
            std::string file_path;
            std::string hash_before;
            std::string hash_after;
            bool verification_passed;
            size_t file_size;
            std::chrono::system_clock::time_point timestamp;
        };
        
        // Hash calculation
        std::string calculate_file_hash(const std::string& file_path);
        std::string calculate_directory_hash(const std::string& directory_path);
        std::unordered_map<std::string, std::string> calculate_directory_manifest(const std::string& directory_path);
        
        // Verification operations
        HashResult verify_file_integrity(const std::string& file_path, const std::string& expected_hash);
        std::vector<HashResult> verify_directory_integrity(const std::string& directory_path, 
                                                          const std::unordered_map<std::string, std::string>& expected_hashes);
        
        // Backup hash management
        bool create_hash_manifest(const std::string& directory_path, const std::string& manifest_file);
        bool verify_against_manifest(const std::string& directory_path, const std::string& manifest_file);
        std::unordered_map<std::string, std::string> load_hash_manifest(const std::string& manifest_file);
        
    private:
        std::string calculate_sha256(const std::string& data);
        std::string calculate_md5(const std::string& data);
        std::string read_file_content(const std::string& file_path);
    };

    /**
     * NAS Backup Manager with Hash Verification
     */
    class NASBackupManager {
    private:
        NASBackupConfig config_;
        std::unique_ptr<HashVerificationSystem> hash_verifier_;
        std::shared_ptr<medusa::security::MedusaEncryptedVault> vault_;
        
        // NAS connection state
        bool nas_connected_;
        std::string nas_mount_point_;
        std::thread backup_monitor_thread_;
        std::atomic<bool> backup_monitoring_active_;
        
    public:
        NASBackupManager(const NASBackupConfig& config,
                        std::shared_ptr<medusa::security::MedusaEncryptedVault> vault);
        ~NASBackupManager();
        
        // NAS connection
        bool connect_to_nas();
        void disconnect_from_nas();
        bool is_nas_connected() const { return nas_connected_; }
        bool test_nas_connectivity();
        
        struct BackupResult {
            bool success;
            std::string backup_file_path;
            std::string nas_backup_path;
            std::string error_message;
            
            // Hash verification results
            std::unordered_map<std::string, std::string> file_hashes_before;
            std::unordered_map<std::string, std::string> file_hashes_after;
            bool hash_verification_passed;
            std::vector<std::string> hash_mismatches;
            
            // Backup statistics
            size_t files_backed_up;
            size_t total_bytes;
            std::chrono::milliseconds backup_duration;
            std::chrono::system_clock::time_point backup_timestamp;
        };
        
        // Backup operations with hash verification
        BackupResult backup_directory_to_nas(const std::string& source_directory,
                                            const std::string& target_name);
        BackupResult backup_file_to_nas(const std::string& source_file,
                                       const std::string& target_name);
        
        // Restore operations with verification
        BackupResult restore_from_nas(const std::string& nas_backup_path,
                                     const std::string& local_restore_path);
        
        // Backup management
        std::vector<std::string> list_nas_backups(const std::string& target_name = "");
        bool delete_nas_backup(const std::string& backup_path);
        bool cleanup_old_nas_backups(const std::string& target_name);
        
        // Hash verification
        bool verify_nas_backup_integrity(const std::string& nas_backup_path);
        std::vector<HashVerificationSystem::HashResult> get_backup_hash_report(const std::string& backup_path);
        
        // Monitoring
        void start_backup_monitoring();
        void stop_backup_monitoring();
        
    private:
        // NAS operations
        bool mount_nas_share();
        bool unmount_nas_share();
        std::string get_nas_credentials();
        
        // Backup helpers
        bool create_compressed_backup(const std::string& source_path, const std::string& backup_file);
        bool extract_compressed_backup(const std::string& backup_file, const std::string& extract_path);
        std::string generate_backup_filename(const std::string& target_name);
        
        // Monitoring worker
        void backup_monitor_worker();
    };

    /**
     * Unified System Configuration
     */
    struct MedusaUnifiedConfig {
        // Core system settings
        std::string server_name = "MedusaServ-Unified";
        std::string version = "0.2.7";
        std::string config_directory = "/home/medusa/config";
        std::string logs_directory = "/home/medusa/logs";
        std::string data_directory = "/home/medusa/data";
        
        // Network settings
        std::string primary_domain = "poweredbymedusa.com";
        std::vector<std::string> subdomains = {"portal", "community", "store", "cad", "api"};
        std::string ssl_certificate_path = "/home/medusa/ssl";
        bool enable_http2 = true;
        bool enable_ipv6 = true;
        
        // Integration settings
        bool enable_synciecorn = true;
        bool enable_roylegate_routing = true;
        bool enable_version_management = true;
        bool enable_nas_backup = true;
        bool enable_icewall_security = true;
        bool enable_ai_command_integration = true;
        
        // Security settings
        std::string vault_path = "/home/medusa/.medusa_vault";
        std::string vault_master_key_id = "unified_system";
        bool require_2fa_for_admin = true;
        bool encrypt_all_communications = true;
        
        // Performance settings
        size_t worker_threads = 0;  // 0 = auto-detect
        size_t max_connections = 10000;
        std::chrono::seconds connection_timeout{30};
        bool enable_compression = true;
        bool enable_caching = true;
        
        // Backup settings
        NASBackupConfig nas_backup;
        bool auto_backup_on_deployment = true;
        std::chrono::hours backup_interval{24};
    };

    /**
     * System Health Status
     */
    struct SystemHealthStatus {
        // Overall status
        enum class HealthLevel { EXCELLENT, GOOD, WARNING, CRITICAL, OFFLINE };
        HealthLevel overall_health;
        std::string status_message;
        
        // Component health
        std::unordered_map<std::string, HealthLevel> component_health;
        std::unordered_map<std::string, std::string> component_messages;
        
        // System metrics
        double cpu_usage_percent;
        double memory_usage_percent;
        double disk_usage_percent;
        size_t active_connections;
        double requests_per_second;
        
        // Database health
        bool triforce_healthy;
        std::chrono::milliseconds database_response_time;
        
        // Backup health
        bool nas_accessible;
        std::chrono::system_clock::time_point last_backup;
        bool last_backup_verified;
        
        // Security status
        bool icewall_active;
        size_t current_threat_level;
        std::vector<std::string> active_alerts;
        
        SystemHealthStatus() : overall_health(HealthLevel::OFFLINE), cpu_usage_percent(0.0),
                              memory_usage_percent(0.0), disk_usage_percent(0.0),
                              active_connections(0), requests_per_second(0.0),
                              triforce_healthy(false), nas_accessible(false),
                              last_backup_verified(false), icewall_active(false),
                              current_threat_level(0) {}
    };

    /**
     * MAIN MEDUSASERV UNIFIED SYSTEM
     */
    class MedusaServUnifiedSystem {
    private:
        MedusaUnifiedConfig config_;
        SystemHealthStatus health_status_;
        
        // Core components - THE INTEGRATION POINT
        std::unique_ptr<MedusaServEngine> web_server_;
        std::unique_ptr<medusa::sync::SynciecornEngine> synciecorn_;
        std::unique_ptr<RoyaleGateAPIEngine> roylegate_api_;
        std::unique_ptr<MedusaVersionManager> version_manager_;
        std::unique_ptr<NASBackupManager> nas_backup_;
        
        // Foundation systems
        std::shared_ptr<medusa::database::TriforceConnector> triforce_;
        std::shared_ptr<medusa::security::MedusaEncryptedVault> vault_;
        std::shared_ptr<medusa::settings::SettingsPermissionManager> settings_;
        std::shared_ptr<medusa::build::MedusaBuildPushPipeline> build_pipeline_;
        
        // System control
        std::atomic<bool> system_running_;
        std::atomic<bool> initialization_complete_;
        std::thread health_monitor_thread_;
        std::thread backup_scheduler_thread_;
        
        // Statistics
        std::chrono::system_clock::time_point system_start_time_;
        std::atomic<uint64_t> total_requests_served_;
        std::atomic<uint64_t> total_bytes_transferred_;
        
    public:
        MedusaServUnifiedSystem(const MedusaUnifiedConfig& config = MedusaUnifiedConfig{});
        ~MedusaServUnifiedSystem();
        
        // System lifecycle
        bool initialize();
        bool start_system();
        void stop_system();
        bool restart_system();
        
        bool is_running() const { return system_running_.load(); }
        bool is_initialized() const { return initialization_complete_.load(); }
        
        // Configuration management
        bool load_configuration(const std::string& config_file);
        bool save_configuration(const std::string& config_file);
        void update_configuration(const MedusaUnifiedConfig& new_config);
        const MedusaUnifiedConfig& get_configuration() const { return config_; }
        
        // System health and monitoring
        const SystemHealthStatus& get_health_status() const { return health_status_; }
        std::string get_system_status_json();
        std::vector<std::string> get_system_logs(size_t limit = 100);
        
        // Component access
        MedusaServEngine* get_web_server() { return web_server_.get(); }
        medusa::sync::SynciecornEngine* get_synciecorn() { return synciecorn_.get(); }
        RoyaleGateAPIEngine* get_roylegate_api() { return roylegate_api_.get(); }
        MedusaVersionManager* get_version_manager() { return version_manager_.get(); }
        NASBackupManager* get_nas_backup() { return nas_backup_.get(); }
        
        // Quick operations
        bool quick_deploy_from_wsl();
        bool quick_backup_all_targets();
        bool quick_restart_services();
        bool quick_sync_bidirectional();
        
        // Emergency operations
        bool emergency_stop();
        bool emergency_backup();
        bool emergency_rollback();
        
        // Settings integration
        std::string handle_settings_request(const std::string& user_id,
                                           const std::string& settings_path,
                                           const std::unordered_map<std::string, std::string>& params = {});
        
        // Yorkshire Champion utilities
        std::string get_yorkshire_system_status();
        void log_yorkshire_system_event(const std::string& event, const std::string& details = "");
        
        // Statistics
        struct SystemStatistics {
            std::chrono::seconds uptime;
            uint64_t total_requests;
            uint64_t total_bytes_transferred;
            double average_response_time_ms;
            double requests_per_second;
            size_t current_connections;
            size_t total_deployments;
            size_t total_backups;
            bool all_services_healthy;
        };
        
        SystemStatistics get_system_statistics();
        
    private:
        // Initialization phases
        bool phase_1_initialize_foundation();  // Vault, Database, Settings
        bool phase_2_initialize_core();        // Web Server, Synciecorn
        bool phase_3_initialize_routing();     // RoyaleGate API
        bool phase_4_initialize_management();  // Version Manager, NAS Backup
        bool phase_5_start_services();         // Start all services
        bool phase_6_verify_integration();     // Verify everything works together
        
        // Component initialization
        bool initialize_encrypted_vault();
        bool initialize_triforce_database();
        bool initialize_settings_system();
        bool initialize_web_server();
        bool initialize_synciecorn();
        bool initialize_roylegate_api();
        bool initialize_version_manager();
        bool initialize_nas_backup();
        bool initialize_build_pipeline();
        
        // Health monitoring
        void health_monitor_worker();
        void update_system_health();
        void check_component_health();
        void check_database_health();
        void check_nas_health();
        void check_security_health();
        
        // Backup scheduling
        void backup_scheduler_worker();
        void schedule_automatic_backup();
        
        // Integration verification
        bool verify_database_integration();
        bool verify_routing_integration();
        bool verify_sync_integration();
        bool verify_backup_integration();
        
        // Error handling
        void handle_component_failure(const std::string& component, const std::string& error);
        bool attempt_component_recovery(const std::string& component);
        void trigger_emergency_procedures(const std::string& reason);
        
        // Configuration helpers
        bool validate_configuration();
        bool apply_configuration_changes();
        void setup_default_configuration();
    };

    // Global unified system instance
    extern std::unique_ptr<MedusaServUnifiedSystem> g_unified_system;
    
    // System management functions
    bool initialize_medusaserv_unified();
    void shutdown_medusaserv_unified();
    MedusaServUnifiedSystem* get_unified_system();
    
    // Quick access functions for common operations
    bool unified_deploy_and_restart();
    bool unified_backup_and_verify();
    bool unified_sync_and_deploy();
    SystemHealthStatus unified_get_health_status();

} // namespace medusa::core