/**
 * LIBMEDUSASERV_PATHING_ENGINE v0.3.0c
 * =====================================
 * Core pathing engine header for automatic path resolution
 * Handles nested folders, compact setups, and all path operations
 * © 2025 The Medusa Project | Yorkshire Champion Standards
 */

#ifndef MEDUSASERV_PATHING_ENGINE_HPP
#define MEDUSASERV_PATHING_ENGINE_HPP

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the pathing engine with base directory
 * @param base_dir Base directory for path resolution (default: /opt/medusaserv)
 * @return 0 on success, -1 on error
 */
int initialize_pathing_engine(const char* base_dir);

/**
 * Resolve any path to absolute path with proper normalization
 * @param input_path Input path (relative or absolute)
 * @return Allocated resolved path string (caller must free with free_path_string)
 */
char* resolve_path(const char* input_path);

/**
 * Check if resolved path exists
 * @param input_path Input path to check
 * @return 1 if exists, 0 if not
 */
int path_exists(const char* input_path);

/**
 * Get file extension from path
 * @param input_path Input file path
 * @return Allocated extension string (caller must free)
 */
char* get_file_extension(const char* input_path);

/**
 * Build web path from root and file path
 * @param web_root Web root directory
 * @param file_path File path relative to web root
 * @return Allocated resolved path (caller must free)
 */
char* build_web_path(const char* web_root, const char* file_path);

/**
 * Find index file in directory
 * @param directory Directory to search
 * @return Allocated path to index file (caller must free)
 */
char* find_index_file(const char* directory);

/**
 * Convert web path to admin path
 * @param web_path Web path starting with /admin/
 * @return Allocated admin path (caller must free)
 */
char* convert_to_admin_path(const char* web_path);

/**
 * Get MIME type from file extension
 * @param input_path File path
 * @return Allocated MIME type string (caller must free)
 */
char* get_mime_type(const char* input_path);

/**
 * Free allocated path string
 * @param path_string String to free
 */
void free_path_string(char* path_string);

/**
 * Clear internal path cache
 */
void clear_path_cache();

/**
 * Get current cache size
 * @return Number of cached paths
 */
int get_cache_size();

/**
 * Validate path for security (prevent directory traversal)
 * @param input_path Path to validate
 * @return 1 if secure, 0 if blocked
 */
int validate_path_security(const char* input_path);

/**
 * Route virtual host requests
 * @param path Request path
 * @param host_root Virtual host root directory
 * @return Allocated resolved file path (caller must free)
 */
char* route_virtual_host(const char* path, const char* host_root);

/**
 * Route portal requests
 * @param path Request path
 * @param portal_root Portal root directory
 * @return Allocated resolved file path (caller must free)
 */
char* route_portal(const char* path, const char* portal_root);

/**
 * Route admin panel requests
 * @param path Request path
 * @param admin_root Admin root directory
 * @return Allocated resolved file path (caller must free)
 */
char* route_admin_panel(const char* path, const char* admin_root);

/**
 * Route panel requests
 * @param path Request path
 * @param panel_root Panel root directory
 * @return Allocated resolved file path (caller must free)
 */
char* route_panel(const char* path, const char* panel_root);

/**
 * Route SSL 443 traffic with enhanced security
 * @param path Request path
 * @param ssl_host SSL hostname
 * @param ssl_root SSL root directory
 * @return Allocated resolved file path (caller must free)
 */
char* route_ssl_traffic(const char* path, const char* ssl_host, const char* ssl_root);

/**
 * Route API requests
 * @param path Request path
 * @param api_root API root directory
 * @return Allocated resolved file path (caller must free)
 */
char* route_api(const char* path, const char* api_root);

/**
 * Route static file requests
 * @param path Request path
 * @param static_root Static files root directory
 * @return Allocated resolved file path (caller must free)
 */
char* route_static_files(const char* path, const char* static_root);

/**
 * Route temporary URL requests for domains during DNS propagation
 * Format: /?user=username/ -> /web/username/working-dir/
 * @param query_string URL query string (e.g., "user=poweredbymedusa")
 * @param web_root Web root directory
 * @return Allocated resolved file path (caller must free)
 */
char* route_temporary_url(const char* query_string, const char* web_root);

/**
 * C wrapper functions for Startup::Procedure namespace hierarchy
 * Perfect traceability: startup_procedure_namespace_function
 */
void startup_procedure_system_initialize_core();
void startup_procedure_system_validate_environment();
void startup_procedure_system_load_configurations();
void startup_procedure_icewall_biohazard_takeover();
void startup_procedure_icewall_security_fortress_init();
void startup_procedure_icewall_firewall_activation();
void startup_procedure_ssl_handshake_verbosity_init();
void startup_procedure_ssl_cipher_suite_validation();
void startup_procedure_ssl_certificate_chain_check();
void startup_procedure_pathing_engine_initialization();
void startup_procedure_pathing_namespace_validation();
void startup_procedure_pathing_route_mapping();
void startup_procedure_server_port_binding();
void startup_procedure_server_service_startup();
void startup_procedure_server_ready_confirmation();
void startup_procedure_validation_security_checks();
void startup_procedure_validation_performance_metrics();
void startup_procedure_validation_health_status();

#ifdef __cplusplus
}

// C++ namespace access for advanced users
namespace medusaserv {
namespace pathing {
    namespace virtualhost {
        char* route(const char* path, const char* host_root);
    }
    namespace portal {
        char* route(const char* path, const char* portal_root);
    }
    namespace admin {
        char* route(const char* path, const char* admin_root);
    }
    namespace panel {
        char* route(const char* path, const char* panel_root);
    }
    namespace ssl {
        char* route(const char* path, const char* ssl_host, const char* ssl_root);
    }
    namespace api {
        char* route(const char* path, const char* api_root);
    }
    namespace static_files {
        char* route(const char* path, const char* static_root);
    }
    namespace temporary_url {
        char* route(const char* query_string, const char* web_root);
    }
}

// Startup Procedure Hierarchy - Perfect traceability and logical organization
namespace Startup {
    namespace Procedure {
        namespace System {
            void initialize_core();
            void validate_environment();
            void load_configurations();
        }
        namespace Icewall {
            void biohazard_takeover();
            void security_fortress_init();
            void firewall_activation();
        }
        namespace SSL {
            void handshake_verbosity_init();
            void cipher_suite_validation();
            void certificate_chain_check();
        }
        namespace Pathing {
            void engine_initialization();
            void namespace_validation();
            void route_mapping();
        }
        namespace Server {
            void port_binding();
            void service_startup();
            void ready_confirmation();
        }
        namespace Validation {
            void security_checks();
            void performance_metrics();
            void health_status();
        }
    }
}

} // end medusaserv namespace
#endif

#endif // MEDUSASERV_PATHING_ENGINE_HPP