/**
 * MEDUSA SECURITY HEADER
 * Core security declarations for Python-C++ wrappers
 */

#ifndef MEDUSA_SECURITY_H
#define MEDUSA_SECURITY_H

#include <string>

namespace medusa {
namespace security {

// Hardware fingerprinting
std::string computeHardwareFingerprint();

// Anti-tampering checks
bool isDebuggerPresent();
bool isVirtualMachine();

// Encryption/Decryption
std::string encryptAES256(const std::string& plaintext);
std::string decryptAES256(const std::string& ciphertext);

// Base64 encoding/decoding
std::string base64Encode(const std::string& input);
std::string base64Decode(const std::string& input);

// Secure memory operations
void secureClearMemory(void* ptr, size_t size);

// Template for secure wrapper execution
template<typename Func>
class SecureExecutor {
private:
    bool m_validated;
    std::string m_fingerprint;
    
public:
    SecureExecutor() : m_validated(false) {
        m_fingerprint = computeHardwareFingerprint();
        m_validated = validate();
    }
    
    bool validate() {
        // Check for debugger
        if (isDebuggerPresent()) {
            fprintf(stderr, "🚨 SECURITY: Debugger detected!\n");
            return false;
        }
        
        // Check for VM
        if (isVirtualMachine()) {
            fprintf(stderr, "⚠️  SECURITY: Virtual machine detected (allowed with warning)\n");
            // Allow VM but log warning
        }
        
        return true;
    }
    
    template<typename... Args>
    auto execute(Func func, Args&&... args) -> decltype(func(args...)) {
        if (!m_validated) {
            throw std::runtime_error("Security validation failed");
        }
        
        // Execute with anti-tampering protection
        return func(std::forward<Args>(args)...);
    }
};

} // namespace security
} // namespace medusa

#endif // MEDUSA_SECURITY_H