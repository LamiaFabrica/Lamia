/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * MEDUSA MASTER-SLAVE SYSTEM v0.3.0c
 * ==================================
 * 
 * Revolutionary intelligent connection system with real-time master switching
 * Ground-up implementation - Yorkshire Champion Standards
 * NO SHORTCUTS - Only ONE master, infinite slaves with role synchronization
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <queue>

namespace MedusaMasterSlave {

    /**
     * @brief Node roles in the master-slave system
     */
    enum class NodeRole {
        MASTER,
        SLAVE,
        CANDIDATE,  // Eligible to become master
        OBSERVER    // Read-only, cannot become master
    };

    /**
     * @brief Node status states
     */
    enum class NodeStatus {
        INITIALIZING,
        ACTIVE,
        SYNCING,
        FAILED,
        OFFLINE,
        ELECTION_IN_PROGRESS
    };

    /**
     * @brief Message types for inter-node communication
     */
    enum class MessageType {
        HEARTBEAT,
        SYNC_REQUEST,
        SYNC_RESPONSE,
        ROLE_CHANGE,
        ELECTION_START,
        ELECTION_VOTE,
        ELECTION_RESULT,
        PERMISSION_UPDATE,
        SETTING_UPDATE,
        DATA_BROADCAST,
        HEALTH_CHECK,
        FAILOVER_TRIGGER
    };

    /**
     * @brief Node information structure
     */
    struct NodeInfo {
        std::string node_id;
        std::string hostname;
        std::string ip_address;
        int port;
        NodeRole role;
        NodeStatus status;
        std::chrono::system_clock::time_point last_heartbeat;
        std::chrono::system_clock::time_point startup_time;
        
        // Performance metrics
        double cpu_usage;
        double memory_usage;
        double network_latency;
        int connection_count;
        
        // Master-specific data
        std::chrono::system_clock::time_point master_elected_at;
        int master_election_votes;
        
        // Capabilities
        std::vector<std::string> supported_protocols;
        std::unordered_map<std::string, std::string> node_metadata;
        
        // Health scoring
        double health_score;
        std::string health_reason;
        
        bool is_healthy() const {
            auto now = std::chrono::system_clock::now();
            auto heartbeat_age = std::chrono::duration_cast<std::chrono::seconds>(now - last_heartbeat);
            return status == NodeStatus::ACTIVE && heartbeat_age.count() < 30 && health_score > 0.7;
        }
    };

    /**
     * @brief Permission and role data structure
     */
    struct PermissionData {
        std::string user_id;
        std::string role_name;
        std::vector<std::string> permissions;
        std::unordered_map<std::string, std::string> metadata;
        std::chrono::system_clock::time_point last_updated;
        std::string updated_by_node;
        
        // Integration-specific permissions
        struct IntegrationPerms {
            bool discord_admin = false;
            bool github_write = false;
            bool patreon_access = false;
            std::vector<std::string> discord_channels;
            std::vector<std::string> github_repos;
            std::vector<std::string> patreon_campaigns;
        } integrations;
    };

    /**
     * @brief System settings structure
     */
    struct SystemSettings {
        std::string setting_key;
        std::string setting_value;
        std::string setting_type; // string, int, bool, json
        std::chrono::system_clock::time_point last_updated;
        std::string updated_by_node;
        bool requires_master_approval;
        
        // Setting metadata
        std::string description;
        std::string category;
        std::vector<std::string> allowed_values;
        bool is_sensitive;
    };

    /**
     * @brief Inter-node message structure
     */
    struct NodeMessage {
        MessageType type;
        std::string sender_id;
        std::string recipient_id; // empty for broadcast
        std::chrono::system_clock::time_point timestamp;
        nlohmann::json payload;
        std::string message_id;
        bool requires_acknowledgment;
        
        // Message routing
        std::vector<std::string> forwarded_by;
        int hop_count;
        std::chrono::system_clock::time_point expires_at;
    };

    /**
     * @brief Medusa Master-Slave Node
     */
    class MedusaNode {
    private:
        // Node identity
        std::string node_id_;
        std::string hostname_;
        std::string ip_address_;
        int port_;
        NodeRole current_role_;
        NodeStatus current_status_;
        
        // Network communication
        int server_socket_;
        std::atomic<bool> running_{false};
        std::thread server_thread_;
        std::thread client_thread_;
        std::thread heartbeat_thread_;
        std::thread election_thread_;
        
        // Node registry
        std::unordered_map<std::string, NodeInfo> known_nodes_;
        std::mutex nodes_mutex_;
        
        // Master tracking
        std::string current_master_id_;
        std::chrono::system_clock::time_point last_master_heartbeat_;
        std::atomic<bool> election_in_progress_{false};
        
        // Data synchronization
        std::unordered_map<std::string, PermissionData> permissions_db_;
        std::unordered_map<std::string, SystemSettings> settings_db_;
        std::mutex data_mutex_;
        
        // Message handling
        std::queue<NodeMessage> message_queue_;
        std::mutex message_mutex_;
        std::condition_variable message_cv_;
        std::thread message_processor_;
        
        // Event handlers
        std::function<void(NodeRole, NodeRole)> role_change_handler_;
        std::function<void(const std::string&)> master_change_handler_;
        std::function<void(const PermissionData&)> permission_update_handler_;
        std::function<void(const SystemSettings&)> setting_update_handler_;
        
        // Configuration
        std::chrono::seconds heartbeat_interval_{10};
        std::chrono::seconds election_timeout_{30};
        std::chrono::seconds master_timeout_{45};
        double min_health_score_{0.5};
        
        // Statistics
        std::atomic<int> messages_sent_{0};
        std::atomic<int> messages_received_{0};
        std::atomic<int> sync_operations_{0};
        std::atomic<int> election_count_{0};
        
        // Private methods
        void server_loop();
        void client_loop();
        void heartbeat_loop();
        void message_processor_loop();
        void election_loop();
        
        void handle_message(const NodeMessage& message);
        void handle_heartbeat(const NodeMessage& message);
        void handle_sync_request(const NodeMessage& message);
        void handle_sync_response(const NodeMessage& message);
        void handle_role_change(const NodeMessage& message);
        void handle_election_message(const NodeMessage& message);
        
        bool send_message(const std::string& target_node, const NodeMessage& message);
        void broadcast_message(const NodeMessage& message);
        
        void start_election();
        void vote_for_master(const std::string& candidate_id);
        double calculate_node_score(const NodeInfo& node);
        std::string select_best_master_candidate();
        
        void sync_with_master();
        void sync_permissions(const nlohmann::json& permissions_data);
        void sync_settings(const nlohmann::json& settings_data);
        
        NodeInfo get_self_info();
        void update_health_metrics();
        
    public:
        MedusaNode(const std::string& node_id, const std::string& hostname, 
                  const std::string& ip_address, int port);
        ~MedusaNode();
        
        // Core operations
        bool start();
        void stop();
        bool is_running() const { return running_; }
        
        // Node management
        NodeRole get_role() const { return current_role_; }
        NodeStatus get_status() const { return current_status_; }
        std::string get_node_id() const { return node_id_; }
        std::string get_current_master() const { return current_master_id_; }
        bool is_master() const { return current_role_ == NodeRole::MASTER; }
        
        // Network management
        void add_known_node(const std::string& node_id, const std::string& hostname, 
                           const std::string& ip, int port);
        void remove_node(const std::string& node_id);
        std::vector<NodeInfo> get_all_nodes();
        NodeInfo get_node_info(const std::string& node_id);
        
        // Master operations (only work if this node is master)
        bool promote_to_master();
        bool demote_from_master();
        void force_election();
        void force_failover(const std::string& new_master_id = "");
        
        // Permission management
        bool set_permission(const std::string& user_id, const std::string& role, 
                           const std::vector<std::string>& permissions);
        PermissionData get_permission(const std::string& user_id);
        std::vector<PermissionData> get_all_permissions();
        bool remove_permission(const std::string& user_id);
        
        // Settings management
        bool set_setting(const std::string& key, const std::string& value, 
                        const std::string& type = "string");
        SystemSettings get_setting(const std::string& key);
        std::vector<SystemSettings> get_all_settings();
        bool remove_setting(const std::string& key);
        
        // Real-time synchronization
        void enable_real_time_sync(bool enabled = true);
        bool sync_now();
        std::chrono::system_clock::time_point get_last_sync_time();
        
        // Event handlers
        void set_role_change_handler(std::function<void(NodeRole, NodeRole)> handler) {
            role_change_handler_ = handler;
        }
        
        void set_master_change_handler(std::function<void(const std::string&)> handler) {
            master_change_handler_ = handler;
        }
        
        void set_permission_update_handler(std::function<void(const PermissionData&)> handler) {
            permission_update_handler_ = handler;
        }
        
        void set_setting_update_handler(std::function<void(const SystemSettings&)> handler) {
            setting_update_handler_ = handler;
        }
        
        // Health and monitoring
        double get_health_score();
        std::string get_health_status();
        std::unordered_map<std::string, std::string> get_node_metrics();
        
        // Statistics
        struct NodeStats {
            int messages_sent;
            int messages_received;
            int sync_operations;
            int election_count;
            std::chrono::system_clock::time_point uptime;
            std::string current_master;
            int connected_nodes;
        };
        
        NodeStats get_stats();
        
        // Configuration
        void set_heartbeat_interval(std::chrono::seconds interval) { heartbeat_interval_ = interval; }
        void set_election_timeout(std::chrono::seconds timeout) { election_timeout_ = timeout; }
        void set_master_timeout(std::chrono::seconds timeout) { master_timeout_ = timeout; }
        void set_min_health_score(double score) { min_health_score_ = score; }
    };

    /**
     * @brief Master-Slave System Manager
     */
    class MedusaMasterSlaveSystem {
    private:
        std::vector<std::shared_ptr<MedusaNode>> nodes_;
        std::string system_name_;
        std::mutex system_mutex_;
        
        // Integration stubs for future real-time connections
        bool real_time_discord_sync_{false};
        bool real_time_github_sync_{false};
        bool real_time_patreon_sync_{false};
        std::string real_time_settings_endpoint_;
        
    public:
        MedusaMasterSlaveSystem(const std::string& system_name);
        ~MedusaMasterSlaveSystem();
        
        // Node management
        std::shared_ptr<MedusaNode> create_node(const std::string& node_id, 
                                               const std::string& hostname,
                                               const std::string& ip, int port);
        bool start_all_nodes();
        void stop_all_nodes();
        
        // System operations
        std::shared_ptr<MedusaNode> get_master_node();
        std::vector<std::shared_ptr<MedusaNode>> get_slave_nodes();
        std::vector<std::shared_ptr<MedusaNode>> get_all_nodes();
        
        bool trigger_system_wide_sync();
        void force_master_election();
        
        // Monitoring
        std::unordered_map<std::string, std::string> get_system_status();
        std::vector<std::string> get_system_health_issues();
        
        // TODO: Real-time integration stubs (for future implementation)
        void enable_real_time_discord_sync(bool enabled = true) { 
            real_time_discord_sync_ = enabled; 
            // TODO: Implement real-time Discord role/permission sync
        }
        
        void enable_real_time_github_sync(bool enabled = true) { 
            real_time_github_sync_ = enabled;
            // TODO: Implement real-time GitHub webhook/permission sync  
        }
        
        void enable_real_time_patreon_sync(bool enabled = true) { 
            real_time_patreon_sync_ = enabled;
            // TODO: Implement real-time Patreon pledge/tier sync
        }
        
        void set_real_time_settings_endpoint(const std::string& endpoint) {
            real_time_settings_endpoint_ = endpoint;
            // TODO: Implement real-time settings API endpoint
        }
        
        // Real-time toggle methods (stubs for settings integration)
        bool get_real_time_discord_enabled() const { return real_time_discord_sync_; }
        bool get_real_time_github_enabled() const { return real_time_github_sync_; }
        bool get_real_time_patreon_enabled() const { return real_time_patreon_sync_; }
        std::string get_real_time_settings_endpoint() const { return real_time_settings_endpoint_; }
    };

} // namespace MedusaMasterSlave