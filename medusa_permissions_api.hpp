/*
 * MEDUSA PERMISSIONS API - REST ENDPOINTS
 * ======================================
 * 
 * "API endpoints for Yorkshire Champion permission management!" 🔐⚔️🛡️
 * Complete REST API for the Medusa Permission System
 * 
 * API ENDPOINTS:
 * - GET /api/godmode/admin/roles - List all roles
 * - GET /api/godmode/admin/permissions - List all permissions  
 * - GET /api/godmode/admin/permission-matrix - Get complete permission matrix
 * - PUT /api/godmode/admin/role-permissions - Update role-permission assignments
 * - POST /api/auth/login - User authentication with JWT
 * - POST /api/auth/refresh - Refresh JWT tokens
 * - DELETE /api/auth/logout - User logout
 * - GET /api/auth/me - Get current user info and permissions
 * 
 * SECURITY FEATURES:
 * - JWT token authentication
 * - Real-time permission validation
 * - Role-based access control
 * - Audit logging for all operations
 * - Rate limiting and brute force protection
 * - Input validation and sanitization
 * 
 * INTEGRATION:
 * - TRIFORCE database backend
 * - JWT session management
 * - Permission matrix real-time updates
 * - WebSocket for live updates
 * 
 * "API security worthy of Yorkshire Champions!" 🏆🔒
 */

#pragma once

#include "medusa_jwt_session_manager.hpp"
#include "medusa_portal_authentication_system.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <regex>

namespace medusa {
namespace portal {
namespace api {

using json = nlohmann::json;
using namespace jwt;

// ========================================
// HTTP REQUEST/RESPONSE STRUCTURES
// ========================================

struct HttpRequest {
    std::string method;          // GET, POST, PUT, DELETE, etc.
    std::string path;           // URL path
    std::string query_string;   // Query parameters
    std::map<std::string, std::string> headers;
    std::string body;           // Request body
    std::string remote_ip;      // Client IP address
    std::string user_agent;     // User agent string
    
    // Parsed data
    json json_body;             // Parsed JSON body
    std::map<std::string, std::string> query_params;  // Parsed query parameters
    std::map<std::string, std::string> path_params;   // Path parameters from routing
};

struct HttpResponse {
    int status_code = 200;
    std::map<std::string, std::string> headers;
    std::string body;
    json json_body;
    
    // Helper methods
    void set_json(const json& data);
    void set_error(int code, const std::string& message, const std::string& details = "");
    void set_success(const json& data, const std::string& message = "");
    void set_cors_headers();
    void set_security_headers();
};

// ========================================
// API ROUTE HANDLER TYPE
// ========================================

using RouteHandler = std::function<HttpResponse(const HttpRequest&, const std::map<std::string, std::string>&)>;

// ========================================
// MEDUSA PERMISSIONS API CONTROLLER
// ========================================

class MedusaPermissionsAPI {
private:
    std::shared_ptr<JWTSessionManager> jwt_manager_;
    std::shared_ptr<TriforceAuthenticationManager> auth_manager_;
    std::shared_ptr<JWTMiddleware> jwt_middleware_;
    std::shared_ptr<PermissionChecker> permission_checker_;
    
    // Route mapping
    std::map<std::string, std::map<std::string, RouteHandler>> routes_;
    
    // Rate limiting
    std::map<std::string, std::vector<std::chrono::system_clock::time_point>> rate_limits_;
    std::mutex rate_limit_mutex_;
    
    // API configuration
    struct APIConfig {
        int max_requests_per_minute = 60;
        int max_auth_requests_per_minute = 10;
        bool enable_cors = true;
        bool enable_audit_logging = true;
        std::string cors_origin = "*";
        std::vector<std::string> trusted_proxies;
    } config_;
    
public:
    MedusaPermissionsAPI(std::shared_ptr<JWTSessionManager> jwt_manager,
                        std::shared_ptr<TriforceAuthenticationManager> auth_manager);
    ~MedusaPermissionsAPI();
    
    // API initialization
    bool initialize();
    void shutdown();
    
    // Request handling
    HttpResponse handle_request(const HttpRequest& request);
    HttpResponse route_request(const std::string& method, const std::string& path, const HttpRequest& request);
    
    // Route registration
    void register_routes();
    void add_route(const std::string& method, const std::string& path, RouteHandler handler);
    
    // Middleware
    bool authenticate_request(const HttpRequest& request, JWTClaims& claims);
    bool authorize_request(const JWTClaims& claims, const std::string& required_permission);
    bool check_rate_limit(const std::string& ip_address, const std::string& endpoint_type = "general");
    
    // ========================================
    // AUTHENTICATION ENDPOINTS
    // ========================================
    
    // POST /api/auth/login
    HttpResponse login(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // POST /api/auth/refresh
    HttpResponse refresh_token(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // DELETE /api/auth/logout
    HttpResponse logout(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // GET /api/auth/me
    HttpResponse get_current_user(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // ========================================
    // ROLE MANAGEMENT ENDPOINTS
    // ========================================
    
    // GET /api/godmode/admin/roles
    HttpResponse get_roles(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // POST /api/godmode/admin/roles
    HttpResponse create_role(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // PUT /api/godmode/admin/roles/{role_id}
    HttpResponse update_role(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // DELETE /api/godmode/admin/roles/{role_id}
    HttpResponse delete_role(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // ========================================
    // PERMISSION MANAGEMENT ENDPOINTS
    // ========================================
    
    // GET /api/godmode/admin/permissions
    HttpResponse get_permissions(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // POST /api/godmode/admin/permissions
    HttpResponse create_permission(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // PUT /api/godmode/admin/permissions/{permission_id}
    HttpResponse update_permission(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // DELETE /api/godmode/admin/permissions/{permission_id}
    HttpResponse delete_permission(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // ========================================
    // PERMISSION MATRIX ENDPOINTS
    // ========================================
    
    // GET /api/godmode/admin/permission-matrix
    HttpResponse get_permission_matrix(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // PUT /api/godmode/admin/role-permissions
    HttpResponse update_role_permissions(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // POST /api/godmode/admin/role-permissions/bulk
    HttpResponse bulk_update_role_permissions(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // ========================================
    // USER MANAGEMENT ENDPOINTS
    // ========================================
    
    // GET /api/godmode/admin/users
    HttpResponse get_users(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // PUT /api/godmode/admin/users/{user_id}/roles
    HttpResponse update_user_roles(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // GET /api/godmode/admin/users/{user_id}/permissions
    HttpResponse get_user_permissions(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // ========================================
    // SESSION MANAGEMENT ENDPOINTS
    // ========================================
    
    // GET /api/godmode/admin/sessions
    HttpResponse get_active_sessions(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // DELETE /api/godmode/admin/sessions/{session_id}
    HttpResponse terminate_session(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // DELETE /api/godmode/admin/users/{user_id}/sessions
    HttpResponse terminate_user_sessions(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // ========================================
    // AUDIT AND MONITORING ENDPOINTS
    // ========================================
    
    // GET /api/godmode/admin/audit/permissions
    HttpResponse get_permission_audit_log(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // GET /api/godmode/admin/audit/auth-attempts
    HttpResponse get_auth_attempts(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // GET /api/godmode/admin/metrics
    HttpResponse get_system_metrics(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // ========================================
    // SECURITY ENDPOINTS
    // ========================================
    
    // POST /api/godmode/admin/security/blacklist-token
    HttpResponse blacklist_token(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // GET /api/godmode/admin/security/risk-analysis
    HttpResponse get_security_risk_analysis(const HttpRequest& request, const std::map<std::string, std::string>& params);
    
    // ========================================
    // UTILITY METHODS
    // ========================================
    
    // Request parsing
    bool parse_json_body(const HttpRequest& request, json& parsed_json);
    std::map<std::string, std::string> parse_query_params(const std::string& query_string);
    std::string extract_bearer_token(const HttpRequest& request);
    
    // Response building
    HttpResponse create_success_response(const json& data, const std::string& message = "Success");
    HttpResponse create_error_response(int status_code, const std::string& error, const std::string& message);
    HttpResponse create_validation_error_response(const std::vector<std::string>& errors);
    HttpResponse create_unauthorized_response(const std::string& reason = "Unauthorized");
    HttpResponse create_forbidden_response(const std::string& reason = "Forbidden");
    HttpResponse create_rate_limited_response();
    
    // Validation
    bool validate_role_data(const json& role_data, std::vector<std::string>& errors);
    bool validate_permission_data(const json& permission_data, std::vector<std::string>& errors);
    bool validate_user_id(const std::string& user_id_str, int& user_id);
    bool validate_pagination_params(const std::map<std::string, std::string>& params, int& page, int& limit);
    
    // Audit logging
    void log_api_request(const HttpRequest& request, const HttpResponse& response, const JWTClaims* claims = nullptr);
    void log_permission_check(const std::string& user_id, const std::string& permission, bool granted, const std::string& resource = "");
    
    // Error handling
    HttpResponse handle_database_error(const std::exception& e);
    HttpResponse handle_jwt_error(const std::exception& e);
    HttpResponse handle_validation_error(const std::vector<std::string>& errors);
    
private:
    // Internal helper methods
    bool extract_path_params(const std::string& route_pattern, const std::string& actual_path, 
                           std::map<std::string, std::string>& params);
    std::string get_client_ip(const HttpRequest& request);
    bool is_trusted_proxy(const std::string& ip);
    void add_security_headers(HttpResponse& response);
    void add_cors_headers(HttpResponse& response, const HttpRequest& request);
    
    // Database operations
    json fetch_roles_from_database();
    json fetch_permissions_from_database();
    json fetch_permission_matrix_from_database();
    bool save_role_to_database(const json& role_data);
    bool save_permission_to_database(const json& permission_data);
    bool update_role_permissions_in_database(const std::string& role_id, const std::vector<std::string>& permission_ids);
    json fetch_users_from_database(int page, int limit, const std::string& search = "");
    json fetch_user_permissions_from_database(int user_id);
    json fetch_active_sessions_from_database();
    json fetch_audit_logs_from_database(const std::string& type, int page, int limit);
    json fetch_system_metrics_from_database();
};

// ========================================
// WEBSOCKET NOTIFICATION SYSTEM
// ========================================

class PermissionMatrixNotifier {
private:
    std::vector<std::function<void(const json&)>> subscribers_;
    std::mutex subscribers_mutex_;
    
public:
    PermissionMatrixNotifier();
    ~PermissionMatrixNotifier();
    
    // Subscription management
    void subscribe(std::function<void(const json&)> callback);
    void unsubscribe(std::function<void(const json&)> callback);
    
    // Notification broadcasting
    void notify_permission_matrix_updated(const json& changes);
    void notify_role_updated(const json& role_data);
    void notify_permission_updated(const json& permission_data);
    void notify_user_roles_updated(int user_id, const json& role_changes);
    
    // WebSocket integration
    void handle_websocket_connection(const std::string& session_id);
    void handle_websocket_disconnection(const std::string& session_id);
    void send_websocket_message(const std::string& session_id, const json& message);
};

// ========================================
// API RESPONSE BUILDERS
// ========================================

namespace ResponseBuilder {
    // Authentication responses
    json build_login_response(const SessionInfo& session, const std::string& access_token, const std::string& refresh_token);
    json build_user_info_response(const MedusaUser& user, const std::vector<std::string>& permissions);
    
    // Role management responses
    json build_roles_response(const std::vector<Role>& roles);
    json build_role_response(const Role& role);
    
    // Permission management responses
    json build_permissions_response(const std::vector<Permission>& permissions);
    json build_permission_response(const Permission& permission);
    
    // Permission matrix responses
    json build_permission_matrix_response(const PermissionMatrix& matrix);
    json build_role_permissions_response(const std::string& role_id, const std::vector<std::string>& permissions);
    
    // User management responses
    json build_users_response(const std::vector<MedusaUser>& users, int total_count, int page, int limit);
    json build_user_permissions_response(int user_id, const std::vector<std::string>& permissions);
    
    // Session management responses
    json build_sessions_response(const std::vector<SessionInfo>& sessions);
    json build_session_response(const SessionInfo& session);
    
    // Audit responses
    json build_audit_log_response(const std::vector<json>& audit_entries, int total_count, int page, int limit);
    json build_metrics_response(const JWTSessionManager::JWTMetrics& metrics);
    
    // Error responses
    json build_error_response(const std::string& error, const std::string& message, const std::vector<std::string>& details = {});
    json build_validation_error_response(const std::vector<std::string>& validation_errors);
}

} // namespace api
} // namespace portal
} // namespace medusa

// C API for web server integration
extern "C" {
    // API initialization
    void* medusa_permissions_api_create(void* jwt_manager, void* auth_manager);
    void medusa_permissions_api_destroy(void* api);
    bool medusa_permissions_api_initialize(void* api);
    
    // Request handling
    const char* medusa_permissions_api_handle_request(void* api, const char* method, const char* path, 
                                                     const char* headers_json, const char* body, 
                                                     const char* remote_ip, const char* user_agent);
    
    // Route management
    void medusa_permissions_api_register_routes(void* api);
    
    // Utility functions
    bool medusa_permissions_api_authenticate(void* api, const char* auth_header, const char* ip, const char* user_agent);
    bool medusa_permissions_api_authorize(void* api, const char* session_id, const char* permission);
    const char* medusa_permissions_api_get_user_permissions(void* api, const char* session_id);
}

#endif // MEDUSA_PERMISSIONS_API_HPP