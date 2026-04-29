#pragma once

/**
 * MEDUSA VERSION MANAGER & DEPLOYMENT SYSTEM v0.2.7
 * ==================================================
 * 
 * Integrated version control, build management, and deployment system
 * Based on your existing Python deployment_automation.py system
 * 
 * Features:
 * - Version control with automatic backups
 * - Zip deployment with extraction
 * - Startup script management
 * - Service control integration
 * - Deployment history tracking
 * - Rollback capabilities
 * 
 * Yorkshire Champion Gold Standard - Underpinning the Web Server
 * 
 * Author: Claude Code (Yorkshire Champion)
 * Purpose: Core deployment system underpinning MedusaServ
 */

#include "../database/medusa_triforce_connector.hpp"
#include "../sync/synciecorn_engine.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <filesystem>
#include <functional>

namespace medusa::core {

    /**
     * Version Information
     */
    struct VersionInfo {
        std::string version_id;
        std::string version_number;  // e.g., "0.2.7"
        std::string build_date;
        std::string build_hash;
        std::string build_target;    // "frontend", "backend", "api", etc.
        std::string deployed_by;
        std::chrono::system_clock::time_point deployment_time;
        
        // File information
        std::string source_zip_file;
        std::string backup_file;
        std::vector<std::string> deployed_files;
        size_t total_files;
        size_t total_size_bytes;
        
        // Status
        bool deployment_successful;
        bool has_backup;
        bool startup_script_executed;
        bool services_restarted;
        
        // Deployment options used
        std::unordered_map<std::string, std::string> deployment_options;
        std::vector<std::string> deployment_log;
        
        VersionInfo() : total_files(0), total_size_bytes(0), 
                       deployment_successful(false), has_backup(false),
                       startup_script_executed(false), services_restarted(false) {}
    };

    /**
     * Deployment Target Configuration
     */
    struct DeploymentTarget {
        std::string target_name;       // "frontend", "backend", "api"
        std::string target_path;       // "/home/medusa/frontend"
        std::string backup_path;       // "/home/medusa/backups"
        std::string temp_path;         // "/tmp/deployments"
        
        // Service management
        std::string service_name;      // "nginx", "medusaserv", etc.
        std::string startup_script;   // "startup.sh"
        std::string sudo_user;        // "medusa"
        
        // Permissions
        std::string file_owner;       // "medusa:medusa"
        std::string file_permissions; // "755"
        std::string dir_permissions;  // "755"
        
        DeploymentTarget() : sudo_user("medusa"), file_owner("medusa:medusa"),
                           file_permissions("755"), dir_permissions("755") {}
    };

    /**
     * Deployment Configuration
     */
    struct DeploymentConfig {
        std::unordered_map<std::string, DeploymentTarget> targets;
        
        // Default paths
        std::string base_backup_path = "/home/medusa/backups";
        std::string base_temp_path = "/tmp/medusa_deployments";
        std::string deployment_log_path = "/home/medusa/logs/deployments.log";
        
        // Default settings
        bool auto_backup = true;
        bool auto_restart_services = true;
        bool verify_checksums = true;
        bool compress_backups = true;
        size_t max_backup_history = 10;
        
        DeploymentConfig() {
            // Default targets based on your system
            targets["frontend"] = DeploymentTarget{
                .target_name = "frontend",
                .target_path = "/home/medusa/frontend",
                .backup_path = "/home/medusa/backups/frontend",
                .temp_path = "/tmp/medusa_deployments/frontend",
                .service_name = "nginx",
                .startup_script = "startup.sh"
            };
            
            targets["backend"] = DeploymentTarget{
                .target_name = "backend", 
                .target_path = "/home/medusa/backend",
                .backup_path = "/home/medusa/backups/backend",
                .temp_path = "/tmp/medusa_deployments/backend",
                .service_name = "medusaserv",
                .startup_script = "startup.sh"
            };
            
            targets["api"] = DeploymentTarget{
                .target_name = "api",
                .target_path = "/home/medusa/api", 
                .backup_path = "/home/medusa/backups/api",
                .temp_path = "/tmp/medusa_deployments/api",
                .service_name = "roylegate_api",
                .startup_script = "startup.sh"
            };
            
            targets["database"] = DeploymentTarget{
                .target_name = "database",
                .target_path = "/home/medusa/database",
                .backup_path = "/home/medusa/backups/database", 
                .temp_path = "/tmp/medusa_deployments/database",
                .service_name = "triforce_connector",
                .startup_script = "startup.sh"
            };
        }
    };

    /**
     * Deployment Result
     */
    struct DeploymentResult {
        bool success;
        std::string deployment_id;
        std::string error_message;
        std::chrono::milliseconds deployment_time;
        
        // Deployment details
        VersionInfo version_info;
        std::vector<std::string> files_deployed;
        std::vector<std::string> services_restarted;
        std::vector<std::string> warnings;
        std::vector<std::string> errors;
        
        // Backup information
        std::string backup_file;
        bool backup_created;
        
        // Rollback information
        bool rollback_available;
        std::string rollback_version_id;
        
        DeploymentResult() : success(false), backup_created(false), 
                           rollback_available(false) {}
    };

    /**
     * Service Controller - Manages system services
     */
    class ServiceController {
    private:
        DeploymentConfig config_;
        
    public:
        ServiceController(const DeploymentConfig& config) : config_(config) {}
        
        // Service control
        bool start_service(const std::string& target);
        bool stop_service(const std::string& target);
        bool restart_service(const std::string& target);
        bool is_service_running(const std::string& target);
        std::string get_service_status(const std::string& target);
        
        // Service management
        bool enable_service(const std::string& target);
        bool disable_service(const std::string& target);
        bool reload_service_config(const std::string& target);
        
        // Multiple services
        bool restart_all_services();
        std::vector<std::string> get_all_service_statuses();
        
    private:
        std::string execute_service_command(const std::string& command);
        bool validate_service_name(const std::string& service_name);
    };

    /**
     * Backup Manager - Handles backup creation and restoration
     */
    class BackupManager {
    private:
        DeploymentConfig config_;
        
    public:
        BackupManager(const DeploymentConfig& config) : config_(config) {}
        
        // Backup operations
        std::string create_backup(const std::string& target);
        bool restore_from_backup(const std::string& backup_file, const std::string& target);
        bool verify_backup(const std::string& backup_file);
        
        // Backup management
        std::vector<std::string> list_backups(const std::string& target);
        bool delete_backup(const std::string& backup_file);
        bool cleanup_old_backups(const std::string& target);
        
        // Backup information
        struct BackupInfo {
            std::string backup_file;
            std::string target;
            std::chrono::system_clock::time_point created;
            size_t file_size;
            std::string checksum;
        };
        
        BackupInfo get_backup_info(const std::string& backup_file);
        std::vector<BackupInfo> get_backup_history(const std::string& target);
        
    private:
        std::string generate_backup_filename(const std::string& target);
        std::string calculate_checksum(const std::string& file_path);
        bool compress_directory(const std::string& source_dir, const std::string& backup_file);
        bool extract_backup(const std::string& backup_file, const std::string& target_dir);
    };

    /**
     * Zip Deployment Handler - Handles zip file deployments
     */
    class ZipDeploymentHandler {
    private:
        DeploymentConfig config_;
        
    public:
        ZipDeploymentHandler(const DeploymentConfig& config) : config_(config) {}
        
        // Zip operations
        bool extract_zip_file(const std::string& zip_file, const std::string& extract_dir);
        bool validate_zip_file(const std::string& zip_file);
        std::vector<std::string> list_zip_contents(const std::string& zip_file);
        
        // Deployment operations
        bool deploy_zip_package(const std::string& zip_file, const std::string& target);
        bool deploy_files_to_target(const std::string& source_dir, const std::string& target);
        
        // File operations
        bool set_permissions(const std::string& target_path);
        bool verify_deployed_files(const std::vector<std::string>& expected_files, const std::string& target_path);
        
        // Startup script handling
        bool execute_startup_script(const std::string& startup_script_path, const std::string& target);
        bool find_and_execute_startup_script(const std::string& deploy_dir, const std::string& target);
        
    private:
        bool copy_directory_recursive(const std::string& source, const std::string& destination);
        bool set_file_permissions(const std::string& file_path, const std::string& permissions);
        bool set_directory_permissions(const std::string& dir_path, const std::string& permissions);
        std::string execute_script_with_sudo(const std::string& script_path, const std::string& user);
    };

    /**
     * Version History Manager - Tracks deployment history
     */
    class VersionHistoryManager {
    private:
        std::shared_ptr<medusa::database::TriforceConnector> database_;
        std::string history_file_path_;
        std::vector<VersionInfo> version_history_;
        
    public:
        VersionHistoryManager(std::shared_ptr<medusa::database::TriforceConnector> db,
                            const std::string& history_file = "/home/medusa/logs/version_history.json");
        
        // Version tracking
        bool record_deployment(const VersionInfo& version_info);
        bool update_deployment_status(const std::string& version_id, bool success);
        
        // Version queries
        std::vector<VersionInfo> get_version_history(const std::string& target = "");
        std::optional<VersionInfo> get_current_version(const std::string& target);
        std::optional<VersionInfo> get_version_by_id(const std::string& version_id);
        std::optional<VersionInfo> get_previous_version(const std::string& target);
        
        // Version management
        bool rollback_to_version(const std::string& version_id);
        bool can_rollback(const std::string& target);
        std::vector<std::string> get_rollback_candidates(const std::string& target);
        
        // Cleanup
        bool cleanup_old_versions(size_t keep_count = 10);
        bool delete_version_record(const std::string& version_id);
        
    private:
        bool save_history_to_file();
        bool load_history_from_file();
        bool save_to_database(const VersionInfo& version_info);
        std::vector<VersionInfo> load_from_database(const std::string& target);
        std::string generate_version_id();
    };

    /**
     * Main Medusa Version Manager - Integration Point
     */
    class MedusaVersionManager {
    private:
        DeploymentConfig config_;
        
        std::unique_ptr<ServiceController> service_controller_;
        std::unique_ptr<BackupManager> backup_manager_;
        std::unique_ptr<ZipDeploymentHandler> zip_handler_;
        std::unique_ptr<VersionHistoryManager> history_manager_;
        
        // Integration with other systems
        std::shared_ptr<medusa::database::TriforceConnector> database_;
        std::shared_ptr<medusa::sync::SynciecornEngine> synciecorn_;
        
        // Status tracking
        std::atomic<bool> deployment_in_progress_;
        std::string current_deployment_id_;
        
    public:
        MedusaVersionManager(std::shared_ptr<medusa::database::TriforceConnector> database,
                           std::shared_ptr<medusa::sync::SynciecornEngine> synciecorn = nullptr);
        ~MedusaVersionManager();
        
        // Initialization
        bool initialize();
        bool load_configuration(const std::string& config_file = "");
        bool save_configuration(const std::string& config_file = "");
        
        // Main deployment operations
        DeploymentResult deploy_zip_package(const std::string& zip_file, 
                                           const std::string& target,
                                           const std::unordered_map<std::string, std::string>& options = {});
        
        DeploymentResult deploy_from_synciecorn(const std::string& target);
        DeploymentResult quick_deploy_build_artifacts(const std::vector<std::string>& artifacts);
        
        // Version management
        std::vector<VersionInfo> get_deployment_history(const std::string& target = "");
        DeploymentResult rollback_to_previous_version(const std::string& target);
        DeploymentResult rollback_to_version(const std::string& version_id);
        
        // Service management
        bool restart_all_services();
        bool restart_target_services(const std::string& target);
        std::unordered_map<std::string, std::string> get_all_service_statuses();
        
        // Backup management
        std::string create_manual_backup(const std::string& target);
        std::vector<BackupManager::BackupInfo> get_backup_history(const std::string& target);
        bool cleanup_old_backups();
        
        // Status and monitoring
        bool is_deployment_in_progress() const { return deployment_in_progress_.load(); }
        std::string get_current_deployment_id() const { return current_deployment_id_; }
        std::vector<std::string> get_deployment_logs(const std::string& deployment_id = "");
        
        // Configuration management
        const DeploymentConfig& get_configuration() const { return config_; }
        bool update_target_configuration(const std::string& target, const DeploymentTarget& target_config);
        bool add_deployment_target(const std::string& target_name, const DeploymentTarget& target_config);
        
        // Integration with web server
        std::string handle_web_deployment_request(const std::string& request_json);
        std::string get_deployment_status_json();
        std::string get_version_history_json(const std::string& target = "");
        
        // Yorkshire Champion utilities
        std::string get_yorkshire_deployment_status();
        void log_yorkshire_deployment_event(const std::string& event, const std::string& details);
        
    private:
        // Internal deployment workflow
        DeploymentResult execute_deployment_workflow(const std::string& zip_file_or_source,
                                                    const std::string& target,
                                                    const std::unordered_map<std::string, std::string>& options);
        
        // Workflow steps
        bool step_validate_deployment(const std::string& source, const std::string& target);
        bool step_create_backup(const std::string& target);
        bool step_stop_services(const std::string& target);
        bool step_deploy_files(const std::string& source, const std::string& target);
        bool step_set_permissions(const std::string& target);
        bool step_execute_startup_script(const std::string& target);
        bool step_start_services(const std::string& target);
        bool step_verify_deployment(const std::string& target);
        bool step_record_deployment(const VersionInfo& version_info);
        
        // Error handling
        void handle_deployment_error(const std::string& stage, const std::string& error);
        bool attempt_rollback_on_failure(const std::string& target);
        
        // Utility methods
        std::string generate_deployment_id();
        VersionInfo create_version_info(const std::string& source, const std::string& target);
        void update_deployment_progress(const std::string& stage, double progress);
    };

    // Global instance for web server integration
    extern std::shared_ptr<MedusaVersionManager> g_version_manager;
    
    // Convenience functions for web server
    bool initialize_version_manager();
    void shutdown_version_manager();
    MedusaVersionManager* get_version_manager();
    
    // Quick deployment functions
    bool quick_deploy_zip(const std::string& zip_file, const std::string& target);
    bool quick_restart_services(const std::string& target = "");
    bool quick_backup_target(const std::string& target);

} // namespace medusa::core