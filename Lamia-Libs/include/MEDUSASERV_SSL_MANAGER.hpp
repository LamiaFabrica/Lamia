/**
 * MEDUSASERV SSL MANAGER v0.3.0c
 * ===============================
 * Complete SSL certificate management with CSR/key/CA-bundle validation
 * Yorkshire Champion standards with hello validation for complete chain verification
 * © 2025 The Medusa Project
 */

#ifndef MEDUSASERV_SSL_MANAGER_HPP
#define MEDUSASERV_SSL_MANAGER_HPP

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize SSL manager with TLS_AES_256_GCM_SHA384 cipher enforcement
 * @param ssl_config_path Path to SSL configuration directory
 * @return 0 on success, -1 on error
 */
int initialize_ssl_manager(const char* ssl_config_path);

/**
 * Validate CSR (Certificate Signing Request)
 * @param csr_data PEM-formatted CSR data
 * @param csr_length Length of CSR data
 * @return 1 if valid, 0 if invalid
 */
int validate_csr(const char* csr_data, size_t csr_length);

/**
 * Validate private key
 * @param key_data PEM-formatted private key data
 * @param key_length Length of key data
 * @param key_type Expected key type (RSA, ECDSA, etc.)
 * @return 1 if valid, 0 if invalid
 */
int validate_private_key(const char* key_data, size_t key_length, const char* key_type);

/**
 * Validate CA bundle (certificate chain)
 * @param ca_bundle_data PEM-formatted CA bundle data
 * @param bundle_length Length of bundle data
 * @return 1 if valid, 0 if invalid
 */
int validate_ca_bundle(const char* ca_bundle_data, size_t bundle_length);

/**
 * Import complete SSL certificate package
 * @param domain Domain name for certificate
 * @param cert_data PEM-formatted certificate data
 * @param key_data PEM-formatted private key data
 * @param ca_bundle_data PEM-formatted CA bundle data (can be NULL)
 * @return 1 on success, 0 on failure
 */
int import_ssl_certificate(const char* domain, const char* cert_data, 
                           const char* key_data, const char* ca_bundle_data);

/**
 * Perform hello validation for complete SSL chain verification
 * @param domain Domain to test
 * @param port SSL port (usually 443)
 * @return 1 if validation successful, 0 if failed
 */
int perform_hello_validation(const char* domain, int port);

/**
 * Verify cipher suite compliance with TLS_AES_256_GCM_SHA384
 * @param domain Domain to test
 * @param port SSL port
 * @return 1 if compliant, 0 if non-compliant
 */
int verify_cipher_compliance(const char* domain, int port);

/**
 * Get SSL certificate information
 * @param domain Domain name
 * @param cert_info Output buffer for certificate information (JSON format)
 * @param buffer_size Size of output buffer
 * @return 0 on success, -1 on error
 */
int get_ssl_certificate_info(const char* domain, char* cert_info, size_t buffer_size);

/**
 * Generate self-signed certificate for development
 * @param domain Domain name
 * @param key_size Key size in bits (2048, 4096)
 * @param days_valid Validity period in days
 * @return 1 on success, 0 on failure
 */
int generate_self_signed_certificate(const char* domain, int key_size, int days_valid);

/**
 * Check certificate expiration
 * @param domain Domain name
 * @param days_until_expiry Output for days until expiration
 * @return 1 if certificate exists and checked, 0 otherwise
 */
int check_certificate_expiration(const char* domain, int* days_until_expiry);

/**
 * Backup SSL certificates
 * @param backup_path Path to backup directory
 * @return Number of certificates backed up, -1 on error
 */
int backup_ssl_certificates(const char* backup_path);

/**
 * Restore SSL certificates from backup
 * @param backup_path Path to backup directory
 * @return Number of certificates restored, -1 on error
 */
int restore_ssl_certificates(const char* backup_path);

/**
 * Get SSL manager statistics
 * @param stats_json Output buffer for statistics (JSON format)
 * @param buffer_size Size of output buffer
 * @return 0 on success, -1 on error
 */
int get_ssl_manager_stats(char* stats_json, size_t buffer_size);

#ifdef __cplusplus
}

// C++ namespace integration for advanced SSL management
namespace medusaserv {
namespace ssl {
    namespace manager {
        namespace validation {
            bool csr_structure_check(const std::string& csr_data);
            bool private_key_format_check(const std::string& key_data);
            bool ca_bundle_chain_check(const std::string& ca_data);
        }
        namespace import {
            bool certificate_package_import(const std::string& domain, 
                                           const std::string& cert,
                                           const std::string& key,
                                           const std::string& ca_bundle);
        }
        namespace hello {
            bool chain_verification(const std::string& domain, int port);
            bool cipher_compliance_check(const std::string& domain, int port);
        }
        namespace compliance {
            bool tls_aes_256_gcm_sha384_check(const std::string& domain);
            bool aes_256_backwards_compatibility(const std::string& domain);
        }
    }
}
}
#endif

#endif // MEDUSASERV_SSL_MANAGER_HPP