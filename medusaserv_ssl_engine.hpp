/**
 * MEDUSASERV SSL/HTTPS ENGINE v0.3.0c
 * ====================================
 * Ground Up HTTPS/SSL implementation using established OpenSSL libraries
 * Professional secure connection handling for portal.poweredbymedusa.com
 * NO shortcuts, NO mock data, application-generated SSL certificates
 * © 2025 The Medusa Project | Roylepython | D Hargreaves
 */

#ifndef MEDUSASERV_SSL_ENGINE_HPP
#define MEDUSASERV_SSL_ENGINE_HPP

#include <string>
#include <memory>
#include <atomic>

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// SSL ENGINE CONFIGURATION
// ========================================

#define MEDUSASERV_SSL_VERSION "0.3.0c"
#define MEDUSASERV_SSL_DEFAULT_PORT 443
#define MEDUSASERV_SSL_REDIRECT_PORT 80
#define MEDUSASERV_SSL_CERT_BUFFER_SIZE 4096
#define MEDUSASERV_SSL_KEY_BUFFER_SIZE 4096

// SSL configuration structure
typedef struct {
    int https_port;
    int http_redirect_port;
    int target_port;
    char domain_name[256];
    char cert_file_path[512];
    char key_file_path[512];
    bool auto_redirect_http;
    bool generate_self_signed;
    bool enable_http2;
    bool strict_security;
} MedusaServSSLConfig;

// SSL statistics
typedef struct {
    uint64_t https_connections;
    uint64_t http_redirects;
    uint64_t ssl_handshakes_success;
    uint64_t ssl_handshakes_failed;
    uint64_t bytes_encrypted;
    double average_handshake_time_ms;
    char ssl_version[32];
    char cipher_suite[64];
} MedusaServSSLStats;

// ========================================
// SSL ENGINE FUNCTIONS
// ========================================

/**
 * Initialize SSL engine with OpenSSL
 * @return 0 on success, -1 on error
 */
int initialize_ssl_engine();

/**
 * Get SSL engine version
 * @return Version string
 */
const char* get_ssl_engine_version();

/**
 * Configure SSL for domain
 * @param config SSL configuration
 * @return 0 on success, -1 on error
 */
int configure_ssl_for_domain(const MedusaServSSLConfig* config);

/**
 * Generate self-signed certificate for domain
 * @param domain Domain name
 * @param cert_path Output certificate path
 * @param key_path Output private key path
 * @return 0 on success, -1 on error
 */
int generate_self_signed_certificate(const char* domain, const char* cert_path, const char* key_path);

/**
 * Start HTTPS server
 * @param config SSL configuration
 * @return 0 on success, -1 on error
 */
int start_https_server(const MedusaServSSLConfig* config);

/**
 * Start HTTP to HTTPS redirect server
 * @param domain Domain to redirect
 * @param target_port HTTPS port to redirect to
 * @return 0 on success, -1 on error
 */
int start_http_redirect_server(const char* domain, int target_port);

/**
 * Stop SSL services
 * @return 0 on success, -1 on error
 */
int stop_ssl_services();

/**
 * Get SSL statistics
 * @param stats Pointer to statistics structure
 * @return 0 on success, -1 on error
 */
int get_ssl_statistics(MedusaServSSLStats* stats);

/**
 * Validate SSL certificate
 * @param cert_path Certificate file path
 * @param key_path Private key file path
 * @return 0 if valid, -1 if invalid
 */
int validate_ssl_certificate(const char* cert_path, const char* key_path);

/**
 * Setup automatic HTTP to HTTPS redirection
 * @param domain Domain name
 * @return 0 on success, -1 on error
 */
int setup_auto_https_redirect(const char* domain);

#ifdef __cplusplus
}
#endif

// ========================================
// C++ INTERFACE
// ========================================

#ifdef __cplusplus

namespace medusaserv {
namespace ssl {

class SSLEngine {
private:
    std::atomic<bool> ssl_initialized_;
    std::atomic<bool> https_running_;
    std::atomic<bool> redirect_running_;
    MedusaServSSLConfig config_;
    MedusaServSSLStats stats_;
    
    void* ssl_context_;
    int https_socket_;
    int redirect_socket_;
    
public:
    SSLEngine();
    ~SSLEngine();
    
    bool initialize();
    bool configure_for_domain(const std::string& domain, int https_port = 443, int target_port = 2000);
    bool generate_certificate(const std::string& domain);
    bool start_https_service();
    bool start_redirect_service();
    bool stop_all_services();
    
    MedusaServSSLStats get_statistics() const;
    bool validate_certificate(const std::string& cert_path, const std::string& key_path);
    
private:
    bool create_ssl_context();
    bool load_certificates();
    void handle_https_connection(int client_socket);
    void handle_redirect_connection(int client_socket);
    bool setup_ssl_socket(int port);
};

} // namespace ssl
} // namespace medusaserv

#endif // __cplusplus

#endif // MEDUSASERV_SSL_ENGINE_HPP