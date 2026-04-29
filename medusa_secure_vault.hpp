#ifndef MEDUSA_SECURE_VAULT_HPP
#define MEDUSA_SECURE_VAULT_HPP

#include <string>
#include <unordered_map>

/**
 * @file medusa_secure_vault.hpp  
 * @brief Header for encrypted credential vault for disabled user access retention
 * @details Double AES-256-GCM encrypted storage for GitHub tokens and server credentials
 */

namespace MedusaVault {
    
class MedusaSecureVault {
private:
    struct EncryptedCredential {
        std::string name;
        std::string encrypted_value;
        std::string iv;
        std::string auth_tag;
        std::string purpose;
        std::string access_level;
    };
    
    std::unordered_map<std::string, EncryptedCredential> vault_;
    std::string master_key_;
    
    std::string doubleEncrypt(const std::string& plaintext);
    std::string doubleDecrypt(const std::string& encrypted_data);
    
public:
    MedusaSecureVault();
    
    // Store credentials in encrypted vault
    bool storeCredential(const std::string& name, const std::string& value, 
                        const std::string& purpose = "github_token");
    
    // Retrieve credentials from encrypted vault  
    std::string getCredential(const std::string& name);
    
    // Check if credential exists
    bool hasCredential(const std::string& name);
    
    // Initialize vault with production credentials
    void initializeVault();
    
    // Get GitHub token for authentication
    std::string getGitHubToken();
};

} // namespace MedusaVault

#endif // MEDUSA_SECURE_VAULT_HPP