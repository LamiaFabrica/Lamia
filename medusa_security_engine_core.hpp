/*
 * Medusa Security Engine - Interface Definition
 * Bleeding Heart Foundation - Security & Encryption Manager Library
 * 
 * SECURITY NOTICE: Absolutely secured interfaces - no credential exposure
 * Yorkshire engineering approach to enterprise-grade security
 */

#pragma once

#include "../foundation/brass_plate.h"
#include <string>
#include <vector>
#include <memory>

#ifdef _WIN32
    #ifdef SECURITY_MANAGER_LIBRARY
        #define SECURITY_ENGINE_API __declspec(dllexport)
    #else
        #define SECURITY_ENGINE_API __declspec(dllimport)
    #endif
#else
    #define SECURITY_ENGINE_API __attribute__((visibility("default")))
#endif

namespace BleedingHeart {
namespace Security {

/*
 * ISecurityEngine - Absolute protection interface
 * 
 * Provides enterprise-grade encryption and authentication
 * with Yorkshire Champion Salt and ENIGMA Scale integration
 */
class SECURITY_ENGINE_API ISecurityEngine : public IComponent {
public:
    virtual ~ISecurityEngine() = default;
    
    // Authentication with absolute security
    virtual bool Authenticate(const std::string& username, const std::string& password) = 0;
    
    // Encrypt sensitive data (requires authentication)
    virtual bool EncryptSensitiveData(const std::string& data, std::string& encrypted_output) = 0;
    
    // Decrypt sensitive data (requires authentication)
    virtual bool DecryptSensitiveData(const std::string& encrypted_input, std::string& decrypted_output) = 0;
    
    // Check authentication status
    virtual bool IsAuthenticated() const = 0;
    
    // Get authenticated username (empty if not authenticated)
    virtual std::string GetAuthenticatedUser() const = 0;
    
    // Secure logout with memory cleanup
    virtual void Logout() = 0;
    
    // Get Yorkshire component name
    virtual std::string GetYorkshireName() const = 0;
    
    // Check if can run as standalone application
    virtual bool CanRunStandalone() const = 0;
};

/*
 * Security Engine Factory - For library/DLL usage
 */
extern "C" {
    SECURITY_ENGINE_API ISecurityEngine* CreateSecurityEngine();
    SECURITY_ENGINE_API void DestroySecurityEngine(ISecurityEngine* engine);
    SECURITY_ENGINE_API const char* GetSecurityEngineName();
    SECURITY_ENGINE_API const char* GetSecurityEngineVersion();
    SECURITY_ENGINE_API bool TestSecurityEngine();
}

} // namespace Security
} // namespace BleedingHeart