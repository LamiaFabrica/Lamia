/*
 * MEDUSA BADGE COMPONENT - C++ Foundation
 * Cross-platform badge component with advanced theming
 * Yorkshire Champion themed badge system
 * 
 * Deep Links for Codebase Tracing:
 * - Foundation Systems: src/theme/core/foundation/
 * - Theme Engine: src/theme/core/medusa_theme_engine.hpp
 * - Integration: src/theme/integrations/mui/
 * - Extensions: src/theme/extensions/materio/
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
#include <future>

// Foundation system includes
#include "../../core/foundation/colors/medusa_color_system.hpp"
#include "../../core/foundation/typography/medusa_typography_system.hpp"
#include "../../core/foundation/shadows/medusa_shadow_system.hpp"
#include "../../core/foundation/spacing/medusa_spacing_system.hpp"
#include "../../core/foundation/icons/iconify/medusa_iconify_system.hpp"

namespace MedusaTheme {
namespace Components {
namespace Badge {

/**
 * Badge Variant Enumeration
 * Different visual variants for badges
 */
enum class BadgeVariant {
    STANDARD,
    DOT,
    TONAL,
    YORKSHIRE_CHAMPION
};

/**
 * Badge Color Enumeration
 * Different color options for badges
 */
enum class BadgeColor {
    DEFAULT,
    PRIMARY,
    SECONDARY,
    SUCCESS,
    ERROR,
    WARNING,
    INFO,
    YORKSHIRE_CHAMPION
};

/**
 * Badge State Enumeration
 * Different states for badge components
 */
enum class BadgeState {
    VISIBLE,
    HIDDEN,
    INVISIBLE,
    LOADING,
    ERROR,
    DISABLED
};

/**
 * Badge Anchor Origin Structure
 * Represents the anchor origin for badge positioning
 */
struct BadgeAnchorOrigin {
    std::string horizontal;  // "left", "right", "center"
    std::string vertical;    // "top", "bottom", "center"
    
    BadgeAnchorOrigin() : horizontal("right"), vertical("top") {}
    BadgeAnchorOrigin(const std::string& h, const std::string& v) : horizontal(h), vertical(v) {}
    
    // Anchor management
    void setHorizontal(const std::string& h);
    void setVertical(const std::string& v);
    std::string getHorizontal() const;
    std::string getVertical() const;
    std::string getAnchorString() const;
};

/**
 * Badge Transform Origin Structure
 * Represents the transform origin for badge positioning
 */
struct BadgeTransformOrigin {
    std::string horizontal;  // "left", "right", "center"
    std::string vertical;    // "top", "bottom", "center"
    
    BadgeTransformOrigin() : horizontal("right"), vertical("top") {}
    BadgeTransformOrigin(const std::string& h, const std::string& v) : horizontal(h), vertical(v) {}
    
    // Transform management
    void setHorizontal(const std::string& h);
    void setVertical(const std::string& v);
    std::string getHorizontal() const;
    std::string getVertical() const;
    std::string getTransformString() const;
};

/**
 * Badge Component Structure
 * Represents a complete badge component
 */
struct BadgeComponent {
    std::string id;
    std::string content;
    std::string wrapped_content;
    
    // Badge properties
    BadgeVariant variant;
    BadgeColor color;
    BadgeState state;
    
    // Positioning properties
    BadgeAnchorOrigin anchor_origin;
    BadgeTransformOrigin transform_origin;
    bool overlap;
    
    // Value properties
    int max_value;
    int current_value;
    bool show_zero;
    
    // Styling properties
    std::string background_color;
    std::string text_color;
    std::string border_color;
    std::string shadow_level;
    std::string typography_variant;
    std::string spacing_level;
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    BadgeComponent() : variant(BadgeVariant::STANDARD), color(BadgeColor::DEFAULT), state(BadgeState::VISIBLE),
                       overlap(true), max_value(99), current_value(0), show_zero(false) {}
    
    BadgeComponent(const std::string& badge_id, const std::string& badge_content)
        : id(badge_id), content(badge_content), variant(BadgeVariant::STANDARD), color(BadgeColor::DEFAULT), state(BadgeState::VISIBLE),
          overlap(true), max_value(99), current_value(0), show_zero(false) {}
    
    // Content management
    void setContent(const std::string& badge_content);
    void setWrappedContent(const std::string& wrapped_content);
    std::string getContent() const;
    std::string getWrappedContent() const;
    std::string getDisplayContent() const;
    
    // Variant management
    void setVariant(BadgeVariant badge_variant);
    BadgeVariant getVariant() const;
    std::string getVariantString() const;
    
    // Color management
    void setColor(BadgeColor badge_color);
    BadgeColor getColor() const;
    std::string getColorString() const;
    
    // State management
    void setState(BadgeState badge_state);
    BadgeState getState() const;
    std::string getStateString() const;
    
    // Positioning management
    void setAnchorOrigin(const BadgeAnchorOrigin& origin);
    void setTransformOrigin(const BadgeTransformOrigin& origin);
    void setOverlap(bool should_overlap);
    BadgeAnchorOrigin getAnchorOrigin() const;
    BadgeTransformOrigin getTransformOrigin() const;
    bool isOverlapping() const;
    
    // Value management
    void setMaxValue(int max);
    void setCurrentValue(int value);
    void setShowZero(bool show);
    int getMaxValue() const;
    int getCurrentValue() const;
    bool shouldShowZero() const;
    std::string getFormattedValue() const;
    
    // Property access
    std::string getProperty(const std::string& property_name) const;
    void setProperty(const std::string& property_name, const std::string& value);
    
    // Component manipulation
    BadgeComponent withVariant(BadgeVariant badge_variant) const;
    BadgeComponent withColor(BadgeColor badge_color) const;
    BadgeComponent withContent(const std::string& badge_content) const;
    BadgeComponent withMaxValue(int max) const;
    BadgeComponent withOverlap(bool should_overlap) const;
};

/**
 * Basic Badge Component
 * Simple badge with badgeContent prop for text and color prop for colors
 */
struct BasicBadge {
    std::string id;
    std::string content;
    BadgeColor color;
    
    // Basic properties
    bool visible;
    std::string background_color;
    std::string text_color;
    
    BasicBadge() : color(BadgeColor::DEFAULT), visible(true) {}
    BasicBadge(const std::string& badge_id, const std::string& badge_content, BadgeColor badge_color = BadgeColor::DEFAULT)
        : id(badge_id), content(badge_content), color(badge_color), visible(true) {}
    
    // Content management
    void setContent(const std::string& badge_content);
    std::string getContent() const;
    
    // Color management
    void setColor(BadgeColor badge_color);
    BadgeColor getColor() const;
    std::string getColorString() const;
    
    // Visibility management
    void setVisible(bool is_visible);
    bool isVisible() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Dot Badge Component
 * Badge with variant='dot' prop for dot badges
 */
struct DotBadge {
    std::string id;
    BadgeColor color;
    
    // Dot properties
    bool visible;
    double size;
    std::string background_color;
    
    DotBadge() : color(BadgeColor::DEFAULT), visible(true), size(8.0) {}
    DotBadge(const std::string& badge_id, BadgeColor badge_color = BadgeColor::DEFAULT)
        : id(badge_id), color(badge_color), visible(true), size(8.0) {}
    
    // Color management
    void setColor(BadgeColor badge_color);
    BadgeColor getColor() const;
    std::string getColorString() const;
    
    // Size management
    void setSize(double dot_size);
    double getSize() const;
    
    // Visibility management
    void setVisible(bool is_visible);
    bool isVisible() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Tonal Badge Component
 * Custom tonal variant of badges with tonal='true' prop
 */
struct TonalBadge {
    std::string id;
    std::string content;
    BadgeColor color;
    
    // Tonal properties
    bool tonal;
    bool visible;
    double opacity;
    std::string background_color;
    std::string text_color;
    
    TonalBadge() : color(BadgeColor::DEFAULT), tonal(true), visible(true), opacity(0.8) {}
    TonalBadge(const std::string& badge_id, const std::string& badge_content, BadgeColor badge_color = BadgeColor::DEFAULT)
        : id(badge_id), content(badge_content), color(badge_color), tonal(true), visible(true), opacity(0.8) {}
    
    // Content management
    void setContent(const std::string& badge_content);
    std::string getContent() const;
    
    // Color management
    void setColor(BadgeColor badge_color);
    BadgeColor getColor() const;
    std::string getColorString() const;
    
    // Tonal management
    void setTonal(bool is_tonal);
    void setOpacity(double tonal_opacity);
    bool isTonal() const;
    double getOpacity() const;
    
    // Visibility management
    void setVisible(bool is_visible);
    bool isVisible() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Badge Renderer
 * Handles badge rendering for different platforms
 */
class BadgeRenderer {
private:
    std::map<std::string, std::string> render_cache_;
    std::mutex render_mutex_;
    
public:
    BadgeRenderer();
    ~BadgeRenderer() = default;
    
    // Component rendering
    std::string renderBadge(const BadgeComponent& badge, const std::string& platform = "default");
    std::string renderBadgeWithStyle(const BadgeComponent& badge, const std::map<std::string, std::string>& styles);
    std::string renderBasicBadge(const BasicBadge& badge, const std::string& platform = "default");
    std::string renderDotBadge(const DotBadge& badge, const std::string& platform = "default");
    std::string renderTonalBadge(const TonalBadge& badge, const std::string& platform = "default");
    
    // Platform-specific rendering
    std::string renderForWindows(const BadgeComponent& badge);
    std::string renderForMacOS(const BadgeComponent& badge);
    std::string renderForLinux(const BadgeComponent& badge);
    std::string renderForWeb(const BadgeComponent& badge);
    std::string renderForMobile(const BadgeComponent& badge);
    std::string renderForConsole(const BadgeComponent& badge);
    
    // Variant-specific rendering
    std::string renderStandardVariant(const BadgeComponent& badge);
    std::string renderDotVariant(const BadgeComponent& badge);
    std::string renderTonalVariant(const BadgeComponent& badge);
    std::string renderYorkshireVariant(const BadgeComponent& badge);
    
    // Color-specific rendering
    std::string renderDefaultColor(const BadgeComponent& badge);
    std::string renderPrimaryColor(const BadgeComponent& badge);
    std::string renderSecondaryColor(const BadgeComponent& badge);
    std::string renderSuccessColor(const BadgeComponent& badge);
    std::string renderErrorColor(const BadgeComponent& badge);
    std::string renderWarningColor(const BadgeComponent& badge);
    std::string renderInfoColor(const BadgeComponent& badge);
    std::string renderYorkshireColor(const BadgeComponent& badge);
    
    // Format-specific rendering
    std::string renderAsCSS(const BadgeComponent& badge);
    std::string renderAsHTML(const BadgeComponent& badge);
    std::string renderAsJSON(const BadgeComponent& badge);
    std::string renderAsXML(const BadgeComponent& badge);
    
    // Positioning rendering
    std::string renderWithAnchorOrigin(const BadgeComponent& badge);
    std::string renderWithTransformOrigin(const BadgeComponent& badge);
    std::string renderWithOverlap(const BadgeComponent& badge);
    
    // Cache management
    void clearCache();
    size_t getCacheSize() const;
    double getCacheHitRate() const;
    
private:
    std::string generateBadgeCSS(const BadgeComponent& badge);
    std::string generateBadgeHTML(const BadgeComponent& badge);
    std::string generateBadgeJSON(const BadgeComponent& badge);
    std::string generateBadgeXML(const BadgeComponent& badge);
    std::string generatePlatformBadge(const BadgeComponent& badge, const std::string& platform);
    std::string generateVariantCSS(BadgeVariant variant);
    std::string generateColorCSS(BadgeColor color);
    std::string generatePositionCSS(const BadgeAnchorOrigin& anchor, const BadgeTransformOrigin& transform, bool overlap);
};

/**
 * Badge Cache Manager
 * Manages badge caching and optimization
 */
class BadgeCacheManager {
private:
    std::unordered_map<std::string, std::string> badge_cache_;
    std::unordered_map<std::string, BadgeComponent> component_cache_;
    std::unordered_map<std::string, std::string> basic_cache_;
    std::unordered_map<std::string, std::string> dot_cache_;
    std::unordered_map<std::string, std::string> tonal_cache_;
    std::mutex cache_mutex_;
    
    size_t max_cache_size_ = 1000;
    size_t cache_hits_ = 0;
    size_t cache_misses_ = 0;
    
public:
    BadgeCacheManager();
    ~BadgeCacheManager() = default;
    
    // Cache operations
    void cacheBadge(const std::string& key, const std::string& rendered_badge);
    void cacheComponent(const std::string& key, const BadgeComponent& component);
    void cacheBasicBadge(const std::string& key, const std::string& rendered_badge);
    void cacheDotBadge(const std::string& key, const std::string& rendered_badge);
    void cacheTonalBadge(const std::string& key, const std::string& rendered_badge);
    bool getCachedBadge(const std::string& key, std::string& rendered_badge);
    bool getCachedComponent(const std::string& key, BadgeComponent& component);
    bool getCachedBasicBadge(const std::string& key, std::string& rendered_badge);
    bool getCachedDotBadge(const std::string& key, std::string& rendered_badge);
    bool getCachedTonalBadge(const std::string& key, std::string& rendered_badge);
    
    // Cache management
    void setMaxCacheSize(size_t size);
    size_t getCacheSize() const;
    void cleanupCache();
    void clearCache();
    
    // Statistics
    double getHitRate() const;
    size_t getBadgeCacheSize() const;
    size_t getComponentCacheSize() const;
    size_t getBasicCacheSize() const;
    size_t getDotCacheSize() const;
    size_t getTonalCacheSize() const;
    
private:
    void evictOldestEntries();
    std::string generateCacheKey(const BadgeComponent& badge, const std::string& platform);
    std::string generateBasicCacheKey(const BasicBadge& badge, const std::string& platform);
    std::string generateDotCacheKey(const DotBadge& badge, const std::string& platform);
    std::string generateTonalCacheKey(const TonalBadge& badge, const std::string& platform);
};

/**
 * Badge System
 * Main system for managing badge components across all platforms
 */
class BadgeSystem {
private:
    static std::unique_ptr<BadgeSystem> instance_;
    static std::mutex instance_mutex_;
    
    // Foundation systems
    std::unique_ptr<Foundation::Colors::ColorSystem> color_system_;
    std::unique_ptr<Foundation::Typography::TypographySystem> typography_system_;
    std::unique_ptr<Foundation::Shadows::ShadowSystem> shadow_system_;
    std::unique_ptr<Foundation::Spacing::SpacingSystem> spacing_system_;
    std::unique_ptr<Foundation::Iconify::IconifySystem> iconify_system_;
    
    // Badge management
    std::unique_ptr<BadgeRenderer> renderer_;
    std::unique_ptr<BadgeCacheManager> cache_manager_;
    
    // Current state
    std::string current_platform_;
    std::string current_theme_;
    bool initialized_ = false;
    
public:
    BadgeSystem();
    ~BadgeSystem() = default;
    
    // Singleton access
    static BadgeSystem& getInstance();
    static void initialize();
    static void shutdown();
    
    // System management
    bool initialize();
    void setPlatform(const std::string& platform);
    void setTheme(const std::string& theme);
    std::string getCurrentPlatform() const;
    std::string getCurrentTheme() const;
    
    // Component operations
    std::string renderBadge(const BadgeComponent& badge);
    std::string renderBasicBadge(const BasicBadge& badge);
    std::string renderDotBadge(const DotBadge& badge);
    std::string renderTonalBadge(const TonalBadge& badge);
    std::string renderBadgeWithStyle(const BadgeComponent& badge, const std::map<std::string, std::string>& styles);
    
    // Component creation
    BadgeComponent createBadge(const std::string& id, const std::string& content = "");
    BasicBadge createBasicBadge(const std::string& id, const std::string& content, BadgeColor color = BadgeColor::DEFAULT);
    DotBadge createDotBadge(const std::string& id, BadgeColor color = BadgeColor::DEFAULT);
    TonalBadge createTonalBadge(const std::string& id, const std::string& content, BadgeColor color = BadgeColor::DEFAULT);
    
    // Predefined badges
    BadgeComponent createStandardBadge(const std::string& id, const std::string& content);
    BadgeComponent createDotBadge(const std::string& id);
    BadgeComponent createTonalBadge(const std::string& id, const std::string& content);
    BadgeComponent createYorkshireBadge(const std::string& id, const std::string& content);
    
    // Predefined colors
    BadgeComponent createPrimaryBadge(const std::string& id, const std::string& content);
    BadgeComponent createSecondaryBadge(const std::string& id, const std::string& content);
    BadgeComponent createSuccessBadge(const std::string& id, const std::string& content);
    BadgeComponent createErrorBadge(const std::string& id, const std::string& content);
    BadgeComponent createWarningBadge(const std::string& id, const std::string& content);
    BadgeComponent createInfoBadge(const std::string& id, const std::string& content);
    
    // Platform-specific operations
    std::string getPlatformSpecificBadge(const BadgeComponent& badge) const;
    BadgeComponent getPlatformAdjustedBadge(const BadgeComponent& badge) const;
    
    // Foundation system access
    Foundation::Colors::ColorSystem& getColorSystem();
    Foundation::Typography::TypographySystem& getTypographySystem();
    Foundation::Shadows::ShadowSystem& getShadowSystem();
    Foundation::Spacing::SpacingSystem& getSpacingSystem();
    Foundation::Iconify::IconifySystem& getIconifySystem();
    
    // Performance and statistics
    double getCacheHitRate() const;
    size_t getMemoryUsage() const;
    void optimizeMemory();
    
    // Theme integration
    void applyTheme(const std::string& theme_name);
    void createYorkshireTheme();
    void createMaterioTheme();
    
private:
    void initializeFoundationSystems();
    void createDefaultBadges();
    void setupBadgeDefaults();
    void validateBadge(const BadgeComponent& badge);
};

/**
 * Badge Utilities
 * Helper functions for badge operations
 */
namespace Utils {
    // Badge utilities
    std::string badgeVariantToString(BadgeVariant variant);
    BadgeVariant stringToBadgeVariant(const std::string& variant);
    std::string badgeColorToString(BadgeColor color);
    BadgeColor stringToBadgeColor(const std::string& color);
    std::string badgeStateToString(BadgeState state);
    BadgeState stringToBadgeState(const std::string& state);
    
    // Validation utilities
    bool isValidBadgeId(const std::string& id);
    bool isValidBadgeComponent(const BadgeComponent& badge);
    bool isValidBadgeVariant(BadgeVariant variant);
    bool isValidBadgeColor(BadgeColor color);
    bool isValidBadgeValue(int value);
    
    // Platform utilities
    std::string detectPlatform();
    bool isPlatformSupported(const std::string& platform);
    std::string getPlatformBadgeFormat(const std::string& platform);
    
    // Color utilities
    std::string getDefaultBadgeBackgroundColor();
    std::string getDefaultBadgeTextColor();
    std::string getColorBackgroundColor(BadgeColor color);
    std::string getColorTextColor(BadgeColor color);
    std::string getYorkshireBadgeBackgroundColor();
    std::string getYorkshireBadgeTextColor();
    
    // Value utilities
    std::string formatBadgeValue(int value, int max_value);
    bool shouldShowBadge(int value, bool show_zero);
    std::string getDisplayValue(int value, int max_value);
    
    // Position utilities
    std::string getAnchorCSS(const BadgeAnchorOrigin& anchor);
    std::string getTransformCSS(const BadgeTransformOrigin& transform);
    std::string getOverlapCSS(bool overlap);
}

} // namespace Badge
} // namespace Components
} // namespace MedusaTheme 