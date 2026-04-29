/**
 * LIBMEDUSASERV_SECURITY_CORE HEADER v0.3.0a
 * ===========================================
 * Ground Up security core header for YOUR MedusaServ
 * Native C++ shared library security interface
 * Professional implementation with maximum protection
 * © 2025 The Medusa Project | Roylepython | D Hargreaves
 */

#ifndef MEDUSASERV_SECURITY_CORE_HPP
#define MEDUSASERV_SECURITY_CORE_HPP

#ifdef __cplusplus
extern "C" {
#endif

// Return codes (shared with other engines)
#define MEDUSASERV_SUCCESS                    0
#define MEDUSASERV_ERROR_GENERIC             -1
#define MEDUSASERV_ERROR_NOT_INITIALIZED     -2
#define MEDUSASERV_ERROR_INVALID_PARAMETER   -3

// Security-specific return codes
#define MEDUSASERV_SECURITY_THREAT_BLOCKED   -10
#define MEDUSASERV_SECURITY_ACCESS_DENIED    -11
#define MEDUSASERV_SECURITY_RATE_LIMITED     -12

// Security structures
typedef struct {
    long threats_blocked;
    long requests_filtered;
    int blocked_ips_count;
    bool ssl_enabled;
    bool ddos_protection_active;
} MedusaServSecurityStats;

typedef struct {
    char threat_type[64];
    char source_ip[16];
    char timestamp[32];
    char description[256];
} MedusaServSecurityEvent;

// Core security functions
int implement_security_framework();
int process_security_policies();
int coordinate_security_modules();

// Threat detection and prevention
int handle_threat_detection(const char* client_ip, const char* request_data);
int implement_access_control(const char* client_ip, const char* resource_path);
int manage_security_logging(const char* event_type, const char* event_data);

// SSL/TLS and encryption
int enable_ssl_tls_support();

// DDoS and attack protection
int implement_ddos_protection();

// IP management
int block_ip_address(const char* ip_address);
int unblock_ip_address(const char* ip_address);

// Security information
int get_security_stats(MedusaServSecurityStats* stats);
const char* get_security_version();

#ifdef __cplusplus
}
#endif

#endif // MEDUSASERV_SECURITY_CORE_HPP