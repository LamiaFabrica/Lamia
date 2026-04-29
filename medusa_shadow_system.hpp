/*
 * MEDUSA SHADOW SYSTEM - C++ Foundation
 * Cross-platform shadow system for all OS/device interfaces
 * Yorkshire Champion themed shadow system
 * 
 * Deep Links for Codebase Tracing:
 * - Source: inc/docs/materio/src/@core/theme/shadows.js
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
#include <chrono>
#include <algorithm>
#include <cmath>

namespace MedusaTheme {
namespace Foundation {
namespace Shadows {

/**
 * Shadow Type Enumeration
 * Different types of shadows for different use cases
 */
enum class ShadowType {
    NONE,
    ELEVATION,      // Material Design elevation shadows
    DEPTH,          // Depth-based shadows
    GLOW,           // Glow effects
    OUTLINE,        // Outline shadows
    CUSTOM          // Custom shadow definitions
};

/**
 * Shadow Direction Enumeration
 * Shadow direction for different lighting scenarios
 */
enum class ShadowDirection {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    CENTER
};

/**
 * Shadow Layer Structure
 * Represents a single shadow layer with its properties
 */
struct ShadowLayer {
    double offset_x;        // Horizontal offset in pixels
    double offset_y;        // Vertical offset in pixels
    double blur_radius;     // Blur radius in pixels
    double spread_radius;   // Spread radius in pixels
    double opacity;         // Opacity (0.0 to 1.0)
    std::string color;      // Shadow color (hex, rgb, rgba)
    
    ShadowLayer() : offset_x(0), offset_y(0), blur_radius(0), spread_radius(0), opacity(1.0) {}
    ShadowLayer(double x, double y, double blur, double spread = 0, double alpha = 1.0, const std::string& shadow_color = "#000000")
        : offset_x(x), offset_y(y), blur_radius(blur), spread_radius(spread), opacity(alpha), color(shadow_color) {}
    
    // Convert to CSS-like string
    std::string toCSSString() const;
    
    // Convert to platform-specific format
    std::string toPlatformString(const std::string& platform) const;
    
    // Shadow manipulation
    ShadowLayer scale(double factor) const;
    ShadowLayer offset(double x, double y) const;
    ShadowLayer blur(double radius) const;
    ShadowLayer spread(double radius) const;
    ShadowLayer withOpacity(double alpha) const;
    ShadowLayer withColor(const std::string& shadow_color) const;
};

/**
 * Shadow Definition Structure
 * Represents a complete shadow with multiple layers
 */
struct ShadowDefinition {
    std::string name;
    ShadowType type;
    std::vector<ShadowLayer> layers;
    ShadowDirection direction;
    double intensity;       // Overall shadow intensity (0.0 to 1.0)
    bool inset;            // Whether this is an inset shadow
    
    ShadowDefinition() : type(ShadowType::NONE), direction(ShadowDirection::BOTTOM_RIGHT), intensity(1.0), inset(false) {}
    ShadowDefinition(const std::string& shadow_name, ShadowType shadow_type = ShadowType::ELEVATION)
        : name(shadow_name), type(shadow_type), direction(ShadowDirection::BOTTOM_RIGHT), intensity(1.0), inset(false) {}
    
    // Add shadow layer
    void addLayer(const ShadowLayer& layer);
    void addLayer(double x, double y, double blur, double spread = 0, double opacity = 1.0, const std::string& color = "#000000");
    
    // Shadow manipulation
    ShadowDefinition scale(double factor) const;
    ShadowDefinition offset(double x, double y) const;
    ShadowDefinition blur(double radius) const;
    ShadowDefinition withIntensity(double intensity_value) const;
    ShadowDefinition withDirection(ShadowDirection dir) const;
    ShadowDefinition asInset(bool inset_shadow = true) const;
    
    // Convert to different formats
    std::string toCSSString() const;
    std::string toPlatformString(const std::string& platform) const;
    std::string toSVGString() const;
    std::string toCanvasString() const;
    
    // Shadow analysis
    double getMaxBlur() const;
    double getMaxSpread() const;
    double getMaxOffset() const;
    std::string getDominantColor() const;
};

/**
 * Shadow Scale Structure
 * Represents a complete shadow scale (0-24 for Material Design)
 */
struct ShadowScale {
    std::map<int, ShadowDefinition> shadows;
    std::string base_color;
    double base_intensity;
    
    ShadowScale() : base_color("#000000"), base_intensity(1.0) {}
    
    // Add shadow to scale
    void addShadow(int level, const ShadowDefinition& shadow);
    void addShadow(int level, const std::vector<ShadowLayer>& layers);
    
    // Get shadow from scale
    ShadowDefinition getShadow(int level) const;
    bool hasShadow(int level) const;
    
    // Generate Material Design shadows
    void generateMaterialShadows(const std::string& color = "#000000");
    void generateYorkshireShadows(const std::string& color = "#000000");
    
    // Scale manipulation
    void setBaseColor(const std::string& color);
    void setBaseIntensity(double intensity);
    void scaleIntensity(double factor);
};

/**
 * Shadow Renderer
 * Handles shadow rendering for different platforms
 */
class ShadowRenderer {
private:
    std::map<std::string, std::string> render_cache_;
    std::mutex render_mutex_;
    
public:
    ShadowRenderer();
    ~ShadowRenderer() = default;
    
    // Shadow rendering
    std::string renderShadow(const ShadowDefinition& shadow, const std::string& platform = "default");
    std::string renderShadowWithStyle(const ShadowDefinition& shadow, const std::map<std::string, std::string>& styles);
    
    // Platform-specific rendering
    std::string renderForWindows(const ShadowDefinition& shadow);
    std::string renderForMacOS(const ShadowDefinition& shadow);
    std::string renderForLinux(const ShadowDefinition& shadow);
    std::string renderForWeb(const ShadowDefinition& shadow);
    std::string renderForMobile(const ShadowDefinition& shadow);
    std::string renderForCanvas(const ShadowDefinition& shadow);
    std::string renderForSVG(const ShadowDefinition& shadow);
    
    // Shadow effects
    std::string createDropShadow(const ShadowDefinition& shadow);
    std::string createBoxShadow(const ShadowDefinition& shadow);
    std::string createTextShadow(const ShadowDefinition& shadow);
    std::string createGlowEffect(const ShadowDefinition& shadow);
    
    // Shadow measurement
    struct ShadowBounds {
        double left;
        double top;
        double right;
        double bottom;
        double width;
        double height;
    };
    
    ShadowBounds calculateShadowBounds(const ShadowDefinition& shadow, double element_width, double element_height);
    
    // Cache management
    void clearCache();
    size_t getCacheSize() const;
    double getCacheHitRate() const;
    
private:
    std::string generateCSSBoxShadow(const std::vector<ShadowLayer>& layers);
    std::string generateCanvasShadow(const std::vector<ShadowLayer>& layers);
    std::string generateSVGShadow(const std::vector<ShadowLayer>& layers);
    std::string colorToRGBA(const std::string& color, double opacity);
};

/**
 * Shadow Cache Manager
 * Manages shadow caching and optimization
 */
class ShadowCacheManager {
private:
    std::unordered_map<std::string, std::string> shadow_cache_;
    std::unordered_map<std::string, ShadowDefinition> definition_cache_;
    std::mutex cache_mutex_;
    
    size_t max_cache_size_ = 1000;
    size_t cache_hits_ = 0;
    size_t cache_misses_ = 0;
    
public:
    ShadowCacheManager();
    ~ShadowCacheManager() = default;
    
    // Cache operations
    void cacheShadow(const std::string& key, const std::string& rendered_shadow);
    void cacheDefinition(const std::string& key, const ShadowDefinition& definition);
    bool getCachedShadow(const std::string& key, std::string& rendered_shadow);
    bool getCachedDefinition(const std::string& key, ShadowDefinition& definition);
    
    // Cache management
    void setMaxCacheSize(size_t size);
    size_t getCacheSize() const;
    void cleanupCache();
    void clearCache();
    
    // Statistics
    double getHitRate() const;
    size_t getShadowCacheSize() const;
    size_t getDefinitionCacheSize() const;
    
private:
    void evictOldestEntries();
    std::string generateCacheKey(const ShadowDefinition& shadow, const std::string& platform);
};

/**
 * Shadow System
 * Main system for managing shadows across all platforms
 */
class ShadowSystem {
private:
    static std::unique_ptr<ShadowSystem> instance_;
    static std::mutex instance_mutex_;
    
    std::unique_ptr<ShadowScale> scale_;
    std::unique_ptr<ShadowRenderer> renderer_;
    std::unique_ptr<ShadowCacheManager> cache_manager_;
    
    std::string current_platform_;
    std::string current_theme_;
    bool initialized_ = false;
    
public:
    ShadowSystem();
    ~ShadowSystem() = default;
    
    // Singleton access
    static ShadowSystem& getInstance();
    static void initialize();
    static void shutdown();
    
    // System management
    bool initialize();
    void setPlatform(const std::string& platform);
    void setTheme(const std::string& theme);
    std::string getCurrentPlatform() const;
    std::string getCurrentTheme() const;
    
    // Shadow access
    ShadowDefinition getShadow(int level) const;
    std::string renderShadow(int level);
    std::string renderShadow(const ShadowDefinition& shadow);
    
    // Scale management
    void setBaseColor(const std::string& color);
    void setBaseIntensity(double intensity);
    void generateMaterialShadows();
    void generateYorkshireShadows();
    void addCustomShadow(int level, const ShadowDefinition& shadow);
    
    // Shadow creation
    ShadowDefinition createShadow(const std::string& name, const std::vector<ShadowLayer>& layers);
    ShadowDefinition createElevationShadow(int level);
    ShadowDefinition createGlowShadow(const std::string& color, double intensity);
    ShadowDefinition createOutlineShadow(const std::string& color, double width);
    
    // Platform-specific operations
    std::string getPlatformSpecificShadow(int level) const;
    ShadowDefinition getPlatformAdjustedShadow(int level) const;
    
    // Shadow utilities
    ShadowRenderer::ShadowBounds calculateBounds(int level, double width, double height);
    std::vector<ShadowDefinition> getShadowsInRange(int start, int end) const;
    
    // Theme integration
    void applyTheme(const std::string& theme_name);
    void createYorkshireTheme();
    void createMaterioTheme();
    
private:
    void initializeDefaultScale();
    void createDefaultShadows();
    void createMaterioShadows();
    void createYorkshireShadows();
};

/**
 * Shadow Utilities
 * Helper functions for shadow operations
 */
namespace Utils {
    // Shadow utilities
    std::string shadowTypeToString(ShadowType type);
    ShadowType stringToShadowType(const std::string& type);
    std::string shadowDirectionToString(ShadowDirection direction);
    ShadowDirection stringToShadowDirection(const std::string& direction);
    
    // Color utilities
    std::string colorToRGBA(const std::string& color, double opacity);
    std::string colorToHex(const std::string& color);
    std::string blendColors(const std::string& color1, const std::string& color2, double ratio);
    
    // Math utilities
    double calculateBlurRadius(double elevation);
    double calculateSpreadRadius(double elevation);
    double calculateOffset(double elevation);
    double calculateOpacity(double elevation);
    
    // Platform utilities
    std::string detectPlatform();
    bool isPlatformSupported(const std::string& platform);
    std::string getPlatformShadowFormat(const std::string& platform);
    
    // Validation utilities
    bool isValidShadowLevel(int level);
    bool isValidOpacity(double opacity);
    bool isValidColor(const std::string& color);
    bool isValidBlurRadius(double radius);
    
    // Material Design utilities
    std::vector<ShadowLayer> generateMaterialLayers(int elevation, const std::string& color);
    ShadowDefinition createMaterialShadow(int elevation, const std::string& color = "#000000");
}

} // namespace Shadows
} // namespace Foundation
} // namespace MedusaTheme 