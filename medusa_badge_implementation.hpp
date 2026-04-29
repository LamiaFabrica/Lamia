/*
 * MEDUSA BADGE IMPLEMENTATION - C++ Foundation
 * Complete badge implementation with all MUI badge features
 * Yorkshire Champion themed badge implementation
 * 
 * Deep Links for Codebase Tracing:
 * - Component Header: src/theme/components/badge/medusa_badge_component.hpp
 * - Foundation Systems: src/theme/core/foundation/
 * - Theme Engine: src/theme/core/medusa_theme_engine.hpp
 */

#pragma once

#include "medusa_badge_component.hpp"
#include <functional>
#include <variant>

namespace MedusaTheme {
namespace Components {
namespace Badge {

/**
 * Badge with Alignment Component
 * Badge with anchorOrigin prop to move badge to any corner
 */
struct BadgeWithAlignment {
    std::string id;
    BadgeComponent badge;
    BadgeAnchorOrigin anchor_origin;
    
    BadgeWithAlignment() {}
    BadgeWithAlignment(const std::string& badge_id, const BadgeComponent& badge_component)
        : id(badge_id), badge(badge_component) {}
    
    // Badge management
    void setBadge(const BadgeComponent& badge_component);
    BadgeComponent getBadge() const;
    
    // Alignment management
    void setAnchorOrigin(const BadgeAnchorOrigin& origin);
    BadgeAnchorOrigin getAnchorOrigin() const;
    std::string getAlignmentCSS() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Badge with Maximum Value Component
 * Badge with max prop to cap the value of badge content
 */
struct BadgeWithMaxValue {
    std::string id;
    BadgeComponent badge;
    int max_value;
    
    BadgeWithMaxValue() : max_value(99) {}
    BadgeWithMaxValue(const std::string& badge_id, const BadgeComponent& badge_component, int max = 99)
        : id(badge_id), badge(badge_component), max_value(max) {}
    
    // Badge management
    void setBadge(const BadgeComponent& badge_component);
    BadgeComponent getBadge() const;
    
    // Max value management
    void setMaxValue(int max);
    int getMaxValue() const;
    std::string getFormattedValue() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Badge with Overlap Component
 * Badge with overlap prop to place badge relative to corner
 */
struct BadgeWithOverlap {
    std::string id;
    BadgeComponent badge;
    bool overlap;
    
    BadgeWithOverlap() : overlap(true) {}
    BadgeWithOverlap(const std::string& badge_id, const BadgeComponent& badge_component, bool should_overlap = true)
        : id(badge_id), badge(badge_component), overlap(should_overlap) {}
    
    // Badge management
    void setBadge(const BadgeComponent& badge_component);
    BadgeComponent getBadge() const;
    
    // Overlap management
    void setOverlap(bool should_overlap);
    bool isOverlapping() const;
    std::string getOverlapCSS() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Badge with Visibility Component
 * Badge with invisible prop to control visibility
 */
struct BadgeWithVisibility {
    std::string id;
    BadgeComponent badge;
    bool invisible;
    
    BadgeWithVisibility() : invisible(false) {}
    BadgeWithVisibility(const std::string& badge_id, const BadgeComponent& badge_component, bool is_invisible = false)
        : id(badge_id), badge(badge_component), invisible(is_invisible) {}
    
    // Badge management
    void setBadge(const BadgeComponent& badge_component);
    BadgeComponent getBadge() const;
    
    // Visibility management
    void setInvisible(bool is_invisible);
    bool isInvisible() const;
    std::string getVisibilityCSS() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Badge with Wrapped Content Component
 * Badge that wraps content with badge overlay
 */
struct BadgeWithWrappedContent {
    std::string id;
    BadgeComponent badge;
    std::string wrapped_content;
    
    BadgeWithWrappedContent() {}
    BadgeWithWrappedContent(const std::string& badge_id, const BadgeComponent& badge_component, const std::string& content = "")
        : id(badge_id), badge(badge_component), wrapped_content(content) {}
    
    // Badge management
    void setBadge(const BadgeComponent& badge_component);
    BadgeComponent getBadge() const;
    
    // Content management
    void setWrappedContent(const std::string& content);
    std::string getWrappedContent() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderWrappedContent() const;
};

/**
 * Badge Factory
 * Factory for creating different types of badges
 */
class BadgeFactory {
public:
    // Basic badge creation
    static BadgeComponent createBadge(const std::string& id, const std::string& content = "");
    static BasicBadge createBasicBadge(const std::string& id, const std::string& content, BadgeColor color = BadgeColor::DEFAULT);
    static DotBadge createDotBadge(const std::string& id, BadgeColor color = BadgeColor::DEFAULT);
    static TonalBadge createTonalBadge(const std::string& id, const std::string& content, BadgeColor color = BadgeColor::DEFAULT);
    
    // Badge with features creation
    static BadgeWithAlignment createBadgeWithAlignment(const std::string& id, const BadgeComponent& badge);
    static BadgeWithMaxValue createBadgeWithMaxValue(const std::string& id, const BadgeComponent& badge, int max = 99);
    static BadgeWithOverlap createBadgeWithOverlap(const std::string& id, const BadgeComponent& badge, bool overlap = true);
    static BadgeWithVisibility createBadgeWithVisibility(const std::string& id, const BadgeComponent& badge, bool invisible = false);
    static BadgeWithWrappedContent createBadgeWithWrappedContent(const std::string& id, const BadgeComponent& badge, const std::string& content = "");
    
    // Predefined variants
    static BadgeComponent createStandardBadge(const std::string& id, const std::string& content);
    static BadgeComponent createDotBadge(const std::string& id);
    static BadgeComponent createTonalBadge(const std::string& id, const std::string& content);
    static BadgeComponent createYorkshireBadge(const std::string& id, const std::string& content);
    
    // Predefined colors
    static BadgeComponent createPrimaryBadge(const std::string& id, const std::string& content);
    static BadgeComponent createSecondaryBadge(const std::string& id, const std::string& content);
    static BadgeComponent createSuccessBadge(const std::string& id, const std::string& content);
    static BadgeComponent createErrorBadge(const std::string& id, const std::string& content);
    static BadgeComponent createWarningBadge(const std::string& id, const std::string& content);
    static BadgeComponent createInfoBadge(const std::string& id, const std::string& content);
    
    // Yorkshire Champion badges
    static BadgeComponent createYorkshireBasicBadge(const std::string& id, const std::string& content);
    static BadgeComponent createYorkshireDotBadge(const std::string& id);
    static BadgeComponent createYorkshireTonalBadge(const std::string& id, const std::string& content);
};

/**
 * Badge Utilities
 * Utility functions for badge operations
 */
namespace Utils {
    // Variant utilities
    std::string getVariantCSS(BadgeVariant variant);
    std::string getVariantHTML(BadgeVariant variant);
    std::string getVariantJSON(BadgeVariant variant);
    
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
    std::string getValueCSS(int value, int max_value);
    
    // Position utilities
    std::string getAnchorCSS(const BadgeAnchorOrigin& anchor);
    std::string getTransformCSS(const BadgeTransformOrigin& transform);
    std::string getOverlapCSS(bool overlap);
    std::string getPositionCSS(const BadgeAnchorOrigin& anchor, const BadgeTransformOrigin& transform, bool overlap);
    
    // Visibility utilities
    std::string getVisibilityCSS(bool invisible);
    std::string getVisibilityHTML(bool invisible);
    std::string getVisibilityJSON(bool invisible);
    
    // Validation utilities
    bool isValidBadgeId(const std::string& id);
    bool isValidBadgeComponent(const BadgeComponent& badge);
    bool isValidBadgeVariant(BadgeVariant variant);
    bool isValidBadgeColor(BadgeColor color);
    bool isValidBadgeValue(int value);
    bool isValidMaxValue(int max_value);
    
    // Platform utilities
    std::string detectPlatform();
    bool isPlatformSupported(const std::string& platform);
    std::string getPlatformBadgeFormat(const std::string& platform);
    
    // Rendering utilities
    std::string generateBadgeCSS(const std::string& variant, const std::string& color, const std::string& position);
    std::string generateBadgeHTML(const std::string& variant, const std::string& color, const std::string& content);
    std::string generateBadgeJSON(const std::string& variant, const std::string& color, const std::string& content);
}

/**
 * Badge Examples
 * Example implementations for different badge types
 */
namespace Examples {
    // Basic badge examples
    BasicBadge createBasicBadgeExample();
    BasicBadge createPrimaryBadgeExample();
    BasicBadge createSecondaryBadgeExample();
    BasicBadge createSuccessBadgeExample();
    BasicBadge createErrorBadgeExample();
    BasicBadge createWarningBadgeExample();
    BasicBadge createInfoBadgeExample();
    
    // Dot badge examples
    DotBadge createDotBadgeExample();
    DotBadge createPrimaryDotBadgeExample();
    DotBadge createErrorDotBadgeExample();
    DotBadge createYorkshireDotBadgeExample();
    
    // Tonal badge examples
    TonalBadge createTonalBadgeExample();
    TonalBadge createPrimaryTonalBadgeExample();
    TonalBadge createSuccessTonalBadgeExample();
    TonalBadge createYorkshireTonalBadgeExample();
    
    // Badge with alignment examples
    BadgeWithAlignment createBadgeWithAlignmentExample();
    BadgeWithAlignment createBadgeWithTopLeftAlignmentExample();
    BadgeWithAlignment createBadgeWithBottomRightAlignmentExample();
    
    // Badge with max value examples
    BadgeWithMaxValue createBadgeWithMaxValueExample();
    BadgeWithMaxValue createBadgeWithMaxValue10Example();
    BadgeWithMaxValue createBadgeWithMaxValue999Example();
    
    // Badge with overlap examples
    BadgeWithOverlap createBadgeWithOverlapExample();
    BadgeWithOverlap createBadgeWithoutOverlapExample();
    
    // Badge with visibility examples
    BadgeWithVisibility createBadgeWithVisibilityExample();
    BadgeWithVisibility createInvisibleBadgeExample();
    
    // Badge with wrapped content examples
    BadgeWithWrappedContent createBadgeWithWrappedContentExample();
    BadgeWithWrappedContent createBadgeWithIconExample();
    BadgeWithWrappedContent createBadgeWithAvatarExample();
    
    // Yorkshire Champion examples
    BadgeComponent createYorkshireBasicBadgeExample();
    BadgeComponent createYorkshireDotBadgeExample();
    BadgeComponent createYorkshireTonalBadgeExample();
}

} // namespace Badge
} // namespace Components
} // namespace MedusaTheme 