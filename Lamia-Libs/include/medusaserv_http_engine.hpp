/**
 * LIBMEDUSASERV_HTTP_ENGINE HEADER v0.3.0a
 * =========================================
 * Ground Up HTTP engine header for YOUR MedusaServ
 * Native C++ shared library HTTP/HTTPS interface
 * Professional implementation with established functions
 * © 2025 The Medusa Project | Roylepython | D Hargreaves
 */

#ifndef MEDUSASERV_HTTP_ENGINE_HPP
#define MEDUSASERV_HTTP_ENGINE_HPP

#ifdef __cplusplus
extern "C" {
#endif

// Return codes (shared with core engine)
#define MEDUSASERV_SUCCESS                0
#define MEDUSASERV_ERROR_GENERIC         -1
#define MEDUSASERV_ERROR_NOT_INITIALIZED -2
#define MEDUSASERV_ERROR_INVALID_PARAMETER -3

// HTTP engine structures
typedef struct {
    int active_connections;
    long total_requests_processed;
    bool server_initialized;
} MedusaServHttpStats;

typedef struct {
    char method[16];
    char path[512];
    char version[16];
    char host[256];
    char user_agent[512];
    int content_length;
} MedusaServHttpRequest;

// HTTP server functions
int create_http_server(int port);
int process_http_requests(int client_socket);
int manage_http_connections();
int implement_http_methods();
int optimize_request_pipeline();
int handle_concurrent_requests();

// HTTP utility functions
int get_http_stats(MedusaServHttpStats* stats);
const char* generate_http_response(const char* request);
const char* get_http_version();

#ifdef __cplusplus
}
#endif

#endif // MEDUSASERV_HTTP_ENGINE_HPP