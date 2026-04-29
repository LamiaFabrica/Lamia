/*
 * MEDUSA AVATAR COMPONENT - C++ Foundation
 * Cross-platform avatar component with advanced theming
 * Yorkshire Champion themed avatar system
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
namespace Avatar {

/**
 * Avatar Variant Enumeration
 * Different visual variants for avatars
 */
enum class AvatarVariant {
    CIRCULAR,
    SQUARE,
    ROUNDED,
    YORKSHIRE_CHAMPION
};

/**
 * Avatar Type Enumeration
 * Different types of avatars
 */
enum class AvatarType {
    IMAGE,
    LETTER,
    ICON,
    CUSTOM
};

/**
 * Avatar Skin Enumeration
 * Different skin variants for letter avatars
 */
enum class AvatarSkin {
    DEFAULT,
    LIGHT,
    LIGHT_STATIC
};

/**
 * Avatar Size Enumeration
 * Different size options for avatars
 */
enum class AvatarSize {
    XS,     // 20px
    SM,     // 32px
    MD,     // 40px
    LG,     // 56px
    XL,     // 96px
    XXL,    // 128px
    CUSTOM
};

/**
 * Avatar State Enumeration
 * Different states for avatar components
 */
enum class AvatarState {
    NORMAL,
    HOVER,
    FOCUSED,
    LOADING,
    ERROR,
    DISABLED
};

/**
 * Avatar Badge Structure
 * Represents a badge on an avatar
 */
struct AvatarBadge {
    std::string id;
    std::string content;
    std::string color;
    std::string background_color;
    
    // Badge properties
    bool visible;
    bool animated;
    std::string position;  // "top-right", "top-left", "bottom-right", "bottom-left"
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    AvatarBadge() : visible(true), animated(false), position("top-right") {}
    AvatarBadge(const std::string& badge_id, const std::string& badge_content)
        : id(badge_id), content(badge_content), visible(true), animated(false), position("top-right") {}
    
    // Badge management
    void setVisible(bool is_visible);
    void setAnimated(bool is_animated);
    void setPosition(const std::string& badge_position);
    bool isVisible() const;
    bool isAnimated() const;
    std::string getPosition() const;
    
    // Property access
    std::string getProperty(const std::string& property_name) const;
    void setProperty(const std::string& property_name, const std::string& value);
};

/**
 * Avatar Tooltip Structure
 * Represents a tooltip for an avatar
 */
struct AvatarTooltip {
    std::string id;
    std::string content;
    std::string title;
    
    // Tooltip properties
    bool enabled;
    std::string position;  // "top", "bottom", "left", "right"
    double delay;
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    AvatarTooltip() : enabled(false), position("top"), delay(0.0) {}
    AvatarTooltip(const std::string& tooltip_id, const std::string& tooltip_content)
        : id(tooltip_id), content(tooltip_content), enabled(true), position("top"), delay(0.0) {}
    
    // Tooltip management
    void setEnabled(bool is_enabled);
    void setPosition(const std::string& tooltip_position);
    void setDelay(double tooltip_delay);
    bool isEnabled() const;
    std::string getPosition() const;
    double getDelay() const;
    
    // Property access
    std::string getProperty(const std::string& property_name) const;
    void setProperty(const std::string& property_name, const std::string& value);
};

/**
 * Avatar Component Structure
 * Represents a complete avatar component
 */
struct AvatarComponent {
    std::string id;
    std::string src;           // For image avatars
    std::string alt;           // Alt text for image avatars
    std::string letters;       // For letter avatars
    std::string icon_name;     // For icon avatars
    
    // Avatar properties
    AvatarType type;
    AvatarVariant variant;
    AvatarSize size;
    AvatarSkin skin;
    AvatarState state;
    
    // Size properties
    double custom_width;
    double custom_height;
    
    // Styling properties
    std::string background_color;
    std::string text_color;
    std::string border_color;
    std::string shadow_level;
    std::string typography_variant;
    std::string spacing_level;
    
    // Badge and tooltip
    AvatarBadge badge;
    AvatarTooltip tooltip;
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    AvatarComponent() : type(AvatarType::LETTER), variant(AvatarVariant::CIRCULAR), size(AvatarSize::MD),
                        skin(AvatarSkin::DEFAULT), state(AvatarState::NORMAL), custom_width(0.0), custom_height(0.0) {}
    
    AvatarComponent(const std::string& avatar_id, AvatarType avatar_type = AvatarType::LETTER)
        : id(avatar_id), type(avatar_type), variant(AvatarVariant::CIRCULAR), size(AvatarSize::MD),
          skin(AvatarSkin::DEFAULT), state(AvatarState::NORMAL), custom_width(0.0), custom_height(0.0) {}
    
    // Type management
    void setType(AvatarType avatar_type);
    AvatarType getType() const;
    std::string getTypeString() const;
    
    // Variant management
    void setVariant(AvatarVariant avatar_variant);
    AvatarVariant getVariant() const;
    std::string getVariantString() const;
    
    // Size management
    void setSize(AvatarSize avatar_size);
    void setCustomSize(double width, double height);
    AvatarSize getSize() const;
    std::string getSizeString() const;
    double getWidth() const;
    double getHeight() const;
    
    // Skin management
    void setSkin(AvatarSkin avatar_skin);
    AvatarSkin getSkin() const;
    std::string getSkinString() const;
    
    // State management
    void setState(AvatarState avatar_state);
    AvatarState getState() const;
    std::string getStateString() const;
    
    // Content management
    void setImage(const std::string& image_src, const std::string& image_alt = "");
    void setLetters(const std::string& avatar_letters);
    void setIcon(const std::string& icon_name);
    std::string getImageSrc() const;
    std::string getImageAlt() const;
    std::string getLetters() const;
    std::string getIconName() const;
    
    // Badge management
    void setBadge(const AvatarBadge& avatar_badge);
    AvatarBadge getBadge() const;
    bool hasBadge() const;
    
    // Tooltip management
    void setTooltip(const AvatarTooltip& avatar_tooltip);
    AvatarTooltip getTooltip() const;
    bool hasTooltip() const;
    
    // Property access
    std::string getProperty(const std::string& property_name) const;
    void setProperty(const std::string& property_name, const std::string& value);
    
    // Component manipulation
    AvatarComponent withType(AvatarType avatar_type) const;
    AvatarComponent withVariant(AvatarVariant avatar_variant) const;
    AvatarComponent withSize(AvatarSize avatar_size) const;
    AvatarComponent withSkin(AvatarSkin avatar_skin) const;
    AvatarComponent withBadge(const AvatarBadge& avatar_badge) const;
    AvatarComponent withTooltip(const AvatarTooltip& avatar_tooltip) const;
};

/**
 * Avatar Group Structure
 * Represents a group of avatars
 */
struct AvatarGroup {
    std::string id;
    std::vector<AvatarComponent> avatars;
    
    // Group properties
    int max_avatars;
    bool pull_up_enabled;
    std::string alignment;  // "left", "center", "right"
    
    // Styling properties
    std::string background_color;
    std::string border_color;
    std::string shadow_level;
    std::string spacing_level;
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    AvatarGroup() : max_avatars(5), pull_up_enabled(false), alignment("left") {}
    AvatarGroup(const std::string& group_id)
        : id(group_id), max_avatars(5), pull_up_enabled(false), alignment("left") {}
    
    // Avatar management
    void addAvatar(const AvatarComponent& avatar);
    void removeAvatar(const std::string& avatar_id);
    void clearAvatars();
    std::vector<AvatarComponent> getAvatars() const;
    std::vector<AvatarComponent> getVisibleAvatars() const;
    
    // Group properties
    void setMaxAvatars(int max);
    void setPullUpEnabled(bool enabled);
    void setAlignment(const std::string& group_alignment);
    int getMaxAvatars() const;
    bool isPullUpEnabled() const;
    std::string getAlignment() const;
    
    // Property access
    std::string getProperty(const std::string& property_name) const;
    void setProperty(const std::string& property_name, const std::string& value);
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Avatar Renderer
 * Handles avatar rendering for different platforms
 */
class AvatarRenderer {
private:
    std::map<std::string, std::string> render_cache_;
    std::mutex render_mutex_;
    
public:
    AvatarRenderer();
    ~AvatarRenderer() = default;
    
    // Component rendering
    std::string renderAvatar(const AvatarComponent& avatar, const std::string& platform = "default");
    std::string renderAvatarWithStyle(const AvatarComponent& avatar, const std::map<std::string, std::string>& styles);
    std::string renderAvatarGroup(const AvatarGroup& group, const std::string& platform = "default");
    
    // Platform-specific rendering
    std::string renderForWindows(const AvatarComponent& avatar);
    std::string renderForMacOS(const AvatarComponent& avatar);
    std::string renderForLinux(const AvatarComponent& avatar);
    std::string renderForWeb(const AvatarComponent& avatar);
    std::string renderForMobile(const AvatarComponent& avatar);
    std::string renderForConsole(const AvatarComponent& avatar);
    
    // Type-specific rendering
    std::string renderImageAvatar(const AvatarComponent& avatar);
    std::string renderLetterAvatar(const AvatarComponent& avatar);
    std::string renderIconAvatar(const AvatarComponent& avatar);
    std::string renderCustomAvatar(const AvatarComponent& avatar);
    
    // Variant-specific rendering
    std::string renderCircularVariant(const AvatarComponent& avatar);
    std::string renderSquareVariant(const AvatarComponent& avatar);
    std::string renderRoundedVariant(const AvatarComponent& avatar);
    std::string renderYorkshireVariant(const AvatarComponent& avatar);
    
    // Size-specific rendering
    std::string renderSizeXS(const AvatarComponent& avatar);
    std::string renderSizeSM(const AvatarComponent& avatar);
    std::string renderSizeMD(const AvatarComponent& avatar);
    std::string renderSizeLG(const AvatarComponent& avatar);
    std::string renderSizeXL(const AvatarComponent& avatar);
    std::string renderSizeXXL(const AvatarComponent& avatar);
    std::string renderCustomSize(const AvatarComponent& avatar);
    
    // Format-specific rendering
    std::string renderAsCSS(const AvatarComponent& avatar);
    std::string renderAsHTML(const AvatarComponent& avatar);
    std::string renderAsJSON(const AvatarComponent& avatar);
    std::string renderAsXML(const AvatarComponent& avatar);
    
    // Badge and tooltip rendering
    std::string renderBadge(const AvatarBadge& badge);
    std::string renderTooltip(const AvatarTooltip& tooltip);
    std::string renderAvatarWithBadge(const AvatarComponent& avatar);
    std::string renderAvatarWithTooltip(const AvatarComponent& avatar);
    
    // Group rendering
    std::string renderAvatarGroup(const AvatarGroup& group);
    std::string renderGroupedAvatars(const std::vector<AvatarComponent>& avatars, int max_count);
    std::string renderPullUpEffect(const AvatarComponent& avatar);
    
    // Cache management
    void clearCache();
    size_t getCacheSize() const;
    double getCacheHitRate() const;
    
private:
    std::string generateAvatarCSS(const AvatarComponent& avatar);
    std::string generateAvatarHTML(const AvatarComponent& avatar);
    std::string generateAvatarJSON(const AvatarComponent& avatar);
    std::string generateAvatarXML(const AvatarComponent& avatar);
    std::string generatePlatformAvatar(const AvatarComponent& avatar, const std::string& platform);
    std::string generateTypeCSS(AvatarType type);
    std::string generateVariantCSS(AvatarVariant variant);
    std::string generateSizeCSS(AvatarSize size, double custom_width, double custom_height);
};

/**
 * Avatar Cache Manager
 * Manages avatar caching and optimization
 */
class AvatarCacheManager {
private:
    std::unordered_map<std::string, std::string> avatar_cache_;
    std::unordered_map<std::string, AvatarComponent> component_cache_;
    std::unordered_map<std::string, std::string> group_cache_;
    std::mutex cache_mutex_;
    
    size_t max_cache_size_ = 1000;
    size_t cache_hits_ = 0;
    size_t cache_misses_ = 0;
    
public:
    AvatarCacheManager();
    ~AvatarCacheManager() = default;
    
    // Cache operations
    void cacheAvatar(const std::string& key, const std::string& rendered_avatar);
    void cacheComponent(const std::string& key, const AvatarComponent& component);
    void cacheGroup(const std::string& key, const std::string& rendered_group);
    bool getCachedAvatar(const std::string& key, std::string& rendered_avatar);
    bool getCachedComponent(const std::string& key, AvatarComponent& component);
    bool getCachedGroup(const std::string& key, std::string& rendered_group);
    
    // Cache management
    void setMaxCacheSize(size_t size);
    size_t getCacheSize() const;
    void cleanupCache();
    void clearCache();
    
    // Statistics
    double getHitRate() const;
    size_t getAvatarCacheSize() const;
    size_t getComponentCacheSize() const;
    size_t getGroupCacheSize() const;
    
private:
    void evictOldestEntries();
    std::string generateCacheKey(const AvatarComponent& avatar, const std::string& platform);
    std::string generateGroupCacheKey(const AvatarGroup& group, const std::string& platform);
};

/**
 * Avatar System
 * Main system for managing avatar components across all platforms
 */
class AvatarSystem {
private:
    static std::unique_ptr<AvatarSystem> instance_;
    static std::mutex instance_mutex_;
    
    // Foundation systems
    std::unique_ptr<Foundation::Colors::ColorSystem> color_system_;
    std::unique_ptr<Foundation::Typography::TypographySystem> typography_system_;
    std::unique_ptr<Foundation::Shadows::ShadowSystem> shadow_system_;
    std::unique_ptr<Foundation::Spacing::SpacingSystem> spacing_system_;
    std::unique_ptr<Foundation::Iconify::IconifySystem> iconify_system_;
    
    // Avatar management
    std::unique_ptr<AvatarRenderer> renderer_;
    std::unique_ptr<AvatarCacheManager> cache_manager_;
    
    // Current state
    std::string current_platform_;
    std::string current_theme_;
    bool initialized_ = false;
    
public:
    AvatarSystem();
    ~AvatarSystem() = default;
    
    // Singleton access
    static AvatarSystem& getInstance();
    static void initialize();
    static void shutdown();
    
    // System management
    bool initialize();
    void setPlatform(const std::string& platform);
    void setTheme(const std::string& theme);
    std::string getCurrentPlatform() const;
    std::string getCurrentTheme() const;
    
    // Component operations
    std::string renderAvatar(const AvatarComponent& avatar);
    std::string renderAvatarGroup(const AvatarGroup& group);
    std::string renderAvatarWithStyle(const AvatarComponent& avatar, const std::map<std::string, std::string>& styles);
    
    // Component creation
    AvatarComponent createAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    AvatarGroup createAvatarGroup(const std::string& id);
    AvatarBadge createAvatarBadge(const std::string& id, const std::string& content);
    AvatarTooltip createAvatarTooltip(const std::string& id, const std::string& content);
    
    // Predefined avatars
    AvatarComponent createImageAvatar(const std::string& id, const std::string& src, const std::string& alt = "");
    AvatarComponent createLetterAvatar(const std::string& id, const std::string& letters);
    AvatarComponent createIconAvatar(const std::string& id, const std::string& icon_name);
    
    // Predefined variants
    AvatarComponent createCircularAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    AvatarComponent createSquareAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    AvatarComponent createRoundedAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    AvatarComponent createYorkshireAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    
    // Platform-specific operations
    std::string getPlatformSpecificAvatar(const AvatarComponent& avatar) const;
    AvatarComponent getPlatformAdjustedAvatar(const AvatarComponent& avatar) const;
    
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
    void createDefaultAvatars();
    void setupAvatarDefaults();
    void validateAvatar(const AvatarComponent& avatar);
};

/**
 * Avatar Utilities
 * Helper functions for avatar operations
 */
namespace Utils {
    // Avatar utilities
    std::string avatarTypeToString(AvatarType type);
    AvatarType stringToAvatarType(const std::string& type);
    std::string avatarVariantToString(AvatarVariant variant);
    AvatarVariant stringToAvatarVariant(const std::string& variant);
    std::string avatarSizeToString(AvatarSize size);
    AvatarSize stringToAvatarSize(const std::string& size);
    std::string avatarSkinToString(AvatarSkin skin);
    AvatarSkin stringToAvatarSkin(const std::string& skin);
    std::string avatarStateToString(AvatarState state);
    AvatarState stringToAvatarState(const std::string& state);
    
    // Validation utilities
    bool isValidAvatarId(const std::string& id);
    bool isValidAvatarComponent(const AvatarComponent& avatar);
    bool isValidAvatarGroup(const AvatarGroup& group);
    bool isValidAvatarType(AvatarType type);
    bool isValidAvatarVariant(AvatarVariant variant);
    bool isValidAvatarSize(AvatarSize size);
    
    // Platform utilities
    std::string detectPlatform();
    bool isPlatformSupported(const std::string& platform);
    std::string getPlatformAvatarFormat(const std::string& platform);
    
    // Size utilities
    double getSizeWidth(AvatarSize size);
    double getSizeHeight(AvatarSize size);
    std::string getSizeCSS(AvatarSize size);
    
    // Color utilities
    std::string getDefaultBackgroundColor();
    std::string getDefaultTextColor();
    std::string getDefaultBorderColor();
    std::string getYorkshireBackgroundColor();
    std::string getYorkshireTextColor();
    std::string getYorkshireBorderColor();
    
    // Icon utilities
    std::string getDefaultAvatarIcon();
    std::string getYorkshireAvatarIcon();
    
    // Letter utilities
    std::string generateInitials(const std::string& name);
    std::string getRandomColor(const std::string& seed);
}

} // namespace Avatar
} // namespace Components
} // namespace MedusaTheme 