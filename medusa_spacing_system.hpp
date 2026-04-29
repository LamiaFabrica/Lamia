/*
 * MEDUSA SPACING SYSTEM - C++ Foundation
 * Cross-platform spacing system for all OS/device interfaces
 * Yorkshire Champion themed spacing system
 * 
 * Deep Links for Codebase Tracing:
 * - Source: Material Design spacing principles
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
namespace Spacing {

/**
 * Spacing Unit Enumeration
 * Different spacing units for different contexts
 */
enum class SpacingUnit {
    PIXELS,         // Absolute pixels
    POINTS,         // Typography points
    REM,            // Relative to root font size
    EM,             // Relative to element font size
    PERCENT,        // Percentage of container
    VIEWPORT_WIDTH, // Viewport width units
    VIEWPORT_HEIGHT, // Viewport height units
    CUSTOM          // Custom units
};

/**
 * Spacing Scale Enumeration
 * Different spacing scales for different design systems
 */
enum class SpacingScale {
    MATERIAL_DESIGN,    // 8px base unit
    YORKSHIRE_CHAMPION, // Custom Yorkshire scale
    COMPACT,           // Compact spacing
    COMFORTABLE,       // Comfortable spacing
    LOOSE,             // Loose spacing
    CUSTOM             // Custom scale
};

/**
 * Spacing Direction Enumeration
 * Spacing directions for different layout contexts
 */
enum class SpacingDirection {
    ALL,            // All sides
    HORIZONTAL,     // Left and right
    VERTICAL,       // Top and bottom
    TOP,            // Top only
    RIGHT,          // Right only
    BOTTOM,         // Bottom only
    LEFT,           // Left only
    CUSTOM          // Custom directions
};

/**
 * Spacing Value Structure
 * Represents a spacing value with unit and context
 */
struct SpacingValue {
    double value;
    SpacingUnit unit;
    std::string context;  // Context for the spacing (e.g., "padding", "margin")
    
    SpacingValue() : value(0.0), unit(SpacingUnit::PIXELS) {}
    SpacingValue(double spacing_value, SpacingUnit spacing_unit = SpacingUnit::PIXELS, const std::string& spacing_context = "")
        : value(spacing_value), unit(spacing_unit), context(spacing_context) {}
    
    // Convert to different units
    double toPixels(double base_size = 16.0, double dpi = 96.0) const;
    double toPoints(double base_size = 16.0) const;
    double toRem(double base_size = 16.0) const;
    double toEm(double element_size = 16.0) const;
    double toPercent(double container_size = 100.0) const;
    
    // Convert to platform-specific format
    std::string toPlatformString(const std::string& platform = "default") const;
    std::string toCSSString() const;
    std::string toCanvasString() const;
    
    // Spacing manipulation
    SpacingValue scale(double factor) const;
    SpacingValue add(const SpacingValue& other) const;
    SpacingValue subtract(const SpacingValue& other) const;
    SpacingValue multiply(double factor) const;
    SpacingValue divide(double factor) const;
    
    // Comparison
    bool operator==(const SpacingValue& other) const;
    bool operator!=(const SpacingValue& other) const;
    bool operator<(const SpacingValue& other) const;
    bool operator>(const SpacingValue& other) const;
    bool operator<=(const SpacingValue& other) const;
    bool operator>=(const SpacingValue& other) const;
};

/**
 * Spacing Definition Structure
 * Represents a complete spacing definition with multiple values
 */
struct SpacingDefinition {
    std::string name;
    SpacingValue top;
    SpacingValue right;
    SpacingValue bottom;
    SpacingValue left;
    SpacingDirection direction;
    
    SpacingDefinition() : direction(SpacingDirection::ALL) {}
    SpacingDefinition(const std::string& spacing_name, const SpacingValue& spacing_value, SpacingDirection spacing_direction = SpacingDirection::ALL)
        : name(spacing_name), top(spacing_value), right(spacing_value), bottom(spacing_value), left(spacing_value), direction(spacing_direction) {}
    SpacingDefinition(const std::string& spacing_name, const SpacingValue& vertical, const SpacingValue& horizontal)
        : name(spacing_name), top(vertical), right(horizontal), bottom(vertical), left(horizontal), direction(SpacingDirection::CUSTOM) {}
    SpacingDefinition(const std::string& spacing_name, const SpacingValue& top_val, const SpacingValue& right_val, const SpacingValue& bottom_val, const SpacingValue& left_val)
        : name(spacing_name), top(top_val), right(right_val), bottom(bottom_val), left(left_val), direction(SpacingDirection::CUSTOM) {}
    
    // Get spacing for specific direction
    SpacingValue getSpacing(SpacingDirection dir) const;
    SpacingValue getHorizontalSpacing() const;
    SpacingValue getVerticalSpacing() const;
    
    // Convert to different formats
    std::string toCSSString() const;
    std::string toPlatformString(const std::string& platform = "default") const;
    std::string toCanvasString() const;
    
    // Spacing manipulation
    SpacingDefinition scale(double factor) const;
    SpacingDefinition add(const SpacingDefinition& other) const;
    SpacingDefinition subtract(const SpacingDefinition& other) const;
    
    // Direction-specific operations
    SpacingDefinition withTop(const SpacingValue& value) const;
    SpacingDefinition withRight(const SpacingValue& value) const;
    SpacingDefinition withBottom(const SpacingValue& value) const;
    SpacingDefinition withLeft(const SpacingValue& value) const;
    SpacingDefinition withHorizontal(const SpacingValue& value) const;
    SpacingDefinition withVertical(const SpacingValue& value) const;
};

/**
 * Spacing Scale Structure
 * Represents a complete spacing scale
 */
struct SpacingScale {
    std::map<int, SpacingValue> values;
    double base_unit;
    double scale_factor;
    SpacingScale scale_type;
    
    SpacingScale() : base_unit(8.0), scale_factor(1.0), scale_type(SpacingScale::MATERIAL_DESIGN) {}
    
    // Add spacing value to scale
    void addValue(int level, const SpacingValue& value);
    void addValue(int level, double value, SpacingUnit unit = SpacingUnit::PIXELS);
    
    // Get spacing from scale
    SpacingValue getValue(int level) const;
    bool hasValue(int level) const;
    
    // Generate spacing scales
    void generateMaterialDesignScale();
    void generateYorkshireScale();
    void generateCustomScale(double base, double factor);
    
    // Scale manipulation
    void setBaseUnit(double unit);
    void setScaleFactor(double factor);
    void setScaleType(SpacingScale scale);
    void scaleValues(double factor);
};

/**
 * Spacing Renderer
 * Handles spacing rendering for different platforms
 */
class SpacingRenderer {
private:
    std::map<std::string, std::string> render_cache_;
    std::mutex render_mutex_;
    
public:
    SpacingRenderer();
    ~SpacingRenderer() = default;
    
    // Spacing rendering
    std::string renderSpacing(const SpacingDefinition& spacing, const std::string& platform = "default");
    std::string renderSpacingWithStyle(const SpacingDefinition& spacing, const std::map<std::string, std::string>& styles);
    
    // Platform-specific rendering
    std::string renderForWindows(const SpacingDefinition& spacing);
    std::string renderForMacOS(const SpacingDefinition& spacing);
    std::string renderForLinux(const SpacingDefinition& spacing);
    std::string renderForWeb(const SpacingDefinition& spacing);
    std::string renderForMobile(const SpacingDefinition& spacing);
    std::string renderForCanvas(const SpacingDefinition& spacing);
    
    // Spacing utilities
    std::string renderMargin(const SpacingDefinition& spacing);
    std::string renderPadding(const SpacingDefinition& spacing);
    std::string renderGap(const SpacingValue& spacing);
    std::string renderPosition(const SpacingDefinition& spacing);
    
    // Layout calculations
    struct LayoutMetrics {
        double total_width;
        double total_height;
        double content_width;
        double content_height;
        double margin_width;
        double margin_height;
        double padding_width;
        double padding_height;
    };
    
    LayoutMetrics calculateLayout(const SpacingDefinition& margin, const SpacingDefinition& padding, double content_width, double content_height);
    
    // Cache management
    void clearCache();
    size_t getCacheSize() const;
    double getCacheHitRate() const;
    
private:
    std::string generateCSSSpacing(const SpacingDefinition& spacing);
    std::string generateCanvasSpacing(const SpacingDefinition& spacing);
    std::string generatePlatformSpacing(const SpacingDefinition& spacing, const std::string& platform);
};

/**
 * Spacing Cache Manager
 * Manages spacing caching and optimization
 */
class SpacingCacheManager {
private:
    std::unordered_map<std::string, std::string> spacing_cache_;
    std::unordered_map<std::string, SpacingDefinition> definition_cache_;
    std::mutex cache_mutex_;
    
    size_t max_cache_size_ = 1000;
    size_t cache_hits_ = 0;
    size_t cache_misses_ = 0;
    
public:
    SpacingCacheManager();
    ~SpacingCacheManager() = default;
    
    // Cache operations
    void cacheSpacing(const std::string& key, const std::string& rendered_spacing);
    void cacheDefinition(const std::string& key, const SpacingDefinition& definition);
    bool getCachedSpacing(const std::string& key, std::string& rendered_spacing);
    bool getCachedDefinition(const std::string& key, SpacingDefinition& definition);
    
    // Cache management
    void setMaxCacheSize(size_t size);
    size_t getCacheSize() const;
    void cleanupCache();
    void clearCache();
    
    // Statistics
    double getHitRate() const;
    size_t getSpacingCacheSize() const;
    size_t getDefinitionCacheSize() const;
    
private:
    void evictOldestEntries();
    std::string generateCacheKey(const SpacingDefinition& spacing, const std::string& platform);
};

/**
 * Spacing System
 * Main system for managing spacing across all platforms
 */
class SpacingSystem {
private:
    static std::unique_ptr<SpacingSystem> instance_;
    static std::mutex instance_mutex_;
    
    std::unique_ptr<SpacingScale> scale_;
    std::unique_ptr<SpacingRenderer> renderer_;
    std::unique_ptr<SpacingCacheManager> cache_manager_;
    
    std::string current_platform_;
    std::string current_theme_;
    bool initialized_ = false;
    
public:
    SpacingSystem();
    ~SpacingSystem() = default;
    
    // Singleton access
    static SpacingSystem& getInstance();
    static void initialize();
    static void shutdown();
    
    // System management
    bool initialize();
    void setPlatform(const std::string& platform);
    void setTheme(const std::string& theme);
    std::string getCurrentPlatform() const;
    std::string getCurrentTheme() const;
    
    // Spacing access
    SpacingValue getSpacing(int level) const;
    std::string renderSpacing(int level);
    std::string renderSpacing(const SpacingDefinition& spacing);
    
    // Scale management
    void setBaseUnit(double unit);
    void setScaleFactor(double factor);
    void setScaleType(SpacingScale scale);
    void generateMaterialDesignScale();
    void generateYorkshireScale();
    void addCustomSpacing(int level, const SpacingValue& spacing);
    
    // Spacing creation
    SpacingDefinition createSpacing(const std::string& name, const SpacingValue& value, SpacingDirection direction = SpacingDirection::ALL);
    SpacingDefinition createSpacing(const std::string& name, const SpacingValue& vertical, const SpacingValue& horizontal);
    SpacingDefinition createSpacing(const std::string& name, const SpacingValue& top, const SpacingValue& right, const SpacingValue& bottom, const SpacingValue& left);
    
    // Common spacing patterns
    SpacingDefinition createMargin(const std::string& name, int level);
    SpacingDefinition createPadding(const std::string& name, int level);
    SpacingDefinition createGap(const std::string& name, int level);
    SpacingDefinition createPosition(const std::string& name, int level);
    
    // Platform-specific operations
    std::string getPlatformSpecificSpacing(int level) const;
    SpacingDefinition getPlatformAdjustedSpacing(int level) const;
    
    // Spacing utilities
    SpacingRenderer::LayoutMetrics calculateLayout(int margin_level, int padding_level, double content_width, double content_height);
    std::vector<SpacingValue> getSpacingsInRange(int start, int end) const;
    
    // Theme integration
    void applyTheme(const std::string& theme_name);
    void createYorkshireTheme();
    void createMaterioTheme();
    
private:
    void initializeDefaultScale();
    void createDefaultSpacings();
    void createMaterioSpacings();
    void createYorkshireSpacings();
};

/**
 * Spacing Utilities
 * Helper functions for spacing operations
 */
namespace Utils {
    // Spacing utilities
    std::string spacingUnitToString(SpacingUnit unit);
    SpacingUnit stringToSpacingUnit(const std::string& unit);
    std::string spacingScaleToString(SpacingScale scale);
    SpacingScale stringToSpacingScale(const std::string& scale);
    std::string spacingDirectionToString(SpacingDirection direction);
    SpacingDirection stringToSpacingDirection(const std::string& direction);
    
    // Conversion utilities
    double pixelsToPoints(double pixels, double dpi = 96.0);
    double pointsToPixels(double points, double dpi = 96.0);
    double remToPixels(double rem, double base_size = 16.0);
    double pixelsToRem(double pixels, double base_size = 16.0);
    double emToPixels(double em, double element_size = 16.0);
    double pixelsToEm(double pixels, double element_size = 16.0);
    
    // Math utilities
    double calculateScaleFactor(int level, double base_unit, double factor);
    double calculateResponsiveSpacing(double base_spacing, double viewport_width, double min_width, double max_width);
    double calculateFluidSpacing(double min_spacing, double max_spacing, double min_viewport, double max_viewport, double current_viewport);
    
    // Platform utilities
    std::string detectPlatform();
    bool isPlatformSupported(const std::string& platform);
    std::string getPlatformSpacingFormat(const std::string& platform);
    
    // Validation utilities
    bool isValidSpacingLevel(int level);
    bool isValidSpacingValue(double value);
    bool isValidSpacingUnit(SpacingUnit unit);
    bool isValidSpacingScale(SpacingScale scale);
    
    // Material Design utilities
    SpacingValue createMaterialSpacing(int level, double base_unit = 8.0);
    SpacingDefinition createMaterialMargin(int level);
    SpacingDefinition createMaterialPadding(int level);
}

} // namespace Spacing
} // namespace Foundation
} // namespace MedusaTheme 