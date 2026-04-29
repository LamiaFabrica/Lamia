/**
 * MEDUSASERV SUBDOMAIN MANAGER v0.3.0c
 * ====================================
 * Full wrap around care subdomain management system
 * © 2025 The Medusa Project
 */

#ifndef MEDUSASERV_SUBDOMAIN_MANAGER_HPP
#define MEDUSASERV_SUBDOMAIN_MANAGER_HPP

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the subdomain manager
 * @param base_domain Base domain for subdomain creation (e.g., "poweredbymedusa.com")
 * @return 1 on success, 0 on failure
 */
int initialize_subdomain_manager(const char* base_domain);

/**
 * Create a new subdomain with template
 * @param subdomain Subdomain name (e.g., "blog", "shop", "portfolio")
 * @param template_type Template type ("basic", "portfolio", "business")
 * @return 1 on success, 0 on failure
 */
int create_subdomain(const char* subdomain, const char* template_type);

/**
 * Delete a subdomain and all its resources
 * @param full_domain Full domain name (e.g., "blog.poweredbymedusa.com")
 * @return 1 on success, 0 on failure
 */
int delete_subdomain(const char* full_domain);

/**
 * Route subdomain requests to correct file paths
 * @param hostname Full hostname (e.g., "blog.poweredbymedusa.com")
 * @param path Request path (e.g., "/", "/about", "/contact")
 * @return Allocated file path string (caller must free with free_subdomain_string)
 */
char* route_subdomain(const char* hostname, const char* path);

/**
 * Free allocated subdomain string
 * @param str String to free
 */
void free_subdomain_string(char* str);

#ifdef __cplusplus
}

// C++ interface for advanced users
namespace medusaserv {
namespace subdomain {
    class SubdomainManager;
    
    struct SubdomainConfig {
        std::string subdomain;
        std::string domain;
        std::string full_domain;
        std::string root_path;
        std::string template_type;
        bool ssl_enabled;
        bool auto_ssl;
        std::string ssl_provider;
        int port;
        std::string status;
        std::string created_date;
        std::string last_modified;
    };
}
}
#endif

#endif // MEDUSASERV_SUBDOMAIN_MANAGER_HPP