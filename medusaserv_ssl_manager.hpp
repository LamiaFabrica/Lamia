/**
 * MEDUSASERV AUTOMATED SSL CERTIFICATE MANAGER v0.3.0c
 * =====================================================
 * Ground Up automated SSL certificate management with expiration monitoring
 * Professional certificate handling - NO manual Apache/NGINX configuration needed
 * NO shortcuts, NO mock data, application-automated certificate management
 * © 2025 The Medusa Project | Roylepython | D Hargreaves
 */

#ifndef MEDUSASERV_SSL_MANAGER_HPP
#define MEDUSASERV_SSL_MANAGER_HPP

#include <string>
#include <vector>
#include <atomic>
#include <ctime>

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// SSL MANAGER CONFIGURATION
// ========================================

#define MEDUSASERV_SSL_MANAGER_VERSION "0.3.0c"
#define MEDUSASERV_SSL_DEFAULT_RENEWAL_DAYS 30
#define MEDUSASERV_SSL_WARNING_DAYS 7
#define MEDUSASERV_SSL_MAX_DOMAINS 10
#define MEDUSASERV_SSL_MAX_EMAIL_LENGTH 256

// SSL certificate information
typedef struct {
    char domain[256];
    char cert_path[512];
    char key_path[512];
    char chain_path[512];
    time_t issue_date;
    time_t expiry_date;
    int days_until_expiry;
    bool auto_renewal_enabled;
    bool monitoring_enabled;
    char issuer[256];
    char serial_number[64];
} MedusaServSSLCertInfo;

// SSL manager configuration
typedef struct {
    bool auto_renewal_enabled;
    bool email_alerts_enabled;
    bool monitoring_enabled;
    int renewal_days_before_expiry;
    int warning_days_before_expiry;
    int check_interval_hours;
    char alert_email[MEDUSASERV_SSL_MAX_EMAIL_LENGTH];
    char smtp_server[256];
    int smtp_port;
    char smtp_username[256];
    char smtp_password[256];
    char acme_directory_url[512];
    char acme_account_email[MEDUSASERV_SSL_MAX_EMAIL_LENGTH];
} MedusaServSSLManagerConfig;

// SSL renewal result
typedef struct {
    bool success;
    char error_message[512];
    char new_cert_path[512];
    char new_key_path[512];
    time_t new_expiry_date;
    int renewal_duration_seconds;
} MedusaServSSLRenewalResult;

// ========================================
// SSL MANAGER FUNCTIONS
// ========================================

/**
 * Initialize SSL certificate manager
 * @return 0 on success, -1 on error
 */
int initialize_ssl_manager();

/**
 * Get SSL manager version
 * @return Version string
 */
const char* get_ssl_manager_version();

/**
 * Configure SSL manager
 * @param config SSL manager configuration
 * @return 0 on success, -1 on error
 */
int configure_ssl_manager(const MedusaServSSLManagerConfig* config);

/**
 * Add domain for SSL management
 * @param domain Domain name
 * @param cert_path Certificate file path
 * @param key_path Private key file path
 * @param auto_renewal Enable automatic renewal
 * @return 0 on success, -1 on error
 */
int add_ssl_domain(const char* domain, const char* cert_path, const char* key_path, bool auto_renewal);

/**
 * Remove domain from SSL management
 * @param domain Domain name
 * @return 0 on success, -1 on error
 */
int remove_ssl_domain(const char* domain);

/**
 * Get certificate information for domain
 * @param domain Domain name
 * @param cert_info Pointer to certificate info structure
 * @return 0 on success, -1 on error
 */
int get_ssl_cert_info(const char* domain, MedusaServSSLCertInfo* cert_info);

/**
 * Check certificate expiry for all managed domains
 * @return Number of certificates checked, -1 on error
 */
int check_certificate_expiry();

/**
 * Renew certificate for domain
 * @param domain Domain name
 * @param result Pointer to renewal result structure
 * @return 0 on success, -1 on error
 */
int renew_ssl_certificate(const char* domain, MedusaServSSLRenewalResult* result);

/**
 * Setup automatic certificate renewal
 * @param domain Domain name
 * @param enable Enable/disable auto renewal
 * @return 0 on success, -1 on error
 */
int setup_auto_renewal(const char* domain, bool enable);

/**
 * Setup email alerts for certificate expiry
 * @param email Email address for alerts
 * @param smtp_server SMTP server address
 * @param smtp_port SMTP port
 * @param username SMTP username
 * @param password SMTP password
 * @return 0 on success, -1 on error
 */
int setup_email_alerts(const char* email, const char* smtp_server, int smtp_port, 
                      const char* username, const char* password);

/**
 * Send certificate expiry alert
 * @param domain Domain name
 * @param days_until_expiry Days until certificate expires
 * @return 0 on success, -1 on error
 */
int send_expiry_alert(const char* domain, int days_until_expiry);

/**
 * Start SSL monitoring service
 * @return 0 on success, -1 on error
 */
int start_ssl_monitoring();

/**
 * Stop SSL monitoring service
 * @return 0 on success, -1 on error
 */
int stop_ssl_monitoring();

/**
 * Get SSL manager status
 * @param status_buffer Buffer for status information
 * @param buffer_size Size of status buffer
 * @return 0 on success, -1 on error
 */
int get_ssl_manager_status(char* status_buffer, size_t buffer_size);

/**
 * Generate SSL management dashboard
 * @param dashboard_buffer Buffer for dashboard HTML
 * @param buffer_size Size of dashboard buffer
 * @return 0 on success, -1 on error
 */
int generate_ssl_dashboard(char* dashboard_buffer, size_t buffer_size);

/**
 * Install certificate from existing files (like your portal.poweredbymedusa.com files)
 * @param domain Domain name
 * @param fullchain_cert_path Path to full chain certificate
 * @param private_key_path Path to private key
 * @param enable_auto_renewal Enable automatic renewal
 * @return 0 on success, -1 on error
 */
int install_existing_certificate(const char* domain, const char* fullchain_cert_path, 
                                const char* private_key_path, bool enable_auto_renewal);

/**
 * Obtain new certificate using ACME (Let's Encrypt)
 * @param domain Domain name
 * @param email Email for ACME account
 * @param cert_output_path Output path for certificate
 * @param key_output_path Output path for private key
 * @return 0 on success, -1 on error
 */
int obtain_acme_certificate(const char* domain, const char* email, 
                           const char* cert_output_path, const char* key_output_path);

#ifdef __cplusplus
}
#endif

// ========================================
// C++ INTERFACE
// ========================================

#ifdef __cplusplus

namespace medusaserv {
namespace ssl {

class SSLManager {
private:
    std::atomic<bool> initialized_;
    std::atomic<bool> monitoring_active_;
    MedusaServSSLManagerConfig config_;
    std::vector<MedusaServSSLCertInfo> managed_certificates_;
    
public:
    SSLManager();
    ~SSLManager();
    
    bool initialize();
    bool configure(const MedusaServSSLManagerConfig& config);
    
    // Certificate Management
    bool add_domain(const std::string& domain, const std::string& cert_path, 
                   const std::string& key_path, bool auto_renewal = true);
    bool remove_domain(const std::string& domain);
    bool install_existing_cert(const std::string& domain, const std::string& fullchain_path, 
                              const std::string& key_path, bool auto_renewal = true);
    bool obtain_new_cert(const std::string& domain, const std::string& email);
    
    // Certificate Information
    MedusaServSSLCertInfo get_cert_info(const std::string& domain) const;
    std::vector<MedusaServSSLCertInfo> get_all_certs() const;
    int check_expiry_all();
    
    // Renewal Management
    bool renew_certificate(const std::string& domain, MedusaServSSLRenewalResult& result);
    bool setup_auto_renewal(const std::string& domain, bool enable);
    bool renew_if_needed(const std::string& domain);
    
    // Alert Management
    bool setup_email_alerts(const std::string& email, const std::string& smtp_server, 
                           int smtp_port, const std::string& username, const std::string& password);
    bool send_expiry_alert(const std::string& domain, int days_until_expiry);
    
    // Monitoring Service
    bool start_monitoring();
    bool stop_monitoring();
    bool is_monitoring_active() const { return monitoring_active_; }
    
    // Dashboard and Status
    std::string get_status() const;
    std::string generate_dashboard() const;
    
private:
    bool validate_certificate(const std::string& cert_path) const;
    time_t get_certificate_expiry(const std::string& cert_path) const;
    bool send_email(const std::string& to, const std::string& subject, const std::string& body) const;
    void monitoring_thread();
    bool parse_certificate_info(const std::string& cert_path, MedusaServSSLCertInfo& info) const;
};

} // namespace ssl
} // namespace medusaserv

#endif // __cplusplus

#endif // MEDUSASERV_SSL_MANAGER_HPP