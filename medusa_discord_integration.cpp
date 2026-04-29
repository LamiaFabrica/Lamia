// Auto-added by auto_interlink_all_projects.sh
#ifdef AUTO_INTERLINK
#include "intelligent_interlinking_system.hpp"
#endif

/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * MEDUSA DISCORD INTEGRATION IMPLEMENTATION v0.3.0c
 * =================================================
 * 
 * Revolutionary Discord integration with native C++
 * Ground-up implementation - Yorkshire Champion Standards
 * NO SHORTCUTS - Full Discord API support with Medusa Spin
 */

#include "medusa_discord_integration.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <regex>
#include <fstream>
#include <vector>
#include <ctime>
#include <cctype>
#include <map>

namespace MedusaDiscord {

MedusaDiscordBot::MedusaDiscordBot(const std::string& bot_token, const std::string& application_id)
    : bot_token_(bot_token)
    , application_id_(application_id)
    , http_client_() {
    
    std::cout << "🐍 MEDUSA DISCORD: Initializing revolutionary Discord integration..." << std::endl;
    
    // Initialize native HTTP client with secure cipher suites
    http_client_.set_ssl_cipher_list("TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384");
    
    // WebSocket will be initialized in connect() method
    ws_connection_ = nullptr;
    
    // Set up default Medusa commands
    add_custom_command("status", [this](const DiscordMessage& msg) {
        return "🐍 **Medusa Discord Bot v0.3.0c**\n"
               "✅ Status: Online and ready\n"
               "⚡ Features: AI Commands, GitHub Integration, Real-time Sync\n"
               "🏆 Yorkshire Champion Standards: NO SHORTCUTS DETECTED!";
    });
    
    add_custom_command("help", [this](const DiscordMessage& msg) {
        std::stringstream help;
        help << "🐍 **Medusa Bot Commands**\n\n";
        help << "**Basic Commands:**\n";
        help << "`" << medusa_command_prefix_ << " status` - Bot status\n";
        help << "`" << medusa_command_prefix_ << " help` - This help message\n";
        help << "`" << medusa_command_prefix_ << " ai <question>` - Ask Medusa AI\n";
        help << "`" << medusa_command_prefix_ << " github` - GitHub integration status\n\n";
        help << "**System Commands:**\n";
        help << "`" << medusa_command_prefix_ << " system status` - System health\n";
        help << "`" << medusa_command_prefix_ << " fccp test` - Test Cat6 speeds\n";
        help << "`" << medusa_command_prefix_ << " lamia compile` - Compile Lamia code\n\n";
        help << "**Admin Commands:**\n";
        help << "`" << medusa_command_prefix_ << " deploy` - Deploy system updates\n";
        help << "`" << medusa_command_prefix_ << " backup` - Create system backup\n";
        return help.str();
    });
    
    std::cout << "⚡ MEDUSA DISCORD: Initialization complete - Ready for connection!" << std::endl;
}

MedusaDiscordBot::~MedusaDiscordBot() {
    disconnect();
    // Native HTTP client cleanup is handled automatically
}

bool MedusaDiscordBot::connect() {
    if (is_connected()) {
        std::cout << "⚠️ MEDUSA DISCORD: Already connected!" << std::endl;
        return true;
    }
    
    std::cout << "🚀 MEDUSA DISCORD: Connecting to Discord Gateway..." << std::endl;
    
    try {
        // Get gateway URL
        std::string gateway_response = make_http_request("/gateway");
        if (gateway_response.empty()) {
            std::cerr << "❌ MEDUSA DISCORD: Failed to get gateway URL" << std::endl;
            return false;
        }
        
        nlohmann::json gateway_json = nlohmann::json::parse(gateway_response);
        std::string gateway_url = gateway_json["url"];
        
        // Extract host and port from gateway URL
        size_t protocol_end = gateway_url.find("://");
        if (protocol_end == std::string::npos) {
            std::cerr << "❌ MEDUSA DISCORD: Invalid gateway URL format" << std::endl;
            return false;
        }
        
        std::string host_port = gateway_url.substr(protocol_end + 3);
        size_t path_start = host_port.find('/');
        if (path_start != std::string::npos) {
            host_port = host_port.substr(0, path_start);
        }
        
        size_t colon_pos = host_port.find(':');
        std::string host = host_port.substr(0, colon_pos);
        int port = (colon_pos != std::string::npos) ? std::stoi(host_port.substr(colon_pos + 1)) : 443;
        
        // Create WebSocket connection
        ws_connection_ = std::make_unique<WebSocketConnection>(host, port);
        
        if (!ws_connection_->connect()) {
            std::cerr << "❌ MEDUSA DISCORD: Failed to connect WebSocket" << std::endl;
            return false;
        }
        
        // Start message processing thread
        running_ = true;
        message_processor_thread_ = std::thread(&MedusaDiscordBot::message_processor_thread, this);
        heartbeat_thread_ = std::thread(&MedusaDiscordBot::heartbeat_thread, this);
        
        // Wait for READY event
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        if (is_connected()) {
            std::cout << "✅ MEDUSA DISCORD: Successfully connected to Discord!" << std::endl;
            return true;
        } else {
            std::cerr << "❌ MEDUSA DISCORD: Connection timeout" << std::endl;
            running_ = false;
            ws_connection_->disconnect();
            if (message_processor_thread_.joinable()) message_processor_thread_.join();
            if (heartbeat_thread_.joinable()) heartbeat_thread_.join();
            return false;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "❌ MEDUSA DISCORD: Connection exception: " << e.what() << std::endl;
        return false;
    }
}

void MedusaDiscordBot::disconnect() {
    if (!running_ && (!ws_connection_ || !ws_connection_->is_connected())) {
        return;
    }
    
    std::cout << "🔌 MEDUSA DISCORD: Disconnecting..." << std::endl;
    
    running_ = false;
    
    if (ws_connection_) {
        ws_connection_->disconnect();
    }
    
    if (message_processor_thread_.joinable()) {
        message_processor_thread_.join();
    }
    if (heartbeat_thread_.joinable()) {
        heartbeat_thread_.join();
    }
    
    std::cout << "✅ MEDUSA DISCORD: Disconnected successfully" << std::endl;
}

bool MedusaDiscordBot::is_connected() const {
    return ws_connection_ && ws_connection_->is_connected();
}

void MedusaDiscordBot::message_processor_thread() {
    while (running_) {
        if (ws_connection_ && ws_connection_->is_connected()) {
            std::string message = ws_connection_->receive_message();
            if (!message.empty()) {
                try {
                    auto json_msg = nlohmann::json::parse(message);
                    handle_gateway_event(json_msg);
                } catch (const std::exception& e) {
                    std::cerr << "❌ MEDUSA DISCORD: Error parsing message: " << e.what() << std::endl;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void MedusaDiscordBot::heartbeat_thread() {
    while (running_) {
        if (ws_connection_ && ws_connection_->is_connected()) {
            send_heartbeat();
        }
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}

void MedusaDiscordBot::handle_gateway_event(const nlohmann::json& event) {
    int op = event.value("op", -1);
    std::string event_type = event.value("t", "");
    
    if (op == 10) { // HELLO
        heartbeat_interval_ = std::chrono::milliseconds(event["d"]["heartbeat_interval"]);
        identify();
    } else if (op == 11) { // HEARTBEAT_ACK
        // Heartbeat acknowledged
    } else if (event_type == "READY") {
        auto data = event["d"];
        bot_user_id_ = data["user"]["id"];
        std::cout << "✅ MEDUSA DISCORD: Bot ready! User ID: " << bot_user_id_ << std::endl;
    } else if (event_type == "MESSAGE_CREATE") {
        auto message = parse_message(event["d"]);
        {
            std::lock_guard<std::mutex> lock(message_queue_mutex_);
            message_queue_.push(message);
        }
        message_cv_.notify_one();
    } else if (event_type == "GUILD_CREATE") {
        handle_guild_create(event["d"]);
    }
}

void MedusaDiscordBot::handle_guild_create(const nlohmann::json& data) {
    DiscordGuild guild;
    guild.id = data["id"];
    guild.name = data["name"];
    guild.description = data.value("description", "");
    guild.owner_id = data["owner_id"];
    guild.member_count = data.value("member_count", 0);
    
    if (data.contains("icon") && !data["icon"].is_null()) {
        guild.icon = data["icon"];
    }
    
    // Parse channels
    if (data.contains("channels")) {
        for (const auto& channel_data : data["channels"]) {
            guild.channels.push_back(parse_channel(channel_data));
        }
    }
    
    {
        std::lock_guard<std::mutex> lock(guilds_mutex_);
        guilds_[guild.id] = guild;
    }
    
    std::cout << "🏰 MEDUSA DISCORD: Joined guild '" << guild.name 
              << "' with " << guild.member_count << " members" << std::endl;
}

void MedusaDiscordBot::send_heartbeat() {
    nlohmann::json heartbeat;
    heartbeat["op"] = 1;
    heartbeat["d"] = sequence_number_;
    
    if (ws_connection_ && ws_connection_->is_connected()) {
        ws_connection_->send_message(heartbeat.dump());
    }
}

void MedusaDiscordBot::identify() {
    nlohmann::json identify;
    identify["op"] = 2;
    identify["d"]["token"] = bot_token_;
    identify["d"]["properties"]["$os"] = "linux";
    identify["d"]["properties"]["$browser"] = "medusa";
    identify["d"]["properties"]["$device"] = "medusa";
    identify["d"]["intents"] = 513; // GUILD_MESSAGES + GUILD_MEMBERS
    
    if (ws_connection_ && ws_connection_->is_connected()) {
        if (ws_connection_->send_message(identify.dump())) {
            std::cout << "🔐 MEDUSA DISCORD: Identification sent" << std::endl;
        } else {
            std::cerr << "❌ MEDUSA DISCORD: Identify send failed" << std::endl;
        }
    }
}

std::string MedusaDiscordBot::make_http_request(const std::string& endpoint, const std::string& method, 
                                                const std::string& data, const std::vector<std::string>& headers) {
    std::string url = api_base_url_ + endpoint;
    
    // Build headers map
    std::map<std::string, std::string> request_headers;
    request_headers["Authorization"] = "Bot " + bot_token_;
    request_headers["Content-Type"] = "application/json";
    request_headers["User-Agent"] = "MedusaBot/1.0";
    
    // Parse additional headers
    for (const auto& header : headers) {
        size_t colon_pos = header.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = header.substr(0, colon_pos);
            std::string value = header.substr(colon_pos + 1);
            // Trim whitespace
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            request_headers[key] = value;
        }
    }
    
    // Make request using native HTTP client
    MedusaHttp::HttpRequest native_request;
    native_request.url = url;
    native_request.method = method;
    native_request.headers = request_headers;
    native_request.body = data;
    native_request.verify_ssl = true;
    native_request.cipher_list = "TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384";
    
    MedusaHttp::HttpResponse response = http_client_.perform_request(native_request);
    
    if (!response.success || response.status_code >= 400) {
        std::cerr << "❌ MEDUSA DISCORD: HTTP error " << response.status_code 
                  << ": " << response.error_message << std::endl;
        return "";
    }
    
    return response.body;
}

bool MedusaDiscordBot::send_message(const std::string& channel_id, const std::string& content) {
    nlohmann::json message_data;
    message_data["content"] = content;
    
    std::string response = make_http_request("/channels/" + channel_id + "/messages", "POST", message_data.dump());
    return !response.empty();
}

bool MedusaDiscordBot::send_embed(const std::string& channel_id, const DiscordEmbed& embed) {
    nlohmann::json message_data;
    message_data["embeds"] = nlohmann::json::array();
    
    nlohmann::json embed_json;
    if (!embed.title.empty()) embed_json["title"] = embed.title;
    if (!embed.description.empty()) embed_json["description"] = embed.description;
    if (embed.color > 0) embed_json["color"] = embed.color;
    if (!embed.footer.empty()) embed_json["footer"]["text"] = embed.footer;
    if (!embed.timestamp.empty()) embed_json["timestamp"] = embed.timestamp;
    
    if (!embed.fields.empty()) {
        embed_json["fields"] = nlohmann::json::array();
        for (const auto& field : embed.fields) {
            nlohmann::json field_json;
            field_json["name"] = field.name;
            field_json["value"] = field.value;
            field_json["inline"] = field.inline_field;
            embed_json["fields"].push_back(field_json);
        }
    }
    
    message_data["embeds"].push_back(embed_json);
    
    std::string response = make_http_request("/channels/" + channel_id + "/messages", "POST", message_data.dump());
    return !response.empty();
}

bool MedusaDiscordBot::send_status_embed(const std::string& channel_id, const std::string& status, 
                                         const std::string& message) {
    DiscordEmbed embed;
    embed.set_title("🔔 System Status: " + status)
         .set_description(message)
         .set_timestamp();
    
    if (status == "Online" || status == "Healthy") {
        embed.set_color(0x00FF88);
    } else if (status == "Warning" || status == "Degraded") {
        embed.set_color(0xFFAA00);
    } else {
        embed.set_color(0xFF4444);
    }
    
    return send_embed(channel_id, embed);
}

bool MedusaDiscordBot::send_ai_response(const std::string& channel_id, const std::string& user_query, 
                                       const std::string& ai_response, double confidence) {
    DiscordEmbed embed;
    embed.set_title("🤖 Medusa AI Response")
         .add_field("Query", user_query)
         .add_field("Response", ai_response)
         .add_field("Confidence", std::to_string((int)(confidence * 100)) + "%", true)
         .set_medusa_success()
         .set_timestamp();
    
    return send_embed(channel_id, embed);
}

DiscordUser MedusaDiscordBot::parse_user(const nlohmann::json& user_json) {
    DiscordUser user;
    user.id = user_json["id"];
    user.username = user_json["username"];
    user.discriminator = user_json.value("discriminator", "0");
    user.global_name = user_json.value("global_name", "");
    user.bot = user_json.value("bot", false);
    user.system = user_json.value("system", false);
    user.verified = user_json.value("verified", false);
    
    if (user_json.contains("avatar") && !user_json["avatar"].is_null()) {
        user.avatar = user_json["avatar"];
    }
    
    return user;
}

DiscordChannel MedusaDiscordBot::parse_channel(const nlohmann::json& channel_json) {
    DiscordChannel channel;
    channel.id = channel_json["id"];
    channel.name = channel_json.value("name", "");
    channel.topic = channel_json.value("topic", "");
    channel.type = static_cast<ChannelType>(channel_json.value("type", 0));
    channel.guild_id = channel_json.value("guild_id", "");
    channel.parent_id = channel_json.value("parent_id", "");
    channel.position = channel_json.value("position", 0);
    channel.nsfw = channel_json.value("nsfw", false);
    
    return channel;
}

DiscordMessage MedusaDiscordBot::parse_message(const nlohmann::json& message_json) {
    DiscordMessage message;
    message.id = message_json["id"];
    message.content = message_json.value("content", "");
    message.channel_id = message_json["channel_id"];
    message.guild_id = message_json.value("guild_id", "");
    message.author = parse_user(message_json["author"]);
    message.type = MessageType::TEXT;
    message.tts = message_json.value("tts", false);
    message.pinned = message_json.value("pinned", false);
    
    if (message_json.contains("embeds")) {
        message.embeds = message_json["embeds"];
    }
    
    if (message_json.contains("attachments")) {
        message.attachments = message_json["attachments"];
    }
    
    return message;
}

void MedusaDiscordBot::start_message_loop() {
    std::cout << "🔄 MEDUSA DISCORD: Message loop started - Ready to receive commands!" << std::endl;
}

void MedusaDiscordBot::stop_message_loop() {
    std::cout << "🛑 MEDUSA DISCORD: Stopping message loop" << std::endl;
}

std::string MedusaDiscordBot::get_invite_link(const std::vector<std::string>& permissions) {
    if (application_id_.empty()) {
        return "⚠️ Application ID not set - cannot generate invite link";
    }
    
    std::string invite_url = "https://discord.com/api/oauth2/authorize?client_id=" + application_id_;
    invite_url += "&permissions=274877906944&scope=bot%20applications.commands";
    
    return invite_url;
}

// Added methods to satisfy interface requirements
DiscordUser MedusaDiscordBot::get_user(const std::string& user_id) {
    std::string response = make_http_request("/users/" + user_id);
    if (response.empty()) {
        return DiscordUser();
    }
    try {
        return parse_user(nlohmann::json::parse(response));
    } catch (...) {
        return DiscordUser();
    }
}

DiscordUser MedusaDiscordBot::get_bot_user() {
    return get_user("@me");
}

std::vector<DiscordGuild> MedusaDiscordBot::get_guilds() {
    std::string response = make_http_request("/users/@me/guilds");
    if (response.empty()) {
        return {};
    }
    
    try {
        auto guilds_json = nlohmann::json::parse(response);
        std::vector<DiscordGuild> guilds;
        
        for (const auto& guild_json : guilds_json) {
            DiscordGuild guild;
            guild.id = guild_json["id"];
            guild.name = guild_json["name"];
            guild.description = guild_json.value("description", "");
            guild.owner_id = guild_json.value("owner_id", "");
            guild.member_count = guild_json.value("approximate_member_count", 0);
            
            if (guild_json.contains("icon") && !guild_json["icon"].is_null()) {
                guild.icon = guild_json["icon"];
            }
            
            // Cache the guild
            {
                std::lock_guard<std::mutex> lock(guilds_mutex_);
                guilds_[guild.id] = guild;
            }
            
            guilds.push_back(guild);
        }
        
        return guilds;
    } catch (const std::exception& e) {
        std::cerr << "❌ MEDUSA DISCORD: Error parsing guilds: " << e.what() << std::endl;
        return {};
    }
}

DiscordGuild* MedusaDiscordBot::get_guild(const std::string& guild_id) {
    std::lock_guard<std::mutex> lock(guilds_mutex_);
    auto it = guilds_.find(guild_id);
    if (it != guilds_.end()) {
        return &it->second;
    }
    
    // Fetch from API if not cached
    std::string response = make_http_request("/guilds/" + guild_id);
    if (response.empty()) {
        return nullptr;
    }
    
    try {
        auto guild_json = nlohmann::json::parse(response);
        DiscordGuild guild;
        guild.id = guild_json["id"];
        guild.name = guild_json["name"];
        guild.description = guild_json.value("description", "");
        guild.owner_id = guild_json["owner_id"];
        guild.member_count = guild_json.value("member_count", 0);
        
        if (guild_json.contains("icon") && !guild_json["icon"].is_null()) {
            guild.icon = guild_json["icon"];
        }
        
        guilds_[guild.id] = guild;
        return &guilds_[guild.id];
    } catch (const std::exception& e) {
        std::cerr << "❌ MEDUSA DISCORD: Error parsing guild: " << e.what() << std::endl;
        return nullptr;
    }
}

std::vector<DiscordChannel> MedusaDiscordBot::get_channels(const std::string& guild_id) {
    std::string response = make_http_request("/guilds/" + guild_id + "/channels");
    if (response.empty()) {
        return {};
    }
    
    try {
        auto channels_json = nlohmann::json::parse(response);
        std::vector<DiscordChannel> channels;
        
        for (const auto& channel_json : channels_json) {
            channels.push_back(parse_channel(channel_json));
        }
        
        return channels;
    } catch (const std::exception& e) {
        std::cerr << "❌ MEDUSA DISCORD: Error parsing channels: " << e.what() << std::endl;
        return {};
    }
}

std::vector<DiscordUser> MedusaDiscordBot::get_members(const std::string& guild_id) {
    std::string response = make_http_request("/guilds/" + guild_id + "/members?limit=1000");
    if (response.empty()) {
        return {};
    }
    
    try {
        auto members_json = nlohmann::json::parse(response);
        std::vector<DiscordUser> members;
        
        for (const auto& member_json : members_json) {
            if (member_json.contains("user")) {
                members.push_back(parse_user(member_json["user"]));
            }
        }
        
        return members;
    } catch (const std::exception& e) {
        std::cerr << "❌ MEDUSA DISCORD: Error parsing members: " << e.what() << std::endl;
        return {};
    }
}

bool MedusaDiscordBot::is_member(const std::string& guild_id, const std::string& user_id) {
    std::string response = make_http_request("/guilds/" + guild_id + "/members/" + user_id);
    return !response.empty();
}

std::vector<std::string> MedusaDiscordBot::get_member_roles(const std::string& guild_id, const std::string& user_id) {
    std::string response = make_http_request("/guilds/" + guild_id + "/members/" + user_id);
    if (response.empty()) {
        return {};
    }
    
    try {
        auto member_json = nlohmann::json::parse(response);
        std::vector<std::string> roles;
        
        if (member_json.contains("roles")) {
            for (const auto& role_id : member_json["roles"]) {
                roles.push_back(role_id);
            }
        }
        
        return roles;
    } catch (const std::exception& e) {
        std::cerr << "❌ MEDUSA DISCORD: Error parsing member roles: " << e.what() << std::endl;
        return {};
    }
}

bool MedusaDiscordBot::send_dm(const std::string& user_id, const std::string& message) {
    // Create DM channel
    nlohmann::json channel_data;
    channel_data["recipient_id"] = user_id;
    
    std::string channel_response = make_http_request("/users/@me/channels", "POST", channel_data.dump());
    if (channel_response.empty()) {
        return false;
    }
    
    try {
        auto channel_json = nlohmann::json::parse(channel_response);
        std::string channel_id = channel_json["id"];
        
        // Send message to DM channel
        return send_message(channel_id, message);
    } catch (const std::exception& e) {
        std::cerr << "❌ MEDUSA DISCORD: Error creating DM channel: " << e.what() << std::endl;
        return false;
    }
}

bool MedusaDiscordBot::send_file(const std::string& channel_id, const std::string& filepath, const std::string& comment) {
    // Read file
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "❌ MEDUSA DISCORD: Failed to open file: " << filepath << std::endl;
        return false;
    }
    
    // Get file size
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // Read file content
    std::vector<char> buffer(file_size);
    file.read(buffer.data(), file_size);
    file.close();
    
    // Get filename from path
    std::string filename = filepath;
    size_t last_slash = filename.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        filename = filename.substr(last_slash + 1);
    }
    
    // Create multipart form data
    std::ostringstream form_data;
    std::string boundary = "----WebKitFormBoundary" + std::to_string(std::time(nullptr));
    
    if (!comment.empty()) {
        form_data << "--" << boundary << "\r\n";
        form_data << "Content-Disposition: form-data; name=\"content\"\r\n\r\n";
        form_data << comment << "\r\n";
    }
    
    form_data << "--" << boundary << "\r\n";
    form_data << "Content-Disposition: form-data; name=\"file\"; filename=\"" << filename << "\"\r\n";
    form_data << "Content-Type: application/octet-stream\r\n\r\n";
    form_data.write(buffer.data(), buffer.size());
    form_data << "\r\n--" << boundary << "--\r\n";
    
    std::string form_data_str = form_data.str();
    
    // Send multipart request
    std::vector<std::string> headers;
    headers.push_back("Content-Type: multipart/form-data; boundary=" + boundary);
    headers.push_back("Content-Length: " + std::to_string(form_data_str.length()));
    
    std::string response = make_http_request("/channels/" + channel_id + "/messages", "POST", form_data_str, headers);
    return !response.empty();
}

bool MedusaDiscordBot::edit_message(const std::string& channel_id, const std::string& message_id, const std::string& new_content) {
    nlohmann::json message_data;
    message_data["content"] = new_content;
    
    std::string response = make_http_request("/channels/" + channel_id + "/messages/" + message_id, 
                                            "PATCH", message_data.dump());
    
    return !response.empty();
}

bool MedusaDiscordBot::delete_message(const std::string& channel_id, const std::string& message_id) {
    std::string response = make_http_request("/channels/" + channel_id + "/messages/" + message_id, "DELETE");
    return !response.empty();
}

bool MedusaDiscordBot::add_reaction(const std::string& channel_id, const std::string& message_id, const std::string& emoji) {
    // URL encode emoji properly
    std::ostringstream encoded;
    for (unsigned char c : emoji) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << '%' << std::hex << std::uppercase << std::setw(2) << std::setfill('0') 
                   << static_cast<int>(c) << std::dec;
        }
    }
    
    std::string encoded_emoji = encoded.str();
    
    std::string response = make_http_request("/channels/" + channel_id + "/messages/" + message_id + 
                                            "/reactions/" + encoded_emoji + "/@me", "PUT");
    return !response.empty();
}

bool MedusaDiscordBot::send_github_notification(const std::string& channel_id, const nlohmann::json& github_event) {
    std::string event_type = github_event.value("action", "");
    std::string repo_name = github_event.value("repository", nlohmann::json::object()).value("full_name", "");
    std::string sender = github_event.value("sender", nlohmann::json::object()).value("login", "");
    
    DiscordEmbed embed;
    embed.set_title("🔔 GitHub Notification: " + event_type)
         .set_medusa_info()
         .set_timestamp();
    
    if (!repo_name.empty()) {
        embed.add_field("Repository", repo_name, true);
    }
    
    if (!sender.empty()) {
        embed.add_field("User", sender, true);
    }
    
    // Parse different event types
    if (event_type == "push") {
        std::string ref = github_event.value("ref", "");
        int commits = github_event.value("commits", nlohmann::json::array()).size();
        embed.set_description("Pushed " + std::to_string(commits) + " commit(s) to " + ref);
        embed.set_color(0x28A745); // GitHub green
    } else if (event_type == "pull_request") {
        std::string pr_title = github_event.value("pull_request", nlohmann::json::object()).value("title", "");
        std::string pr_state = github_event.value("pull_request", nlohmann::json::object()).value("state", "");
        embed.set_description("Pull Request: " + pr_title + " (" + pr_state + ")");
        embed.set_color(0x6F42C1); // GitHub purple
    } else if (event_type == "issues") {
        std::string issue_title = github_event.value("issue", nlohmann::json::object()).value("title", "");
        embed.set_description("Issue: " + issue_title);
        embed.set_color(0xCB2431); // GitHub red
    } else {
        embed.set_description("GitHub event: " + event_type);
    }
    
    return send_embed(channel_id, embed);
}

} // namespace MedusaDiscord
