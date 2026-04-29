/**
 * MEDUSASERV SSL VERBOSE ENGINE v0.3.0c
 * ======================================
 * Character-precise SSL verbosing for handshake debugging
 * Perfect integration with Startup::Procedure::SSL namespace hierarchy
 * © 2025 The Medusa Project | Yorkshire Champion Standards
 */

#ifndef MEDUSASERV_SSL_VERBOSE_ENGINE_HPP
#define MEDUSASERV_SSL_VERBOSE_ENGINE_HPP

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize SSL verbose engine with character-precise debugging
 * @param verbosity_level 1=Basic, 2=Detailed, 3=Character-Precise, 4=Maximum
 * @return 0 on success, -1 on error
 */
int initialize_ssl_verbose_engine(int verbosity_level);

/**
 * Log SSL handshake step with character-precise details
 * @param step_name Name of handshake step
 * @param data Raw data being processed
 * @param data_length Length of data
 * @param direction 0=Incoming, 1=Outgoing
 */
void log_ssl_handshake_step(const char* step_name, const unsigned char* data, 
                           size_t data_length, int direction);

/**
 * Log SSL cipher suite negotiation
 * @param offered_ciphers Array of offered cipher suite names
 * @param cipher_count Number of cipher suites
 * @param selected_cipher Selected cipher suite name
 */
void log_ssl_cipher_negotiation(const char** offered_ciphers, int cipher_count, 
                               const char* selected_cipher);

/**
 * Log SSL certificate chain validation
 * @param cert_subject Certificate subject
 * @param cert_issuer Certificate issuer
 * @param validation_result 1=Valid, 0=Invalid
 * @param error_details Error details if invalid
 */
void log_ssl_certificate_validation(const char* cert_subject, const char* cert_issuer,
                                   int validation_result, const char* error_details);

/**
 * Log SSL session information
 * @param session_id Session ID (hex string)
 * @param protocol_version SSL/TLS protocol version
 * @param cipher_suite Final negotiated cipher suite
 * @param key_exchange Key exchange method
 */
void log_ssl_session_info(const char* session_id, const char* protocol_version,
                         const char* cipher_suite, const char* key_exchange);

/**
 * Character-precise hex dump of SSL data
 * @param description Description of data
 * @param data Raw data
 * @param length Data length
 */
void ssl_hex_dump(const char* description, const unsigned char* data, size_t length);

/**
 * Validate TLS_AES_256_GCM_SHA384 cipher suite enforcement
 * @param negotiated_cipher The negotiated cipher suite
 * @return 1 if compliant, 0 if non-compliant
 */
int validate_cipher_compliance(const char* negotiated_cipher);

/**
 * Get SSL verbose statistics
 * @return Number of SSL connections debugged
 */
int get_ssl_debug_connections_count();

/**
 * Clear SSL verbose engine logs
 */
void clear_ssl_verbose_logs();

#ifdef __cplusplus
}

// C++ namespace integration with Startup::Procedure hierarchy
namespace medusaserv {
namespace ssl {
    namespace verbose {
        namespace handshake {
            void character_precise_logging(const unsigned char* data, size_t length);
            void step_by_step_analysis(const char* step, const unsigned char* data, size_t length);
        }
        namespace cipher {
            void negotiation_analysis(const char** offered, int count, const char* selected);
            void compliance_verification(const char* cipher);
        }
        namespace certificate {
            void chain_validation_verbose(const char* subject, const char* issuer, int valid);
            void csr_analysis(const char* csr_data);
        }
        namespace session {
            void session_establishment_verbose(const char* session_id, const char* version);
            void security_parameters_dump(const char* cipher, const char* key_exchange);
        }
    }
}
}
#endif

#endif // MEDUSASERV_SSL_VERBOSE_ENGINE_HPP