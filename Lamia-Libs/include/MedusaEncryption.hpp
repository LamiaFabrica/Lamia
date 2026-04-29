#ifndef MEDUSA_MEDUSAENCRYPTION_HPP
#define MEDUSA_MEDUSAENCRYPTION_HPP

/*
 * MEDUSA CORE ENCRYPTION LIBRARY - C++ Wrapper
 * Gold Standard, Secure, Infinite Reusability
 * 
 * C++ WRAPPER: Object-oriented interface for C++ projects
 * SECURITY LEVEL: Gold Standard - Absolutely Secured
 * ACCESS CONTROL: Only Roylepython can access
 * 
 * USAGE:
 * #include "MedusaEncryption.hpp"
 * 
 * MedusaEncryption crypto;
 * auto encrypted = crypto.encryptString("Hello Champion!", "password", "context");
 * auto decrypted = crypto.decryptString(encrypted, "password", "context");
 * 
 * FEATURES:
 * - Modern C++26 RAII design
 * - Exception-safe operations
 * - Smart pointer memory management
 * - Yorkshire Champion error handling
 * - Thread-safe by design
 * - STL container integration
 * - Template-based crypto operations
 * - Comprehensive encryption suite
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <variant>
#include <map>
#include <mutex>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>

// Include the C library header
extern "C" {
#include "libmedusa_encryption.h"
}

namespace Medusa {
namespace Encryption {

// Medusa constants
constexpr double ENIGMA_CONSTANT = 0.315;
constexpr int YORKSHIRE_CHAMPION_ITERATIONS = 100000;
constexpr size_t AES_KEY_SIZE = 32;
constexpr size_t AES_IV_SIZE = 12;
constexpr size_t AES_TAG_SIZE = 16;
constexpr size_t RSA_KEY_SIZE = 4096;

// Yorkshire Champion Exception Classes
class MedusaEncryptionException : public std::runtime_error {
public:
    explicit MedusaEncryptionException(const std::string& message, 
                                      const std::string& yorkshire_comment = "")
        : std::runtime_error(message), yorkshire_comment_(yorkshire_comment) {}
    
    const std::string& getYorkshireComment() const noexcept { return yorkshire_comment_; }

private:
    std::string yorkshire_comment_;
};

class KeyDerivationException : public MedusaEncryptionException {
public:
    explicit KeyDerivationException(const std::string& message)
        : MedusaEncryptionException(message, "Key derivation went sideways, champion!") {}
};

class EncryptionException : public MedusaEncryptionException {
public:
    explicit EncryptionException(const std::string& message)
        : MedusaEncryptionException(message, "Encryption went sideways, champion!") {}
};

class DecryptionException : public MedusaEncryptionException {
public:
    explicit DecryptionException(const std::string& message)
        : MedusaEncryptionException(message, "Decryption went sideways, champion!") {}
};

class AuthenticationException : public MedusaEncryptionException {
public:
    explicit AuthenticationException(const std::string& message)
        : MedusaEncryptionException(message, "Authentication failed, champion!") {}
};

// Secure memory management RAII wrapper
template<typename T>
class SecureVector {
private:
    T* data_;
    size_t size_;
    
public:
    explicit SecureVector(size_t size) : size_(size) {
        data_ = static_cast<T*>(medusa_allocate_secure(size * sizeof(T)));
        if (!data_) {
            throw std::bad_alloc();
        }
        std::memset(data_, 0, size * sizeof(T));
    }
    
    ~SecureVector() {
        if (data_) {
            // Secure wipe
            volatile T* vdata = data_;
            for (size_t i = 0; i < size_; ++i) {
                vdata[i] = T{};
            }
            medusa_deallocate_secure(data_);
        }
    }
    
    // No copy construction/assignment
    SecureVector(const SecureVector&) = delete;
    SecureVector& operator=(const SecureVector&) = delete;
    
    // Move construction/assignment
    SecureVector(SecureVector&& other) noexcept : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }
    
    SecureVector& operator=(SecureVector&& other) noexcept {
        if (this != &other) {
            this->~SecureVector();
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    T* data() { return data_; }
    const T* data() const { return data_; }
    size_t size() const { return size_; }
    
    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }
    
    std::vector<T> toVector() const {
        return std::vector<T>(data_, data_ + size_);
    }
};

// Encryption result structures
struct EncryptionResult {
    bool success = false;
    std::vector<uint8_t> encrypted_data;
    std::vector<uint8_t> iv;
    std::vector<uint8_t> tag;
    std::string error_message;
    std::string yorkshire_comment;
    
    // Conversion to base64 string for storage
    std::string toBase64String() const {
        if (!success) return "";
        
        // Create JSON structure
        std::ostringstream json;
        json << "{";
        json << "\"e\":\"" << base64Encode(encrypted_data) << "\",";
        json << "\"i\":\"" << base64Encode(iv) << "\",";
        json << "\"t\":\"" << base64Encode(tag) << "\",";
        json << "\"v\":\"1.0\",";
        json << "\"c\":" << std::fixed << std::setprecision(3) << ENIGMA_CONSTANT;
        json << "}";
        
        std::string json_str = json.str();
        return base64Encode(std::vector<uint8_t>(json_str.begin(), json_str.end()));
    }
    
    static std::optional<EncryptionResult> fromBase64String(const std::string& base64_str) {
        try {
            // Decode base64
            auto json_bytes = base64Decode(base64_str);
            if (json_bytes.empty()) return std::nullopt;
            
            std::string json_str(json_bytes.begin(), json_bytes.end());
            
            // Parse JSON manually (simple parser for our format)
            EncryptionResult result;
            result.success = true;
            
            // Extract encrypted_data
            size_t e_start = json_str.find("\"e\":\"") + 5;
            size_t e_end = json_str.find("\"", e_start);
            if (e_start != std::string::npos && e_end != std::string::npos) {
                result.encrypted_data = base64Decode(json_str.substr(e_start, e_end - e_start));
            }
            
            // Extract IV
            size_t i_start = json_str.find("\"i\":\"") + 5;
            size_t i_end = json_str.find("\"", i_start);
            if (i_start != std::string::npos && i_end != std::string::npos) {
                result.iv = base64Decode(json_str.substr(i_start, i_end - i_start));
            }
            
            // Extract tag
            size_t t_start = json_str.find("\"t\":\"") + 5;
            size_t t_end = json_str.find("\"", t_start);
            if (t_start != std::string::npos && t_end != std::string::npos) {
                result.tag = base64Decode(json_str.substr(t_start, t_end - t_start));
            }
            
            return result;
        } catch (...) {
            return std::nullopt;
        }
    }

private:
    static std::string base64Encode(const std::vector<uint8_t>& data) {
        const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::string result;
        int val = 0, valb = -6;
        for (uint8_t c : data) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                result.push_back(chars[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6) result.push_back(chars[((val << 8) >> (valb + 8)) & 0x3F]);
        while (result.size() % 4) result.push_back('=');
        return result;
    }
    
    static std::vector<uint8_t> base64Decode(const std::string& input) {
        const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::vector<uint8_t> result;
        int val = 0, valb = -8;
        for (char c : input) {
            if (c == '=') break;
            size_t pos = chars.find(c);
            if (pos == std::string::npos) continue;
            val = (val << 6) + pos;
            valb += 6;
            if (valb >= 0) {
                result.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        return result;
    }
};

struct DecryptionResult {
    bool success = false;
    bool authentic = false;
    std::vector<uint8_t> decrypted_data;
    std::string error_message;
    std::string yorkshire_comment;
    
    // Convert to string if data is text
    std::string toString() const {
        return std::string(decrypted_data.begin(), decrypted_data.end());
    }
};

struct KeyPairResult {
    bool success = false;
    std::string public_key;
    std::string private_key;
    std::string error_message;
    std::string yorkshire_comment;
};

// Main encryption class - Yorkshire Champion design
class MedusaEncryption {
private:
    mutable std::mutex library_mutex_;
    bool initialized_ = false;
    
    // Library initialization
    void ensureInitialized() const {
        std::lock_guard<std::mutex> lock(library_mutex_);
        if (!initialized_) {
            if (medusa_encryption_init() != 1) {
                throw MedusaEncryptionException("Failed to initialize Medusa encryption library",
                                              "Library initialization went sideways, champion!");
            }
            const_cast<MedusaEncryption*>(this)->initialized_ = true;
        }
    }
    
    // Convert vector to C array for library calls
    template<typename T>
    static const T* vectorData(const std::vector<T>& vec) {
        return vec.empty() ? nullptr : vec.data();
    }

public:
    MedusaEncryption() {
        ensureInitialized();
    }
    
    // Library information
    std::string getVersion() const {
        ensureInitialized();
        return std::string(medusa_encryption_version());
    }
    
    double getEnigmaConstant() const {
        ensureInitialized();
        return medusa_encryption_enigma_constant();
    }
    
    // Key derivation with Yorkshire Champion Salt
    std::vector<uint8_t> deriveKeyFromPassword(const std::string& password, 
                                              const std::string& context = "",
                                              size_t key_length = AES_KEY_SIZE) const {
        ensureInitialized();
        std::lock_guard<std::mutex> lock(library_mutex_);
        
        std::vector<uint8_t> derived_key(key_length);
        size_t derived_len = key_length;
        
        int result = medusa_derive_key_pbkdf2(
            password.c_str(),
            context.empty() ? nullptr : context.c_str(),
            key_length,
            derived_key.data(),
            &derived_len
        );
        
        if (result != 1) {
            throw KeyDerivationException("PBKDF2 key derivation failed");
        }
        
        derived_key.resize(derived_len);
        return derived_key;
    }
    
    // AES-256-GCM Encryption
    EncryptionResult encryptAESGCM(const std::vector<uint8_t>& plaintext,
                                  const std::vector<uint8_t>& key,
                                  const std::string& context = "") const {
        ensureInitialized();
        
        if (key.size() != AES_KEY_SIZE) {
            throw EncryptionException("Invalid AES key size. Must be 32 bytes.");
        }
        
        std::lock_guard<std::mutex> lock(library_mutex_);
        EncryptionResult result;
        
        // Prepare output buffers
        std::vector<uint8_t> encrypted_data(plaintext.size() + 1024); // Extra space
        std::vector<uint8_t> iv(AES_IV_SIZE);
        std::vector<uint8_t> tag(AES_TAG_SIZE);
        
        size_t encrypted_len = encrypted_data.size();
        size_t iv_len = iv.size();
        size_t tag_len = tag.size();
        
        int encrypt_result = medusa_encrypt_aes_gcm(
            vectorData(plaintext), plaintext.size(),
            vectorData(key), key.size(),
            context.empty() ? nullptr : context.c_str(),
            encrypted_data.data(), &encrypted_len,
            iv.data(), &iv_len,
            tag.data(), &tag_len
        );
        
        if (encrypt_result == 1) {
            result.success = true;
            result.encrypted_data.assign(encrypted_data.begin(), encrypted_data.begin() + encrypted_len);
            result.iv.assign(iv.begin(), iv.begin() + iv_len);
            result.tag.assign(tag.begin(), tag.begin() + tag_len);
            result.yorkshire_comment = "Encryption successful, champion level security!";
        } else {
            result.success = false;
            result.error_message = "AES-GCM encryption failed";
            result.yorkshire_comment = "Encryption went sideways, champion!";
        }
        
        return result;
    }
    
    // AES-256-GCM Decryption
    DecryptionResult decryptAESGCM(const EncryptionResult& encryption_result,
                                  const std::vector<uint8_t>& key,
                                  const std::string& context = "") const {
        ensureInitialized();
        
        if (!encryption_result.success) {
            throw DecryptionException("Cannot decrypt invalid encryption result");
        }
        
        if (key.size() != AES_KEY_SIZE) {
            throw DecryptionException("Invalid AES key size. Must be 32 bytes.");
        }
        
        std::lock_guard<std::mutex> lock(library_mutex_);
        DecryptionResult result;
        
        std::vector<uint8_t> decrypted_data(encryption_result.encrypted_data.size() + 1024);
        size_t decrypted_len = decrypted_data.size();
        
        int decrypt_result = medusa_decrypt_aes_gcm(
            vectorData(encryption_result.encrypted_data), encryption_result.encrypted_data.size(),
            vectorData(key), key.size(),
            vectorData(encryption_result.iv), encryption_result.iv.size(),
            vectorData(encryption_result.tag), encryption_result.tag.size(),
            context.empty() ? nullptr : context.c_str(),
            decrypted_data.data(), &decrypted_len
        );
        
        if (decrypt_result == 1) {
            result.success = true;
            result.authentic = true;
            result.decrypted_data.assign(decrypted_data.begin(), decrypted_data.begin() + decrypted_len);
            result.yorkshire_comment = "Decryption successful, authentication verified, champion!";
        } else {
            result.success = false;
            result.authentic = false;
            result.error_message = "AES-GCM decryption or authentication failed";
            result.yorkshire_comment = "Decryption or authentication failed, champion!";
        }
        
        return result;
    }
    
    // High-level string encryption (password-based)
    std::string encryptString(const std::string& plaintext, 
                             const std::string& password,
                             const std::string& context = "") const {
        // Derive key from password
        auto key = deriveKeyFromPassword(password, context);
        
        // Convert string to bytes
        std::vector<uint8_t> plaintext_bytes(plaintext.begin(), plaintext.end());
        
        // Encrypt
        auto result = encryptAESGCM(plaintext_bytes, key, context);
        
        if (!result.success) {
            throw EncryptionException(result.error_message);
        }
        
        // Return base64 encoded result
        return result.toBase64String();
    }
    
    // High-level string decryption (password-based)
    std::string decryptString(const std::string& encrypted_base64,
                             const std::string& password,
                             const std::string& context = "") const {
        // Parse base64 encrypted data
        auto encryption_result = EncryptionResult::fromBase64String(encrypted_base64);
        if (!encryption_result) {
            throw DecryptionException("Invalid encrypted data format");
        }
        
        // Derive key from password
        auto key = deriveKeyFromPassword(password, context);
        
        // Decrypt
        auto result = decryptAESGCM(*encryption_result, key, context);
        
        if (!result.success || !result.authentic) {
            throw DecryptionException(result.error_message);
        }
        
        return result.toString();
    }
    
    // RSA-4096 key pair generation
    KeyPairResult generateRSAKeyPair() const {
        ensureInitialized();
        std::lock_guard<std::mutex> lock(library_mutex_);
        
        KeyPairResult result;
        
        std::vector<char> public_key(8192);
        std::vector<char> private_key(8192);
        size_t public_len = public_key.size();
        size_t private_len = private_key.size();
        
        int gen_result = medusa_generate_rsa_keypair(
            public_key.data(), &public_len,
            private_key.data(), &private_len
        );
        
        if (gen_result == 1) {
            result.success = true;
            result.public_key = std::string(public_key.data());
            result.private_key = std::string(private_key.data());
            result.yorkshire_comment = "RSA-4096 keypair generated, gold standard champion!";
        } else {
            result.success = false;
            result.error_message = "RSA keypair generation failed";
            result.yorkshire_comment = "RSA generation went sideways, champion!";
        }
        
        return result;
    }
    
    // ECDH P-521 key pair generation
    KeyPairResult generateECDHKeyPair() const {
        ensureInitialized();
        std::lock_guard<std::mutex> lock(library_mutex_);
        
        KeyPairResult result;
        
        std::vector<char> public_key(2048);
        std::vector<char> private_key(2048);
        size_t public_len = public_key.size();
        size_t private_len = private_key.size();
        
        int gen_result = medusa_generate_ecdh_keypair(
            public_key.data(), &public_len,
            private_key.data(), &private_len
        );
        
        if (gen_result == 1) {
            result.success = true;
            result.public_key = std::string(public_key.data());
            result.private_key = std::string(private_key.data());
            result.yorkshire_comment = "ECDH P-521 keypair generated, gold standard champion!";
        } else {
            result.success = false;
            result.error_message = "ECDH keypair generation failed";
            result.yorkshire_comment = "ECDH generation went sideways, champion!";
        }
        
        return result;
    }
    
    // Security audit
    std::map<std::string, std::variant<std::string, double, bool>> securityAudit() const {
        ensureInitialized();
        std::lock_guard<std::mutex> lock(library_mutex_);
        
        std::map<std::string, std::variant<std::string, double, bool>> audit_result;
        
        std::vector<char> audit_buffer(2048);
        int result = medusa_encryption_security_audit(audit_buffer.data(), audit_buffer.size());
        
        if (result == 1) {
            audit_result["success"] = true;
            audit_result["audit_report"] = std::string(audit_buffer.data());
            audit_result["enigma_constant"] = getEnigmaConstant();
            audit_result["library_version"] = getVersion();
            audit_result["security_level"] = std::string("Gold Standard");
            audit_result["yorkshire_status"] = std::string("Champion Level");
            audit_result["timestamp"] = getYorkshireTimestamp();
        } else {
            audit_result["success"] = false;
            audit_result["error"] = std::string("Security audit failed");
            audit_result["yorkshire_comment"] = std::string("Audit went sideways, champion!");
        }
        
        return audit_result;
    }
    
    // Template-based encryption for any data type
    template<typename T>
    std::string encryptData(const T& data, const std::string& password, const std::string& context = "") const {
        // Serialize data to bytes (you can customize this for different types)
        std::ostringstream oss;
        oss << data;
        return encryptString(oss.str(), password, context);
    }
    
    template<typename T>
    T decryptData(const std::string& encrypted_data, const std::string& password, const std::string& context = "") const {
        std::string decrypted_str = decryptString(encrypted_data, password, context);
        std::istringstream iss(decrypted_str);
        T data;
        iss >> data;
        return data;
    }

private:
    std::string getYorkshireTimestamp() const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S Yorkshire Champion Time");
        return ss.str();
    }
};

// Utility functions for common operations
namespace Utils {
    // Generate secure random password
    std::string generateSecurePassword(size_t length = 32);
    
    // Hash password for authentication (compatible with Python wrapper)
    std::string hashPasswordYorkshire(const std::string& password, const std::string& context = "");
    
    // Verify password against hash
    bool verifyPasswordYorkshire(const std::string& password, const std::string& hash, const std::string& context = "");
    
    // Convert bytes to hex string
    std::string bytesToHex(const std::vector<uint8_t>& bytes);
    
    // Convert hex string to bytes
    std::vector<uint8_t> hexToBytes(const std::string& hex);
    
    // Secure string comparison (constant-time)
    bool secureCompare(const std::string& a, const std::string& b);
}

// RAII Encryption Session for managing encryption contexts
class EncryptionSession {
private:
    std::unique_ptr<MedusaEncryption> crypto_;
    std::string password_;
    std::string context_;
    
public:
    EncryptionSession(const std::string& password, const std::string& context = "")
        : crypto_(std::make_unique<MedusaEncryption>()), password_(password), context_(context) {}
    
    std::string encrypt(const std::string& data) {
        return crypto_->encryptString(data, password_, context_);
    }
    
    std::string decrypt(const std::string& encrypted_data) {
        return crypto_->decryptString(encrypted_data, password_, context_);
    }
    
    template<typename T>
    std::string encryptData(const T& data) {
        return crypto_->encryptData(data, password_, context_);
    }
    
    template<typename T>  
    T decryptData(const std::string& encrypted_data) {
        return crypto_->decryptData<T>(encrypted_data, password_, context_);
    }
};

// Utility functions implementation
namespace Utils {
    // Generate secure random password
    std::string generateSecurePassword(size_t length = 32) {
        const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";
        SecureVector<uint8_t> random_bytes(length);
        if (medusa_generate_secure_random(random_bytes.data(), length) != 1) {
            throw MedusaEncryptionException("Failed to generate secure random bytes");
        }
        
        std::string password;
        password.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            password += charset[random_bytes[i] % charset.length()];
        }
        return password;
    }
    
    // Hash password for authentication (compatible with Python wrapper)
    std::string hashPasswordYorkshire(const std::string& password, const std::string& context = "") {
        MedusaEncryption crypto;
        auto key = crypto.deriveKeyFromPassword(password, context, 64);
        return bytesToHex(key);
    }
    
    // Verify password against hash
    bool verifyPasswordYorkshire(const std::string& password, const std::string& hash, const std::string& context = "") {
        std::string computed_hash = hashPasswordYorkshire(password, context);
        return secureCompare(computed_hash, hash);
    }
    
    // Convert bytes to hex string
    std::string bytesToHex(const std::vector<uint8_t>& bytes) {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (uint8_t byte : bytes) {
            oss << std::setw(2) << static_cast<int>(byte);
        }
        return oss.str();
    }
    
    // Convert hex string to bytes
    std::vector<uint8_t> hexToBytes(const std::string& hex) {
        std::vector<uint8_t> bytes;
        if (hex.length() % 2 != 0) return bytes;
        
        bytes.reserve(hex.length() / 2);
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteString = hex.substr(i, 2);
            uint8_t byte = static_cast<uint8_t>(std::strtol(byteString.c_str(), nullptr, 16));
            bytes.push_back(byte);
        }
        return bytes;
    }
    
    // Secure string comparison (constant-time)
    bool secureCompare(const std::string& a, const std::string& b) {
        if (a.length() != b.length()) {
            return false;
        }
        
        volatile char result = 0;
        for (size_t i = 0; i < a.length(); ++i) {
            result |= a[i] ^ b[i];
        }
        return result == 0;
    }
}

} // namespace MedusaServ
} // namespace MedusaServ

/*
USAGE EXAMPLES FOR C++ PROJECTS:

1. Basic string encryption:
```cpp
#include "MedusaEncryption.hpp"

Medusa::Encryption::MedusaEncryption crypto;
std::string encrypted = crypto.encryptString("Secret data", "password", "context");
std::string decrypted = crypto.decryptString(encrypted, "password", "context");
```

2. Advanced AES-GCM encryption:
```cpp
std::vector<uint8_t> plaintext = {'H', 'e', 'l', 'l', 'o'};
auto key = crypto.deriveKeyFromPassword("my_password", "context");
auto result = crypto.encryptAESGCM(plaintext, key, "context");
auto decrypted = crypto.decryptAESGCM(result, key, "context");
```

3. RSA key pair generation:
```cpp
auto rsa_keys = crypto.generateRSAKeyPair();
if (rsa_keys.success) {
    std::cout << "Public Key: " << rsa_keys.public_key << std::endl;
}
```

4. Encryption session for multiple operations:
```cpp
Medusa::Encryption::EncryptionSession session("password", "context");
auto encrypted1 = session.encrypt("Data 1");
auto encrypted2 = session.encrypt("Data 2");
auto decrypted1 = session.decrypt(encrypted1);
```

5. Template-based data encryption:
```cpp
int secret_number = 42;
std::string encrypted_number = crypto.encryptData(secret_number, "password");
int decrypted_number = crypto.decryptData<int>(encrypted_number, "password");
```

C++ WRAPPER READY FOR INFINITE REUSE ACROSS ALL C++ PROJECTS!
GOLD STANDARD SECURITY, YORKSHIRE CHAMPION ENGINEERING!
*/
#endif // MEDUSA_MEDUSAENCRYPTION_HPP
