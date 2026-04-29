/**
 * LIBMEDUSASERV_COMPATIBILITY_ENGINE HEADER v0.3.0a
 * ===================================================
 * Ground Up cross-webserver compatibility header
 * Native C++ shared library interface for Apache/NGINX/IIS/Tomcat
 * Professional implementation with 100% compatibility
 * © 2025 The Medusa Project | Roylepython | D Hargreaves
 */

#ifndef MEDUSASERV_COMPATIBILITY_ENGINE_HPP
#define MEDUSASERV_COMPATIBILITY_ENGINE_HPP

#ifdef __cplusplus
extern "C" {
#endif

// Return codes (shared with other engines)
#define MEDUSASERV_SUCCESS                0
#define MEDUSASERV_ERROR_GENERIC         -1
#define MEDUSASERV_ERROR_NOT_INITIALIZED -2
#define MEDUSASERV_ERROR_INVALID_PARAMETER -3

// Compatibility structures
typedef struct {
    int apache_compatibility;
    int nginx_compatibility;
    int iis_compatibility;
    int tomcat_compatibility;
    int overall_compatibility;
} MedusaServCompatibilityMatrix;

// Core compatibility functions
int initialize_compatibility_engine();

// Apache compatibility functions
int load_apache_configuration_native(const char* config_path);
int process_htaccess_files_native(const char* document_root);
int implement_mod_rewrite_native();
int handle_virtual_hosts_native();

// NGINX compatibility functions
int load_nginx_configuration_native(const char* config_path);
int implement_nginx_directives_native();
int handle_upstream_servers_native();

// IIS compatibility functions
int load_iis_configuration_native(const char* config_path);
int process_webconfig_native(const char* webconfig_path);

// Tomcat compatibility functions
int load_tomcat_configuration_native(const char* config_path);
int implement_servlet_support_native();

// Utility functions
int get_compatibility_matrix(MedusaServCompatibilityMatrix* matrix);
const char* get_compatibility_version();

#ifdef __cplusplus
}
#endif

#endif // MEDUSASERV_COMPATIBILITY_ENGINE_HPP