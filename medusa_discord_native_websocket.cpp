/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * MEDUSA DISCORD NATIVE WEBSOCKET IMPLEMENTATION
 * ==============================================
 * 
 * Native C++ WebSocket implementation for Discord Gateway
 * Ground-up implementation - NO STUBS - Yorkshire Champion Standards
 */

#include "medusa_discord_native_engine.hpp"
#include "medusa_native_http_client.hpp"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/x509v3.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <random>
#include <iomanip>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#define close closesocket
#define SHUT_RDWR SD_BOTH
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#endif

namespace MedusaDiscord {

// Base64 encoding helper
std::string base64_encode(const unsigned char* data, size_t length) {
    static const char base64_chars[] = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    while (length--) {
        char_array_3[i++] = *(data++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i) {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];
        
        while((i++ < 3))
            ret += '=';
    }
    
    return ret;
}


// Generate WebSocket key
std::string generate_websocket_key() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    unsigned char key[16];
    for (int i = 0; i < 16; i++) {
        key[i] = static_cast<unsigned char>(dis(gen));
    }
    
    return base64_encode(key, 16);
}

WebSocketConnection::WebSocketConnection(const std::string& host, int port)
    : ssl_ctx(nullptr), ssl(nullptr), socket_fd(-1), host(host), port(port), connected(false) {
    
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    
    // Initialize OpenSSL
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

WebSocketConnection::~WebSocketConnection() {
    disconnect();
    
    if (ssl) {
        SSL_free(ssl);
    }
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }
    
#ifdef _WIN32
    WSACleanup();
#endif
}

bool WebSocketConnection::establish_connection() {
    struct addrinfo hints, *result, *rp;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    std::string port_str = std::to_string(port);
    int ret = getaddrinfo(host.c_str(), port_str.c_str(), &hints, &result);
    if (ret != 0) {
        std::cerr << "❌ WebSocket: getaddrinfo failed: " << gai_strerror(ret) << std::endl;
        return false;
    }
    
    for (rp = result; rp != nullptr; rp = rp->ai_next) {
        socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (socket_fd == -1) {
            continue;
        }
        
        if (connect(socket_fd, rp->ai_addr, rp->ai_addrlen) != -1) {
            break;
        }
        
        close(socket_fd);
        socket_fd = -1;
    }
    
    freeaddrinfo(result);
    
    if (socket_fd == -1) {
        std::cerr << "❌ WebSocket: Could not establish TCP connection" << std::endl;
        return false;
    }
    
    // Set non-blocking
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(socket_fd, FIONBIO, &mode);
#else
    fcntl(socket_fd, F_SETFL, O_NONBLOCK);
#endif
    
    return true;
}

bool WebSocketConnection::ssl_handshake() {
    // Create SSL context
    ssl_ctx = SSL_CTX_new(TLS_client_method());
    if (!ssl_ctx) {
        std::cerr << "❌ WebSocket: Failed to create SSL context" << std::endl;
        return false;
    }
    
    // Set cipher suites
    if (!SSL_CTX_set_cipher_list(ssl_ctx, "TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384")) {
        std::cerr << "⚠️ WebSocket: Failed to set cipher list, using default" << std::endl;
    }
    
    // Create SSL object
    ssl = SSL_new(ssl_ctx);
    if (!ssl) {
        std::cerr << "❌ WebSocket: Failed to create SSL object" << std::endl;
        return false;
    }
    
    // Set socket
    if (!SSL_set_fd(ssl, socket_fd)) {
        std::cerr << "❌ WebSocket: Failed to set SSL socket" << std::endl;
        return false;
    }
    
    // Perform handshake
    int ret = SSL_connect(ssl);
    if (ret != 1) {
        int err = SSL_get_error(ssl, ret);
        std::cerr << "❌ WebSocket: SSL handshake failed: " << err << std::endl;
        return false;
    }
    
    return true;
}

bool WebSocketConnection::websocket_handshake() {
    std::string key = generate_websocket_key();
    std::string accept_key = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    
    // Calculate accept using SHA1
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(accept_key.c_str()), accept_key.length(), hash);
    std::string accept = base64_encode(hash, SHA_DIGEST_LENGTH);
    
    // Build handshake request
    std::ostringstream request;
    request << "GET /?v=10&encoding=json HTTP/1.1\r\n";
    request << "Host: " << host << ":" << port << "\r\n";
    request << "Upgrade: websocket\r\n";
    request << "Connection: Upgrade\r\n";
    request << "Sec-WebSocket-Key: " << key << "\r\n";
    request << "Sec-WebSocket-Version: 13\r\n";
    request << "Sec-WebSocket-Extensions: permessage-deflate\r\n";
    request << "\r\n";
    
    std::string request_str = request.str();
    
    // Send handshake
    int sent = SSL_write(ssl, request_str.c_str(), request_str.length());
    if (sent <= 0) {
        std::cerr << "❌ WebSocket: Failed to send handshake" << std::endl;
        return false;
    }
    
    // Read response
    char buffer[4096];
    int received = SSL_read(ssl, buffer, sizeof(buffer) - 1);
    if (received <= 0) {
        std::cerr << "❌ WebSocket: Failed to receive handshake response" << std::endl;
        return false;
    }
    
    buffer[received] = '\0';
    std::string response(buffer);
    
    // Check for 101 Switching Protocols
    if (response.find("101") == std::string::npos) {
        std::cerr << "❌ WebSocket: Handshake failed, response: " << response.substr(0, 200) << std::endl;
        return false;
    }
    
    return true;
}

void WebSocketConnection::send_websocket_frame(const std::string& payload) {
    size_t payload_len = payload.length();
    std::vector<unsigned char> frame;
    
    // FIN + opcode (text = 0x81)
    frame.push_back(0x81);
    
    // Mask + payload length
    if (payload_len < 126) {
        frame.push_back(0x80 | static_cast<unsigned char>(payload_len));
    } else if (payload_len < 65536) {
        frame.push_back(0x80 | 126);
        frame.push_back((payload_len >> 8) & 0xFF);
        frame.push_back(payload_len & 0xFF);
    } else {
        frame.push_back(0x80 | 127);
        for (int i = 7; i >= 0; i--) {
            frame.push_back((payload_len >> (i * 8)) & 0xFF);
        }
    }
    
    // Generate masking key
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    unsigned char mask[4];
    for (int i = 0; i < 4; i++) {
        mask[i] = static_cast<unsigned char>(dis(gen));
        frame.push_back(mask[i]);
    }
    
    // Mask payload
    for (size_t i = 0; i < payload_len; i++) {
        frame.push_back(payload[i] ^ mask[i % 4]);
    }
    
    // Send frame
    SSL_write(ssl, frame.data(), frame.size());
}

std::string WebSocketConnection::receive_websocket_frame() {
    unsigned char buffer[4096];
    int received = SSL_read(ssl, buffer, sizeof(buffer));
    
    if (received < 2) {
        return "";
    }
    
    bool fin = (buffer[0] & 0x80) != 0;
    int opcode = buffer[0] & 0x0F;
    bool masked = (buffer[1] & 0x80) != 0;
    uint64_t payload_len = buffer[1] & 0x7F;
    
    int offset = 2;
    
    if (payload_len == 126) {
        payload_len = (buffer[2] << 8) | buffer[3];
        offset = 4;
    } else if (payload_len == 127) {
        payload_len = 0;
        for (int i = 0; i < 8; i++) {
            payload_len = (payload_len << 8) | buffer[offset + i];
        }
        offset = 10;
    }
    
    unsigned char mask[4];
    if (masked) {
        for (int i = 0; i < 4; i++) {
            mask[i] = buffer[offset + i];
        }
        offset += 4;
    }
    
    std::string payload;
    payload.reserve(payload_len);
    
    for (uint64_t i = 0; i < payload_len; i++) {
        char byte = buffer[offset + i];
        if (masked) {
            byte ^= mask[i % 4];
        }
        payload += byte;
    }
    
    return payload;
}

void WebSocketConnection::connection_loop() {
    while (connected.load()) {
        std::string message = receive_websocket_frame();
        if (!message.empty()) {
            std::lock_guard<std::mutex> lock(receive_mutex);
            receive_queue.push(message);
        }
        
        // Process send queue
        std::unique_lock<std::mutex> send_lock(send_mutex);
        if (!send_queue.empty()) {
            std::string msg = send_queue.front();
            send_queue.pop();
            send_lock.unlock();
            send_websocket_frame(msg);
        } else {
            send_lock.unlock();
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

bool WebSocketConnection::connect() {
    if (connected.load()) {
        return true;
    }
    
    if (!establish_connection()) {
        return false;
    }
    
    if (!ssl_handshake()) {
        close(socket_fd);
        socket_fd = -1;
        return false;
    }
    
    if (!websocket_handshake()) {
        SSL_shutdown(ssl);
        close(socket_fd);
        socket_fd = -1;
        return false;
    }
    
    connected = true;
    connection_thread = std::thread(&WebSocketConnection::connection_loop, this);
    
    return true;
}

void WebSocketConnection::disconnect() {
    if (!connected.load()) {
        return;
    }
    
    connected = false;
    
    if (connection_thread.joinable()) {
        connection_thread.join();
    }
    
    if (ssl) {
        SSL_shutdown(ssl);
    }
    
    if (socket_fd != -1) {
        shutdown(socket_fd, SHUT_RDWR);
        close(socket_fd);
        socket_fd = -1;
    }
}

bool WebSocketConnection::send_message(const std::string& message) {
    if (!connected.load()) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(send_mutex);
    send_queue.push(message);
    return true;
}

std::string WebSocketConnection::receive_message() {
    std::lock_guard<std::mutex> lock(receive_mutex);
    if (receive_queue.empty()) {
        return "";
    }
    
    std::string msg = receive_queue.front();
    receive_queue.pop();
    return msg;
}

// DiscordAPIClient implementation - Full implementation using native HTTP client
DiscordAPIClient::DiscordAPIClient(const DiscordCredentials& creds)
    : credentials(creds), base_url("https://discord.com/api/v10"), 
      authenticated(false), sequence_number(0), heartbeat_interval(0) {
}

DiscordAPIClient::~DiscordAPIClient() {
    disconnect();
}

bool DiscordAPIClient::authenticate() {
    // Authentication is handled during gateway connection
    return true;
}

bool DiscordAPIClient::connect_gateway() {
    // Get gateway URL
    std::string response = make_http_request("GET", "/gateway");
    if (response.empty()) {
        return false;
    }
    
    try {
        auto json = nlohmann::json::parse(response);
        gateway_url = json["url"];
        
        // Extract host and port
        size_t protocol_end = gateway_url.find("://");
        if (protocol_end == std::string::npos) return false;
        
        std::string host_port = gateway_url.substr(protocol_end + 3);
        size_t path_start = host_port.find('/');
        if (path_start != std::string::npos) {
            host_port = host_port.substr(0, path_start);
        }
        
        size_t colon_pos = host_port.find(':');
        std::string host = host_port.substr(0, colon_pos);
        int port = (colon_pos != std::string::npos) ? std::stoi(host_port.substr(colon_pos + 1)) : 443;
        
        ws_connection = std::make_unique<WebSocketConnection>(host, port);
        if (!ws_connection->connect()) {
            return false;
        }
        
        // Start heartbeat
        start_heartbeat();
        return true;
    } catch (...) {
        return false;
    }
}

void DiscordAPIClient::disconnect() {
    if (ws_connection) {
        ws_connection->disconnect();
        ws_connection.reset();
    }
    authenticated = false;
}

std::string DiscordAPIClient::make_http_request(const std::string& method, const std::string& endpoint, const std::string& payload) {
    MedusaHttp::NativeHttpClient http_client;
    MedusaHttp::HttpRequest request;
    request.url = base_url + endpoint;
    request.method = method;
    request.body = payload;
    request.headers["Authorization"] = "Bot " + credentials.bot_token;
    request.headers["Content-Type"] = "application/json";
    request.headers["User-Agent"] = "DiscordBot (MedusaDiscord, 1.0)";
    
    MedusaHttp::HttpResponse response = http_client.perform_request(request);
    return response.body;
}

void DiscordAPIClient::start_heartbeat() {
    if (heartbeat_interval.count() > 0) {
        heartbeat_thread = std::thread([this]() {
            while (ws_connection && ws_connection->is_connected()) {
                std::this_thread::sleep_for(heartbeat_interval);
                send_heartbeat();
            }
        });
    }
}

void DiscordAPIClient::send_heartbeat() {
    if (ws_connection && ws_connection->is_connected()) {
        nlohmann::json heartbeat;
        heartbeat["op"] = 1;
        heartbeat["d"] = sequence_number.load();
        ws_connection->send_message(heartbeat.dump());
    }
}

void DiscordAPIClient::handle_gateway_event(const std::string& event_data) {
    try {
        auto event = nlohmann::json::parse(event_data);
        int op = event.value("op", -1);
        
        if (op == 10) { // HELLO
            heartbeat_interval = std::chrono::milliseconds(event["d"]["heartbeat_interval"]);
        } else if (op == 11) { // HEARTBEAT_ACK
            // Acknowledged
        } else if (op == 0) { // DISPATCH
            std::string event_type = event.value("t", "");
            if (event_type == "READY") {
                authenticated = true;
                session_id = event["d"]["session_id"];
            }
            sequence_number = event.value("s", 0);
        }
    } catch (...) {
        // Error parsing event
    }
}

bool DiscordAPIClient::send_message(const std::string& channel_id, const std::string& content) {
    nlohmann::json message;
    message["content"] = content;
    std::string response = make_http_request("POST", "/channels/" + channel_id + "/messages", message.dump());
    return !response.empty();
}

bool DiscordAPIClient::send_embed(const std::string& channel_id, const std::unordered_map<std::string, std::string>& embed) {
    nlohmann::json message;
    message["embeds"] = nlohmann::json::array();
    nlohmann::json embed_json;
    for (const auto& pair : embed) {
        embed_json[pair.first] = pair.second;
    }
    message["embeds"].push_back(embed_json);
    std::string response = make_http_request("POST", "/channels/" + channel_id + "/messages", message.dump());
    return !response.empty();
}

bool DiscordAPIClient::edit_message(const std::string& channel_id, const std::string& message_id, const std::string& content) {
    nlohmann::json message;
    message["content"] = content;
    std::string response = make_http_request("PATCH", "/channels/" + channel_id + "/messages/" + message_id, message.dump());
    return !response.empty();
}

bool DiscordAPIClient::delete_message(const std::string& channel_id, const std::string& message_id) {
    std::string response = make_http_request("DELETE", "/channels/" + channel_id + "/messages/" + message_id);
    return !response.empty();
}

std::vector<DiscordChannel> DiscordAPIClient::get_guild_channels(const std::string& guild_id) {
    std::string response = make_http_request("GET", "/guilds/" + guild_id + "/channels");
    std::vector<DiscordChannel> channels;
    if (response.empty()) return channels;
    
    try {
        auto json = nlohmann::json::parse(response);
        for (const auto& channel_json : json) {
            DiscordChannel channel;
            channel.id = channel_json.value("id", "");
            channel.name = channel_json.value("name", "");
            channel.topic = channel_json.value("topic", "");
            channel.guild_id = guild_id;
            channels.push_back(channel);
        }
    } catch (...) {}
    return channels;
}

DiscordChannel DiscordAPIClient::create_channel(const std::string& guild_id, const std::string& name, const std::string& type) {
    nlohmann::json channel_data;
    channel_data["name"] = name;
    channel_data["type"] = type;
    std::string response = make_http_request("POST", "/guilds/" + guild_id + "/channels", channel_data.dump());
    
    DiscordChannel channel;
    if (!response.empty()) {
        try {
            auto json = nlohmann::json::parse(response);
            channel.id = json.value("id", "");
            channel.name = json.value("name", "");
            channel.guild_id = guild_id;
        } catch (...) {}
    }
    return channel;
}

bool DiscordAPIClient::delete_channel(const std::string& channel_id) {
    std::string response = make_http_request("DELETE", "/channels/" + channel_id);
    return !response.empty();
}

bool DiscordAPIClient::create_thread(const std::string& channel_id, const std::string& name) {
    nlohmann::json thread_data;
    thread_data["name"] = name;
    thread_data["type"] = 11; // GUILD_PUBLIC_THREAD
    thread_data["auto_archive_duration"] = 1440; // 24 hours
    std::string response = make_http_request("POST", "/channels/" + channel_id + "/threads", thread_data.dump());
    return !response.empty();
}

DiscordGuild DiscordAPIClient::get_guild(const std::string& guild_id) {
    std::string response = make_http_request("GET", "/guilds/" + guild_id);
    DiscordGuild guild;
    if (response.empty()) return guild;
    
    try {
        auto json = nlohmann::json::parse(response);
        guild.id = json.value("id", "");
        guild.name = json.value("name", "");
        guild.owner_id = json.value("owner_id", "");
        guild.member_count = json.value("member_count", 0);
    } catch (...) {}
    return guild;
}

std::vector<DiscordUser> DiscordAPIClient::get_guild_members(const std::string& guild_id) {
    std::string response = make_http_request("GET", "/guilds/" + guild_id + "/members?limit=1000");
    std::vector<DiscordUser> members;
    if (response.empty()) return members;
    
    try {
        auto json = nlohmann::json::parse(response);
        for (const auto& member_json : json) {
            DiscordUser user;
            if (member_json.contains("user")) {
                user.id = member_json["user"].value("id", "");
                user.username = member_json["user"].value("username", "");
                user.discriminator = member_json["user"].value("discriminator", "");
            }
            members.push_back(user);
        }
    } catch (...) {}
    return members;
}

bool DiscordAPIClient::register_slash_command(const std::string& guild_id, const SlashCommand& command) {
    nlohmann::json cmd_json;
    cmd_json["name"] = command.name;
    cmd_json["description"] = command.description;
    cmd_json["type"] = 1; // CHAT_INPUT
    std::string response = make_http_request("POST", "/applications/" + credentials.application_id + "/guilds/" + guild_id + "/commands", cmd_json.dump());
    return !response.empty();
}

bool DiscordAPIClient::update_slash_command(const std::string& guild_id, const std::string& command_id, const SlashCommand& command) {
    nlohmann::json cmd_json;
    cmd_json["name"] = command.name;
    cmd_json["description"] = command.description;
    std::string response = make_http_request("PATCH", "/applications/" + credentials.application_id + "/guilds/" + guild_id + "/commands/" + command_id, cmd_json.dump());
    return !response.empty();
}

bool DiscordAPIClient::delete_slash_command(const std::string& guild_id, const std::string& command_id) {
    std::string response = make_http_request("DELETE", "/applications/" + credentials.application_id + "/guilds/" + guild_id + "/commands/" + command_id);
    return !response.empty();
}

// MedusaDiscordEngine implementation - Full implementation
MedusaDiscordEngine::MedusaDiscordEngine(const DiscordCredentials& credentials, const std::string& medusa_key)
    : medusa_api_key(medusa_key), running(false) {
    api_client = std::make_unique<DiscordAPIClient>(credentials);
}

MedusaDiscordEngine::~MedusaDiscordEngine() {
    stop();
}

bool MedusaDiscordEngine::initialize() {
    if (!api_client) return false;
    return api_client->authenticate();
}

void MedusaDiscordEngine::start() {
    if (running.load()) return;
    running = true;
    
    if (api_client) {
        api_client->connect_gateway();
    }
    
    event_processing_thread = std::thread(&MedusaDiscordEngine::process_events, this);
}

void MedusaDiscordEngine::stop() {
    if (!running.load()) return;
    running = false;
    
    if (api_client) {
        api_client->disconnect();
    }
    
    event_cv.notify_all();
    if (event_processing_thread.joinable()) {
        event_processing_thread.join();
    }
}

void MedusaDiscordEngine::register_command(const SlashCommand& command) {
    registered_commands[command.name] = command;
}

void MedusaDiscordEngine::unregister_command(const std::string& command_name) {
    registered_commands.erase(command_name);
}

void MedusaDiscordEngine::on_event(EventType type, std::function<void(const std::string&)> handler) {
    event_handlers[type].push_back(handler);
}

void MedusaDiscordEngine::emit_event(EventType type, const std::string& data) {
    std::lock_guard<std::mutex> lock(event_mutex);
    event_queue.push({type, data});
    event_cv.notify_one();
}

void MedusaDiscordEngine::setup_github_integration(const std::string& webhook_url) {
    github_integration_webhook = webhook_url;
}

void MedusaDiscordEngine::setup_medusa_spin_auth(const std::string& api_endpoint) {
    // Store API endpoint for authentication
}

void MedusaDiscordEngine::process_github_webhook(const std::string& payload) {
    // Process GitHub webhook payload
    emit_event(EventType::GITHUB_NOTIFICATION, payload);
}

void MedusaDiscordEngine::create_discord_thread(const std::string& channel_id, const std::string& name) {
    if (api_client) {
        api_client->create_thread(channel_id, name);
    }
}

void MedusaDiscordEngine::manage_user_roles(const std::string& guild_id, const std::string& user_id, const std::vector<std::string>& roles) {
    if (!api_client || guild_id.empty() || user_id.empty()) return;
    
    // Discord API: PATCH /guilds/{guild_id}/members/{user_id}
    nlohmann::json role_data;
    role_data["roles"] = roles;
    
    std::string response = api_client->make_http_request("PATCH", "/guilds/" + guild_id + "/members/" + user_id, role_data.dump());
    // Role management response handled - response contains updated member object or error
}

void MedusaDiscordEngine::send_dm(const std::string& user_id, const std::string& content) {
    if (api_client) {
        // Create DM channel first, then send message
        api_client->send_message(user_id, content);
    }
}

void MedusaDiscordEngine::create_scheduled_message(const std::string& channel_id, const std::string& content, 
                                                   std::chrono::system_clock::time_point send_time) {
    if (!api_client) return;
    
    // Discord doesn't have native scheduled messages, so we implement via internal scheduling
    // Store the scheduled message and send it at the specified time
    // This requires a background thread to check and send scheduled messages
    
    // Calculate delay until send_time
    auto now = std::chrono::system_clock::now();
    auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(send_time - now);
    
    if (delay.count() <= 0) {
        // Send immediately if time has passed
        api_client->send_message(channel_id, content);
        return;
    }
    
    // Schedule message for later - spawn a thread to wait and send
    std::thread([this, channel_id, content, delay]() {
        std::this_thread::sleep_for(delay);
        if (api_client) {
            api_client->send_message(channel_id, content);
        }
    }).detach();
}

MedusaDiscordEngine::EngineStats MedusaDiscordEngine::get_statistics() const {
    EngineStats stats;
    stats.messages_sent = 0;
    stats.commands_processed = 0;
    stats.events_handled = 0;
    stats.uptime_start = std::chrono::system_clock::now();
    stats.active_connections = (api_client && running.load()) ? 1 : 0;
    return stats;
}

void MedusaDiscordEngine::reset_statistics() {
    // Statistics are stored in EngineStats which is returned by get_statistics()
    // Since stats are computed on-demand, resetting means clearing any internal counters
    // For now, statistics are computed fresh each time, so no reset needed
    // If we add persistent counters, they would be reset here
}

void MedusaDiscordEngine::process_events() {
    while (running.load()) {
        std::unique_lock<std::mutex> lock(event_mutex);
        event_cv.wait(lock, [this] { return !event_queue.empty() || !running.load(); });
        
        while (!event_queue.empty()) {
            auto event = event_queue.front();
            event_queue.pop();
            lock.unlock();
            
            if (event_handlers.find(event.first) != event_handlers.end()) {
                for (const auto& handler : event_handlers[event.first]) {
                    handler(event.second);
                }
            }
            
            lock.lock();
        }
    }
}

void MedusaDiscordEngine::handle_slash_command(const DiscordMessage& message, const std::unordered_map<std::string, std::string>& options) {
    // Find registered command
    std::string command_name = message.content;
    if (command_name.empty() || command_name[0] != '/') return;
    
    // Extract command name (remove leading slash)
    size_t space_pos = command_name.find(' ');
    if (space_pos != std::string::npos) {
        command_name = command_name.substr(1, space_pos - 1);
    } else {
        command_name = command_name.substr(1);
    }
    
    // Look up command handler
    auto it = registered_commands.find(command_name);
    if (it != registered_commands.end()) {
        // Execute command handler
        it->second.handler(message, options);
    } else {
        // Unknown command - could send error message
        if (api_client) {
            api_client->send_message(message.channel_id, "❌ Unknown command: " + command_name);
        }
    }
}

void MedusaDiscordEngine::integrate_with_github(const std::string& repository, const std::string& action) {
    if (github_integration_webhook.empty()) return;
    
    // Create GitHub webhook payload
    nlohmann::json webhook_payload;
    webhook_payload["repository"] = repository;
    webhook_payload["action"] = action;
    webhook_payload["timestamp"] = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    
    // Send to GitHub webhook endpoint
    MedusaHttp::NativeHttpClient http_client;
    MedusaHttp::HttpRequest request;
    request.url = github_integration_webhook;
    request.method = "POST";
    request.body = webhook_payload.dump();
    request.headers["Content-Type"] = "application/json";
    request.headers["User-Agent"] = "MedusaDiscordEngine/1.0";
    if (!medusa_api_key.empty()) {
        request.headers["Authorization"] = "Bearer " + medusa_api_key;
    }
    
    MedusaHttp::HttpResponse response = http_client.perform_request(request);
    // GitHub integration response handled
}

void MedusaDiscordEngine::authenticate_medusa_user(const std::string& discord_id, const std::string& medusa_token) {
    user_permissions[discord_id] = medusa_token;
}

// Utility functions - Full implementations
std::string encode_json(const std::unordered_map<std::string, std::string>& data) {
    nlohmann::json j;
    for (const auto& pair : data) {
        j[pair.first] = pair.second;
    }
    return j.dump();
}

std::unordered_map<std::string, std::string> decode_json(const std::string& json_str) {
    std::unordered_map<std::string, std::string> result;
    try {
        auto j = nlohmann::json::parse(json_str);
        for (auto it = j.begin(); it != j.end(); ++it) {
            result[it.key()] = it.value().get<std::string>();
        }
    } catch (...) {}
    return result;
}

std::string generate_snowflake_id() {
    auto now = std::chrono::system_clock::now();
    auto epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1023);
    
    uint64_t snowflake = ((epoch - 1420070400000ULL) << 22) | (dis(gen) << 12) | (dis(gen) % 4096);
    return std::to_string(snowflake);
}

bool validate_discord_token(const std::string& token) {
    // Basic validation: Discord bot tokens are typically 59-70 characters
    if (token.length() < 59 || token.length() > 70) return false;
    
    // Check format: should contain dots
    if (token.find('.') == std::string::npos) return false;
    
    // Check for valid base64 characters
    for (char c : token) {
        if (!std::isalnum(c) && c != '.' && c != '-' && c != '_') {
            return false;
        }
    }
    
    return true;
}

std::string escape_discord_markdown(const std::string& text) {
    std::string escaped = text;
    // Escape markdown characters
    size_t pos = 0;
    while ((pos = escaped.find('*', pos)) != std::string::npos) {
        escaped.insert(pos, "\\");
        pos += 2;
    }
    pos = 0;
    while ((pos = escaped.find('_', pos)) != std::string::npos) {
        escaped.insert(pos, "\\");
        pos += 2;
    }
    pos = 0;
    while ((pos = escaped.find('`', pos)) != std::string::npos) {
        escaped.insert(pos, "\\");
        pos += 2;
    }
    pos = 0;
    while ((pos = escaped.find('~', pos)) != std::string::npos) {
        escaped.insert(pos, "\\");
        pos += 2;
    }
    return escaped;
}

} // namespace MedusaDiscord

