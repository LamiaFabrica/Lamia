/**
 * LIBMEDUSASERV_CORE_ENGINE HEADER v0.3.0a
 * =========================================
 * Ground Up core engine header for YOUR MedusaServ
 * Native C++ shared library interface definitions
 * Professional implementation with established functions
 * © 2025 The Medusa Project | Roylepython | D Hargreaves
 */

#ifndef MEDUSASERV_CORE_ENGINE_HPP
#define MEDUSASERV_CORE_ENGINE_HPP

#ifdef __cplusplus
extern "C" {
#endif

// Return codes
#define MEDUSASERV_SUCCESS                0
#define MEDUSASERV_ERROR_GENERIC         -1
#define MEDUSASERV_ERROR_NOT_INITIALIZED -2
#define MEDUSASERV_ERROR_INVALID_PARAMETER -3
#define MEDUSASERV_ERROR_OUT_OF_MEMORY   -4

// Core engine structures
typedef struct {
    bool initialized;
    bool running;
    long uptime_seconds;
} MedusaServCoreStatus;

typedef struct {
    long uptime_seconds;
    long memory_usage_kb;
    int cpu_usage_percent;
    int connection_count;
    long requests_processed;
} MedusaServHealthMetrics;

// Core engine functions
int initialize_medusaserv_core();
int manage_server_lifecycle();
int coordinate_subsystems();
int optimize_core_performance();
int monitor_system_health();
int handle_graceful_shutdown();

// Status and information functions
int get_core_status(MedusaServCoreStatus* status);
const char* get_core_version();

#ifdef __cplusplus
}
#endif

#endif // MEDUSASERV_CORE_ENGINE_HPP