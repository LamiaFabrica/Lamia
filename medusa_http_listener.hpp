#ifndef MEDUSA_HTTP_LISTENER_HPP
#define MEDUSA_HTTP_LISTENER_HPP

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>
#include <chrono>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

// OpenSSL forward declarations - will be included in implementation
typedef struct ssl_ctx_st SSL_CTX;
typedef struct evp_cipher_ctx_st EVP_CIPHER_CTX;

/**
 * @file medusa_http_listener.hpp
 * @brief Native C++ HTTP/1.1 and HTTP/2 Listener with AES-256-GCM Security
 * @details GROUND UP PHILOSOPHY - No external libraries (libcurl, etc.)
 * 
 * Section 2: MedusaServ Core HTTP Listener
 * - Native C++ HTTP/1.1 and HTTP/2 support
 * - AES-256-GCM security for all connections
 * - WebSocket support for real-time features
 * - Direct integration with MDS parser for .mde file serving
 * - No external dependencies - ground-up implementation
 */

namespace MedusaHTTP {

/**
 * @enum HTTPVersion
 * @brief Supported HTTP versions
 */
enum class HTTPVersion {
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2_0
};

/**
 * @enum HTTPMethod
 * @brief HTTP request methods
 */
enum class HTTPMethod {
    GET,
    POST,
    PUT,
    DELETE,
    PATCH,
    HEAD,
    OPTIONS,
    TRACE,
    CONNECT
};

/**
 * @struct HTTPRequest
 * @brief Native HTTP request structure
 */
struct HTTPRequest {
    HTTPMethod method;
    HTTPVersion version;
    std::string path;
    std::string query_string;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    std::string client_ip;
    int client_port;
    std::chrono::system_clock::time_point timestamp;
    
    // Security context
    bool is_secure;
    std::string session_id;
    std::string user_agent;
    std::string referer;
};

/**
 * @struct HTTPResponse
 * @brief Native HTTP response structure
 */
struct HTTPResponse {
    int status_code;
    std::string status_message;
    HTTPVersion version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    bool should_close;
    
    // Security headers
    std::string content_security_policy;
    std::string x_frame_options;
    std::string x_content_type_options;
    std::string x_xss_protection;
    std::string strict_transport_security;
};

/**
 * @struct WebSocketFrame
 * @brief WebSocket frame structure
 */
struct WebSocketFrame {
    bool fin;
    uint8_t opcode;
    bool mask;
    uint64_t payload_length;
    std::string payload;
    std::string masking_key;
};

/**
 * @class AES256GCMSecurity
 * @brief AES-256-GCM encryption implementation
 */
class AES256GCMSecurity {
private:
    EVP_CIPHER_CTX* encrypt_ctx_;
    EVP_CIPHER_CTX* decrypt_ctx_;
    std::vector<uint8_t> key_;
    std::vector<uint8_t> iv_;
    std::mutex crypto_mutex_;
    
    void initializeKey();
    void generateIV();
    
public:
    AES256GCMSecurity();
    ~AES256GCMSecurity();
    
    /**
     * @brief Encrypt data using AES-256-GCM
     */
    std::vector<uint8_t> encrypt(const std::string& plaintext);
    
    /**
     * @brief Decrypt data using AES-256-GCM
     */
    std::string decrypt(const std::vector<uint8_t>& ciphertext);
    
    /**
     * @brief Generate authentication tag
     */
    std::vector<uint8_t> generateTag(const std::vector<uint8_t>& ciphertext);
    
    /**
     * @brief Verify authentication tag
     */
    bool verifyTag(const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& tag);
};

/**
 * @class HTTPParser
 * @brief Native HTTP request/response parser
 */
class HTTPParser {
private:
    static HTTPMethod parseMethod(const std::string& method_str);
    static HTTPVersion parseVersion(const std::string& version_str);
    static std::string parsePath(const std::string& request_line);
    static std::unordered_map<std::string, std::string> parseHeaders(const std::vector<std::string>& header_lines);
    
public:
    /**
     * @brief Parse HTTP request from raw data
     */
    static HTTPRequest parseRequest(const std::string& raw_request, const std::string& client_ip, int client_port);
    
    /**
     * @brief Parse HTTP response from raw data
     */
    static HTTPResponse parseResponse(const std::string& raw_response);
    
    /**
     * @brief Serialize HTTP response to string
     */
    static std::string serializeResponse(const HTTPResponse& response);
    
    /**
     * @brief Parse WebSocket frame
     */
    static WebSocketFrame parseWebSocketFrame(const std::string& frame_data);
    
    /**
     * @brief Serialize WebSocket frame
     */
    static std::string serializeWebSocketFrame(const WebSocketFrame& frame);
};

/**
 * @class HTTPListener
 * @brief Native C++ HTTP/1.1 and HTTP/2 listener
 */
class HTTPListener {
private:
    int server_socket_;
    SSL_CTX* ssl_ctx_;
    std::atomic<bool> running_;
    std::thread listener_thread_;
    mutable std::mutex connection_mutex_;
    std::vector<std::thread> worker_threads_;
    std::vector<int> client_sockets_;
    
    // Security
    std::unique_ptr<AES256GCMSecurity> security_;
    
    // Configuration
    int port_;
    int max_connections_;
    int thread_pool_size_;
    bool enable_ssl_;
    std::string ssl_cert_path_;
    std::string ssl_key_path_;
    
    // Request handlers
    std::unordered_map<std::string, std::function<HTTPResponse(const HTTPRequest&)>> route_handlers_;
    std::function<HTTPResponse(const HTTPRequest&)> default_handler_;
    
    // WebSocket handlers
    std::unordered_map<std::string, std::function<void(const WebSocketFrame&, int)>> websocket_handlers_;
    
    // Internal methods
    bool initializeSocket();
    bool initializeSSL();
    void listenerLoop();
    void handleConnection(int client_socket);
    void handleHTTPRequest(int client_socket);
    void handleWebSocketConnection(int client_socket, const HTTPRequest& request);
    HTTPResponse processRequest(const HTTPRequest& request);
    void sendResponse(int client_socket, const HTTPResponse& response);
    void addSecurityHeaders(HTTPResponse& response);
    bool isWebSocketUpgrade(const HTTPRequest& request);
    std::string generateWebSocketAcceptKey(const std::string& client_key);
    
public:
    HTTPListener(int port = 8080, int max_connections = 1000, int thread_pool_size = 10);
    ~HTTPListener();
    
    /**
     * @brief Initialize the HTTP listener
     */
    bool initialize();
    
    /**
     * @brief Start the HTTP listener
     */
    bool start();
    
    /**
     * @brief Stop the HTTP listener
     */
    void stop();
    
    /**
     * @brief Configure SSL/TLS
     */
    bool configureSSL(const std::string& cert_path, const std::string& key_path);
    
    /**
     * @brief Register HTTP route handler
     */
    void registerRoute(const std::string& path, std::function<HTTPResponse(const HTTPRequest&)> handler);
    
    /**
     * @brief Register WebSocket handler
     */
    void registerWebSocket(const std::string& path, std::function<void(const WebSocketFrame&, int)> handler);
    
    /**
     * @brief Set default request handler
     */
    void setDefaultHandler(std::function<HTTPResponse(const HTTPRequest&)> handler);
    
    /**
     * @brief Get current connection count
     */
    size_t getConnectionCount() const;
    
    /**
     * @brief Get server statistics
     */
    struct ServerStats {
        size_t total_requests;
        size_t active_connections;
        size_t ssl_connections;
        size_t websocket_connections;
        std::chrono::system_clock::time_point start_time;
    };
    
    ServerStats getStats() const;
};

/**
 * @class HTTP2Listener
 * @brief HTTP/2 specific listener implementation
 */
class HTTP2Listener : public HTTPListener {
private:
    // HTTP/2 specific fields
    std::unordered_map<int, uint32_t> stream_ids_;
    std::mutex stream_mutex_;
    
    // HTTP/2 frame parsing
    struct HTTP2Frame {
        uint32_t length;
        uint8_t type;
        uint8_t flags;
        uint32_t stream_id;
        std::string payload;
    };
    
    HTTP2Frame parseHTTP2Frame(const std::string& frame_data);
    std::string serializeHTTP2Frame(const HTTP2Frame& frame);
    void handleHTTP2Stream(int client_socket, uint32_t stream_id);
    
public:
    HTTP2Listener(int port = 8080, int max_connections = 1000, int thread_pool_size = 10);
    
    /**
     * @brief Initialize HTTP/2 listener
     */
    bool initializeHTTP2();
    
    /**
     * @brief Handle HTTP/2 connection upgrade
     */
    bool handleHTTP2Upgrade(const HTTPRequest& request, int client_socket);
};

} // namespace MedusaHTTP

#endif // MEDUSA_HTTP_LISTENER_HPP
