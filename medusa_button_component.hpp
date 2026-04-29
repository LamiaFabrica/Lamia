/*
 * MEDUSA BUTTON COMPONENT - C++ Foundation
 * Cross-platform button component with advanced theming
 * Yorkshire Champion themed button system
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
namespace Button {

/**
 * Button Variant Enumeration
 * Different visual variants for buttons
 */
enum class ButtonVariant {
    TEXT,
    OUTLINED,
    CONTAINED,
    YORKSHIRE_CHAMPION
};

/**
 * Button Color Enumeration
 * Different color options for buttons
 */
enum class ButtonColor {
    INHERIT,
    PRIMARY,
    SECONDARY,
    SUCCESS,
    ERROR,
    INFO,
    WARNING,
    YORKSHIRE_CHAMPION
};

/**
 * Button Size Enumeration
 * Different size options for buttons
 */
enum class ButtonSize {
    SMALL,
    MEDIUM,
    LARGE,
    YORKSHIRE_CHAMPION
};

/**
 * Button State Enumeration
 * Different states for button components
 */
enum class ButtonState {
    NORMAL,
    HOVER,
    FOCUSED,
    ACTIVE,
    DISABLED,
    LOADING,
    ERROR
};

/**
 * Button Type Enumeration
 * Different types of buttons
 */
enum class ButtonType {
    BUTTON,
    SUBMIT,
    RESET,
    YORKSHIRE_CHAMPION
};

/**
 * Icon Position Enumeration
 * Different positions for icons in buttons
 */
enum class IconPosition {
    START,
    END,
    ONLY,
    YORKSHIRE_CHAMPION
};

/**
 * Button Component Structure
 * Represents a complete button component
 */
struct ButtonComponent {
    std::string id;
    std::string text;
    std::string icon_name;
    
    // Button properties
    ButtonVariant variant;
    ButtonColor color;
    ButtonSize size;
    ButtonState state;
    ButtonType type;
    IconPosition icon_position;
    
    // Icon properties
    bool has_icon;
    std::string start_icon;
    std::string end_icon;
    double icon_size;
    
    // Styling properties
    std::string background_color;
    std::string text_color;
    std::string border_color;
    std::string shadow_level;
    std::string typography_variant;
    std::string spacing_level;
    
    // Interactive properties
    bool disabled;
    bool loading;
    bool full_width;
    bool disable_elevation;
    bool disable_focus_ripple;
    bool disable_ripple;
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    ButtonComponent() : variant(ButtonVariant::TEXT), color(ButtonColor::INHERIT), size(ButtonSize::MEDIUM),
                        state(ButtonState::NORMAL), type(ButtonType::BUTTON), icon_position(IconPosition::ONLY),
                        has_icon(false), icon_size(24.0), disabled(false), loading(false), full_width(false),
                        disable_elevation(false), disable_focus_ripple(false), disable_ripple(false) {}
    
    ButtonComponent(const std::string& button_id, const std::string& button_text)
        : id(button_id), text(button_text), variant(ButtonVariant::TEXT), color(ButtonColor::INHERIT), size(ButtonSize::MEDIUM),
          state(ButtonState::NORMAL), type(ButtonType::BUTTON), icon_position(IconPosition::ONLY),
          has_icon(false), icon_size(24.0), disabled(false), loading(false), full_width(false),
          disable_elevation(false), disable_focus_ripple(false), disable_ripple(false) {}
    
    // Content management
    void setText(const std::string& button_text);
    void setIcon(const std::string& icon_name, IconPosition position = IconPosition::START);
    void setStartIcon(const std::string& icon_name);
    void setEndIcon(const std::string& icon_name);
    std::string getText() const;
    std::string getIconName() const;
    std::string getStartIcon() const;
    std::string getEndIcon() const;
    
    // Variant management
    void setVariant(ButtonVariant button_variant);
    ButtonVariant getVariant() const;
    std::string getVariantString() const;
    
    // Color management
    void setColor(ButtonColor button_color);
    ButtonColor getColor() const;
    std::string getColorString() const;
    
    // Size management
    void setSize(ButtonSize button_size);
    ButtonSize getSize() const;
    std::string getSizeString() const;
    
    // State management
    void setState(ButtonState button_state);
    ButtonState getState() const;
    std::string getStateString() const;
    
    // Type management
    void setType(ButtonType button_type);
    ButtonType getType() const;
    std::string getTypeString() const;
    
    // Icon management
    void setIconPosition(IconPosition position);
    void setIconSize(double size);
    IconPosition getIconPosition() const;
    double getIconSize() const;
    bool hasIcon() const;
    
    // Interactive management
    void setDisabled(bool is_disabled);
    void setLoading(bool is_loading);
    void setFullWidth(bool is_full_width);
    void setDisableElevation(bool disable);
    void setDisableFocusRipple(bool disable);
    void setDisableRipple(bool disable);
    bool isDisabled() const;
    bool isLoading() const;
    bool isFullWidth() const;
    bool isElevationDisabled() const;
    bool isFocusRippleDisabled() const;
    bool isRippleDisabled() const;
    
    // Property access
    std::string getProperty(const std::string& property_name) const;
    void setProperty(const std::string& property_name, const std::string& value);
    
    // Component manipulation
    ButtonComponent withVariant(ButtonVariant button_variant) const;
    ButtonComponent withColor(ButtonColor button_color) const;
    ButtonComponent withSize(ButtonSize button_size) const;
    ButtonComponent withIcon(const std::string& icon_name, IconPosition position = IconPosition::START) const;
    ButtonComponent withDisabled(bool is_disabled) const;
    ButtonComponent withLoading(bool is_loading) const;
};

/**
 * Contained Button Component
 * Button with variant='contained' prop for contained buttons
 */
struct ContainedButton {
    std::string id;
    std::string text;
    ButtonColor color;
    ButtonSize size;
    
    // Contained properties
    bool disabled;
    bool full_width;
    std::string background_color;
    std::string text_color;
    std::string shadow_level;
    
    ContainedButton() : color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM), disabled(false), full_width(false) {}
    ContainedButton(const std::string& button_id, const std::string& button_text, ButtonColor button_color = ButtonColor::PRIMARY)
        : id(button_id), text(button_text), color(button_color), size(ButtonSize::MEDIUM), disabled(false), full_width(false) {}
    
    // Content management
    void setText(const std::string& button_text);
    std::string getText() const;
    
    // Color management
    void setColor(ButtonColor button_color);
    ButtonColor getColor() const;
    std::string getColorString() const;
    
    // Size management
    void setSize(ButtonSize button_size);
    ButtonSize getSize() const;
    std::string getSizeString() const;
    
    // Interactive management
    void setDisabled(bool is_disabled);
    void setFullWidth(bool is_full_width);
    bool isDisabled() const;
    bool isFullWidth() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Text Button Component
 * Button with variant='text' prop for text-only buttons
 */
struct TextButton {
    std::string id;
    std::string text;
    ButtonColor color;
    ButtonSize size;
    
    // Text properties
    bool disabled;
    bool full_width;
    std::string text_color;
    std::string typography_variant;
    
    TextButton() : color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM), disabled(false), full_width(false) {}
    TextButton(const std::string& button_id, const std::string& button_text, ButtonColor button_color = ButtonColor::PRIMARY)
        : id(button_id), text(button_text), color(button_color), size(ButtonSize::MEDIUM), disabled(false), full_width(false) {}
    
    // Content management
    void setText(const std::string& button_text);
    std::string getText() const;
    
    // Color management
    void setColor(ButtonColor button_color);
    ButtonColor getColor() const;
    std::string getColorString() const;
    
    // Size management
    void setSize(ButtonSize button_size);
    ButtonSize getSize() const;
    std::string getSizeString() const;
    
    // Interactive management
    void setDisabled(bool is_disabled);
    void setFullWidth(bool is_full_width);
    bool isDisabled() const;
    bool isFullWidth() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Outlined Button Component
 * Button with variant='outlined' prop for outlined buttons
 */
struct OutlinedButton {
    std::string id;
    std::string text;
    ButtonColor color;
    ButtonSize size;
    
    // Outlined properties
    bool disabled;
    bool full_width;
    std::string border_color;
    std::string text_color;
    double border_width;
    
    OutlinedButton() : color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM), disabled(false), full_width(false), border_width(1.0) {}
    OutlinedButton(const std::string& button_id, const std::string& button_text, ButtonColor button_color = ButtonColor::PRIMARY)
        : id(button_id), text(button_text), color(button_color), size(ButtonSize::MEDIUM), disabled(false), full_width(false), border_width(1.0) {}
    
    // Content management
    void setText(const std::string& button_text);
    std::string getText() const;
    
    // Color management
    void setColor(ButtonColor button_color);
    ButtonColor getColor() const;
    std::string getColorString() const;
    
    // Size management
    void setSize(ButtonSize button_size);
    ButtonSize getSize() const;
    std::string getSizeString() const;
    
    // Border management
    void setBorderWidth(double width);
    double getBorderWidth() const;
    
    // Interactive management
    void setDisabled(bool is_disabled);
    void setFullWidth(bool is_full_width);
    bool isDisabled() const;
    bool isFullWidth() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Icon Button Component
 * Button with icon component inside for icon buttons
 */
struct IconButton {
    std::string id;
    std::string icon_name;
    ButtonColor color;
    ButtonSize size;
    
    // Icon properties
    bool disabled;
    double icon_size;
    double font_size;  // Added: fontSize prop for icons
    std::string icon_color;
    std::string background_color;
    
    IconButton() : color(ButtonColor::DEFAULT), size(ButtonSize::MEDIUM), disabled(false), icon_size(24.0), font_size(24.0) {}
    IconButton(const std::string& button_id, const std::string& icon_name, ButtonColor button_color = ButtonColor::DEFAULT)
        : id(button_id), icon_name(icon_name), color(button_color), size(ButtonSize::MEDIUM), disabled(false), icon_size(24.0), font_size(24.0) {}
    
    // Icon management
    void setIcon(const std::string& icon_name);
    void setIconSize(double size);
    void setFontSize(double size);  // Added: fontSize prop
    std::string getIconName() const;
    double getIconSize() const;
    double getFontSize() const;  // Added: fontSize getter
    
    // Color management
    void setColor(ButtonColor button_color);
    ButtonColor getColor() const;
    std::string getColorString() const;
    
    // Size management
    void setSize(ButtonSize button_size);
    ButtonSize getSize() const;
    std::string getSizeString() const;
    
    // Interactive management
    void setDisabled(bool is_disabled);
    bool isDisabled() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderSmallSize() const;  // Added: small size rendering
    std::string renderLargeSize() const;  // Added: large size rendering
};

/**
 * Button with Icon and Label Component
 * Button with startIcon or endIcon prop for buttons with icons
 */
struct ButtonWithIcon {
    std::string id;
    std::string text;
    std::string start_icon;
    std::string end_icon;
    ButtonVariant variant;
    ButtonColor color;
    ButtonSize size;
    
    // Icon properties
    bool disabled;
    double icon_size;
    IconPosition icon_position;
    
    ButtonWithIcon() : variant(ButtonVariant::CONTAINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
                       disabled(false), icon_size(24.0), icon_position(IconPosition::START) {}
    ButtonWithIcon(const std::string& button_id, const std::string& button_text)
        : id(button_id), text(button_text), variant(ButtonVariant::CONTAINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
          disabled(false), icon_size(24.0), icon_position(IconPosition::START) {}
    
    // Content management
    void setText(const std::string& button_text);
    void setStartIcon(const std::string& icon_name);
    void setEndIcon(const std::string& icon_name);
    std::string getText() const;
    std::string getStartIcon() const;
    std::string getEndIcon() const;
    
    // Variant management
    void setVariant(ButtonVariant button_variant);
    ButtonVariant getVariant() const;
    std::string getVariantString() const;
    
    // Color management
    void setColor(ButtonColor button_color);
    ButtonColor getColor() const;
    std::string getColorString() const;
    
    // Size management
    void setSize(ButtonSize button_size);
    ButtonSize getSize() const;
    std::string getSizeString() const;
    
    // Icon management
    void setIconSize(double size);
    void setIconPosition(IconPosition position);
    double getIconSize() const;
    IconPosition getIconPosition() const;
    
    // Interactive management
    void setDisabled(bool is_disabled);
    bool isDisabled() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderStartIcon() const;
    std::string renderEndIcon() const;
};

/**
 * Custom Icon Button Component
 * Custom icon button with default color and all button props
 */
struct CustomIconButton {
    std::string id;
    std::string icon_name;
    ButtonColor color;
    ButtonSize size;
    
    // Custom properties
    bool disabled;
    double icon_size;
    std::string default_color;
    std::string background_color;
    std::string icon_color;
    
    CustomIconButton() : color(ButtonColor::DEFAULT), size(ButtonSize::MEDIUM), disabled(false), icon_size(24.0) {}
    CustomIconButton(const std::string& button_id, const std::string& icon_name, ButtonColor button_color = ButtonColor::DEFAULT)
        : id(button_id), icon_name(icon_name), color(button_color), size(ButtonSize::MEDIUM), disabled(false), icon_size(24.0) {}
    
    // Icon management
    void setIcon(const std::string& icon_name);
    void setIconSize(double size);
    std::string getIconName() const;
    double getIconSize() const;
    
    // Color management
    void setColor(ButtonColor button_color);
    void setDefaultColor(const std::string& color);
    ButtonColor getColor() const;
    std::string getColorString() const;
    std::string getDefaultColor() const;
    
    // Size management
    void setSize(ButtonSize button_size);
    ButtonSize getSize() const;
    std::string getSizeString() const;
    
    // Interactive management
    void setDisabled(bool is_disabled);
    bool isDisabled() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Floating Action Button Component
 * Floating action button with color and variant props
 */
struct FloatingActionButton {
    std::string id;
    std::string icon_name;
    ButtonColor color;
    ButtonSize size;
    
    // FAB properties
    bool disabled;
    bool extended;
    std::string extended_text;
    std::string background_color;
    std::string icon_color;
    std::string shadow_level;
    
    // Added: FAB variant support
    std::string variant;  // "circular" or "extended"
    
    FloatingActionButton() : color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM), disabled(false), extended(false), variant("circular") {}
    FloatingActionButton(const std::string& button_id, const std::string& icon_name, ButtonColor button_color = ButtonColor::PRIMARY)
        : id(button_id), icon_name(icon_name), color(button_color), size(ButtonSize::MEDIUM), disabled(false), extended(false), variant("circular") {}
    
    // Icon management
    void setIcon(const std::string& icon_name);
    std::string getIconName() const;
    
    // Color management
    void setColor(ButtonColor button_color);
    ButtonColor getColor() const;
    std::string getColorString() const;
    
    // Size management
    void setSize(ButtonSize button_size);
    ButtonSize getSize() const;
    std::string getSizeString() const;
    
    // Extended management
    void setExtended(bool is_extended, const std::string& text = "");
    void setExtendedText(const std::string& text);
    bool isExtended() const;
    std::string getExtendedText() const;
    
    // Added: Variant management
    void setVariant(const std::string& fab_variant);
    std::string getVariant() const;
    bool isCircular() const;
    bool isExtendedVariant() const;
    
    // Interactive management
    void setDisabled(bool is_disabled);
    bool isDisabled() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderCircular() const;
    std::string renderExtended() const;
    std::string renderSmallSize() const;  // Added: small size rendering
    std::string renderMediumSize() const;  // Added: medium size rendering
    std::string renderLargeSize() const;  // Added: large size rendering
};

/**
 * Button Group Component
 * Group of buttons with various configurations
 */
struct ButtonGroup {
    std::string id;
    std::vector<ButtonComponent> buttons;
    
    // Group properties
    ButtonVariant variant;
    ButtonColor color;
    ButtonSize size;
    std::string orientation;  // "horizontal", "vertical"
    bool disabled;
    bool full_width;
    
    // Added: Button Group specific properties
    bool split_button;  // Added: Split button functionality
    std::string dropdown_icon;  // Added: Dropdown icon for split buttons
    bool expanded;  // Added: Expanded state for split buttons
    std::vector<ButtonComponent> dropdown_buttons;  // Added: Dropdown buttons for split functionality
    
    // Styling properties
    std::string background_color;
    std::string border_color;
    std::string shadow_level;
    std::string spacing_level;
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    ButtonGroup() : variant(ButtonVariant::OUTLINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
                    orientation("horizontal"), disabled(false), full_width(false), split_button(false), expanded(false) {}
    ButtonGroup(const std::string& group_id)
        : id(group_id), variant(ButtonVariant::OUTLINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
          orientation("horizontal"), disabled(false), full_width(false), split_button(false), expanded(false) {}
    
    // Button management
    void addButton(const ButtonComponent& button);
    void removeButton(const std::string& button_id);
    void clearButtons();
    std::vector<ButtonComponent> getButtons() const;
    
    // Group properties
    void setVariant(ButtonVariant group_variant);
    void setColor(ButtonColor group_color);
    void setSize(ButtonSize group_size);
    void setOrientation(const std::string& group_orientation);
    void setDisabled(bool is_disabled);
    void setFullWidth(bool is_full_width);
    ButtonVariant getVariant() const;
    ButtonColor getColor() const;
    ButtonSize getSize() const;
    std::string getOrientation() const;
    bool isDisabled() const;
    bool isFullWidth() const;
    
    // Added: Split button management
    void setSplitButton(bool is_split_button);
    void setDropdownIcon(const std::string& icon_name);
    void setExpanded(bool is_expanded);
    void addDropdownButton(const ButtonComponent& button);
    void removeDropdownButton(const std::string& button_id);
    void clearDropdownButtons();
    bool isSplitButton() const;
    std::string getDropdownIcon() const;
    bool isExpanded() const;
    std::vector<ButtonComponent> getDropdownButtons() const;
    
    // Property access
    std::string getProperty(const std::string& property_name) const;
    void setProperty(const std::string& property_name, const std::string& value);
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderSplitButton() const;  // Added: Split button rendering
    std::string renderDropdown() const;  // Added: Dropdown rendering
};

/**
 * Split Button Component
 * Button group with dropdown functionality
 */
struct SplitButton {
    std::string id;
    ButtonComponent main_button;
    std::vector<ButtonComponent> dropdown_buttons;
    
    // Split properties
    bool disabled;
    bool expanded;
    std::string dropdown_icon;
    std::string background_color;
    std::string border_color;
    
    SplitButton() : disabled(false), expanded(false) {}
    SplitButton(const std::string& split_id, const ButtonComponent& main_btn)
        : id(split_id), main_button(main_btn), disabled(false), expanded(false) {}
    
    // Button management
    void setMainButton(const ButtonComponent& button);
    void addDropdownButton(const ButtonComponent& button);
    void removeDropdownButton(const std::string& button_id);
    void clearDropdownButtons();
    ButtonComponent getMainButton() const;
    std::vector<ButtonComponent> getDropdownButtons() const;
    
    // Split management
    void setDisabled(bool is_disabled);
    void setExpanded(bool is_expanded);
    void setDropdownIcon(const std::string& icon_name);
    bool isDisabled() const;
    bool isExpanded() const;
    std::string getDropdownIcon() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderMainButton() const;
    std::string renderDropdown() const;
};

/**
 * Button Renderer
 * Handles button rendering for different platforms
 */
class ButtonRenderer {
private:
    std::map<std::string, std::string> render_cache_;
    std::mutex render_mutex_;
    
public:
    ButtonRenderer();
    ~ButtonRenderer() = default;
    
    // Component rendering
    std::string renderButton(const ButtonComponent& button, const std::string& platform = "default");
    std::string renderButtonWithStyle(const ButtonComponent& button, const std::map<std::string, std::string>& styles);
    std::string renderContainedButton(const ContainedButton& button, const std::string& platform = "default");
    std::string renderTextButton(const TextButton& button, const std::string& platform = "default");
    std::string renderOutlinedButton(const OutlinedButton& button, const std::string& platform = "default");
    std::string renderIconButton(const IconButton& button, const std::string& platform = "default");
    std::string renderButtonWithIcon(const ButtonWithIcon& button, const std::string& platform = "default");
    std::string renderCustomIconButton(const CustomIconButton& button, const std::string& platform = "default");
    std::string renderFloatingActionButton(const FloatingActionButton& button, const std::string& platform = "default");
    std::string renderButtonGroup(const ButtonGroup& group, const std::string& platform = "default");
    std::string renderSplitButton(const SplitButton& split, const std::string& platform = "default");
    
    // Platform-specific rendering
    std::string renderForWindows(const ButtonComponent& button);
    std::string renderForMacOS(const ButtonComponent& button);
    std::string renderForLinux(const ButtonComponent& button);
    std::string renderForWeb(const ButtonComponent& button);
    std::string renderForMobile(const ButtonComponent& button);
    std::string renderForConsole(const ButtonComponent& button);
    
    // Variant-specific rendering
    std::string renderTextVariant(const ButtonComponent& button);
    std::string renderOutlinedVariant(const ButtonComponent& button);
    std::string renderContainedVariant(const ButtonComponent& button);
    std::string renderYorkshireVariant(const ButtonComponent& button);
    
    // Color-specific rendering
    std::string renderInheritColor(const ButtonComponent& button);
    std::string renderPrimaryColor(const ButtonComponent& button);
    std::string renderSecondaryColor(const ButtonComponent& button);
    std::string renderSuccessColor(const ButtonComponent& button);
    std::string renderErrorColor(const ButtonComponent& button);
    std::string renderInfoColor(const ButtonComponent& button);
    std::string renderWarningColor(const ButtonComponent& button);
    std::string renderYorkshireColor(const ButtonComponent& button);
    
    // Size-specific rendering
    std::string renderSmallSize(const ButtonComponent& button);
    std::string renderMediumSize(const ButtonComponent& button);
    std::string renderLargeSize(const ButtonComponent& button);
    std::string renderYorkshireSize(const ButtonComponent& button);
    
    // Format-specific rendering
    std::string renderAsCSS(const ButtonComponent& button);
    std::string renderAsHTML(const ButtonComponent& button);
    std::string renderAsJSON(const ButtonComponent& button);
    std::string renderAsXML(const ButtonComponent& button);
    
    // Icon rendering
    std::string renderIcon(const std::string& icon_name, double size, const std::string& color);
    std::string renderStartIcon(const ButtonComponent& button);
    std::string renderEndIcon(const ButtonComponent& button);
    
    // Cache management
    void clearCache();
    size_t getCacheSize() const;
    double getCacheHitRate() const;
    
private:
    std::string generateButtonCSS(const ButtonComponent& button);
    std::string generateButtonHTML(const ButtonComponent& button);
    std::string generateButtonJSON(const ButtonComponent& button);
    std::string generateButtonXML(const ButtonComponent& button);
    std::string generatePlatformButton(const ButtonComponent& button, const std::string& platform);
    std::string generateVariantCSS(ButtonVariant variant);
    std::string generateColorCSS(ButtonColor color);
    std::string generateSizeCSS(ButtonSize size);
};

/**
 * Button Cache Manager
 * Manages button caching and optimization
 */
class ButtonCacheManager {
private:
    std::unordered_map<std::string, std::string> button_cache_;
    std::unordered_map<std::string, ButtonComponent> component_cache_;
    std::unordered_map<std::string, std::string> contained_cache_;
    std::unordered_map<std::string, std::string> text_cache_;
    std::unordered_map<std::string, std::string> outlined_cache_;
    std::unordered_map<std::string, std::string> icon_cache_;
    std::unordered_map<std::string, std::string> fab_cache_;
    std::unordered_map<std::string, std::string> group_cache_;
    std::mutex cache_mutex_;
    
    size_t max_cache_size_ = 1000;
    size_t cache_hits_ = 0;
    size_t cache_misses_ = 0;
    
public:
    ButtonCacheManager();
    ~ButtonCacheManager() = default;
    
    // Cache operations
    void cacheButton(const std::string& key, const std::string& rendered_button);
    void cacheComponent(const std::string& key, const ButtonComponent& component);
    void cacheContainedButton(const std::string& key, const std::string& rendered_button);
    void cacheTextButton(const std::string& key, const std::string& rendered_button);
    void cacheOutlinedButton(const std::string& key, const std::string& rendered_button);
    void cacheIconButton(const std::string& key, const std::string& rendered_button);
    void cacheFloatingActionButton(const std::string& key, const std::string& rendered_button);
    void cacheButtonGroup(const std::string& key, const std::string& rendered_group);
    bool getCachedButton(const std::string& key, std::string& rendered_button);
    bool getCachedComponent(const std::string& key, ButtonComponent& component);
    bool getCachedContainedButton(const std::string& key, std::string& rendered_button);
    bool getCachedTextButton(const std::string& key, std::string& rendered_button);
    bool getCachedOutlinedButton(const std::string& key, std::string& rendered_button);
    bool getCachedIconButton(const std::string& key, std::string& rendered_button);
    bool getCachedFloatingActionButton(const std::string& key, std::string& rendered_button);
    bool getCachedButtonGroup(const std::string& key, std::string& rendered_group);
    
    // Cache management
    void setMaxCacheSize(size_t size);
    size_t getCacheSize() const;
    void cleanupCache();
    void clearCache();
    
    // Statistics
    double getHitRate() const;
    size_t getButtonCacheSize() const;
    size_t getComponentCacheSize() const;
    size_t getContainedCacheSize() const;
    size_t getTextCacheSize() const;
    size_t getOutlinedCacheSize() const;
    size_t getIconCacheSize() const;
    size_t getFabCacheSize() const;
    size_t getGroupCacheSize() const;
    
private:
    void evictOldestEntries();
    std::string generateCacheKey(const ButtonComponent& button, const std::string& platform);
    std::string generateContainedCacheKey(const ContainedButton& button, const std::string& platform);
    std::string generateTextCacheKey(const TextButton& button, const std::string& platform);
    std::string generateOutlinedCacheKey(const OutlinedButton& button, const std::string& platform);
    std::string generateIconCacheKey(const IconButton& button, const std::string& platform);
    std::string generateFabCacheKey(const FloatingActionButton& button, const std::string& platform);
    std::string generateGroupCacheKey(const ButtonGroup& group, const std::string& platform);
};

/**
 * Button System
 * Main system for managing button components across all platforms
 */
class ButtonSystem {
private:
    static std::unique_ptr<ButtonSystem> instance_;
    static std::mutex instance_mutex_;
    
    // Foundation systems
    std::unique_ptr<Foundation::Colors::ColorSystem> color_system_;
    std::unique_ptr<Foundation::Typography::TypographySystem> typography_system_;
    std::unique_ptr<Foundation::Shadows::ShadowSystem> shadow_system_;
    std::unique_ptr<Foundation::Spacing::SpacingSystem> spacing_system_;
    std::unique_ptr<Foundation::Iconify::IconifySystem> iconify_system_;
    
    // Button management
    std::unique_ptr<ButtonRenderer> renderer_;
    std::unique_ptr<ButtonCacheManager> cache_manager_;
    
    // Current state
    std::string current_platform_;
    std::string current_theme_;
    bool initialized_ = false;
    
public:
    ButtonSystem();
    ~ButtonSystem() = default;
    
    // Singleton access
    static ButtonSystem& getInstance();
    static void initialize();
    static void shutdown();
    
    // System management
    bool initialize();
    void setPlatform(const std::string& platform);
    void setTheme(const std::string& theme);
    std::string getCurrentPlatform() const;
    std::string getCurrentTheme() const;
    
    // Component operations
    std::string renderButton(const ButtonComponent& button);
    std::string renderContainedButton(const ContainedButton& button);
    std::string renderTextButton(const TextButton& button);
    std::string renderOutlinedButton(const OutlinedButton& button);
    std::string renderIconButton(const IconButton& button);
    std::string renderButtonWithIcon(const ButtonWithIcon& button);
    std::string renderCustomIconButton(const CustomIconButton& button);
    std::string renderFloatingActionButton(const FloatingActionButton& button);
    std::string renderButtonGroup(const ButtonGroup& group);
    std::string renderSplitButton(const SplitButton& split);
    std::string renderButtonWithStyle(const ButtonComponent& button, const std::map<std::string, std::string>& styles);
    
    // Component creation
    ButtonComponent createButton(const std::string& id, const std::string& text = "");
    ContainedButton createContainedButton(const std::string& id, const std::string& text, ButtonColor color = ButtonColor::PRIMARY);
    TextButton createTextButton(const std::string& id, const std::string& text, ButtonColor color = ButtonColor::PRIMARY);
    OutlinedButton createOutlinedButton(const std::string& id, const std::string& text, ButtonColor color = ButtonColor::PRIMARY);
    IconButton createIconButton(const std::string& id, const std::string& icon_name, ButtonColor color = ButtonColor::DEFAULT);
    ButtonWithIcon createButtonWithIcon(const std::string& id, const std::string& text);
    CustomIconButton createCustomIconButton(const std::string& id, const std::string& icon_name, ButtonColor color = ButtonColor::DEFAULT);
    FloatingActionButton createFloatingActionButton(const std::string& id, const std::string& icon_name, ButtonColor color = ButtonColor::PRIMARY);
    ButtonGroup createButtonGroup(const std::string& id);
    SplitButton createSplitButton(const std::string& id, const ButtonComponent& main_button);
    
    // Predefined buttons
    ButtonComponent createPrimaryButton(const std::string& id, const std::string& text);
    ButtonComponent createSecondaryButton(const std::string& id, const std::string& text);
    ButtonComponent createSuccessButton(const std::string& id, const std::string& text);
    ButtonComponent createErrorButton(const std::string& id, const std::string& text);
    ButtonComponent createWarningButton(const std::string& id, const std::string& text);
    ButtonComponent createInfoButton(const std::string& id, const std::string& text);
    ButtonComponent createYorkshireButton(const std::string& id, const std::string& text);
    
    // Platform-specific operations
    std::string getPlatformSpecificButton(const ButtonComponent& button) const;
    ButtonComponent getPlatformAdjustedButton(const ButtonComponent& button) const;
    
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
    void createDefaultButtons();
    void setupButtonDefaults();
    void validateButton(const ButtonComponent& button);
};

/**
 * Button Utilities
 * Helper functions for button operations
 */
namespace Utils {
    // Button utilities
    std::string buttonVariantToString(ButtonVariant variant);
    ButtonVariant stringToButtonVariant(const std::string& variant);
    std::string buttonColorToString(ButtonColor color);
    ButtonColor stringToButtonColor(const std::string& color);
    std::string buttonSizeToString(ButtonSize size);
    ButtonSize stringToButtonSize(const std::string& size);
    std::string buttonStateToString(ButtonState state);
    ButtonState stringToButtonState(const std::string& state);
    std::string buttonTypeToString(ButtonType type);
    ButtonType stringToButtonType(const std::string& type);
    std::string iconPositionToString(IconPosition position);
    IconPosition stringToIconPosition(const std::string& position);
    
    // Validation utilities
    bool isValidButtonId(const std::string& id);
    bool isValidButtonComponent(const ButtonComponent& button);
    bool isValidButtonVariant(ButtonVariant variant);
    bool isValidButtonColor(ButtonColor color);
    bool isValidButtonSize(ButtonSize size);
    bool isValidIconName(const std::string& icon_name);
    
    // Platform utilities
    std::string detectPlatform();
    bool isPlatformSupported(const std::string& platform);
    std::string getPlatformButtonFormat(const std::string& platform);
    
    // Color utilities
    std::string getDefaultButtonBackgroundColor();
    std::string getDefaultButtonTextColor();
    std::string getColorBackgroundColor(ButtonColor color);
    std::string getColorTextColor(ButtonColor color);
    std::string getYorkshireButtonBackgroundColor();
    std::string getYorkshireButtonTextColor();
    
    // Size utilities
    double getButtonHeight(ButtonSize size);
    double getButtonPadding(ButtonSize size);
    double getIconSize(ButtonSize size);
    std::string getSizeCSS(ButtonSize size);
    
    // Icon utilities
    std::string getDefaultButtonIcon();
    std::string getYorkshireButtonIcon();
    std::string getIconCSS(const std::string& icon_name, double size);
}

} // namespace Button
} // namespace Components
} // namespace MedusaTheme 