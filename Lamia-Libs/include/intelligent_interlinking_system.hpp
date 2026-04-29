/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * INTELLIGENT INTERLINKING SYSTEM HEADER v0.3.0c
 * ==============================================
 * 
 * Multi-Session Mutating Intelligence with Scoring-Based Resolution
 * Auto-interlinking .so library system for ALL projects with CPP/HPP pairs
 * Yorkshire Champion Standards - NO SHORTCUTS
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <chrono>

namespace IntelligentInterlinkingSystem {

// Forward declarations
class MutatingInterlinkingEngine;
struct ProjectSession;
struct IntelligentLibrary;

/**
 * @brief Intelligence Metrics Structure
 */
struct IntelligenceMetrics {
    std::atomic<double> global_score{1.0};
    std::atomic<int> total_projects{0};
    std::atomic<int> total_libraries{0};
    std::atomic<int> compiled_libraries{0};
    std::atomic<int> linked_libraries{0};
    std::atomic<int> failed_libraries{0};
    std::chrono::steady_clock::time_point last_update;
};

/**
 * @brief Interlinking Result Structure
 */
struct InterlinkingResult {
    bool success = false;
    int projects_scanned = 0;
    int libraries_found = 0;
    int libraries_compiled = 0;
    int libraries_linked = 0;
    int libraries_exported = 0;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    std::chrono::milliseconds processing_time{0};
    double intelligence_score = 0.0;
};

/**
 * @brief Main Intelligent Interlinking Interface
 */
class IntelligentInterlinkingInterface {
public:
    /**
     * @brief Initialize the interlinking system
     */
    virtual bool initialize() = 0;
    
    /**
     * @brief Scan all projects for CPP/HPP pairs
     */
    virtual InterlinkingResult scan_all_projects() = 0;
    
    /**
     * @brief Auto-compile all CPP files to .so libraries
     */
    virtual InterlinkingResult auto_compile_all() = 0;
    
    /**
     * @brief Interlink all libraries with dependency system
     */
    virtual InterlinkingResult interlink_all() = 0;
    
    /**
     * @brief Validate all interlinking
     */
    virtual bool validate_all() = 0;
    
    /**
     * @brief Export compiled libraries to repository
     */
    virtual bool export_to_repository() = 0;
    
    /**
     * @brief Get current intelligence metrics
     */
    virtual IntelligenceMetrics get_intelligence_metrics() const = 0;
    
    /**
     * @brief Get detailed status report
     */
    virtual std::string get_status_report() const = 0;
    
    /**
     * @brief Force rescan of all projects
     */
    virtual void force_rescan() = 0;
    
    virtual ~IntelligentInterlinkingInterface() = default;
};

} // namespace IntelligentInterlinkingSystem

// C Interface for integration with other systems
extern "C" {
    /**
     * @brief Initialize intelligent interlinking system
     * @return true if successful, false otherwise
     */
    bool initialize_intelligent_interlinking();
    
    /**
     * @brief Scan all projects and discover CPP/HPP pairs
     * @return true if successful, false otherwise
     */
    bool scan_all_projects_for_interlinking();
    
    /**
     * @brief Auto-compile and interlink all CPP/HPP pairs
     * @return true if successful, false otherwise
     */
    bool auto_compile_interlink_all();
    
    /**
     * @brief Validate all interlinking
     * @return true if all validations pass, false otherwise
     */
    bool validate_all_interlinking();
    
    /**
     * @brief Export compiled libraries to repository
     * @return true if successful, false otherwise
     */
    bool export_to_repository();
    
    /**
     * @brief Get intelligence score
     * @return current global intelligence score
     */
    double get_intelligence_score();
    
    /**
     * @brief Get library count
     * @return total number of libraries managed
     */
    int get_total_library_count();
    
    /**
     * @brief Get compiled library count
     * @return number of successfully compiled libraries
     */
    int get_compiled_library_count();
    
    /**
     * @brief Get status report as JSON string
     * @return JSON formatted status report
     */
    const char* get_interlinking_status_json();
    
    /**
     * @brief Force rescan all projects
     */
    void force_rescan_all_projects();
    
    /**
     * @brief Cleanup and shutdown interlinking system
     */
    void shutdown_intelligent_interlinking();
}

// Convenience macros for integration
#define INTERLINK_INIT() initialize_intelligent_interlinking()
#define INTERLINK_COMPILE_ALL() auto_compile_interlink_all()
#define INTERLINK_VALIDATE() validate_all_interlinking()
#define INTERLINK_EXPORT() export_to_repository()
#define INTERLINK_STATUS() get_interlinking_status_json()
#define INTERLINK_SCORE() get_intelligence_score()
#define INTERLINK_RESCAN() force_rescan_all_projects()
#define INTERLINK_SHUTDOWN() shutdown_intelligent_interlinking()

// Auto-inclusion in projects
#ifdef AUTO_INTERLINK
__attribute__((constructor))
static inline void auto_init_interlinking() {
    INTERLINK_INIT();
}

__attribute__((destructor))  
static inline void auto_shutdown_interlinking() {
    INTERLINK_SHUTDOWN();
}
#endif