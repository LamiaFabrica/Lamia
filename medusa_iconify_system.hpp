/*
 * MEDUSA ICONIFY SYSTEM - C++ Foundation
 * Complete Iconify icon set integration for C++ window styling
 * Yorkshire Champion themed icon system
 * 
 * Deep Links for Codebase Tracing:
 * - Source: @iconify/json package (entire icon set)
 * - Integration: src/theme/integrations/mui/
 * - Extensions: src/theme/extensions/materio/
 * - Components: src/theme/components/
 */

#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <cstdint>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <future>

namespace MedusaTheme {
namespace Foundation {
namespace Iconify {

/**
 * Icon Data Structure
 * Represents an individual icon with its properties
 */
struct IconData {
    std::string name;
    std::string prefix;
    std::string body;           // SVG body content
    std::map<std::string, std::string> attributes;
    std::vector<std::string> tags;
    std::string category;
    std::string author;
    std::string license;
    std::string version;
    int width = 24;
    int height = 24;
    bool deprecated = false;
    
    // Icon state
    bool loaded = false;
    bool cached = false;
    std::chrono::system_clock::time_point last_accessed;
};

/**
 * Icon Set Structure
 * Represents a complete icon set (e.g., mdi, tabler, etc.)
 */
struct IconSet {
    std::string prefix;
    std::string name;
    std::string description;
    std::string author;
    std::string license;
    std::string version;
    std::string website;
    std::vector<std::string> categories;
    std::map<std::string, IconData> icons;
    int total_icons = 0;
    bool loaded = false;
    
    // Set metadata
    std::chrono::system_clock::time_point last_updated;
    std::string source_path;
    size_t memory_usage = 0;
};

/**
 * Icon Renderer
 * Handles SVG rendering and conversion for C++ window systems
 */
class IconRenderer {
private:
    std::map<std::string, std::string> render_cache_;
    std::mutex render_mutex_;
    
public:
    IconRenderer();
    ~IconRenderer() = default;
    
    // SVG rendering
    std::string renderSVG(const IconData& icon, int size = 24, const std::string& color = "#000000");
    std::string renderSVGWithAttributes(const IconData& icon, const std::map<std::string, std::string>& attributes);
    
    // Color manipulation
    std::string applyColorToSVG(const std::string& svg, const std::string& color);
    std::string applyThemeToSVG(const std::string& svg, const std::string& theme_name);
    
    // Size manipulation
    std::string resizeSVG(const std::string& svg, int width, int height);
    std::string scaleSVG(const std::string& svg, double scale);
    
    // Format conversion
    std::string svgToPNG(const std::string& svg, int size = 24);
    std::string svgToBase64(const std::string& svg);
    std::vector<uint8_t> svgToBytes(const std::string& svg);
    
    // Cache management
    void clearCache();
    size_t getCacheSize() const;
    double getCacheHitRate() const;
    
private:
    std::string processSVGAttributes(const std::string& svg, const std::map<std::string, std::string>& attributes);
    std::string generateCacheKey(const IconData& icon, int size, const std::string& color);
};

/**
 * Icon Cache Manager
 * Manages icon caching and memory optimization
 */
class IconCacheManager {
private:
    std::unordered_map<std::string, IconData> icon_cache_;
    std::unordered_map<std::string, std::string> svg_cache_;
    std::mutex cache_mutex_;
    
    size_t max_memory_usage_ = 100 * 1024 * 1024; // 100MB
    size_t current_memory_usage_ = 0;
    size_t cache_hits_ = 0;
    size_t cache_misses_ = 0;
    
public:
    IconCacheManager();
    ~IconCacheManager() = default;
    
    // Cache operations
    void cacheIcon(const std::string& key, const IconData& icon);
    void cacheSVG(const std::string& key, const std::string& svg);
    bool getCachedIcon(const std::string& key, IconData& icon);
    bool getCachedSVG(const std::string& key, std::string& svg);
    
    // Memory management
    void setMaxMemoryUsage(size_t bytes);
    size_t getCurrentMemoryUsage() const;
    void cleanupCache();
    void clearCache();
    
    // Statistics
    double getHitRate() const;
    size_t getCacheSize() const;
    size_t getSVGCacheSize() const;
    
private:
    void evictOldestEntries();
    size_t estimateMemoryUsage(const IconData& icon);
    size_t estimateMemoryUsage(const std::string& svg);
};

/**
 * Icon Set Manager
 * Manages loading and access to icon sets
 */
class IconSetManager {
private:
    std::map<std::string, IconSet> icon_sets_;
    std::string icon_sets_path_;
    std::mutex sets_mutex_;
    
public:
    IconSetManager(const std::string& sets_path = "assets/icons/iconify");
    ~IconSetManager() = default;
    
    // Set management
    bool loadIconSet(const std::string& prefix);
    bool unloadIconSet(const std::string& prefix);
    bool isIconSetLoaded(const std::string& prefix) const;
    
    // Icon access
    IconData getIcon(const std::string& name);
    IconData getIcon(const std::string& prefix, const std::string& name);
    std::vector<IconData> searchIcons(const std::string& query);
    std::vector<IconData> getIconsByCategory(const std::string& category);
    
    // Set information
    std::vector<std::string> getLoadedSets() const;
    std::vector<std::string> getAvailableSets() const;
    IconSet getIconSet(const std::string& prefix) const;
    
    // Bulk operations
    void loadAllSets();
    void unloadAllSets();
    void preloadPopularSets();
    
private:
    bool loadIconSetFromJSON(const std::string& prefix, const std::string& json_path);
    std::vector<std::string> scanForIconSets();
    void updateSetMetadata(IconSet& set);
};

/**
 * Iconify System
 * Main system for managing the entire Iconify icon set
 */
class IconifySystem {
private:
    static std::unique_ptr<IconifySystem> instance_;
    static std::mutex instance_mutex_;
    
    std::unique_ptr<IconSetManager> set_manager_;
    std::unique_ptr<IconRenderer> renderer_;
    std::unique_ptr<IconCacheManager> cache_manager_;
    
    bool initialized_ = false;
    std::string default_theme_ = "light";
    
public:
    IconifySystem();
    ~IconifySystem() = default;
    
    // Singleton access
    static IconifySystem& getInstance();
    static void initialize(const std::string& icon_sets_path = "assets/icons/iconify");
    static void shutdown();
    
    // System management
    bool initialize(const std::string& icon_sets_path);
    void setDefaultTheme(const std::string& theme);
    std::string getDefaultTheme() const;
    
    // Icon operations
    std::string getIconSVG(const std::string& name, int size = 24, const std::string& color = "");
    std::string getIconSVG(const std::string& prefix, const std::string& name, int size = 24, const std::string& color = "");
    IconData getIconData(const std::string& name);
    IconData getIconData(const std::string& prefix, const std::string& name);
    
    // Search and discovery
    std::vector<IconData> searchIcons(const std::string& query, int limit = 50);
    std::vector<IconData> getIconsByCategory(const std::string& category);
    std::vector<IconData> getPopularIcons(int limit = 100);
    
    // Set management
    bool loadIconSet(const std::string& prefix);
    bool unloadIconSet(const std::string& prefix);
    std::vector<std::string> getLoadedSets() const;
    std::vector<std::string> getAvailableSets() const;
    
    // Performance and statistics
    double getCacheHitRate() const;
    size_t getTotalIcons() const;
    size_t getMemoryUsage() const;
    void optimizeMemory();
    
    // Theme integration
    std::string getIconForTheme(const std::string& name, const std::string& theme);
    std::string getIconForComponent(const std::string& component, const std::string& state = "default");
    
private:
    void initializeDefaultSets();
    void preloadCommonIcons();
    std::string resolveIconName(const std::string& name);
};

/**
 * Icon Utilities
 * Helper functions for icon operations
 */
namespace Utils {
    std::string generateIconKey(const std::string& prefix, const std::string& name);
    std::string sanitizeIconName(const std::string& name);
    bool isValidIconName(const std::string& name);
    std::string getIconCategory(const std::string& name);
    std::vector<std::string> getIconTags(const std::string& name);
    
    // Color utilities
    std::string applyColorToSVG(const std::string& svg, const std::string& color);
    std::string applyThemeColors(const std::string& svg, const std::string& theme);
    
    // Size utilities
    std::string resizeSVG(const std::string& svg, int width, int height);
    std::string scaleSVG(const std::string& svg, double scale);
    
    // Format utilities
    std::string svgToBase64(const std::string& svg);
    std::vector<uint8_t> svgToBytes(const std::string& svg);
}

} // namespace Iconify
} // namespace Foundation
} // namespace MedusaTheme 