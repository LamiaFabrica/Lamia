/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * MIT LICENSE WITH MEDUSASERV RESTRICTIONS
 * 
 * This software is licensed under the MIT License with additional restrictions
 * prohibiting use by Kickstarter, WebHostingTalk, and any entity that discriminates
 * against disabled individuals until they implement proper protections and
 * reasonable adjustments under UK Law.
 * 
 * See LICENSE.md for full license terms.
 * See STANDING_AGAINST_DISABILITY_DISCRIMINATION.md for our commitment.
 * 
 * Yorkshire Champion Standards - Standing Against Discrimination
 */

/**
 * MEDUSASERV COMPREHENSIVE DEBUG SYSTEM v0.4.0
 * =============================================
 * Advanced debugging, profiling, and validation system
 * Memory leak detection, performance analysis, and error reporting
 * © 2025 The Medusa Initiative | Yorkshire Champion Standards
 */

#ifndef MEDUSASERV_DEBUG_SYSTEM_HPP
#define MEDUSASERV_DEBUG_SYSTEM_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <fstream>
#include <sstream>
#include <mutex>
#include <thread>
#include <atomic>
#include <cassert>

namespace MedusaServ {
namespace DebugSystem {

// Debug levels
enum class DebugLevel {
    TRACE = 0,
    MDEBUG = 1,
    INFO = 2,
    WARNING = 3,
    MERROR = 4,
    CRITICAL = 5
};

// Debug categories
enum class DebugCategory {
    GENERAL,
    NETWORK,
    DATABASE,
    SECURITY,
    PERFORMANCE,
    MEMORY,
    YORKSHIRE_CHAMPION
};

// Advanced logging system
class AdvancedLogger {
private:
    std::mutex log_mutex_;
    std::ofstream log_file_;
    DebugLevel min_level_;
    bool console_output_;
    std::atomic<uint64_t> log_counter_;
    
    static const std::unordered_map<DebugLevel, std::string> LEVEL_STRINGS;
    static const std::unordered_map<DebugCategory, std::string> CATEGORY_STRINGS;
    
public:
    AdvancedLogger(const std::string& log_file_path = "medusaserv_debug.log", 
                  DebugLevel min_level = DebugLevel::MDEBUG,
                  bool console_output = true) 
        : min_level_(min_level), console_output_(console_output), log_counter_(0) {
        
        log_file_.open(log_file_path, std::ios::app);
        if (!log_file_.is_open()) {
            std::cerr << "❌ Failed to open debug log file: " << log_file_path << std::endl;
        }
        
        log(DebugLevel::INFO, DebugCategory::GENERAL, 
            "🚀 MedusaServ Advanced Debug System v0.4.0 initialized");
        log(DebugLevel::INFO, DebugCategory::YORKSHIRE_CHAMPION,
            "🏆 Yorkshire Champion debugging standards active");
    }
    
    ~AdvancedLogger() {
        log(DebugLevel::INFO, DebugCategory::GENERAL, 
            "🛑 Debug system shutdown - Total logs: " + std::to_string(log_counter_.load()));
        if (log_file_.is_open()) {
            log_file_.close();
        }
    }
    
    void log(DebugLevel level, DebugCategory category, const std::string& message) {
        if (level < min_level_) return;
        
        log_counter_++;
        
        std::lock_guard<std::mutex> lock(log_mutex_);
        
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::stringstream timestamp;
        timestamp << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        timestamp << "." << std::setfill('0') << std::setw(3) << ms.count();
        
        std::string log_entry = "[" + timestamp.str() + "] " +
                               "[" + LEVEL_STRINGS.at(level) + "] " +
                               "[" + CATEGORY_STRINGS.at(category) + "] " +
                               "[TID:" + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())) + "] " +
                               message;
        
        if (console_output_) {
            std::cout << log_entry << std::endl;
        }
        
        if (log_file_.is_open()) {
            log_file_ << log_entry << std::endl;
            log_file_.flush();
        }
    }
    
    void set_min_level(DebugLevel level) { min_level_ = level; }
    void enable_console_output(bool enabled) { console_output_ = enabled; }
    uint64_t get_log_count() const { return log_counter_.load(); }
};

// Memory tracking system
class MemoryTracker {
private:
    struct MemoryAllocation {
        size_t size;
        std::string file;
        int line;
        std::chrono::steady_clock::time_point timestamp;
    };
    
    std::mutex memory_mutex_;
    std::unordered_map<void*, MemoryAllocation> allocations_;
    std::atomic<size_t> total_allocated_;
    std::atomic<size_t> peak_memory_;
    std::atomic<uint64_t> allocation_count_;
    
public:
    MemoryTracker() : total_allocated_(0), peak_memory_(0), allocation_count_(0) {
        std::cout << "🧠 Memory Tracker initialized - Yorkshire Champion memory management" << std::endl;
    }
    
    void record_allocation(void* ptr, size_t size, const std::string& file, int line) {
        if (!ptr) return;
        
        std::lock_guard<std::mutex> lock(memory_mutex_);
        
        allocations_[ptr] = {
            size, file, line, std::chrono::steady_clock::now()
        };
        
        total_allocated_ += size;
        allocation_count_++;
        
        if (total_allocated_.load() > peak_memory_.load()) {
            peak_memory_ = total_allocated_.load();
        }
    }
    
    void record_deallocation(void* ptr) {
        if (!ptr) return;
        
        std::lock_guard<std::mutex> lock(memory_mutex_);
        
        auto it = allocations_.find(ptr);
        if (it != allocations_.end()) {
            total_allocated_ -= it->second.size;
            allocations_.erase(it);
        }
    }
    
    void print_memory_report() {
        std::lock_guard<std::mutex> lock(memory_mutex_);
        
        std::cout << std::endl << "🧠 MEMORY TRACKER REPORT" << std::endl;
        std::cout << "=========================" << std::endl;
        std::cout << "Current allocations: " << allocations_.size() << std::endl;
        std::cout << "Total allocated: " << (total_allocated_.load() / 1024.0 / 1024.0) << " MB" << std::endl;
        std::cout << "Peak memory usage: " << (peak_memory_.load() / 1024.0 / 1024.0) << " MB" << std::endl;
        std::cout << "Total allocation count: " << allocation_count_.load() << std::endl;
        
        if (!allocations_.empty()) {
            std::cout << std::endl << "⚠️ POTENTIAL MEMORY LEAKS:" << std::endl;
            for (const auto& [ptr, alloc] : allocations_) {
                auto duration = std::chrono::steady_clock::now() - alloc.timestamp;
                auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
                
                std::cout << "  " << ptr << " (" << alloc.size << " bytes) - "
                         << alloc.file << ":" << alloc.line 
                         << " (age: " << seconds << "s)" << std::endl;
            }
        } else {
            std::cout << "✅ No memory leaks detected - Yorkshire Champion!" << std::endl;
        }
    }
    
    size_t get_current_memory_usage() const { return total_allocated_.load(); }
    size_t get_peak_memory_usage() const { return peak_memory_.load(); }
    uint64_t get_allocation_count() const { return allocation_count_.load(); }
};

// Performance profiler
class PerformanceProfiler {
private:
    struct ProfilePoint {
        std::chrono::steady_clock::time_point start_time;
        std::chrono::nanoseconds total_time;
        uint64_t call_count;
        std::string name;
    };
    
    std::mutex profile_mutex_;
    std::unordered_map<std::string, ProfilePoint> profile_points_;
    
public:
    PerformanceProfiler() {
        std::cout << "⚡ Performance Profiler initialized - Yorkshire Champion performance tracking" << std::endl;
    }
    
    void start_timing(const std::string& name) {
        std::lock_guard<std::mutex> lock(profile_mutex_);
        
        auto& point = profile_points_[name];
        point.name = name;
        point.start_time = std::chrono::steady_clock::now();
    }
    
    void end_timing(const std::string& name) {
        auto end_time = std::chrono::steady_clock::now();
        
        std::lock_guard<std::mutex> lock(profile_mutex_);
        
        auto it = profile_points_.find(name);
        if (it != profile_points_.end()) {
            auto duration = end_time - it->second.start_time;
            it->second.total_time += duration;
            it->second.call_count++;
        }
    }
    
    void print_performance_report() {
        std::lock_guard<std::mutex> lock(profile_mutex_);
        
        std::cout << std::endl << "⚡ PERFORMANCE PROFILER REPORT" << std::endl;
        std::cout << "===============================" << std::endl;
        
        for (const auto& [name, point] : profile_points_) {
            if (point.call_count == 0) continue;
            
            double avg_ms = (point.total_time.count() / 1000000.0) / point.call_count;
            double total_ms = point.total_time.count() / 1000000.0;
            
            std::cout << name << ":" << std::endl;
            std::cout << "  Calls: " << point.call_count << std::endl;
            std::cout << "  Total time: " << total_ms << " ms" << std::endl;
            std::cout << "  Average time: " << avg_ms << " ms" << std::endl;
            std::cout << "  Yorkshire Champion: " << (avg_ms < 1.0 ? "✅" : "⚠️") << std::endl;
            std::cout << std::endl;
        }
    }
};

// Error tracking system  
class ErrorTracker {
private:
    struct ErrorRecord {
        std::string message;
        std::string file;
        int line;
        DebugLevel level;
        std::chrono::steady_clock::time_point timestamp;
        size_t occurrence_count;
    };
    
    std::mutex error_mutex_;
    std::vector<ErrorRecord> error_history_;
    std::unordered_map<std::string, size_t> error_counts_;
    
public:
    ErrorTracker() {
        std::cout << "🚨 Error Tracker initialized - Yorkshire Champion error management" << std::endl;
    }
    
    void record_error(const std::string& message, const std::string& file, int line, DebugLevel level) {
        std::lock_guard<std::mutex> lock(error_mutex_);
        
        std::string error_key = file + ":" + std::to_string(line) + ":" + message;
        error_counts_[error_key]++;
        
        ErrorRecord record = {
            message, file, line, level,
            std::chrono::steady_clock::now(),
            error_counts_[error_key]
        };
        
        error_history_.push_back(record);
        
        // Keep only last 1000 errors to prevent memory bloat
        if (error_history_.size() > 1000) {
            error_history_.erase(error_history_.begin(), error_history_.begin() + 100);
        }
    }
    
    void print_error_report() {
        std::lock_guard<std::mutex> lock(error_mutex_);
        
        std::cout << std::endl << "🚨 ERROR TRACKER REPORT" << std::endl;
        std::cout << "========================" << std::endl;
        std::cout << "Total errors recorded: " << error_history_.size() << std::endl;
        
        // Show most frequent errors
        std::vector<std::pair<size_t, std::string>> frequent_errors;
        for (const auto& [key, count] : error_counts_) {
            frequent_errors.emplace_back(count, key);
        }
        
        std::sort(frequent_errors.begin(), frequent_errors.end(), std::greater<>());
        
        std::cout << std::endl << "Most frequent errors:" << std::endl;
        for (size_t i = 0; i < std::min(size_t(10), frequent_errors.size()); ++i) {
            std::cout << "  " << frequent_errors[i].first << "x: " << frequent_errors[i].second << std::endl;
        }
        
        // Show recent errors
        if (!error_history_.empty()) {
            std::cout << std::endl << "Recent errors:" << std::endl;
            size_t start = error_history_.size() > 5 ? error_history_.size() - 5 : 0;
            for (size_t i = start; i < error_history_.size(); ++i) {
                const auto& error = error_history_[i];
                std::cout << "  " << error.file << ":" << error.line 
                         << " - " << error.message << std::endl;
            }
        }
    }
    
    size_t get_error_count() const {
        std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(error_mutex_));
        return error_history_.size();
    }
};

// Master debug system
class MasterDebugSystem {
private:
    std::unique_ptr<AdvancedLogger> logger_;
    std::unique_ptr<MemoryTracker> memory_tracker_;
    std::unique_ptr<PerformanceProfiler> profiler_;
    std::unique_ptr<ErrorTracker> error_tracker_;
    
    bool debug_enabled_;
    std::atomic<bool> running_;
    std::thread monitor_thread_;
    
    void monitor_loop() {
        while (running_.load()) {
            std::this_thread::sleep_for(std::chrono::seconds(60));
            
            if (debug_enabled_) {
                logger_->log(DebugLevel::INFO, DebugCategory::PERFORMANCE,
                           "🔄 Periodic system check - Memory: " + 
                           std::to_string(memory_tracker_->get_current_memory_usage() / 1024 / 1024) + " MB");
            }
        }
    }
    
public:
    MasterDebugSystem() : debug_enabled_(true), running_(true) {
        logger_ = std::make_unique<AdvancedLogger>();
        memory_tracker_ = std::make_unique<MemoryTracker>();
        profiler_ = std::make_unique<PerformanceProfiler>();
        error_tracker_ = std::make_unique<ErrorTracker>();
        
        monitor_thread_ = std::thread([this]() { monitor_loop(); });
        
        std::cout << std::endl << "🐛 MEDUSASERV MASTER DEBUG SYSTEM v0.4.0" << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << "✅ Advanced logging: ACTIVE" << std::endl;
        std::cout << "✅ Memory tracking: ACTIVE" << std::endl;
        std::cout << "✅ Performance profiling: ACTIVE" << std::endl;
        std::cout << "✅ Error tracking: ACTIVE" << std::endl;
        std::cout << "🏆 Yorkshire Champion debugging standards" << std::endl;
        std::cout << std::endl;
    }
    
    ~MasterDebugSystem() {
        running_ = false;
        if (monitor_thread_.joinable()) {
            monitor_thread_.join();
        }
    }
    
    void log(DebugLevel level, DebugCategory category, const std::string& message) {
        if (logger_) {
            logger_->log(level, category, message);
        }
        
        if (level >= DebugLevel::MERROR && error_tracker_) {
            error_tracker_->record_error(message, __FILE__, __LINE__, level);
        }
    }
    
    void start_profiling(const std::string& name) {
        if (profiler_ && debug_enabled_) {
            profiler_->start_timing(name);
        }
    }
    
    void end_profiling(const std::string& name) {
        if (profiler_ && debug_enabled_) {
            profiler_->end_timing(name);
        }
    }
    
    void track_memory_allocation(void* ptr, size_t size, const std::string& file, int line) {
        if (memory_tracker_ && debug_enabled_) {
            memory_tracker_->record_allocation(ptr, size, file, line);
        }
    }
    
    void track_memory_deallocation(void* ptr) {
        if (memory_tracker_ && debug_enabled_) {
            memory_tracker_->record_deallocation(ptr);
        }
    }
    
    void print_comprehensive_report() {
        std::cout << std::endl << "📊 COMPREHENSIVE DEBUG REPORT v0.4.0" << std::endl;
        std::cout << "=====================================" << std::endl;
        
        if (memory_tracker_) memory_tracker_->print_memory_report();
        if (profiler_) profiler_->print_performance_report();
        if (error_tracker_) error_tracker_->print_error_report();
        
        std::cout << std::endl << "🏆 Yorkshire Champion Status: " 
                 << (error_tracker_->get_error_count() == 0 ? "CHAMPION ✅" : "NEEDS WORK ⚠️") << std::endl;
        std::cout << "📈 Total log entries: " << logger_->get_log_count() << std::endl;
        std::cout << "✅ Debug system ready for v0.4.0 release!" << std::endl;
    }
    
    void enable_debug() { debug_enabled_ = true; }
    void disable_debug() { debug_enabled_ = false; }
    bool is_debug_enabled() const { return debug_enabled_; }
};

// Global debug system instance
extern MasterDebugSystem g_debug_system;

} // namespace DebugSystem
} // namespace MedusaServ

// Convenience macros for debugging
#define MEDUSA_LOG(level, category, message) \
    MedusaServ::DebugSystem::g_debug_system.log(level, category, message)

#define MEDUSA_LOG_INFO(message) \
    MEDUSA_LOG(MedusaServ::DebugSystem::DebugLevel::INFO, MedusaServ::DebugSystem::DebugCategory::GENERAL, message)

#define MEDUSA_LOG_ERROR(message) \
    MEDUSA_LOG(MedusaServ::DebugSystem::DebugLevel::MERROR, MedusaServ::DebugSystem::DebugCategory::GENERAL, message)

#define MEDUSA_LOG_YORKSHIRE(message) \
    MEDUSA_LOG(MedusaServ::DebugSystem::DebugLevel::INFO, MedusaServ::DebugSystem::DebugCategory::YORKSHIRE_CHAMPION, message)

#define MEDUSA_PROFILE_START(name) \
    MedusaServ::DebugSystem::g_debug_system.start_profiling(name)

#define MEDUSA_PROFILE_END(name) \
    MedusaServ::DebugSystem::g_debug_system.end_profiling(name)

#define MEDUSA_TRACK_MALLOC(ptr, size) \
    MedusaServ::DebugSystem::g_debug_system.track_memory_allocation(ptr, size, __FILE__, __LINE__)

#define MEDUSA_TRACK_FREE(ptr) \
    MedusaServ::DebugSystem::g_debug_system.track_memory_deallocation(ptr)

// Static member definitions
const std::unordered_map<MedusaServ::DebugSystem::DebugLevel, std::string> 
MedusaServ::DebugSystem::AdvancedLogger::LEVEL_STRINGS = {
    {DebugLevel::TRACE, "TRACE"},
    {DebugLevel::MDEBUG, "DEBUG"},
    {DebugLevel::INFO, "INFO"},
    {DebugLevel::WARNING, "WARN"},
    {DebugLevel::MERROR, "ERROR"},
    {DebugLevel::CRITICAL, "CRIT"}
};

const std::unordered_map<MedusaServ::DebugSystem::DebugCategory, std::string>
MedusaServ::DebugSystem::AdvancedLogger::CATEGORY_STRINGS = {
    {DebugCategory::GENERAL, "GENERAL"},
    {DebugCategory::NETWORK, "NETWORK"},
    {DebugCategory::DATABASE, "DATABASE"},
    {DebugCategory::SECURITY, "SECURITY"},
    {DebugCategory::PERFORMANCE, "PERFORMANCE"},
    {DebugCategory::MEMORY, "MEMORY"},
    {DebugCategory::YORKSHIRE_CHAMPION, "YORKSHIRE"}
};

#endif // MEDUSASERV_DEBUG_SYSTEM_HPP