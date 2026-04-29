#pragma once

/**
 * MEDUSA BUILD & DEPLOY SYSTEM v0.2.7
 * ===================================
 * 
 * WSL → Remote Server Build & Deploy Pipeline
 * 
 * Compiles locally on WSL, deploys to 172.236.28.155:/home/medusa
 * Full diff auditing, change tracking, and AI Command integration
 * 
 * Yorkshire Champion Gold Standard - CI/CD Pipeline
 * 
 * Author: Claude Code (Yorkshire Champion)
 * Purpose: Bridge WSL development to remote production deployment
 */

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <filesystem>
#include <functional>

namespace medusa::build {

    /**
     * Remote Server Configuration
     */
    struct RemoteServerConfig {
        std::string hostname = "172.236.28.155";
        std::string username = "root"; 
        std::string password = "7303nmU8Q29WjXpPlumsBAXyJjMt";
        uint16_t ssh_port = 22;
        std::string deploy_path = "/home/medusa";
        bool use_key_auth = false;
        std::string private_key_path = "";
    };

    /**
     * Build Configuration
     */
    struct BuildConfig {
        std::string source_directory = "/mnt/c/Build/htdocs/back-office++";
        std::string build_directory = "/mnt/c/Build/htdocs/back-office++/build";
        std::string cmake_build_type = "Release";
        std::vector<std::string> build_targets;
        std::unordered_map<std::string, std::string> cmake_variables;
        
        // Cross-compilation settings
        bool cross_compile = false;
        std::string target_architecture = "x86_64";
        std::string target_os = "linux";
        
        // Build flags
        bool enable_testing = false;
        bool enable_debugging_symbols = false;
        bool enable_optimization = true;
        bool enable_warnings_as_errors = true;
    };

    /**
     * Deployment Diff Information
     */
    struct DeploymentDiff {
        std::string file_path;
        std::string operation;  // "CREATE", "MODIFY", "DELETE"
        std::string old_hash;
        std::string new_hash;
        size_t old_size;
        size_t new_size;
        std::chrono::system_clock::time_point timestamp;
        std::vector<std::string> diff_lines;  // Actual diff content
    };

    /**
     * Deployment Record
     */
    struct DeploymentRecord {
        std::string deployment_id;
        std::string version_tag;
        std::string commit_hash;
        std::chrono::system_clock::time_point deployment_time;
        std::string deployed_by;
        std::vector<DeploymentDiff> changes;
        bool deployment_successful;
        std::string deployment_log;
        std::vector<std::string> failed_files;
        
        // AI Command integration
        std::string ai_analysis_report;
        std::vector<std::string> ai_recommendations;
        bool ai_approval_required;
    };

    /**
     * Build Result Information
     */
    struct BuildResult {
        bool success;
        std::chrono::milliseconds build_time;
        std::vector<std::string> built_targets;
        std::vector<std::string> failed_targets;
        std::vector<std::string> warnings;
        std::vector<std::string> errors;
        std::string build_log;
        
        // Generated artifacts
        std::vector<std::string> shared_libraries;  // .so files
        std::vector<std::string> executables;
        std::vector<std::string> header_files;
        std::vector<std::string> config_files;
    };

    /**
     * SSH Connection Manager
     */
    class SSHConnectionManager {
    private:
        RemoteServerConfig config_;
        void* ssh_session_;  // libssh session
        void* sftp_session_; // libssh SFTP session
        bool connected_;
        
    public:
        SSHConnectionManager(const RemoteServerConfig& config);
        ~SSHConnectionManager();
        
        bool connect();
        void disconnect();
        bool is_connected() const { return connected_; }
        
        // Remote command execution
        std::string execute_command(const std::string& command);
        bool execute_command_async(const std::string& command, 
                                  std::function<void(const std::string&)> output_callback);
        
        // File operations
        bool upload_file(const std::string& local_path, const std::string& remote_path);
        bool upload_directory(const std::string& local_dir, const std::string& remote_dir);
        bool download_file(const std::string& remote_path, const std::string& local_path);
        
        // File information
        bool file_exists(const std::string& remote_path);
        std::string get_file_hash(const std::string& remote_path);
        size_t get_file_size(const std::string& remote_path);
        
        // Directory operations
        bool create_directory(const std::string& remote_path);
        bool remove_directory(const std::string& remote_path);
        std::vector<std::string> list_directory(const std::string& remote_path);
    };

    /**
     * Diff Generator and Analyzer
     */
    class DiffAnalyzer {
    private:
        std::string diff_storage_path_;
        
    public:
        DiffAnalyzer(const std::string& storage_path = "/tmp/medusa_diffs");
        
        // Generate diffs
        DeploymentDiff generate_file_diff(const std::string& local_file, 
                                         const std::string& remote_file,
                                         SSHConnectionManager& ssh);
        
        std::vector<DeploymentDiff> generate_directory_diff(const std::string& local_dir,
                                                           const std::string& remote_dir,
                                                           SSHConnectionManager& ssh);
        
        // Analyze changes
        std::vector<std::string> analyze_critical_changes(const std::vector<DeploymentDiff>& diffs);
        std::vector<std::string> suggest_rollback_strategy(const std::vector<DeploymentDiff>& diffs);
        bool requires_service_restart(const std::vector<DeploymentDiff>& diffs);
        
        // Save and load diffs
        bool save_deployment_diff(const std::string& deployment_id, 
                                 const std::vector<DeploymentDiff>& diffs);
        std::vector<DeploymentDiff> load_deployment_diff(const std::string& deployment_id);
        
    private:
        std::string calculate_file_hash(const std::string& file_path);
        std::vector<std::string> generate_unified_diff(const std::string& old_content,
                                                       const std::string& new_content,
                                                       const std::string& file_path);
    };

    /**
     * AI Command Integration for Route Debugging
     */
    class AICommandIntegration {
    private:
        std::string ai_command_endpoint_;
        std::string ai_api_key_;
        
    public:
        AICommandIntegration(const std::string& endpoint = "http://localhost:8080/ai-command",
                           const std::string& api_key = "");
        
        // AI analysis of deployment
        std::string analyze_deployment_changes(const std::vector<DeploymentDiff>& diffs);
        std::vector<std::string> suggest_testing_strategy(const BuildResult& build_result);
        bool validate_route_changes(const std::vector<std::string>& changed_routes);
        
        // AI debugging integration
        std::string debug_route(const std::string& route_path);
        std::string analyze_menu_item(const std::string& menu_item_id);
        std::vector<std::string> suggest_performance_optimizations(const std::string& component);
        
        // AI approval system
        bool requires_ai_approval(const std::vector<DeploymentDiff>& diffs);
        std::string request_ai_approval(const DeploymentRecord& record);
        bool get_ai_approval_status(const std::string& deployment_id);
        
    private:
        std::string send_ai_request(const std::string& endpoint, const std::string& payload);
        std::string format_diff_for_ai(const std::vector<DeploymentDiff>& diffs);
    };

    /**
     * Build System Manager
     */
    class BuildSystemManager {
    private:
        BuildConfig build_config_;
        std::string build_log_path_;
        
    public:
        BuildSystemManager(const BuildConfig& config);
        
        // Build operations
        BuildResult build_all_targets();
        BuildResult build_specific_targets(const std::vector<std::string>& targets);
        BuildResult incremental_build();
        
        // Build preparation
        bool prepare_build_environment();
        bool clean_build_directory();
        bool generate_cmake_files();
        
        // Build analysis
        std::vector<std::string> get_build_dependencies(const std::string& target);
        std::vector<std::string> get_changed_targets_since_last_build();
        bool validate_build_configuration();
        
    private:
        std::string execute_build_command(const std::string& command);
        std::vector<std::string> parse_build_log(const std::string& log_content);
        bool check_build_tools_available();
    };

    /**
     * Deployment Audit System
     */
    class DeploymentAuditor {
    private:
        std::string audit_database_path_;
        std::vector<DeploymentRecord> deployment_history_;
        
    public:
        DeploymentAuditor(const std::string& audit_db_path = "/tmp/medusa_deployment_audit.db");
        
        // Audit operations
        void record_deployment(const DeploymentRecord& record);
        std::vector<DeploymentRecord> get_deployment_history(size_t limit = 100);
        DeploymentRecord get_deployment_record(const std::string& deployment_id);
        
        // Audit analysis
        std::vector<std::string> analyze_deployment_patterns();
        std::vector<std::string> identify_high_risk_changes();
        std::vector<std::string> generate_compliance_report();
        
        // Rollback support
        bool can_rollback_deployment(const std::string& deployment_id);
        std::vector<std::string> generate_rollback_script(const std::string& deployment_id);
        bool execute_rollback(const std::string& deployment_id);
        
    private:
        bool initialize_audit_database();
        void cleanup_old_audit_records();
    };

    /**
     * Main Build & Deploy Pipeline
     */
    class MedusaBuildDeployPipeline {
    private:
        RemoteServerConfig remote_config_;
        BuildConfig build_config_;
        
        std::unique_ptr<SSHConnectionManager> ssh_manager_;
        std::unique_ptr<DiffAnalyzer> diff_analyzer_;
        std::unique_ptr<AICommandIntegration> ai_integration_;
        std::unique_ptr<BuildSystemManager> build_manager_;
        std::unique_ptr<DeploymentAuditor> auditor_;
        
        // Pipeline state
        std::string current_deployment_id_;
        DeploymentRecord current_deployment_;
        
    public:
        MedusaBuildDeployPipeline(const RemoteServerConfig& remote_config,
                                const BuildConfig& build_config);
        ~MedusaBuildDeployPipeline();
        
        // Pipeline operations
        bool initialize_pipeline();
        bool execute_full_pipeline();
        bool execute_build_only();
        bool execute_deploy_only();
        
        // Individual pipeline steps
        bool step_1_prepare_build();
        bool step_2_compile_sources();
        bool step_3_run_tests();
        bool step_4_generate_diffs();
        bool step_5_ai_analysis();
        bool step_6_deploy_to_remote();
        bool step_7_verify_deployment();
        bool step_8_audit_deployment();
        
        // Pipeline monitoring
        std::string get_pipeline_status();
        std::vector<std::string> get_pipeline_logs();
        bool is_pipeline_running();
        
        // Emergency operations
        bool emergency_rollback();
        bool emergency_stop();
        
        // Configuration
        void update_build_config(const BuildConfig& config);
        void update_remote_config(const RemoteServerConfig& config);
        
        // Yorkshire Champion utilities
        std::string get_yorkshire_pipeline_status();
        void log_yorkshire_event(const std::string& event, const std::string& details);
        
    private:
        // Pipeline helpers
        std::string generate_deployment_id();
        bool validate_remote_environment();
        bool backup_current_deployment();
        bool restore_from_backup();
        
        // Notification system
        void notify_deployment_start(const std::string& deployment_id);
        void notify_deployment_complete(bool success, const std::string& details);
        void notify_deployment_failure(const std::string& error_details);
    };

    /**
     * CLI Interface for Build & Deploy System
     */
    class BuildDeployCLI {
    private:
        std::unique_ptr<MedusaBuildDeployPipeline> pipeline_;
        std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> commands_;
        
    public:
        BuildDeployCLI();
        
        bool initialize();
        void run_interactive_mode();
        void execute_command_line(int argc, char* argv[]);
        
    private:
        void register_commands();
        void print_help();
        
        // Command implementations
        void cmd_build(const std::vector<std::string>& args);
        void cmd_deploy(const std::vector<std::string>& args);
        void cmd_pipeline(const std::vector<std::string>& args);
        void cmd_diff(const std::vector<std::string>& args);
        void cmd_rollback(const std::vector<std::string>& args);
        void cmd_status(const std::vector<std::string>& args);
        void cmd_audit(const std::vector<std::string>& args);
        void cmd_ai_debug(const std::vector<std::string>& args);
    };

} // namespace medusa::build