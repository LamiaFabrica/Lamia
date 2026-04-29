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
 * MEDUSASERV URL ROUTER v1.0.0
 * ============================
 * Temporary URL system with User= parameter support
 * Handles localhost/User=username/ routing with security sandboxing
 * © 2025 The Medusa Initiative | Yorkshire Champion Standards
 */

#ifndef MEDUSASERV_URL_ROUTER_HPP
#define MEDUSASERV_URL_ROUTER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <regex>
#include <chrono>
#include <memory>
#include <iostream>

namespace MedusaServ {
namespace URLRouting {

struct TemporaryRoute {
    std::string username;
    std::string target_path;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point last_accessed;
    bool is_active;
    std::unordered_map<std::string, std::string> permissions;
    
    TemporaryRoute(const std::string& user, const std::string& path)
        : username(user), target_path(path), 
          created_at(std::chrono::system_clock::now()),
          last_accessed(std::chrono::system_clock::now()),
          is_active(true) {
        
        // Default permissions for temporary routes
        permissions["read"] = "true";
        permissions["write"] = "false";
        permissions["admin"] = "false";
        permissions["api_access"] = "limited";
    }
};

class URLRouter {
private:
    std::unordered_map<std::string, std::unique_ptr<TemporaryRoute>> temp_routes_;
    std::regex user_param_pattern_;
    std::chrono::hours expiry_time_;
    
public:
    URLRouter() 
        : user_param_pattern_(R"(/User=([^/]+)/?(.*)?)"),
          expiry_time_(24) { // 24 hours expiry
        
        std::cout << "🔗 MedusaServ URL Router initialized" << std::endl;
        std::cout << "   Temporary URL support: Active" << std::endl;
        std::cout << "   Route expiry: 24 hours" << std::endl;
    }
    
    /**
     * Parse incoming URL and extract User parameter
     */
    struct ParsedURL {
        bool has_user_param = false;
        std::string username;
        std::string remaining_path;
        std::string original_url;
    };
    
    ParsedURL parseURL(const std::string& url) {
        ParsedURL result;
        result.original_url = url;
        
        std::smatch matches;
        if (std::regex_search(url, matches, user_param_pattern_)) {
            result.has_user_param = true;
            result.username = matches[1].str();
            result.remaining_path = matches[2].str();
            
            std::cout << "🔗 Parsed temporary URL:" << std::endl;
            std::cout << "   Username: " << result.username << std::endl;
            std::cout << "   Path: /" << result.remaining_path << std::endl;
        }
        
        return result;
    }
    
    /**
     * Create or update temporary route
     */
    bool createTemporaryRoute(const std::string& username, const std::string& target_path = "/") {
        // Security validation
        if (!isValidUsername(username)) {
            std::cerr << "❌ Invalid username for temporary route: " << username << std::endl;
            return false;
        }
        
        // Create or update route
        auto route_key = "temp_" + username;
        temp_routes_[route_key] = std::make_unique<TemporaryRoute>(username, target_path);
        
        std::cout << "✅ Created temporary route: /User=" << username << "/" << std::endl;
        std::cout << "   Target: " << target_path << std::endl;
        std::cout << "   Expires: 24 hours from now" << std::endl;
        
        return true;
    }
    
    /**
     * Route request through temporary URL system
     */
    std::string routeRequest(const std::string& url) {
        auto parsed = parseURL(url);
        
        if (!parsed.has_user_param) {
            // Normal routing
            return url;
        }
        
        // Check if temporary route exists
        auto route_key = "temp_" + parsed.username;
        auto route_it = temp_routes_.find(route_key);
        
        if (route_it == temp_routes_.end()) {
            // Create new temporary route
            if (createTemporaryRoute(parsed.username)) {
                route_it = temp_routes_.find(route_key);
            } else {
                return "/error/invalid_route";
            }
        }
        
        auto& route = route_it->second;
        
        // Check if route has expired
        if (isRouteExpired(route.get())) {
            std::cout << "⏰ Temporary route expired for user: " << parsed.username << std::endl;
            temp_routes_.erase(route_it);
            return "/error/route_expired";
        }
        
        // Update access time
        route->last_accessed = std::chrono::system_clock::now();
        
        // Build target path
        std::string target_path = "/web/site/" + parsed.username + "/";
        if (!parsed.remaining_path.empty()) {
            target_path += parsed.remaining_path;
        }
        
        std::cout << "🎯 Routed temporary URL: " << url << std::endl;
        std::cout << "   Target: " << target_path << std::endl;
        
        return target_path;
    }
    
    /**
     * Get route information for debugging
     */
    void printRouteStatus() {
        std::cout << std::endl << "🔗 TEMPORARY ROUTE STATUS" << std::endl;
        std::cout << "========================" << std::endl;
        
        if (temp_routes_.empty()) {
            std::cout << "No active temporary routes" << std::endl;
            return;
        }
        
        auto now = std::chrono::system_clock::now();
        
        for (const auto& [key, route] : temp_routes_) {
            auto hours_since_created = std::chrono::duration_cast<std::chrono::hours>(
                now - route->created_at).count();
            auto hours_since_accessed = std::chrono::duration_cast<std::chrono::hours>(
                now - route->last_accessed).count();
            
            std::cout << "Route: /User=" << route->username << "/" << std::endl;
            std::cout << "  Target: " << route->target_path << std::endl;
            std::cout << "  Created: " << hours_since_created << " hours ago" << std::endl;
            std::cout << "  Last Access: " << hours_since_accessed << " hours ago" << std::endl;
            std::cout << "  Status: " << (route->is_active ? "Active" : "Inactive") << std::endl;
            std::cout << "  Expires In: " << (24 - hours_since_created) << " hours" << std::endl;
            std::cout << std::endl;
        }
    }
    
    /**
     * Clean up expired routes
     */
    void cleanupExpiredRoutes() {
        auto it = temp_routes_.begin();
        while (it != temp_routes_.end()) {
            if (isRouteExpired(it->second.get())) {
                std::cout << "🧹 Cleaning up expired route for user: " << it->second->username << std::endl;
                it = temp_routes_.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    /**
     * Get active route count
     */
    size_t getActiveRouteCount() const {
        return temp_routes_.size();
    }

private:
    bool isValidUsername(const std::string& username) {
        // Basic security validation
        if (username.empty() || username.length() > 50) {
            return false;
        }
        
        // Only allow alphanumeric and safe characters
        std::regex safe_pattern(R"(^[a-zA-Z0-9_-]+$)");
        return std::regex_match(username, safe_pattern);
    }
    
    bool isRouteExpired(const TemporaryRoute* route) {
        auto now = std::chrono::system_clock::now();
        auto time_since_created = std::chrono::duration_cast<std::chrono::hours>(
            now - route->created_at);
        
        return time_since_created >= expiry_time_;
    }
};

// Global URL Router instance
extern URLRouter g_url_router;

} // namespace URLRouting
} // namespace MedusaServ

#endif // MEDUSASERV_URL_ROUTER_HPP