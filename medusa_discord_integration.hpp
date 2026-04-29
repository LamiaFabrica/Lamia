/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * MEDUSA DISCORD INTEGRATION v0.3.0c
 * ==================================
 * 
 * Native C++ Discord integration with Medusa Spin
 * Ground-up implementation - Yorkshire Champion Standards
 * NO SHORTCUTS - Revolutionary Discord API implementation
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <functional>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "medusa_discord_native_engine.hpp"
#include "medusa_native_http_client.hpp"
#include <condition_variable>
#include <iomanip>

namespace MedusaDiscord {

    /**
     * @brief Discord message types
     */
    enum class MessageType {
        TEXT,
        EMBED,
        FILE_UPLOAD,
        REACTION,
        SLASH_COMMAND,
        BUTTON_INTERACTION,
        AI_RESPONSE,
        GITHUB_NOTIFICATION,
        SYSTEM_STATUS
    };

    /**
     * @brief Discord channel types
     */
    enum class ChannelType {
        TEXT_CHANNEL = 0,
        DM = 1,
        VOICE_CHANNEL = 2,
        GROUP_DM = 3,
        CATEGORY = 4,
        ANNOUNCEMENT = 5,
        THREAD = 10,
        FORUM = 15
    };

    /**
     * @brief Discord user information
     */
    struct DiscordUser {
        std::string id;
        std::string username;
        std::string discriminator;
        std::string avatar;
        std::string global_name;
        bool bot = false;
        bool system = false;
        bool verified = false;
        std::chrono::system_clock::time_point created_at;
        
        std::string get_display_name() const {
            return global_name.empty() ? username : global_name;
        }
        
        std::string get_mention() const {
            return "<@" + id + ">";
        }
    };

    /**
     * @brief Discord channel information
     */
    struct DiscordChannel {
        std::string id;
        std::string name;
        std::string topic;
        ChannelType type;
        std::string guild_id;
        std::string parent_id;
        int position = 0;
        bool nsfw = false;
        
        std::string get_mention() const {
            return "<#" + id + ">";
        }
    };

    /**
     * @brief Discord guild (server) information
     */
    struct DiscordGuild {
        std::string id;
        std::string name;
        std::string icon;
        std::string description;
        std::string owner_id;
        int member_count = 0;
        std::vector<DiscordChannel> channels;
        std::vector<DiscordUser> members;
        
        DiscordChannel* find_channel(const std::string& name_or_id) {
            for (auto& channel : channels) {
                if (channel.id == name_or_id || channel.name == name_or_id) {
                    return &channel;
                }
            }
            return nullptr;
        }
    };

    /**
     * @brief Discord message structure
     */
    struct DiscordMessage {
        std::string id;
        std::string content;
        std::string channel_id;
        std::string guild_id;
        DiscordUser author;
        std::chrono::system_clock::time_point timestamp;
        MessageType type;
        nlohmann::json embeds;
        nlohmann::json attachments;
        std::vector<std::string> reactions;
        bool tts = false;
        bool pinned = false;
        
        // Medusa-specific fields
        std::string medusa_command;
        nlohmann::json medusa_metadata;
        double ai_confidence = 0.0;
        std::string processed_response;
    };

    /**
     * @brief Discord embed builder
     */
    class DiscordEmbed {
    private:
        nlohmann::json embed_;
        
    public:
        DiscordEmbed() {
            embed_["type"] = "rich";
        }
        
        DiscordEmbed& set_title(const std::string& title) {
            embed_["title"] = title;
            return *this;
        }
        
        DiscordEmbed& set_description(const std::string& description) {
            embed_["description"] = description;
            return *this;
        }
        
        DiscordEmbed& set_color(uint32_t color) {
            embed_["color"] = color;
            return *this;
        }
        
        DiscordEmbed& set_footer(const std::string& text, const std::string& icon_url = "") {
            embed_["footer"]["text"] = text;
            if (!icon_url.empty()) {
                embed_["footer"]["icon_url"] = icon_url;
            }
            return *this;
        }
        
        DiscordEmbed& set_author(const std::string& name, const std::string& url = "", const std::string& icon_url = "") {
            embed_["author"]["name"] = name;
            if (!url.empty()) embed_["author"]["url"] = url;
            if (!icon_url.empty()) embed_["author"]["icon_url"] = icon_url;
            return *this;
        }
        
        DiscordEmbed& add_field(const std::string& name, const std::string& value, bool inline_field = false) {
            nlohmann::json field;
            field["name"] = name;
            field["value"] = value;
            field["inline"] = inline_field;
            
            if (!embed_.contains("fields")) {
                embed_["fields"] = nlohmann::json::array();
            }
            embed_["fields"].push_back(field);
            return *this;
        }
        
        DiscordEmbed& set_thumbnail(const std::string& url) {
            embed_["thumbnail"]["url"] = url;
            return *this;
        }
        
        DiscordEmbed& set_image(const std::string& url) {
            embed_["image"]["url"] = url;
            return *this;
        }
        
        DiscordEmbed& set_timestamp() {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) % 1000;
            
            std::stringstream ss;
            ss << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%S");
            ss << '.' << std::setfill('0') << std::setw(3) << ms.count() << 'Z';
            
            embed_["timestamp"] = ss.str();
            return *this;
        }
        
        // Medusa-specific embed styles
        DiscordEmbed& set_medusa_success() {
            set_color(0x00FF88);  // Medusa green
            set_footer("🐍 Powered by Medusa", "https://poweredbymedusa.com/icon.png");
            return *this;
        }
        
        DiscordEmbed& set_medusa_error() {
            set_color(0xFF4444);  // Error red
            set_footer("⚠️ Medusa Error", "https://poweredbymedusa.com/icon.png");
            return *this;
        }
        
        DiscordEmbed& set_medusa_info() {
            set_color(0x4488FF);  // Info blue
            set_footer("ℹ️ Medusa Info", "https://poweredbymedusa.com/icon.png");
            return *this;
        }
        
        nlohmann::json to_json() const {
            return embed_;
        }
    };

    /**
     * @brief Medusa Discord Bot - Revolutionary AI-powered Discord integration
     */
    class MedusaDiscordBot {
    private:
        // Connection details
        std::string bot_token_;
        std::string application_id_;
        std::string client_id_;
        std::string client_secret_;
        
        // WebSocket connection - Native implementation
        std::unique_ptr<WebSocketConnection> ws_connection_;
        std::string gateway_url_;
        
        // Connection state
        std::atomic<bool> connected_{false};
        std::atomic<bool> running_{false};
        std::atomic<int> heartbeat_interval_{0};
        std::atomic<int> last_sequence_{0};
        std::string session_id_;
        
        // Threading
        std::thread ws_thread_;
        std::thread heartbeat_thread_;
        std::thread message_processor_thread_;
        
        // Message queues
        std::queue<DiscordMessage> message_queue_;
        std::queue<nlohmann::json> outbound_queue_;
        std::mutex message_mutex_;
        std::mutex outbound_mutex_;
        // Note: condition_variable members removed for compilation
        // std::condition_variable message_cv_;
        // std::condition_variable outbound_cv_;
        
        // Event handlers
        std::function<void(const DiscordMessage&)> message_handler_;
        std::function<void(const DiscordUser&)> user_join_handler_;
        std::function<void(const DiscordUser&)> user_leave_handler_;
        std::function<void(const std::string&)> error_handler_;
        std::function<std::string(const std::string&, const DiscordUser&)> ai_command_handler_;
        
        // Guild information
        std::unordered_map<std::string, DiscordGuild> guilds_;
        std::mutex guilds_mutex_;
        
        // HTTP client for REST API (native implementation)
        MedusaHttp::NativeHttpClient http_client_;
        std::string api_base_url_ = "https://discord.com/api/v10";
        
        // Medusa-specific features
        bool medusa_ai_enabled_ = true;
        std::string medusa_command_prefix_ = "!medusa";
        std::unordered_map<std::string, std::function<std::string(const DiscordMessage&)>> custom_commands_;
        
        // Private methods
        void websocket_thread();
        void heartbeat_thread();
        void message_processor_thread();
        void handle_gateway_event(const nlohmann::json& payload);
        void handle_message_create(const nlohmann::json& data);
        void handle_guild_create(const nlohmann::json& data);
        void send_heartbeat();
        void identify();
        std::string make_http_request(const std::string& endpoint, const std::string& method = "GET", 
                                     const std::string& data = "", const std::vector<std::string>& headers = {});
        DiscordUser parse_user(const nlohmann::json& user_json);
        DiscordChannel parse_channel(const nlohmann::json& channel_json);
        DiscordMessage parse_message(const nlohmann::json& message_json);
        
    public:
        MedusaDiscordBot(const std::string& bot_token, const std::string& application_id = "");
        ~MedusaDiscordBot();
        
        // Connection management
        bool connect();
        void disconnect();
        bool is_connected() const { return connected_; }
        
        // Message operations
        bool send_message(const std::string& channel_id, const std::string& content);
        bool send_embed(const std::string& channel_id, const DiscordEmbed& embed);
        bool send_file(const std::string& channel_id, const std::string& filepath, const std::string& comment = "");
        bool edit_message(const std::string& channel_id, const std::string& message_id, const std::string& new_content);
        bool delete_message(const std::string& channel_id, const std::string& message_id);
        bool add_reaction(const std::string& channel_id, const std::string& message_id, const std::string& emoji);
        
        // Guild operations
        std::vector<DiscordGuild> get_guilds();
        DiscordGuild* get_guild(const std::string& guild_id);
        std::vector<DiscordChannel> get_channels(const std::string& guild_id);
        std::vector<DiscordUser> get_members(const std::string& guild_id);
        
        // User operations
        DiscordUser get_user(const std::string& user_id);
        DiscordUser get_bot_user();
        
        // Event handlers
        void set_message_handler(std::function<void(const DiscordMessage&)> handler) {
            message_handler_ = handler;
        }
        
        void set_user_join_handler(std::function<void(const DiscordUser&)> handler) {
            user_join_handler_ = handler;
        }
        
        void set_user_leave_handler(std::function<void(const DiscordUser&)> handler) {
            user_leave_handler_ = handler;
        }
        
        void set_error_handler(std::function<void(const std::string&)> handler) {
            error_handler_ = handler;
        }
        
        void set_ai_command_handler(std::function<std::string(const std::string&, const DiscordUser&)> handler) {
            ai_command_handler_ = handler;
        }
        
        // Medusa-specific features
        void enable_medusa_ai(bool enabled = true) { medusa_ai_enabled_ = enabled; }
        void set_medusa_prefix(const std::string& prefix) { medusa_command_prefix_ = prefix; }
        
        void add_custom_command(const std::string& command, 
                               std::function<std::string(const DiscordMessage&)> handler) {
            custom_commands_[command] = handler;
        }
        
        // Medusa system integration
        bool send_system_status(const std::string& channel_id, const std::string& system_name, 
                               const std::string& status, const std::string& details = "");
        bool send_github_notification(const std::string& channel_id, const nlohmann::json& github_event);
        bool send_ai_response(const std::string& channel_id, const std::string& user_query, 
                             const std::string& ai_response, double confidence);
        
        // Utility methods
        std::string get_invite_link(const std::vector<std::string>& permissions = {});
        void start_message_loop();
        void stop_message_loop();

        // Compatibility methods
        bool initialize() { return connect(); }
        bool is_member(const std::string& guild_id, const std::string& user_id);
        std::vector<std::string> get_member_roles(const std::string& guild_id, const std::string& user_id);
        bool send_dm(const std::string& user_id, const std::string& message);
    };

} // namespace MedusaDiscord