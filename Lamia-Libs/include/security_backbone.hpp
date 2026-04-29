#ifndef SECURITY_BACKBONE_HPP
#define SECURITY_BACKBONE_HPP

// Conditional Node.js includes - Gold Standard compatibility
#ifdef MEDUSA_FULL_NODE_INTEGRATION
    #include <node.h>
    #include <node_api.h>
    #include <v8.h>
#else
    // Minimal Node.js interface for Gold Standard build
    namespace node {
        int Start(int argc, char** argv);
    }
    namespace v8 {
        class Isolate {};
        class Context {};
        class Value {};
        enum class MemoryPressureLevel { kNone, kModerate, kCritical };
        
        template<typename T> class Persistent {};
        template<typename T> class FunctionCallbackInfo {
        public:
            void GetReturnValue() const {}
        };
    }
#endif
#include <immintrin.h>  // AES-NI intrinsics
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <chrono>

// Hardware acceleration detection
#ifdef __AES__
#define MEDUSA_HW_AES_SUPPORT 1
#else
#define MEDUSA_HW_AES_SUPPORT 0
#endif

namespace medusa {
namespace security {

// AES-NI Hardware Accelerated Encryption
class HardwareAES {
private:
    __m128i round_keys[11];
    bool hw_support;
    
public:
    HardwareAES();
    ~HardwareAES();
    
    bool initialize(const uint8_t* key);
    void encrypt_block(const uint8_t* plaintext, uint8_t* ciphertext);
    void decrypt_block(const uint8_t* ciphertext, uint8_t* plaintext);
    
    // Bulk operations for maximum performance
    void encrypt_bulk(const uint8_t* data, size_t size, uint8_t* output);
    void decrypt_bulk(const uint8_t* data, size_t size, uint8_t* output);
    
    bool has_hardware_support() const { return hw_support; }
};

// Memory Protection and Scrambling
class MemoryProtector {
private:
    std::atomic<bool> active{true};
    std::thread scrambler_thread;
    std::vector<void*> protected_regions;
    
    void scramble_loop();
    void clear_memory_region(void* ptr, size_t size);
    
public:
    MemoryProtector();
    ~MemoryProtector();
    
    void protect_region(void* ptr, size_t size);
    void unprotect_region(void* ptr);
    void secure_zero(void* ptr, size_t size);
    void disable_core_dumps();
    void randomize_aslr();
    
    // Test mode controls
    void enable_test_mode() { printf("🧪 MemoryProtector test mode enabled\n"); }
    void disable_test_mode() { printf("🔒 MemoryProtector test mode disabled\n"); }
};

// Anti-Debugging and Runtime Protection
class RuntimeProtector {
private:
    std::atomic<bool> debugger_detected{false};
    std::atomic<bool> test_mode{false};
    std::atomic<bool> protection_active{true};
    std::thread detection_thread;
    
    void detection_loop();
    bool check_debugger_presence();
    bool check_memory_breakpoints();
    bool check_timing_attacks();
    void inject_nanomites();
    
public:
    RuntimeProtector();
    ~RuntimeProtector();
    
    bool is_debugger_present() const { return debugger_detected.load(); }
    void enable_anti_debugging();
    void scramble_execution_flow();
    void inject_decoy_code();
    
    // Test mode controls
    void enable_test_mode() { test_mode.store(true); printf("🧪 Test mode enabled - reduced security checks\n"); }
    void disable_test_mode() { test_mode.store(false); printf("🔒 Test mode disabled - full security active\n"); }
    void shutdown_protection() { protection_active.store(false); }
    bool is_test_mode() const { return test_mode.load(); }
};

// Selective Code Obfuscation Manager
class ObfuscationManager {
private:
    struct CodeRegion {
        std::string identifier;
        uint8_t* encrypted_code;
        size_t code_size;
        uint8_t protection_level; // 0-10 scale
        bool is_decrypted;
        std::chrono::steady_clock::time_point last_access;
    };
    
    std::unordered_map<std::string, CodeRegion> code_regions;
    HardwareAES crypto;
    MemoryProtector memory;
    RuntimeProtector runtime;
    
    // Protection layer methods
    uint8_t* apply_maximum_protection(const uint8_t* code, size_t size);
    uint8_t* apply_high_protection(const uint8_t* code, size_t size);
    uint8_t* apply_basic_protection(const uint8_t* code, size_t size);
    
    // Obfuscation techniques
    std::vector<uint8_t> apply_code_obfuscation(const uint8_t* code, size_t size);
    void apply_xor_layer(uint8_t* data, size_t size);
    void apply_memory_scrambling(uint8_t* data, size_t size);
    
    // Decryption methods
    uint8_t* decrypt_maximum_protection(uint8_t* encrypted_data, size_t size);
    uint8_t* decrypt_high_protection(uint8_t* encrypted_data, size_t size);
    uint8_t* decrypt_basic_protection(uint8_t* encrypted_data, size_t size);
    
    // Reversal methods
    std::vector<uint8_t> reverse_code_obfuscation(uint8_t* data, size_t size);
    void reverse_xor_layer(uint8_t* data, size_t size);
    void reverse_memory_scrambling(uint8_t* data, size_t size);
    
    // Utility methods
    uint8_t* create_decoy_code(size_t size);
    void schedule_reencryption(const std::string& id, int seconds);
    
public:
    ObfuscationManager();
    ~ObfuscationManager();
    
    bool register_code_region(const std::string& id, const uint8_t* code, size_t size, uint8_t protection_level);
    uint8_t* decrypt_code_jit(const std::string& id);
    void re_encrypt_code(const std::string& id);
    void cleanup_expired_code(int max_age_seconds = 30);
    
    // Selective protection based on criticality
    void set_protection_profile(const std::string& profile);
};

// Node.js Runtime Wrapper with Security
class SecureNodeRuntime {
private:
    v8::Isolate* isolate;
    v8::Persistent<v8::Context> context;
    
    HardwareAES crypto;
    MemoryProtector memory;
    RuntimeProtector runtime;
    ObfuscationManager obfuscator;
    
    // Encrypted Node.js modules
    struct EncryptedModule {
        std::string name;
        std::vector<uint8_t> encrypted_source;
        bool is_critical;
    };
    
    std::vector<EncryptedModule> encrypted_modules;
    
    bool initialize_v8();
    bool load_encrypted_modules();
    std::string decrypt_module_source(const EncryptedModule& module);
    
    // Security hooks
    static void security_check_callback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void memory_pressure_callback(v8::Isolate* isolate, v8::MemoryPressureLevel level);
    
public:
    SecureNodeRuntime();
    ~SecureNodeRuntime();
    
    bool initialize();
    bool execute_secure_script(const std::string& script_path);
    bool run_main_server();
    
    // Menu system specific optimizations
    bool preload_menu_modules();
    bool enable_menu_hot_reload();
    
    // Test mode controls
    void enable_test_mode();
    void disable_test_mode();
    
    void shutdown();
};

// Lightspeed Build System Controller
class BuildOptimizer {
private:
    struct BuildProfile {
        bool use_selective_obfuscation;
        bool enable_hardware_crypto;
        bool use_aggressive_caching;
        int optimization_level; // 0-3
    };
    
    BuildProfile current_profile;
    
public:
    BuildOptimizer();
    
    void set_development_profile();
    void set_production_profile();
    void set_lightspeed_profile();
    
    bool optimize_build_cache();
    bool precompile_critical_modules();
    bool generate_optimized_bundles();
    
    // Fast incremental builds
    bool detect_changes();
    bool rebuild_modified_only();
    
    // Validation methods
    bool validate_frontend_integrity();
    bool validate_backend_integrity();
    bool validate_menu_system_integrity();
    
    std::chrono::milliseconds get_estimated_build_time();
};

// Main Security Backbone Controller
class SecurityBackbone {
private:
    std::unique_ptr<SecureNodeRuntime> node_runtime;
    std::unique_ptr<BuildOptimizer> build_optimizer;
    
    bool initialized;
    std::string master_key;
    
public:
    SecurityBackbone();
    ~SecurityBackbone();
    
    bool initialize(const std::string& config_path);
    bool start_secure_server();
    bool ensure_menu_functionality();
    
    // Test mode controls
    void enable_test_mode();
    void disable_test_mode();
    
    // Performance monitoring
    struct PerformanceMetrics {
        std::chrono::milliseconds startup_time;
        std::chrono::milliseconds menu_load_time;
        double cpu_usage;
        size_t memory_usage;
        size_t crypto_operations_per_sec;
    };
    
    PerformanceMetrics get_performance_metrics();
    
    void shutdown();
};

} // namespace MedusaServ
} // namespace MedusaServ

#endif // SECURITY_BACKBONE_HPP