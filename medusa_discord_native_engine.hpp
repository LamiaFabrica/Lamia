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

// OpenSSL includes - NO forward declarations
#include <openssl/ssl.h>
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
        READY,
        GITHUB_NOTIFICATION
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

    struct DiscordChannel {
        std::string id;
        std::string name;
        std::string type;
        std::string guild_id;
        std::string topic;
        bool nsfw;
        int position;
    };

    struct DiscordGuild {
        std::string id;
        std::string name;
        std::string icon;
        std::string owner_id;
        int member_count;
        std::vector<DiscordChannel> channels;
    };

    struct DiscordUser {
        std::string id;
        std::string username;
        std::string discriminator;
        std::string avatar;
        bool bot;
        bool verified;
    };

    struct SlashCommand {
        std::string name;
        std::string description;
        std::vector<std::pair<std::string, std::string>> options;
        std::function<void(const DiscordMessage&, const std::unordered_map<std::string, std::string>&)> handler;
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
        
        void start_heartbeat();
        void send_heartbeat();
        void handle_gateway_event(const std::string& event_data);
        
    public:
        std::string make_http_request(const std::string& method, const std::string& endpoint, 
                                     const std::string& payload = "");
        DiscordAPIClient(const DiscordCredentials& creds);
        ~DiscordAPIClient();
        
        bool authenticate();
        bool connect_gateway();
        void disconnect();
        
        // Message operations
        bool send_message(const std::string& channel_id, const std::string& content);
        bool send_embed(const std::string& channel_id, const std::unordered_map<std::string, std::string>& embed);
        bool edit_message(const std::string& channel_id, const std::string& message_id, const std::string& content);
        bool delete_message(const std::string& channel_id, const std::string& message_id);
        
        // Channel operations
        std::vector<DiscordChannel> get_guild_channels(const std::string& guild_id);
        DiscordChannel create_channel(const std::string& guild_id, const std::string& name, const std::string& type);
        bool delete_channel(const std::string& channel_id);
        bool create_thread(const std::string& channel_id, const std::string& name);
        
        // Guild operations
        DiscordGuild get_guild(const std::string& guild_id);
        std::vector<DiscordUser> get_guild_members(const std::string& guild_id);
        
        // Slash commands
        bool register_slash_command(const std::string& guild_id, const SlashCommand& command);
        bool update_slash_command(const std::string& guild_id, const std::string& command_id, const SlashCommand& command);
        bool delete_slash_command(const std::string& guild_id, const std::string& command_id);
    };

    class MedusaDiscordEngine {
    private:
        std::unique_ptr<DiscordAPIClient> api_client;
        std::unordered_map<std::string, SlashCommand> registered_commands;
        std::unordered_map<EventType, std::vector<std::function<void(const std::string&)>>> event_handlers;
        std::thread event_processing_thread;
        std::queue<std::pair<EventType, std::string>> event_queue;
        std::mutex event_mutex;
        std::condition_variable event_cv;
        std::atomic<bool> running;
        
        // Medusa Spin Integration
        std::string medusa_api_key;
        std::string github_integration_webhook;
        std::unordered_map<std::string, std::string> user_permissions;
        
        void process_events();
        void handle_slash_command(const DiscordMessage& message, const std::unordered_map<std::string, std::string>& options);
        void integrate_with_github(const std::string& repository, const std::string& action);
        void authenticate_medusa_user(const std::string& discord_id, const std::string& medusa_token);

    public:
        MedusaDiscordEngine(const DiscordCredentials& credentials, const std::string& medusa_key);
        ~MedusaDiscordEngine();
        
        bool initialize();
        void start();
        void stop();
        
        // Command registration
        void register_command(const SlashCommand& command);
        void unregister_command(const std::string& command_name);
        
        // Event handling
        void on_event(EventType type, std::function<void(const std::string&)> handler);
        void emit_event(EventType type, const std::string& data);
        
        // Medusa-specific functionality
        void setup_github_integration(const std::string& webhook_url);
        void setup_medusa_spin_auth(const std::string& api_endpoint);
        void process_github_webhook(const std::string& payload);
        
        // Advanced features
        void create_discord_thread(const std::string& channel_id, const std::string& name);
        void manage_user_roles(const std::string& guild_id, const std::string& user_id, const std::vector<std::string>& roles);
        void send_dm(const std::string& user_id, const std::string& content);
        void create_scheduled_message(const std::string& channel_id, const std::string& content, 
                                     std::chrono::system_clock::time_point send_time);
        
        // Statistics and monitoring
        struct EngineStats {
            int messages_sent;
            int commands_processed;
            int events_handled;
            std::chrono::system_clock::time_point uptime_start;
            int active_connections;
        };
        
        EngineStats get_statistics() const;
        void reset_statistics();
    };

    // Utility functions
    std::string encode_json(const std::unordered_map<std::string, std::string>& data);
    std::unordered_map<std::string, std::string> decode_json(const std::string& json);
    std::string generate_snowflake_id();
    bool validate_discord_token(const std::string& token);
    std::string escape_discord_markdown(const std::string& text);

} // namespace MedusaDiscord