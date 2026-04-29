#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <chrono>

// OpenSSL forward declarations
#include <openssl/ossl_typ.h>

namespace MedusaDiscord {

    enum class MessageType {
        TEXT,
        EMBED,
        FILE_ATTACHMENT,
        SLASH_COMMAND,
        BUTTON_INTERACTION,
        MODAL_SUBMIT
    };

    enum class EventType {
        MESSAGE_CREATE,
        MESSAGE_UPDATE,
        MESSAGE_DELETE,
        GUILD_CREATE,
        GUILD_MEMBER_ADD,
        INTERACTION_CREATE,
        READY
    };

    struct DiscordCredentials {
        std::string bot_token;
        std::string application_id;
        std::string guild_id;
        std::string client_secret;
    };

    struct DiscordMessage {
        std::string id;
        std::string channel_id;
        std::string guild_id;
        std::string author_id;
        std::string author_username;
        std::string content;
        MessageType type;
        std::chrono::system_clock::time_point timestamp;
        std::vector<std::string> attachments;
        std::unordered_map<std::string, std::string> embeds;
    };

    class WebSocketConnection {
    private:
        SSL_CTX* ssl_ctx;
        SSL* ssl;
        int socket_fd;
        std::string host;
        int port;
        std::atomic<bool> connected;
        std::thread connection_thread;
        std::mutex send_mutex;
        std::queue<std::string> send_queue;
        std::condition_variable send_cv;
        std::mutex receive_mutex;
        std::queue<std::string> receive_queue;
        
        bool establish_connection();
        bool ssl_handshake();
        bool websocket_handshake();
        void connection_loop();
        void send_websocket_frame(const std::string& payload);
        std::string receive_websocket_frame();
        void send_close_frame();
        void send_pong_frame(const std::string& data);

    public:
        WebSocketConnection(const std::string& host, int port);
        ~WebSocketConnection();
        
        bool connect();
        void disconnect();
        bool send_message(const std::string& message);
        std::string receive_message();
        bool is_connected() const { return connected.load(); }
    };

    class DiscordAPIClient {
    private:
        DiscordCredentials credentials;
        std::string base_url;
        std::string gateway_url;
        std::unique_ptr<WebSocketConnection> ws_connection;
        std::atomic<bool> authenticated;
        std::atomic<int> sequence_number;
        std::string session_id;
        std::chrono::milliseconds heartbeat_interval;
        std::thread heartbeat_thread;
        
        std::string make_http_request(const std::string& method, const std::string& endpoint, 
                                     const std::string& payload = "");
        void start_heartbeat();
        void send_heartbeat();
        void handle_gateway_event(const std::string& event_data);
        bool process_hello_event(const std::string& event_data);
        bool send_identify();
        bool process_ready_event(const std::string& event_data);
        
    public:
        DiscordAPIClient(const DiscordCredentials& creds);
        ~DiscordAPIClient();
        
        bool authenticate();
        bool connect_gateway();
        void disconnect();
        
        bool send_message(const std::string& channel_id, const std::string& content);
        bool send_embed(const std::string& channel_id, const std::unordered_map<std::string, std::string>& embed);
    };

    struct SlashCommand {
        std::string name;
        std::string description;
        std::vector<std::pair<std::string, std::string>> options;
        std::function<void(const DiscordMessage&, const std::unordered_map<std::string, std::string>&)> handler;
    };

    struct EventStats {
        std::atomic<int> messages_sent{0};
        std::atomic<int> commands_processed{0};
        std::atomic<int> events_handled{0};
    };

    class MedusaDiscordEngine {
    private:
        std::unique_ptr<DiscordAPIClient> api_client;
        std::unordered_map<std::string, SlashCommand> registered_commands;
        std::unordered_map<EventType, std::vector<std::function<void(const std::string&)>>> event_handlers;
        std::thread event_processing_thread;
        std::thread monitoring_thread;
        std::thread metrics_thread;
        std::queue<std::pair<EventType, std::string>> event_queue;
        std::mutex event_mutex;
        std::condition_variable event_cv;
        std::atomic<bool> running;
        
        // Medusa integration
        std::string medusa_api_key;
        std::string github_integration_webhook;
        std::string medusa_spin_endpoint;
        std::unordered_map<std::string, std::string> user_permissions;
        
        // System status tracking
        EventStats event_stats;
        std::chrono::system_clock::time_point startup_time;
        
        // Pearl metrics
        bool pearl_metrics_active = false;
        std::unordered_map<std::string, std::string> pearl_metrics;
        std::chrono::system_clock::time_point last_pearl_update;
        
        // Triforce database status
        std::unordered_map<std::string, std::string> triforce_status;
        double triforce_health_score = 0.0;
        std::chrono::system_clock::time_point last_triforce_sync;
        
        // CalDAV integration
        bool caldav_active = false;
        std::string caldav_server;
        std::string caldav_username;
        std::chrono::system_clock::time_point last_caldav_sync;
        
        // GitHub integration
        bool github_active = false;
        
        // Private methods
        void process_events();
        void monitoring_loop();
        void metrics_collection_loop();
        void register_default_commands();
        void setup_event_handlers();
        
        // Initialization methods
        bool initialize_medusa_spin();
        bool initialize_pearl_metrics();
        bool initialize_triforce_integration();
        bool initialize_caldav_sync();
        
        // Command handlers
        void handle_status_command(const DiscordMessage& msg, const std::unordered_map<std::string, std::string>& options);
        void handle_pearl_metrics_command(const DiscordMessage& msg, const std::unordered_map<std::string, std::string>& options);
        void handle_triforce_status_command(const DiscordMessage& msg, const std::unordered_map<std::string, std::string>& options);
        void handle_github_sync_command(const DiscordMessage& msg, const std::unordered_map<std::string, std::string>& options);
        void handle_task_sync_command(const DiscordMessage& msg, const std::unordered_map<std::string, std::string>& options);
        void handle_deploy_command(const DiscordMessage& msg, const std::unordered_map<std::string, std::string>& options);
        void handle_legacy_command(const std::unordered_map<std::string, std::string>& message_data);
        void handle_mention_command(const std::unordered_map<std::string, std::string>& message_data);
        
        // Utility methods
        void send_welcome_message(const std::string& user_id, const std::string& guild_id);
        void log_system_event(const std::string& event, const std::string& details);
        void update_system_health();
        void check_system_alerts();
        void update_integration_status();
        void update_pearl_metrics();
        void update_triforce_status();
        void sync_caldav_tasks();
        void update_github_status();

    public:
        MedusaDiscordEngine(const DiscordCredentials& credentials, const std::string& medusa_key);
        ~MedusaDiscordEngine();
        
        bool initialize();
        void start();
        void stop();
        
        void register_command(const SlashCommand& command);
        void on_event(EventType type, std::function<void(const std::string&)> handler);
        void emit_event(EventType type, const std::string& data);
        
        void setup_github_integration(const std::string& webhook_url);
        void setup_medusa_spin_auth(const std::string& api_endpoint);
        
        struct EngineStats {
            int messages_sent;
            int commands_processed;
            int events_handled;
            std::chrono::system_clock::time_point uptime_start;
            int active_connections;
        };
        
        EngineStats get_statistics() const;
    };

    // Utility functions
    std::string encode_json(const std::unordered_map<std::string, std::string>& data);
    std::unordered_map<std::string, std::string> decode_json(const std::string& json);
    std::string generate_snowflake_id();
    bool validate_discord_token(const std::string& token);
    std::string escape_discord_markdown(const std::string& text);
    std::string format_timestamp(std::chrono::system_clock::time_point time);
    std::string calculate_health_percentage(const std::unordered_map<std::string, std::string>& metrics);
    std::string generate_status_embed(const std::string& title, 
                                     const std::unordered_map<std::string, std::string>& data,
                                     int color);
    std::string generate_metrics_chart(const std::unordered_map<std::string, std::string>& metrics);
    std::string get_trend_indicator(double current, double previous);
    std::string format_uptime(std::chrono::system_clock::time_point start_time);

    // CURL callback function
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

} // namespace MedusaDiscord