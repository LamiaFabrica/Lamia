/*
 * MEDUSA JWT SESSION MANAGER - SECURE TOKEN-BASED AUTHENTICATION
 * ================================================================
 * 
 * "JWT sessions powered by TRIFORCE database validation!" 🔐⚔️🛡️
 * Comprehensive JWT token management with real-time database permission validation
 * 
 * SECURITY FEATURES:
 * - JWT tokens with cryptographic signatures
 * - Real-time database permission validation
 * - Session expiration and refresh tokens
 * - IP address and user agent validation
 * - Brute force protection with rate limiting
 * - Session hijacking protection
 * - Secure token storage and transmission
 * 
 * TRIFORCE DATABASE INTEGRATION:
 * - PostgreSQL: Session storage, security logs, brute force tracking
 * - MongoDB: User preferences, cached permissions, session metadata
 * - Neo4j: Permission inheritance, role relationships
 * 
 * PERMISSION SYSTEM INTEGRATION:
 * - Real-time permission matrix validation
 * - Role-based access control (RBAC)
 * - Fine-grained permission checking
 * - Permission inheritance through role hierarchies
 * - Audit logging for all permission checks
 * 
 * JWT TOKEN STRUCTURE:
 * {
 *   "sub": "user_id",
 *   "username": "username",
 *   "role": "user_role",
 *   "permissions": ["perm1", "perm2"],
 *   "session_id": "unique_session_id",
 *   "ip": "client_ip",
 *   "exp": timestamp,
 *   "iat": timestamp,
 *   "jti": "jwt_id"
 * }
 * 
 * "Security that Yorkshire Champions would be proud of!" 🏆🔒
 */

#pragma once

/**
 * C++26 COMPLIANT VERSION
 * Migrated to use std::expected for error handling
 * Uses std::ranges for permission filtering
 */

#include "medusa_portal_authentication_system.hpp"
#include <jwt-cpp/jwt.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <chrono>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <future>
#include <regex>
#include <expected>  // C++26 std::expected for error handling
#include <ranges>    // C++26 ranges for filtering
#include <system_error>

namespace medusa {
namespace portal {
namespace jwt {

// ========================================
// C++26: JWT ERROR TYPES
// ========================================

enum class JWTError {
    None = 0,
    InvalidToken,
    ExpiredToken,
    InvalidSignature,
    MissingClaims,
    InvalidSession,
    RateLimited,
    PermissionDenied,
    DatabaseError,
    SystemError
};

enum class SessionError {
    None = 0,
    SessionNotFound,
    SessionExpired,
    SessionInvalid,
    TooManySessions,
    SystemError
};

enum class PermissionError {
    None = 0,
    PermissionNotFound,
    InsufficientPermissions,
    RoleMismatch,
    CacheExpired,
    DatabaseError
};

// ========================================
// JWT CONFIGURATION AND CONSTANTS
// ========================================

struct JWTConfig {
    // Token configuration
    std::string secret_key = "medusa_jwt_secret_yorkshire_champion_2024";
    std::string issuer = "medusa_portal";
    std::string audience = "medusa_users";
    
    // Token lifetimes
    std::chrono::seconds access_token_lifetime{3600};      // 1 hour
    std::chrono::seconds refresh_token_lifetime{604800};   // 7 days
    std::chrono::seconds session_lifetime{86400};          // 24 hours
    
    // Security settings
    bool require_ip_validation = true;
    bool require_user_agent_validation = true;
    bool enable_refresh_tokens = true;
    bool enable_session_rotation = true;
    int max_sessions_per_user = 5;
    
    // Rate limiting
    int max_auth_attempts_per_minute = 10;
    int max_token_refresh_per_minute = 30;
    std::chrono::seconds lockout_duration{300};  // 5 minutes
    
    // Database validation
    bool enable_realtime_permission_validation = true;
    bool cache_permissions = true;
    std::chrono::seconds permission_cache_ttl{300};  // 5 minutes
};

// ========================================
// PERMISSION SYSTEM STRUCTURES
// ========================================

struct Permission {
    std::string id;
    std::string name;
    std::string description;
    std::string category;
    bool is_system_permission = false;
};

struct Role {
    std::string id;
    std::string name;
    std::string description;
    std::vector<Permission> permissions;
    int hierarchy_level = 0;  // Higher = more privileged
};

struct PermissionMatrix {
    std::map<std::string, Role> roles;
    std::map<std::string, Permission> permissions;
    std::map<std::string, std::vector<std::string>> role_permissions;  // role_id -> permission_ids
    std::chrono::system_clock::time_point last_updated;
};

// ========================================
// JWT TOKEN STRUCTURES
// ========================================

struct JWTClaims {
    std::string subject;          // user_id
    std::string username;
    std::string role_id;
    std::vector<std::string> permissions;
    std::string session_id;
    std::string ip_address;
    std::string user_agent;
    std::chrono::system_clock::time_point issued_at;
    std::chrono::system_clock::time_point expires_at;
    std::string jwt_id;
    
    // Security metadata
    std::string device_fingerprint;
    std::string geolocation;
    bool is_elevated_session = false;
    std::chrono::system_clock::time_point last_activity;
};

struct SessionInfo {
    std::string session_id;
    std::string user_id;
    std::string access_token;
    std::string refresh_token;
    JWTClaims claims;
    
    // Security tracking
    std::string ip_address;
    std::string user_agent;
    std::string device_fingerprint;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point last_activity;
    std::chrono::system_clock::time_point expires_at;
    bool is_active = true;
    
    // Permission cache
    std::vector<std::string> cached_permissions;
    std::chrono::system_clock::time_point permissions_cached_at;
};

struct AuthenticationAttempt {
    std::string ip_address;
    std::string username;
    std::chrono::system_clock::time_point timestamp;
    bool success;
    std::string failure_reason;
    std::string user_agent;
};

// ========================================
// JWT SESSION MANAGER
// ========================================

class JWTSessionManager {
private:
    JWTConfig config_;
    std::shared_ptr<TriforceAuthenticationManager> auth_manager_;
    
    // Session storage
    std::unordered_map<std::string, SessionInfo> active_sessions_;
    std::unordered_map<std::string, std::vector<std::string>> user_sessions_;  // user_id -> session_ids
    mutable std::shared_mutex sessions_mutex_;
    
    // Permission system
    PermissionMatrix permission_matrix_;
    mutable std::shared_mutex permissions_mutex_;
    
    // Rate limiting and security
    std::unordered_map<std::string, std::vector<AuthenticationAttempt>> auth_attempts_;  // ip -> attempts
    std::unordered_set<std::string> blacklisted_tokens_;
    std::unordered_map<std::string, std::chrono::system_clock::time_point> ip_lockouts_;
    mutable std::shared_mutex security_mutex_;
    
    // Background maintenance
    std::thread maintenance_thread_;
    std::atomic<bool> shutdown_requested_{false};
    
    // Performance metrics
    std::atomic<uint64_t> tokens_issued_{0};
    std::atomic<uint64_t> tokens_validated_{0};
    std::atomic<uint64_t> tokens_rejected_{0};
    std::atomic<uint64_t> permissions_checked_{0};
    
public:
    JWTSessionManager(std::shared_ptr<TriforceAuthenticationManager> auth_manager, 
                     const JWTConfig& config = JWTConfig());
    ~JWTSessionManager();
    
    // System lifecycle
    bool initialize();
    void shutdown();
    
    // C++26: Token generation and validation (std::expected)
    std::expected<std::string, JWTError> generate_access_token(
        const MedusaUser& user, 
        const std::string& ip_address, 
        const std::string& user_agent
    );
    std::expected<std::string, JWTError> generate_refresh_token(const std::string& session_id);
    
    // C++26: Token validation (std::expected)
    std::expected<JWTClaims, JWTError> validate_access_token(const std::string& token);
    std::expected<std::string, JWTError> validate_refresh_token(const std::string& refresh_token);
    std::expected<SessionInfo, SessionError> validate_session(const std::string& session_id);
    
    // C++26: Session management (std::expected)
    std::expected<std::string, SessionError> create_session(
        const MedusaUser& user, 
        const std::string& ip_address, 
        const std::string& user_agent
    );
    std::expected<std::pair<std::string, std::string>, SessionError> refresh_session(
        const std::string& refresh_token
    );
    std::expected<void, SessionError> invalidate_session(const std::string& session_id);
    std::expected<void, SessionError> invalidate_all_user_sessions(const std::string& user_id);
    
    // C++26: Permission validation (std::expected)
    std::expected<bool, PermissionError> check_permission(
        const std::string& session_id, 
        const std::string& permission
    );
    std::expected<bool, PermissionError> check_permissions(
        const std::string& session_id, 
        const std::vector<std::string>& permissions
    );
    std::expected<bool, PermissionError> check_role_access(
        const std::string& session_id, 
        const std::string& required_role
    );
    std::expected<std::vector<std::string>, PermissionError> get_user_permissions(
        const std::string& session_id
    );
    
    // C++26: Permission matrix management (std::expected)
    std::expected<void, PermissionError> load_permission_matrix();
    std::expected<void, PermissionError> update_permission_matrix(const PermissionMatrix& matrix);
    PermissionMatrix get_permission_matrix() const;
    std::expected<void, PermissionError> add_permission_to_role(
        const std::string& role_id, 
        const std::string& permission_id
    );
    std::expected<void, PermissionError> remove_permission_from_role(
        const std::string& role_id, 
        const std::string& permission_id
    );
    
    // Security features
    bool is_ip_rate_limited(const std::string& ip_address);
    bool is_user_rate_limited(const std::string& username);
    void record_authentication_attempt(const std::string& ip_address, const std::string& username, 
                                     bool success, const std::string& failure_reason = "");
    void blacklist_token(const std::string& token);
    bool is_token_blacklisted(const std::string& token) const;
    
    // C++26: Session information (std::expected, std::ranges)
    std::expected<std::vector<SessionInfo>, SessionError> get_user_sessions(const std::string& user_id);
    std::vector<SessionInfo> get_active_sessions() const;  // C++26: Uses ranges internally
    std::expected<SessionInfo, SessionError> get_session_info(const std::string& session_id);
    int get_active_session_count() const;
    
    // C++26: Administrative functions (std::expected)
    std::expected<void, SessionError> force_logout_user(const std::string& user_id);
    std::expected<void, SessionError> force_logout_session(const std::string& session_id);
    void cleanup_expired_sessions();
    void cleanup_old_auth_attempts();
    
    // Metrics and monitoring
    struct JWTMetrics {
        uint64_t tokens_issued;
        uint64_t tokens_validated;
        uint64_t tokens_rejected;
        uint64_t permissions_checked;
        uint64_t active_sessions;
        double avg_token_validation_time_ms;
        double avg_permission_check_time_ms;
    };
    
    JWTMetrics get_metrics() const;
    
private:
    // JWT operations
    std::string create_jwt_token(const JWTClaims& claims);
    bool parse_jwt_token(const std::string& token, JWTClaims& claims);
    std::string generate_secure_random(size_t length = 32);
    std::string generate_device_fingerprint(const std::string& user_agent, const std::string& ip);
    
    // Database operations
    bool save_session_to_database(const SessionInfo& session);
    bool load_session_from_database(const std::string& session_id, SessionInfo& session);
    bool delete_session_from_database(const std::string& session_id);
    bool load_user_permissions_from_database(const std::string& user_id, std::vector<std::string>& permissions);
    bool save_auth_attempt_to_database(const AuthenticationAttempt& attempt);
    
    // Permission system operations
    bool load_permissions_from_database();
    bool load_roles_from_database();
    bool load_role_permissions_from_database();
    std::vector<std::string> resolve_inherited_permissions(const std::string& role_id);
    bool is_permission_cached_valid(const SessionInfo& session);
    void update_permission_cache(SessionInfo& session);
    
    // Security operations
    bool validate_ip_address(const std::string& expected_ip, const std::string& actual_ip);
    bool validate_user_agent(const std::string& expected_ua, const std::string& actual_ua);
    bool is_session_expired(const SessionInfo& session);
    bool is_session_valid(const SessionInfo& session, const std::string& ip, const std::string& user_agent);
    
    // Background maintenance
    void maintenance_worker();
    void cleanup_expired_tokens();
    void cleanup_rate_limit_data();
    void refresh_permission_cache();
    
    // Utility functions
    std::string hash_token(const std::string& token);
    std::chrono::system_clock::time_point string_to_timepoint(const std::string& time_str);
    std::string timepoint_to_string(const std::chrono::system_clock::time_point& tp);
    bool is_ip_address_valid(const std::string& ip);
    std::string extract_user_agent_signature(const std::string& user_agent);
};

// ========================================
// JWT MIDDLEWARE FOR WEB INTEGRATION
// ========================================

class JWTMiddleware {
private:
    std::shared_ptr<JWTSessionManager> session_manager_;
    
public:
    JWTMiddleware(std::shared_ptr<JWTSessionManager> session_manager);
    
    // HTTP middleware functions
    bool authenticate_request(const std::string& auth_header, const std::string& ip_address,
                             const std::string& user_agent, JWTClaims& claims);
    bool authorize_request(const std::string& session_id, const std::string& required_permission);
    bool authorize_request_multiple(const std::string& session_id, 
                                   const std::vector<std::string>& required_permissions);
    
    // Helper functions for web frameworks
    std::string extract_bearer_token(const std::string& auth_header);
    std::map<std::string, std::string> create_auth_response_headers(const std::string& access_token,
                                                                   const std::string& refresh_token = "");
    json create_auth_response_body(const SessionInfo& session, const std::string& message = "");
    
    // Error responses
    json create_unauthorized_response(const std::string& reason = "Invalid token");
    json create_forbidden_response(const std::string& reason = "Insufficient permissions");
    json create_rate_limited_response(const std::string& reason = "Too many requests");
};

// ========================================
// PERMISSION CHECKER UTILITY CLASS
// ========================================

class PermissionChecker {
private:
    std::shared_ptr<JWTSessionManager> session_manager_;
    
public:
    PermissionChecker(std::shared_ptr<JWTSessionManager> session_manager);
    
    // Permission validation decorators
    bool require_permission(const std::string& session_id, const std::string& permission);
    bool require_any_permission(const std::string& session_id, const std::vector<std::string>& permissions);
    bool require_all_permissions(const std::string& session_id, const std::vector<std::string>& permissions);
    bool require_role(const std::string& session_id, const std::string& role);
    bool require_minimum_role_level(const std::string& session_id, int min_level);
    
    // Predefined permission checks
    bool can_create_items(const std::string& session_id);
    bool can_execute_api(const std::string& session_id);
    bool can_audit(const std::string& session_id);
    bool can_modify_permissions(const std::string& session_id);
    bool can_manage_users(const std::string& session_id);
    bool can_access_database(const std::string& session_id);
    bool can_manage_system(const std::string& session_id);
    
    // Permission categories
    bool has_items_permissions(const std::string& session_id);
    bool has_api_permissions(const std::string& session_id);
    bool has_ai_command_permissions(const std::string& session_id);
    bool has_audit_permissions(const std::string& session_id);
    bool has_permissions_system_access(const std::string& session_id);
    bool has_admin_permissions(const std::string& session_id);
};

} // namespace jwt
} // namespace portal
} // namespace medusa

// C API for web integration
extern "C" {
    // JWT session management
    bool medusa_jwt_validate_token(const char* token, const char* ip, const char* user_agent);
    bool medusa_jwt_check_permission(const char* session_id, const char* permission);
    const char* medusa_jwt_create_session(const char* username, const char* password, 
                                         const char* ip, const char* user_agent);
    bool medusa_jwt_invalidate_session(const char* session_id);
    
    // Permission matrix operations
    const char* medusa_jwt_get_permission_matrix();
    bool medusa_jwt_update_role_permission(const char* role_id, const char* permission_id, bool grant);
    const char* medusa_jwt_get_user_permissions(const char* session_id);
    
    // Security operations
    bool medusa_jwt_is_rate_limited(const char* ip_address);
    void medusa_jwt_blacklist_token(const char* token);
    const char* medusa_jwt_get_security_metrics();
}