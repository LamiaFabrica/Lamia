/*
 * MEDUSA AVATAR IMPLEMENTATION - C++ Foundation
 * Complete avatar implementation with all MUI avatar features
 * Yorkshire Champion themed avatar implementation
 * 
 * Deep Links for Codebase Tracing:
 * - Component Header: src/theme/components/avatar/medusa_avatar_component.hpp
 * - Foundation Systems: src/theme/core/foundation/
 * - Theme Engine: src/theme/core/medusa_theme_engine.hpp
 */

#pragma once

#include "medusa_avatar_component.hpp"
#include <functional>
#include <variant>

namespace MedusaTheme {
namespace Components {
namespace Avatar {

/**
 * Image Avatar Component
 * Avatar with src and alt props for basic image avatar
 */
struct ImageAvatar {
    std::string id;
    std::string src;
    std::string alt;
    
    // Image properties
    AvatarVariant variant;
    AvatarSize size;
    double custom_width;
    double custom_height;
    
    // Styling properties
    std::string border_color;
    std::string shadow_level;
    std::string spacing_level;
    
    ImageAvatar() : variant(AvatarVariant::CIRCULAR), size(AvatarSize::MD), custom_width(0.0), custom_height(0.0) {}
    ImageAvatar(const std::string& avatar_id, const std::string& image_src, const std::string& image_alt = "")
        : id(avatar_id), src(image_src), alt(image_alt), variant(AvatarVariant::CIRCULAR), size(AvatarSize::MD), custom_width(0.0), custom_height(0.0) {}
    
    // Image management
    void setImage(const std::string& image_src, const std::string& image_alt = "");
    std::string getImageSrc() const;
    std::string getImageAlt() const;
    
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
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Letter Avatar Component
 * Avatar with letters inside for letter avatar
 */
struct LetterAvatar {
    std::string id;
    std::string letters;
    
    // Letter properties
    AvatarVariant variant;
    AvatarSize size;
    AvatarSkin skin;
    double custom_width;
    double custom_height;
    
    // Styling properties
    std::string background_color;
    std::string text_color;
    std::string border_color;
    std::string shadow_level;
    std::string typography_variant;
    std::string spacing_level;
    
    LetterAvatar() : variant(AvatarVariant::CIRCULAR), size(AvatarSize::MD), skin(AvatarSkin::DEFAULT),
                     custom_width(0.0), custom_height(0.0) {}
    LetterAvatar(const std::string& avatar_id, const std::string& avatar_letters)
        : id(avatar_id), letters(avatar_letters), variant(AvatarVariant::CIRCULAR), size(AvatarSize::MD), skin(AvatarSkin::DEFAULT),
          custom_width(0.0), custom_height(0.0) {}
    
    // Letter management
    void setLetters(const std::string& avatar_letters);
    std::string getLetters() const;
    
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
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Icon Avatar Component
 * Avatar with icon as child for icon avatar
 */
struct IconAvatar {
    std::string id;
    std::string icon_name;
    
    // Icon properties
    AvatarVariant variant;
    AvatarSize size;
    double custom_width;
    double custom_height;
    
    // Styling properties
    std::string background_color;
    std::string icon_color;
    std::string border_color;
    std::string shadow_level;
    std::string spacing_level;
    
    IconAvatar() : variant(AvatarVariant::CIRCULAR), size(AvatarSize::MD), custom_width(0.0), custom_height(0.0) {}
    IconAvatar(const std::string& avatar_id, const std::string& icon_name)
        : id(avatar_id), icon_name(icon_name), variant(AvatarVariant::CIRCULAR), size(AvatarSize::MD), custom_width(0.0), custom_height(0.0) {}
    
    // Icon management
    void setIcon(const std::string& icon_name);
    std::string getIconName() const;
    
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
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Avatar with Badge Component
 * Avatar with badge component as child
 */
struct AvatarWithBadge {
    std::string id;
    AvatarComponent avatar;
    AvatarBadge badge;
    
    AvatarWithBadge() {}
    AvatarWithBadge(const std::string& avatar_id, const AvatarComponent& avatar_component)
        : id(avatar_id), avatar(avatar_component) {}
    
    // Avatar management
    void setAvatar(const AvatarComponent& avatar_component);
    AvatarComponent getAvatar() const;
    
    // Badge management
    void setBadge(const AvatarBadge& avatar_badge);
    AvatarBadge getBadge() const;
    bool hasBadge() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderBadge() const;
};

/**
 * Avatar with Tooltip Component
 * Avatar with tooltip for hover effect
 */
struct AvatarWithTooltip {
    std::string id;
    AvatarComponent avatar;
    AvatarTooltip tooltip;
    
    AvatarWithTooltip() {}
    AvatarWithTooltip(const std::string& avatar_id, const AvatarComponent& avatar_component)
        : id(avatar_id), avatar(avatar_component) {}
    
    // Avatar management
    void setAvatar(const AvatarComponent& avatar_component);
    AvatarComponent getAvatar() const;
    
    // Tooltip management
    void setTooltip(const AvatarTooltip& avatar_tooltip);
    AvatarTooltip getTooltip() const;
    bool hasTooltip() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderTooltip() const;
};

/**
 * Avatar Group with PullUp Component
 * Avatar group with pull-up effect on hover
 */
struct AvatarGroupWithPullUp {
    std::string id;
    std::vector<AvatarComponent> avatars;
    
    // Group properties
    int max_avatars;
    bool pull_up_enabled;
    std::string alignment;
    
    // Styling properties
    std::string background_color;
    std::string border_color;
    std::string shadow_level;
    std::string spacing_level;
    
    AvatarGroupWithPullUp() : max_avatars(5), pull_up_enabled(true), alignment("left") {}
    AvatarGroupWithPullUp(const std::string& group_id)
        : id(group_id), max_avatars(5), pull_up_enabled(true), alignment("left") {}
    
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
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderPullUpEffect() const;
};

/**
 * Avatar Group with Tooltip Component
 * Avatar group with tooltip on hover
 */
struct AvatarGroupWithTooltip {
    std::string id;
    std::vector<AvatarComponent> avatars;
    std::vector<AvatarTooltip> tooltips;
    
    // Group properties
    int max_avatars;
    bool pull_up_enabled;
    std::string alignment;
    
    // Styling properties
    std::string background_color;
    std::string border_color;
    std::string shadow_level;
    std::string spacing_level;
    
    AvatarGroupWithTooltip() : max_avatars(5), pull_up_enabled(true), alignment("left") {}
    AvatarGroupWithTooltip(const std::string& group_id)
        : id(group_id), max_avatars(5), pull_up_enabled(true), alignment("left") {}
    
    // Avatar management
    void addAvatar(const AvatarComponent& avatar);
    void addAvatarWithTooltip(const AvatarComponent& avatar, const AvatarTooltip& tooltip);
    void removeAvatar(const std::string& avatar_id);
    void clearAvatars();
    std::vector<AvatarComponent> getAvatars() const;
    std::vector<AvatarComponent> getVisibleAvatars() const;
    std::vector<AvatarTooltip> getTooltips() const;
    
    // Group properties
    void setMaxAvatars(int max);
    void setPullUpEnabled(bool enabled);
    void setAlignment(const std::string& group_alignment);
    int getMaxAvatars() const;
    bool isPullUpEnabled() const;
    std::string getAlignment() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderTooltips() const;
    std::string renderPullUpEffect() const;
};

/**
 * Avatar Factory
 * Factory for creating different types of avatars
 */
class AvatarFactory {
public:
    // Basic avatar creation
    static AvatarComponent createAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    
    // Image avatar creation
    static ImageAvatar createImageAvatar(const std::string& id, const std::string& src, const std::string& alt = "");
    
    // Letter avatar creation
    static LetterAvatar createLetterAvatar(const std::string& id, const std::string& letters);
    
    // Icon avatar creation
    static IconAvatar createIconAvatar(const std::string& id, const std::string& icon_name);
    
    // Avatar with badge creation
    static AvatarWithBadge createAvatarWithBadge(const std::string& id, const AvatarComponent& avatar);
    
    // Avatar with tooltip creation
    static AvatarWithTooltip createAvatarWithTooltip(const std::string& id, const AvatarComponent& avatar);
    
    // Avatar group creation
    static AvatarGroup createAvatarGroup(const std::string& id);
    static AvatarGroupWithPullUp createAvatarGroupWithPullUp(const std::string& id);
    static AvatarGroupWithTooltip createAvatarGroupWithTooltip(const std::string& id);
    
    // Predefined variants
    static AvatarComponent createCircularAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    static AvatarComponent createSquareAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    static AvatarComponent createRoundedAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    static AvatarComponent createYorkshireAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    
    // Predefined sizes
    static AvatarComponent createXSAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    static AvatarComponent createSMAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    static AvatarComponent createMDAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    static AvatarComponent createLGAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    static AvatarComponent createXLAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    static AvatarComponent createXXLAvatar(const std::string& id, AvatarType type = AvatarType::LETTER);
    
    // Yorkshire Champion avatars
    static AvatarComponent createYorkshireImageAvatar(const std::string& id, const std::string& src, const std::string& alt = "");
    static AvatarComponent createYorkshireLetterAvatar(const std::string& id, const std::string& letters);
    static AvatarComponent createYorkshireIconAvatar(const std::string& id, const std::string& icon_name);
};

/**
 * Avatar Utilities
 * Utility functions for avatar operations
 */
namespace Utils {
    // Size utilities
    double getSizeWidth(AvatarSize size);
    double getSizeHeight(AvatarSize size);
    std::string getSizeCSS(AvatarSize size);
    std::string getCustomSizeCSS(double width, double height);
    
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
    std::string getLetterAvatarColor(const std::string& letters);
    
    // Badge utilities
    AvatarBadge createDefaultBadge(const std::string& id, const std::string& content);
    AvatarBadge createYorkshireBadge(const std::string& id, const std::string& content);
    
    // Tooltip utilities
    AvatarTooltip createDefaultTooltip(const std::string& id, const std::string& content);
    AvatarTooltip createYorkshireTooltip(const std::string& id, const std::string& content);
    
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
    
    // Rendering utilities
    std::string generateAvatarCSS(const std::string& type, const std::string& variant, const std::string& size);
    std::string generateBadgeCSS(const std::string& position);
    std::string generateTooltipCSS(const std::string& position);
    std::string generatePullUpCSS();
}

/**
 * Avatar Examples
 * Example implementations for different avatar types
 */
namespace Examples {
    // Image avatar examples
    ImageAvatar createImageAvatarExample();
    ImageAvatar createCircularImageAvatarExample();
    ImageAvatar createSquareImageAvatarExample();
    ImageAvatar createRoundedImageAvatarExample();
    
    // Letter avatar examples
    LetterAvatar createLetterAvatarExample();
    LetterAvatar createLightLetterAvatarExample();
    LetterAvatar createLightStaticLetterAvatarExample();
    LetterAvatar createYorkshireLetterAvatarExample();
    
    // Icon avatar examples
    IconAvatar createIconAvatarExample();
    IconAvatar createCircularIconAvatarExample();
    IconAvatar createSquareIconAvatarExample();
    IconAvatar createYorkshireIconAvatarExample();
    
    // Avatar with badge examples
    AvatarWithBadge createAvatarWithBadgeExample();
    AvatarWithBadge createAvatarWithYorkshireBadgeExample();
    
    // Avatar with tooltip examples
    AvatarWithTooltip createAvatarWithTooltipExample();
    AvatarWithTooltip createAvatarWithYorkshireTooltipExample();
    
    // Avatar group examples
    AvatarGroup createAvatarGroupExample();
    AvatarGroupWithPullUp createAvatarGroupWithPullUpExample();
    AvatarGroupWithTooltip createAvatarGroupWithTooltipExample();
    
    // Yorkshire Champion examples
    AvatarComponent createYorkshireImageAvatarExample();
    AvatarComponent createYorkshireLetterAvatarExample();
    AvatarComponent createYorkshireIconAvatarExample();
}

} // namespace Avatar
} // namespace Components
} // namespace MedusaTheme 