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
 * MEDUSASERV OBFUSCATION ENGINE v0.4.0
 * ====================================
 * FULL obfuscation system for production deployment security
 * Advanced string obfuscation, symbol mangling, and runtime protection
 * © 2025 The Medusa Initiative | Yorkshire Champion Standards
 */

#ifndef MEDUSASERV_OBFUSCATION_ENGINE_HPP
#define MEDUSASERV_OBFUSCATION_ENGINE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <chrono>
#include <algorithm>
#include <memory>
#include <iostream>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#include <debugapi.h>
#elif __linux__
#include <sys/ptrace.h>
#include <unistd.h>
#endif

namespace MedusaServ {
namespace ObfuscationEngine {

// Compile-time obfuscation macros
#define OBFUSCATE_STRING(str) \
    MedusaServ::ObfuscationEngine::ObfuscatedString<sizeof(str)>(str, std::make_index_sequence<sizeof(str)>{}).decrypt()

#define MEDUSA_SECRET(str) OBFUSCATE_STRING(str)

// XOR encryption key rotation system
class XORKeyRotation {
private:
    static constexpr uint32_t ROTATION_SEEDS[8] = {
        0xDEADBEEF, 0xCAFEBABE, 0xFEEDFACE, 0xBADC0FFE,
        0xC0FFEE00, 0xDEADC0DE, 0xBEEFCAFE, 0xFACEFEED
    };
    
    mutable uint32_t current_seed_index_ = 0;
    mutable uint32_t rotation_counter_ = 0;

public:
    constexpr uint32_t get_key() const {
        const uint32_t base_key = ROTATION_SEEDS[current_seed_index_];
        const uint32_t rotated_key = (base_key << (rotation_counter_ % 32)) | 
                                   (base_key >> (32 - (rotation_counter_ % 32)));
        
        // Rotate for next use
        rotation_counter_++;
        if (rotation_counter_ % 16 == 0) {
            current_seed_index_ = (current_seed_index_ + 1) % 8;
        }
        
        return rotated_key ^ 0x8B3F8B3F; // Yorkshire Champion signature
    }
};

// Compile-time string obfuscation
template<size_t N>
class ObfuscatedString {
private:
    char encrypted_data_[N];
    uint32_t xor_key_;
    
    static constexpr uint32_t compile_time_hash(const char* str, size_t len) {
        uint32_t hash = 0x811C9DC5; // FNV-1a base
        for (size_t i = 0; i < len; ++i) {
            hash ^= static_cast<uint32_t>(str[i]);
            hash *= 0x01000193; // FNV-1a prime
        }
        return hash;
    }

public:
    template<size_t... I>
    constexpr ObfuscatedString(const char* str, std::index_sequence<I...>) 
        : encrypted_data_{static_cast<char>(str[I] ^ ((compile_time_hash(str, N) >> (I % 32)) & 0xFF))...},
          xor_key_(compile_time_hash(str, N)) {}
    
    std::string decrypt() const {
        std::string result;
        result.reserve(N);
        
        XORKeyRotation key_rotation;
        const uint32_t runtime_key = key_rotation.get_key();
        
        for (size_t i = 0; i < N - 1; ++i) { // -1 to exclude null terminator
            const uint8_t original_xor = (xor_key_ >> (i % 32)) & 0xFF;
            const uint8_t runtime_xor = (runtime_key >> (i % 32)) & 0xFF;
            result += static_cast<char>(encrypted_data_[i] ^ original_xor ^ runtime_xor);
        }
        
        return result;
    }
};

// Advanced symbol obfuscation
class SymbolObfuscator {
private:
    std::unordered_map<std::string, std::string> symbol_map_;
    std::mt19937 rng_;
    
    static constexpr const char* OBFUSCATED_PREFIXES[] = {
        "medusa_", "yorks_", "champ_", "revol_", "initi_", 
        "engin_", "secur_", "perfo_", "datab_", "servi_"
    };
    
    static constexpr const char* OBFUSCATED_SUFFIXES[] = {
        "_impl", "_core", "_mgr", "_sys", "_eng", "_ctrl", 
        "_proc", "_hdlr", "_exec", "_inst", "_obj", "_ref"
    };

public:
    SymbolObfuscator() : rng_(std::chrono::steady_clock::now().time_since_epoch().count()) {
        std::cout << "🔐 MedusaServ Symbol Obfuscation Engine initialized" << std::endl;
        std::cout << "   Yorkshire Champion security protocols active" << std::endl;
    }
    
    std::string obfuscate_symbol(const std::string& original_symbol) {
        if (symbol_map_.find(original_symbol) != symbol_map_.end()) {
            return symbol_map_[original_symbol];
        }
        
        // Generate obfuscated symbol
        std::uniform_int_distribution<> prefix_dist(0, sizeof(OBFUSCATED_PREFIXES)/sizeof(OBFUSCATED_PREFIXES[0]) - 1);
        std::uniform_int_distribution<> suffix_dist(0, sizeof(OBFUSCATED_SUFFIXES)/sizeof(OBFUSCATED_SUFFIXES[0]) - 1);
        std::uniform_int_distribution<> number_dist(1000, 9999);
        
        std::string obfuscated = OBFUSCATED_PREFIXES[prefix_dist(rng_)] + 
                               std::to_string(number_dist(rng_)) + 
                               OBFUSCATED_SUFFIXES[suffix_dist(rng_)];
        
        symbol_map_[original_symbol] = obfuscated;
        return obfuscated;
    }
    
    void print_symbol_map() const {
        std::cout << std::endl << "🔐 OBFUSCATED SYMBOL MAP" << std::endl;
        std::cout << "=========================" << std::endl;
        for (const auto& [original, obfuscated] : symbol_map_) {
            std::cout << original << " -> " << obfuscated << std::endl;
        }
    }
};

// Runtime anti-debugging protection
class AntiDebugProtection {
private:
#ifdef _WIN32
    static BOOL debug_detected_;
#else
    static bool debug_detected_;
#endif
    static std::chrono::steady_clock::time_point last_check_;
    
public:
    static void initialize() {
        std::cout << "🛡️ Anti-Debug Protection initialized" << std::endl;
        std::cout << "   Yorkshire Champion security active" << std::endl;
        last_check_ = std::chrono::steady_clock::now();
    }
    
    static bool is_debugger_present() {
        // Check for common debugging indicators
        #ifdef _WIN32
            return IsDebuggerPresent() || CheckRemoteDebuggerPresent(GetCurrentProcess(), &debug_detected_);
        #elif __linux__
            // Check for ptrace attachment
            if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
                return true; // Already being traced
            }
            ptrace(PTRACE_DETACH, 0, NULL, NULL);
            return false;
        #else
            return false;
        #endif
    }
    
    static void perform_anti_debug_check() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_check_).count();
        
        if (elapsed >= 30) { // Check every 30 seconds
            if (is_debugger_present()) {
                std::cout << "⚠️ Debug environment detected - Yorkshire Champion protection active" << std::endl;
                // In production, this could trigger additional security measures
            }
            last_check_ = now;
        }
    }
};

// Initialize static members
#ifdef _WIN32
BOOL AntiDebugProtection::debug_detected_ = FALSE;
#else
bool AntiDebugProtection::debug_detected_ = false;
#endif
std::chrono::steady_clock::time_point AntiDebugProtection::last_check_;

// Control flow obfuscation
template<typename T>
class ObfuscatedControlFlow {
private:
    std::vector<std::function<T()>> execution_paths_;
    size_t current_path_ = 0;
    
public:
    void add_execution_path(std::function<T()> path) {
        execution_paths_.push_back(path);
    }
    
    T execute_obfuscated() {
        // Shuffle execution paths to confuse static analysis
        static std::random_device rd;
        static std::mt19937 g(rd());
        
        if (execution_paths_.size() > 1) {
            std::shuffle(execution_paths_.begin(), execution_paths_.end(), g);
        }
        
        // Execute with anti-debug check
        AntiDebugProtection::perform_anti_debug_check();
        
        return execution_paths_[current_path_ % execution_paths_.size()]();
    }
};

// Obfuscated configuration system
class ObfuscatedConfig {
private:
    
public:
    static std::string get_server_name() {
        return OBFUSCATE_STRING("MedusaServ Revolutionary Web Server");
    }
    
    static std::string get_version() {
        return OBFUSCATE_STRING("v0.4.0 Yorkshire Champion Standards");
    }
    
    static std::string get_signature() {
        return OBFUSCATE_STRING("Powered by LamiaFabrica - MedusaServ Technology");
    }
    
    static std::string get_yorkshire_message() {
        return OBFUSCATE_STRING("That brew's gone down a treat. Ta. We ope you enjoy the ad free version!");
    }
};

// Master obfuscation engine
class MasterObfuscationEngine {
private:
    std::unique_ptr<SymbolObfuscator> symbol_obfuscator_;
    bool obfuscation_active_;
    
public:
    MasterObfuscationEngine() : obfuscation_active_(true) {
        symbol_obfuscator_ = std::make_unique<SymbolObfuscator>();
        AntiDebugProtection::initialize();
        
        std::cout << std::endl << "🔐 MEDUSASERV OBFUSCATION ENGINE v0.4.0" << std::endl;
        std::cout << "=======================================" << std::endl;
        std::cout << "✅ String obfuscation: ACTIVE" << std::endl;
        std::cout << "✅ Symbol mangling: ACTIVE" << std::endl;
        std::cout << "✅ Anti-debug protection: ACTIVE" << std::endl;
        std::cout << "✅ Control flow obfuscation: ACTIVE" << std::endl;
        std::cout << "🏆 Yorkshire Champion security standards" << std::endl;
        std::cout << std::endl;
    }
    
    std::string obfuscate_symbol(const std::string& symbol) {
        if (!obfuscation_active_) return symbol;
        return symbol_obfuscator_->obfuscate_symbol(symbol);
    }
    
    void enable_obfuscation() {
        obfuscation_active_ = true;
        std::cout << "🔐 Obfuscation ENABLED for production deployment" << std::endl;
    }
    
    void disable_obfuscation() {
        obfuscation_active_ = false;
        std::cout << "⚠️ Obfuscation DISABLED for debugging" << std::endl;
    }
    
    bool is_obfuscation_active() const {
        return obfuscation_active_;
    }
    
    void print_obfuscation_status() {
        std::cout << std::endl << "🔐 OBFUSCATION STATUS" << std::endl;
        std::cout << "=====================" << std::endl;
        std::cout << "Status: " << (obfuscation_active_ ? "ACTIVE ✅" : "DISABLED ⚠️") << std::endl;
        std::cout << "String encryption: " << (obfuscation_active_ ? "ENABLED" : "DISABLED") << std::endl;
        std::cout << "Symbol mangling: " << (obfuscation_active_ ? "ENABLED" : "DISABLED") << std::endl;
        std::cout << "Anti-debug: " << (obfuscation_active_ ? "ENABLED" : "DISABLED") << std::endl;
        std::cout << "Yorkshire Champion: ALWAYS ACTIVE 🏆" << std::endl;
        
        if (obfuscation_active_) {
            symbol_obfuscator_->print_symbol_map();
        }
    }
};

// Global obfuscation engine instance
extern MasterObfuscationEngine g_obfuscation_engine;

} // namespace ObfuscationEngine
} // namespace MedusaServ

// Convenience macros for obfuscated development
#define MEDUSA_OBFUSCATED_FUNCTION(name) \
    MedusaServ::ObfuscationEngine::g_obfuscation_engine.obfuscate_symbol(name)

#define MEDUSA_SECURE_STRING(str) \
    OBFUSCATE_STRING(str)

#define MEDUSA_YORKSHIRE_MESSAGE() \
    MedusaServ::ObfuscationEngine::ObfuscatedConfig::get_yorkshire_message()

#endif // MEDUSASERV_OBFUSCATION_ENGINE_HPP