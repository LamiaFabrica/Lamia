#ifndef MEDUSA_NATIVE_SERVER_HPP
#define MEDUSA_NATIVE_SERVER_HPP

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <thread>
#include <mutex>

/**
 * @file medusa_native_server.hpp
 * @brief Native C++ Server that outputs MDE files directly to browser
 * @details GROUND UP PHILOSOPHY - No file generation, direct output from native code
 * 
 * Architecture:
 * MedusaServ (Bespoke Server) <-> Medusa (Native C++ App) -> Direct HTML5.2 Output
 * 
 * NO SHORTCUTS: Everything written in native C++ from scratch
 */

namespace MedusaNative {

/**
 * @struct NativeMDEPage
 * @brief Native C++ representation of MDE page structure
 */
struct NativeMDEPage {
    // Required 4 sections - generated natively in C++
    std::string top_section_html;
    std::string footer_section_html; 
    std::string sidebar_section_html;
    std::string main_content_html;
    
    // XML-clustered JavaScript (included but not delivered)
    std::string xml_clustered_js;
    
    // SEO metadata
    std::string seo_url;
    std::unordered_map<std::string, std::string> meta_tags;
    
    // Security hashes
    std::string integrity_hash;
    std::string payload_hash;
};

/**
 * @class NativeHTTPServer
 * @brief Native C++ HTTP server that outputs MDE directly
 */
class NativeHTTPServer {
private:
    int server_socket_;
    std::thread server_thread_;
    std::mutex request_mutex_;
    bool running_;
    
    // Native request handling
    void handleClientConnection(int client_socket);
    void processHTTPRequest(const std::string& request, int client_socket);
    
    // Native HTML generation
    std::string generateHTML5Output(const NativeMDEPage& page);
    std::string generateCSS3Styles();
    std::string generateResponseHeaders(size_t content_length);
    
    // Path resolution for logical folder structure
    std::filesystem::path resolvePagePath(const std::string& url);
    
public:
    NativeHTTPServer();
    ~NativeHTTPServer();
    
    /**
     * @brief Start native server on specified port
     */
    bool startServer(int port = 8080);
    
    /**
     * @brief Stop native server
     */
    void stopServer();
    
    /**
     * @brief Process MDS and output HTML5.2 directly to client
     */
    void serveMDEPage(const std::string& url, int client_socket);
};

/**
 * @class NativeMDSProcessor  
 * @brief Native C++ MDS processor that outputs directly
 */
class NativeMDSProcessor {
private:
    std::unordered_map<std::string, NativeMDEPage> page_cache_;
    std::mutex cache_mutex_;
    
    // Native XML parsing (no external libraries)
    bool parseNativeMDS(const std::string& mds_content, NativeMDEPage& page);
    std::string extractTagContent(const std::string& xml, const std::string& tag);
    std::vector<std::string> extractAttributes(const std::string& element);
    
    // Native encryption handling
    std::string decryptPayload(const std::string& encrypted_payload);
    bool verifyIntegrity(const std::string& content, const std::string& hash);
    
    // Native HTML5.2 generation
    std::string generateNativeSection(const std::string& section_name, 
                                     const std::vector<std::string>& elements);
    
public:
    /**
     * @brief Process MDS file and return native MDE page
     */
    NativeMDEPage processMDSFile(const std::filesystem::path& mds_path);
    
    /**
     * @brief Generate HTML5.2 output natively
     */
    std::string generateNativeHTML(const NativeMDEPage& page);
    
    /**
     * @brief Cache page for performance
     */
    void cachePage(const std::string& url, const NativeMDEPage& page);
    
    /**
     * @brief Get cached page
     */
    bool getCachedPage(const std::string& url, NativeMDEPage& page);
};

/**
 * @class NativePathResolver
 * @brief Native path resolution for logical folder structure
 */
class NativePathResolver {
private:
    std::filesystem::path base_path_;
    
public:
    explicit NativePathResolver(const std::string& base = "/mnt/c/Build/medusa_dashboard_complete_v0.2.8a");
    
    // Native path generation for logical structure
    std::filesystem::path getIncludePath(const std::string& lang, 
                                        const std::string& category,
                                        const std::string& function);
    
    std::filesystem::path getLibraryPath(const std::string& lib_name);
    std::filesystem::path getConfigPath(const std::string& config_name);
    std::filesystem::path getImagePath(bool is_public, const std::string& image_name);
    std::filesystem::path getUserPath(const std::string& user_slug, const std::string& lang);
    
    // SEO URL generation (Index.mde -> Index)
    std::string generateSEOUrl(const std::string& file_path);
    bool isValidPath(const std::string& requested_path);
};

/**
 * @class NativeSecurityManager
 * @brief Native security for request validation
 */
class NativeSecurityManager {
private:
    std::unordered_map<std::string, int> rate_limits_;
    std::mutex security_mutex_;
    
public:
    /**
     * @brief Validate incoming request for security
     */
    bool validateRequest(const std::string& request);
    
    /**
     * @brief Check rate limiting
     */
    bool checkRateLimit(const std::string& client_ip);
    
    /**
     * @brief Validate URL for directory traversal
     */
    bool validateURL(const std::string& url);
    
    /**
     * @brief Generate security headers
     */
    std::string generateSecurityHeaders();
};

/**
 * @class MedusaServNative
 * @brief Main native server class - GROUND UP IMPLEMENTATION
 */
class MedusaServNative {
private:
    NativeHTTPServer http_server_;
    NativeMDSProcessor mds_processor_;
    NativePathResolver path_resolver_;
    NativeSecurityManager security_manager_;
    
    std::vector<std::string> audit_log_;
    
public:
    /**
     * @brief Initialize native MedusaServ
     */
    bool initialize();
    
    /**
     * @brief Start serving MDE pages natively
     */
    bool startServing(int port = 8080);
    
    /**
     * @brief Stop native server
     */
    void shutdown();
    
    /**
     * @brief Generate JSONB audit report
     */
    std::string generateJSONBAudit();
    
    /**
     * @brief Get server status
     */
    std::string getServerStatus();
};

} // namespace MedusaNative

#endif // MEDUSA_NATIVE_SERVER_HPP