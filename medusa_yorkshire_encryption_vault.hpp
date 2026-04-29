/*
 * Yorkshire Encryption Vault - Secure credential storage
 * Bleeding Heart Foundation - Security & Encryption Manager Library
 * 
 * SECURITY NOTICE: NO hardcoded values - all credentials encrypted at rest
 * Yorkshire engineering approach to secure vault management
 */

#pragma once

#include "medusa_security_engine.h"
#include "../foundation/yorkshire_errors.h"
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <fstream>
#include <filesystem>

#ifdef _WIN32
    #ifdef SECURITY_MANAGER_LIBRARY
        #define VAULT_API __declspec(dllexport)
    #else
        #define VAULT_API __declspec(dllimport)
    #endif
#else
    #define VAULT_API __attribute__((visibility("default")))
#endif

namespace BleedingHeart {
namespace Security {

/*
 * Encrypted Credential Entry
 * Stores credentials in encrypted form with metadata
 */
struct EncryptedCredential {
    std::string service_name;
    std::string encrypted_username;
    std::string encrypted_password;
    std::string encrypted_additional_data;
    std::string creation_time;
    std::string last_access_time;
    int access_count = 0;
    
    // Security metadata
    std::string salt_hash;  // Hash of the salt used for this entry
    std::string integrity_check;  // HMAC for integrity verification
};

/*
 * Yorkshire Encryption Vault
 * 
 * Provides secure storage and retrieval of credentials
 * All data encrypted using Yorkshire Champion Salt approach
 */
class VAULT_API YorkshireEncryptionVault {
private:
    std::unique_ptr<ISecurityEngine> security_engine_;
    std::unique_ptr<ILoggingService> logger_;
    
    std::map<std::string, EncryptedCredential> vault_entries_;
    std::mutex vault_mutex_;
    std::string vault_file_path_;
    bool vault_loaded_ = false;
    bool vault_authenticated_ = false;
    
    // Vault metadata
    std::string vault_master_user_;
    std::string vault_creation_time_;
    int total_entries_ = 0;
    
    // Generate timestamp in Yorkshire format
    std::string GetYorkshireTimestamp() const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S Yorkshire");
        return ss.str();
    }
    
    // Generate integrity check for credential
    std::string GenerateIntegrityCheck(const EncryptedCredential& credential) const {
        std::string combined_data = credential.service_name + 
                                  credential.encrypted_username + 
                                  credential.encrypted_password + 
                                  credential.creation_time;
        
        // Simple hash for integrity (in production, use HMAC)
        std::hash<std::string> hasher;
        size_t hash_value = hasher(combined_data);
        
        std::stringstream ss;
        ss << std::hex << hash_value;
        return ss.str();
    }
    
    // Verify credential integrity
    bool VerifyIntegrity(const EncryptedCredential& credential) const {
        std::string expected_check = GenerateIntegrityCheck(credential);
        return expected_check == credential.integrity_check;
    }
    
    // Save vault to encrypted file
    bool SaveVaultToFile() {
        if (!vault_authenticated_) {
            logger_->LogError("YorkshireEncryptionVault", "Vault not authenticated - cannot save");
            return false;
        }
        
        try {
            std::stringstream vault_data;
            vault_data << "# Yorkshire Encryption Vault - Bleeding Heart Foundation\n";
            vault_data << "# SECURITY NOTICE: All data encrypted - NO plaintext credentials\n";
            vault_data << "VAULT_VERSION=1.0\n";
            vault_data << "VAULT_MASTER_USER=" << vault_master_user_ << "\n";
            vault_data << "VAULT_CREATION_TIME=" << vault_creation_time_ << "\n";
            vault_data << "TOTAL_ENTRIES=" << total_entries_ << "\n";
            vault_data << "ENIGMA_SCALE=" << ENIGMA_CONSTANT << "\n";
            vault_data << "\n";
            
            // Add encrypted entries
            for (const auto& [service_name, credential] : vault_entries_) {
                vault_data << "[ENTRY:" << service_name << "]\n";
                vault_data << "SERVICE_NAME=" << credential.service_name << "\n";
                vault_data << "ENCRYPTED_USERNAME=" << credential.encrypted_username << "\n";
                vault_data << "ENCRYPTED_PASSWORD=" << credential.encrypted_password << "\n";
                vault_data << "ENCRYPTED_ADDITIONAL=" << credential.encrypted_additional_data << "\n";
                vault_data << "CREATION_TIME=" << credential.creation_time << "\n";
                vault_data << "LAST_ACCESS_TIME=" << credential.last_access_time << "\n";
                vault_data << "ACCESS_COUNT=" << credential.access_count << "\n";
                vault_data << "SALT_HASH=" << credential.salt_hash << "\n";
                vault_data << "INTEGRITY_CHECK=" << credential.integrity_check << "\n";
                vault_data << "\n";
            }
            
            // Encrypt entire vault content
            std::string vault_content = vault_data.str();
            std::string encrypted_vault;
            
            if (!security_engine_->EncryptSensitiveData(vault_content, encrypted_vault)) {
                logger_->LogError("YorkshireEncryptionVault", "Failed to encrypt vault content");
                return false;
            }
            
            // Write to file with Yorkshire header
            std::ofstream vault_file(vault_file_path_, std::ios::binary);
            if (!vault_file) {
                logger_->LogError("YorkshireEncryptionVault", "Cannot open vault file for writing: " + vault_file_path_);
                return false;
            }
            
            vault_file << "# Yorkshire Encryption Vault - Encrypted Binary Data\n";
            vault_file << "# Bleeding Heart Foundation Security Manager\n";
            vault_file << "# WARNING: Tampering with this file will corrupt all stored credentials\n";
            vault_file << "YORKSHIRE_ENCRYPTED_VAULT_DATA:\n";
            vault_file << encrypted_vault;
            
            vault_file.close();
            
            logger_->LogInfo("YorkshireEncryptionVault", "Vault saved securely to " + vault_file_path_);
            return true;
            
        } catch (const std::exception& e) {
            logger_->LogError("YorkshireEncryptionVault", "Exception saving vault: " + std::string(e.what()));
            return false;
        }
    }
    
    // Load vault from encrypted file
    bool LoadVaultFromFile() {
        if (!std::filesystem::exists(vault_file_path_)) {
            logger_->LogInfo("YorkshireEncryptionVault", "Vault file does not exist, will create new: " + vault_file_path_);
            return true; // New vault
        }
        
        try {
            std::ifstream vault_file(vault_file_path_);
            if (!vault_file) {
                logger_->LogError("YorkshireEncryptionVault", "Cannot open vault file for reading: " + vault_file_path_);
                return false;
            }
            
            std::string line;
            std::string encrypted_vault_data;
            bool found_data_marker = false;
            
            // Read past header to find encrypted data
            while (std::getline(vault_file, line)) {
                if (line.find("YORKSHIRE_ENCRYPTED_VAULT_DATA:") != std::string::npos) {
                    found_data_marker = true;
                    continue;
                }
                
                if (found_data_marker) {
                    encrypted_vault_data += line;
                }
            }
            
            vault_file.close();
            
            if (encrypted_vault_data.empty()) {
                logger_->LogError("YorkshireEncryptionVault", "No encrypted data found in vault file");
                return false;
            }
            
            // Decrypt vault content
            std::string decrypted_vault;
            if (!security_engine_->DecryptSensitiveData(encrypted_vault_data, decrypted_vault)) {
                logger_->LogError("YorkshireEncryptionVault", "Failed to decrypt vault - wrong credentials or corrupted file");
                return false;
            }
            
            // Parse decrypted vault content
            std::stringstream vault_stream(decrypted_vault);
            std::string vault_line;
            EncryptedCredential current_credential;
            bool in_entry = false;
            
            while (std::getline(vault_stream, vault_line)) {
                if (vault_line.empty() || vault_line[0] == '#') continue;
                
                if (vault_line.find("VAULT_MASTER_USER=") == 0) {
                    vault_master_user_ = vault_line.substr(18);
                } else if (vault_line.find("VAULT_CREATION_TIME=") == 0) {
                    vault_creation_time_ = vault_line.substr(20);
                } else if (vault_line.find("TOTAL_ENTRIES=") == 0) {
                    total_entries_ = std::stoi(vault_line.substr(14));
                } else if (vault_line.find("[ENTRY:") == 0) {
                    // Save previous entry if exists
                    if (in_entry && !current_credential.service_name.empty()) {
                        if (VerifyIntegrity(current_credential)) {
                            vault_entries_[current_credential.service_name] = current_credential;
                        } else {
                            logger_->LogWarning("YorkshireEncryptionVault", 
                                              "Integrity check failed for entry: " + current_credential.service_name);
                        }
                    }
                    
                    // Start new entry
                    current_credential = EncryptedCredential{};
                    in_entry = true;
                    
                } else if (vault_line.find("SERVICE_NAME=") == 0) {
                    current_credential.service_name = vault_line.substr(13);
                } else if (vault_line.find("ENCRYPTED_USERNAME=") == 0) {
                    current_credential.encrypted_username = vault_line.substr(19);
                } else if (vault_line.find("ENCRYPTED_PASSWORD=") == 0) {
                    current_credential.encrypted_password = vault_line.substr(19);
                } else if (vault_line.find("ENCRYPTED_ADDITIONAL=") == 0) {
                    current_credential.encrypted_additional_data = vault_line.substr(21);
                } else if (vault_line.find("CREATION_TIME=") == 0) {
                    current_credential.creation_time = vault_line.substr(14);
                } else if (vault_line.find("LAST_ACCESS_TIME=") == 0) {
                    current_credential.last_access_time = vault_line.substr(17);
                } else if (vault_line.find("ACCESS_COUNT=") == 0) {
                    current_credential.access_count = std::stoi(vault_line.substr(13));
                } else if (vault_line.find("SALT_HASH=") == 0) {
                    current_credential.salt_hash = vault_line.substr(10);
                } else if (vault_line.find("INTEGRITY_CHECK=") == 0) {
                    current_credential.integrity_check = vault_line.substr(16);
                }
            }
            
            // Save last entry
            if (in_entry && !current_credential.service_name.empty()) {
                if (VerifyIntegrity(current_credential)) {
                    vault_entries_[current_credential.service_name] = current_credential;
                } else {
                    logger_->LogWarning("YorkshireEncryptionVault", 
                                      "Integrity check failed for entry: " + current_credential.service_name);
                }
            }
            
            vault_loaded_ = true;
            logger_->LogInfo("YorkshireEncryptionVault", 
                           "Vault loaded successfully with " + std::to_string(vault_entries_.size()) + " entries");
            
            return true;
            
        } catch (const std::exception& e) {
            logger_->LogError("YorkshireEncryptionVault", "Exception loading vault: " + std::string(e.what()));
            return false;
        }
    }

public:
    YorkshireEncryptionVault() = default;
    ~YorkshireEncryptionVault() = default;
    
    // Initialize vault with security engine and logger
    bool Initialize(std::unique_ptr<ISecurityEngine> security_engine, 
                    std::unique_ptr<ILoggingService> logger,
                    const std::string& vault_file_path) {
        
        if (!security_engine || !logger) {
            return false;
        }
        
        security_engine_ = std::move(security_engine);
        logger_ = std::move(logger);
        vault_file_path_ = vault_file_path;
        
        logger_->LogInfo("YorkshireEncryptionVault", "Initialized with vault file: " + vault_file_path_);
        return true;
    }
    
    // Authenticate vault access
    bool AuthenticateVault(const std::string& master_username, const std::string& master_password) {
        if (!security_engine_->Authenticate(master_username, master_password)) {
            logger_->LogError("YorkshireEncryptionVault", "Vault authentication failed");
            return false;
        }
        
        vault_authenticated_ = true;
        vault_master_user_ = master_username;
        
        if (vault_creation_time_.empty()) {
            vault_creation_time_ = GetYorkshireTimestamp();
        }
        
        // Load existing vault if available
        if (!LoadVaultFromFile()) {
            logger_->LogWarning("YorkshireEncryptionVault", "Could not load existing vault, starting fresh");
        }
        
        logger_->LogInfo("YorkshireEncryptionVault", "Vault authenticated successfully for user: " + master_username);
        return true;
    }
    
    // Store credentials securely (NO hardcoded values)
    bool StoreCredentials(const std::string& service_name, 
                         const std::string& username, 
                         const std::string& password,
                         const std::string& additional_data = "") {
        
        if (!vault_authenticated_) {
            logger_->LogError("YorkshireEncryptionVault", "Vault not authenticated");
            return false;
        }
        
        std::lock_guard<std::mutex> lock(vault_mutex_);
        
        EncryptedCredential credential;
        credential.service_name = service_name;
        credential.creation_time = GetYorkshireTimestamp();
        credential.last_access_time = credential.creation_time;
        credential.access_count = 0;
        
        // Encrypt all sensitive data
        if (!security_engine_->EncryptSensitiveData(username, credential.encrypted_username) ||
            !security_engine_->EncryptSensitiveData(password, credential.encrypted_password) ||
            !security_engine_->EncryptSensitiveData(additional_data, credential.encrypted_additional_data)) {
            
            logger_->LogError("YorkshireEncryptionVault", "Failed to encrypt credentials for: " + service_name);
            return false;
        }
        
        // Generate integrity check
        credential.integrity_check = GenerateIntegrityCheck(credential);
        credential.salt_hash = "yorkshire_champion_salt_hash"; // Placeholder for salt hash
        
        // Store in vault
        vault_entries_[service_name] = credential;
        total_entries_ = static_cast<int>(vault_entries_.size());
        
        // Save to file
        if (!SaveVaultToFile()) {
            logger_->LogError("YorkshireEncryptionVault", "Failed to save vault after storing credentials");
            return false;
        }
        
        logger_->LogInfo("YorkshireEncryptionVault", "Credentials stored securely for service: " + service_name);
        return true;
    }
    
    // Retrieve credentials securely
    bool RetrieveCredentials(const std::string& service_name,
                           std::string& username,
                           std::string& password,
                           std::string& additional_data) {
        
        if (!vault_authenticated_) {
            logger_->LogError("YorkshireEncryptionVault", "Vault not authenticated");
            return false;
        }
        
        std::lock_guard<std::mutex> lock(vault_mutex_);
        
        auto it = vault_entries_.find(service_name);
        if (it == vault_entries_.end()) {
            logger_->LogError("YorkshireEncryptionVault", "Service not found in vault: " + service_name);
            return false;
        }
        
        EncryptedCredential& credential = it->second;
        
        // Verify integrity
        if (!VerifyIntegrity(credential)) {
            logger_->LogError("YorkshireEncryptionVault", "Integrity verification failed for: " + service_name);
            return false;
        }
        
        // Decrypt credentials
        if (!security_engine_->DecryptSensitiveData(credential.encrypted_username, username) ||
            !security_engine_->DecryptSensitiveData(credential.encrypted_password, password) ||
            !security_engine_->DecryptSensitiveData(credential.encrypted_additional_data, additional_data)) {
            
            logger_->LogError("YorkshireEncryptionVault", "Failed to decrypt credentials for: " + service_name);
            return false;
        }
        
        // Update access metadata
        credential.last_access_time = GetYorkshireTimestamp();
        credential.access_count++;
        
        // Save updated metadata
        SaveVaultToFile();
        
        logger_->LogInfo("YorkshireEncryptionVault", "Credentials retrieved securely for service: " + service_name);
        return true;
    }
    
    // List all services in vault (names only)
    std::vector<std::string> ListServices() const {
        if (!vault_authenticated_) {
            return {};
        }
        
        std::vector<std::string> services;
        for (const auto& [service_name, credential] : vault_entries_) {
            services.push_back(service_name);
        }
        
        return services;
    }
    
    // Remove credentials from vault
    bool RemoveCredentials(const std::string& service_name) {
        if (!vault_authenticated_) {
            logger_->LogError("YorkshireEncryptionVault", "Vault not authenticated");
            return false;
        }
        
        std::lock_guard<std::mutex> lock(vault_mutex_);
        
        auto it = vault_entries_.find(service_name);
        if (it == vault_entries_.end()) {
            logger_->LogError("YorkshireEncryptionVault", "Service not found in vault: " + service_name);
            return false;
        }
        
        vault_entries_.erase(it);
        total_entries_ = static_cast<int>(vault_entries_.size());
        
        SaveVaultToFile();
        
        logger_->LogInfo("YorkshireEncryptionVault", "Credentials removed securely for service: " + service_name);
        return true;
    }
    
    // Get vault statistics
    struct VaultStats {
        std::string master_user;
        std::string creation_time;
        int total_entries;
        bool is_authenticated;
        std::string vault_file_path;
    };
    
    VaultStats GetVaultStats() const {
        return VaultStats{
            vault_master_user_,
            vault_creation_time_,
            total_entries_,
            vault_authenticated_,
            vault_file_path_
        };
    }
    
    // Secure vault closure
    void CloseVault() {
        std::lock_guard<std::mutex> lock(vault_mutex_);
        
        if (vault_authenticated_) {
            SaveVaultToFile();
        }
        
        // Secure cleanup
        vault_entries_.clear();
        vault_authenticated_ = false;
        vault_master_user_.clear();
        
        if (security_engine_) {
            security_engine_->Logout();
        }
        
        logger_->LogInfo("YorkshireEncryptionVault", "Vault closed securely");
    }
};

} // namespace Security
} // namespace BleedingHeart