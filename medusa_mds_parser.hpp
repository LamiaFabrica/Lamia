#ifndef MEDUSA_MDS_PARSER_HPP
#define MEDUSA_MDS_PARSER_HPP

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <openssl/sha.h>
#include <openssl/evp.h>

/**
 * @file medusa_mds_parser.hpp
 * @brief Medusa MDS Parser - C++ App for generating MDE cached files
 * @details Medusa creates MDE files that MedusaServ serves to browsers
 * 
 * Folder Structure:
 * ./inc/<lang>/<category>/[function|feature|service]
 * ./libs - .so libraries  
 * ./cnf - config files
 * ./img/public - website images
 * ./img/private - admin images (not indexed)
 * ./usr/<user_slug>/inc/<lang> - user specific
 * ./ext - extensions
 * ./mod - modules
 * ./plg - plugins
 */

namespace Medusa {

/**
 * @struct PathResolver
 * @brief Manages relative function paths and folder structure
 */
class PathResolver {
private:
    std::filesystem::path base_path_;
    std::string current_lang_;
    std::string current_category_;
    
public:
    PathResolver(const std::string& base = ".");
    
    // Dynamic path generation
    std::filesystem::path getIncPath(const std::string& lang, 
                                     const std::string& category,
                                     const std::string& function);
    std::filesystem::path getLibPath(const std::string& lib_name);
    std::filesystem::path getConfigPath(const std::string& config_name);
    std::filesystem::path getImagePath(bool is_public, const std::string& image_name);
    std::filesystem::path getUserPath(const std::string& user_slug, 
                                      const std::string& lang);
    std::filesystem::path getExtensionPath(const std::string& ext_name);
    std::filesystem::path getModulePath(const std::string& mod_name);
    std::filesystem::path getPluginPath(const std::string& plugin_name);
    
    // SEO URL shortcoding (Index.mde -> Index)
    std::string generateSEOUrl(const std::filesystem::path& mde_path);
};

/**
 * @struct MDESection
 * @brief Required sections for every MDE file
 */
struct MDESection {
    std::string top_section;      // Header/navigation
    std::string footer_section;   // Footer content
    std::string sidebar_section;  // Side navigation
    std::string main_content;     // Primary content
    
    // XML-clustered JavaScript (included but not delivered)
    std::string xml_clustered_js;
};

/**
 * @struct MDEMetadata
 * @brief MDE file metadata for caching and serving
 */
struct MDEMetadata {
    std::string lang;              // Language slug (en, es, fr, de)
    std::string category;          // Category slug  
    std::string function_type;     // function|feature|service
    std::string cache_key;         // Unique cache identifier
    std::string integrity_hash;    // SHA-256 for tamper detection
    uint64_t timestamp;            // Creation timestamp
    bool is_user_specific;         // User-specific content flag
    std::string user_slug;         // User identifier if applicable
};

/**
 * @class MDSParser
 * @brief Parses MDS files and generates MDE cached output
 */
class MDSParser {
private:
    PathResolver path_resolver_;
    std::string master_key_;
    
    // Double encryption components
    std::string inner_key_;
    std::string outer_key_;
    
    // Audit logging
    std::vector<std::string> audit_log_;
    
    /**
     * @brief Parse MDS XML structure
     */
    bool parseXMLStructure(const std::string& xml_content,
                          MDESection& sections);
    
    /**
     * @brief Apply double AES-256-GCM encryption
     */
    std::string doubleEncrypt(const std::string& data);
    
    /**
     * @brief Generate SHA-256 integrity hash
     */
    std::string generateIntegrityHash(const std::string& content);
    
    /**
     * @brief Optimize for gold standard performance
     */
    void optimizeContent(std::string& content);
    
public:
    explicit MDSParser(const std::string& base_path = ".");
    
    /**
     * @brief Parse MDS and generate MDE cached file
     * @param mds_path Path to source .mds file
     * @param lang Language slug
     * @param category Category slug  
     * @param function_type function|feature|service
     * @return Path to generated MDE file
     */
    std::filesystem::path generateMDE(const std::filesystem::path& mds_path,
                                      const std::string& lang,
                                      const std::string& category,
                                      const std::string& function_type);
    
    /**
     * @brief Generate user-specific MDE file
     */
    std::filesystem::path generateUserMDE(const std::filesystem::path& mds_path,
                                          const std::string& user_slug,
                                          const std::string& lang);
    
    /**
     * @brief Create HTML5.2 compliant output from MDE
     */
    std::string renderToHTML5(const MDESection& sections,
                              const MDEMetadata& metadata);
    
    /**
     * @brief Generate JSONB audit for validation
     */
    std::string generateJSONBAudit();
    
    /**
     * @brief Get parser status
     */
    std::string getStatus() const;
};

/**
 * @class MDECache
 * @brief Manages MDE file caching for MedusaServ
 */
class MDECache {
private:
    PathResolver path_resolver_;
    std::unordered_map<std::string, std::string> cache_map_;
    std::unordered_map<std::string, MDEMetadata> metadata_map_;
    
    // Gold standard cache optimization
    size_t max_cache_size_;
    size_t current_cache_size_;
    
public:
    explicit MDECache(size_t max_size = 1024 * 1024 * 100); // 100MB default
    
    /**
     * @brief Store MDE in cache
     */
    bool store(const std::string& cache_key,
               const std::string& content,
               const MDEMetadata& metadata);
    
    /**
     * @brief Retrieve MDE from cache
     */
    std::string retrieve(const std::string& cache_key);
    
    /**
     * @brief Check if MDE exists and is valid
     */
    bool isValid(const std::string& cache_key);
    
    /**
     * @brief Invalidate cache entry
     */
    void invalidate(const std::string& cache_key);
    
    /**
     * @brief Clear entire cache
     */
    void clear();
    
    /**
     * @brief Get cache statistics as JSONB
     */
    std::string getCacheStats() const;
};

/**
 * @class LibraryLoader
 * @brief Manages .so library loading from ./libs
 */
class LibraryLoader {
private:
    PathResolver path_resolver_;
    std::unordered_map<std::string, void*> loaded_libs_;
    
public:
    explicit LibraryLoader();
    ~LibraryLoader();
    
    /**
     * @brief Load .so library
     */
    bool loadLibrary(const std::string& lib_name);
    
    /**
     * @brief Get function pointer from loaded library
     */
    template<typename FuncType>
    FuncType getFunction(const std::string& lib_name,
                        const std::string& func_name);
    
    /**
     * @brief Unload library
     */
    void unloadLibrary(const std::string& lib_name);
};

} // namespace Medusa

#endif // MEDUSA_MDS_PARSER_HPP