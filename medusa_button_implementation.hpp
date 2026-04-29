/*
 * MEDUSA BUTTON IMPLEMENTATION - C++ Foundation
 * Complete button implementation with all MUI button features
 * Yorkshire Champion themed button implementation
 * 
 * Deep Links for Codebase Tracing:
 * - Component Header: src/theme/components/button/medusa_button_component.hpp
 * - Foundation Systems: src/theme/core/foundation/
 * - Theme Engine: src/theme/core/medusa_theme_engine.hpp
 */

#pragma once

#include "medusa_button_component.hpp"
#include <functional>
#include <variant>

namespace MedusaTheme {
namespace Components {
namespace Button {

/**
 * Button with Icon and Label Component
 * Button with startIcon or endIcon prop for buttons with icons
 */
struct ButtonWithIconAndLabel {
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
    
    ButtonWithIconAndLabel() : variant(ButtonVariant::CONTAINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
                               disabled(false), icon_size(24.0), icon_position(IconPosition::START) {}
    ButtonWithIconAndLabel(const std::string& button_id, const std::string& button_text)
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
 * Button with Different Sizes Component
 * Button with size prop for different sized buttons
 */
struct ButtonWithSizes {
    std::string id;
    std::string text;
    ButtonVariant variant;
    ButtonColor color;
    ButtonSize size;
    
    // Size properties
    bool disabled;
    double height;
    double padding;
    double icon_size;
    
    ButtonWithSizes() : variant(ButtonVariant::CONTAINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
                        disabled(false), height(36.0), padding(16.0), icon_size(24.0) {}
    ButtonWithSizes(const std::string& button_id, const std::string& button_text)
        : id(button_id), text(button_text), variant(ButtonVariant::CONTAINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
          disabled(false), height(36.0), padding(16.0), icon_size(24.0) {}
    
    // Content management
    void setText(const std::string& button_text);
    std::string getText() const;
    
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
    double getHeight() const;
    double getPadding() const;
    double getIconSize() const;
    
    // Interactive management
    void setDisabled(bool is_disabled);
    bool isDisabled() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderSmallSize() const;
    std::string renderMediumSize() const;
    std::string renderLargeSize() const;
};

/**
 * Button with Different Colors Component
 * Button with color prop for different colored buttons
 */
struct ButtonWithColors {
    std::string id;
    std::string text;
    ButtonVariant variant;
    ButtonColor color;
    ButtonSize size;
    
    // Color properties
    bool disabled;
    std::string background_color;
    std::string text_color;
    std::string border_color;
    
    ButtonWithColors() : variant(ButtonVariant::CONTAINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
                         disabled(false) {}
    ButtonWithColors(const std::string& button_id, const std::string& button_text)
        : id(button_id), text(button_text), variant(ButtonVariant::CONTAINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
          disabled(false) {}
    
    // Content management
    void setText(const std::string& button_text);
    std::string getText() const;
    
    // Variant management
    void setVariant(ButtonVariant button_variant);
    ButtonVariant getVariant() const;
    std::string getVariantString() const;
    
    // Color management
    void setColor(ButtonColor button_color);
    ButtonColor getColor() const;
    std::string getColorString() const;
    std::string getBackgroundColor() const;
    std::string getTextColor() const;
    std::string getBorderColor() const;
    
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
    std::string renderPrimaryColor() const;
    std::string renderSecondaryColor() const;
    std::string renderSuccessColor() const;
    std::string renderErrorColor() const;
    std::string renderWarningColor() const;
    std::string renderInfoColor() const;
};

/**
 * Customized Button Component
 * Customized button with advanced styling
 */
struct CustomizedButton {
    std::string id;
    std::string text;
    ButtonVariant variant;
    ButtonColor color;
    ButtonSize size;
    
    // Custom properties
    bool disabled;
    bool full_width;
    std::string custom_style;
    std::map<std::string, std::string> custom_properties;
    
    CustomizedButton() : variant(ButtonVariant::CONTAINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
                         disabled(false), full_width(false) {}
    CustomizedButton(const std::string& button_id, const std::string& button_text)
        : id(button_id), text(button_text), variant(ButtonVariant::CONTAINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
          disabled(false), full_width(false) {}
    
    // Content management
    void setText(const std::string& button_text);
    std::string getText() const;
    
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
    
    // Custom management
    void setCustomStyle(const std::string& style);
    void setCustomProperty(const std::string& property, const std::string& value);
    void setFullWidth(bool is_full_width);
    std::string getCustomStyle() const;
    std::string getCustomProperty(const std::string& property) const;
    bool isFullWidth() const;
    
    // Interactive management
    void setDisabled(bool is_disabled);
    bool isDisabled() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderCustomStyle() const;
};

/**
 * Floating Action Button with Variants Component
 * FAB with circular and extended variants
 */
struct FloatingActionButtonWithVariants {
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
    
    FloatingActionButtonWithVariants() : color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM), disabled(false), extended(false) {}
    FloatingActionButtonWithVariants(const std::string& button_id, const std::string& icon_name, ButtonColor button_color = ButtonColor::PRIMARY)
        : id(button_id), icon_name(icon_name), color(button_color), size(ButtonSize::MEDIUM), disabled(false), extended(false) {}
    
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
    
    // Variant management
    void setExtended(bool is_extended, const std::string& text = "");
    void setExtendedText(const std::string& text);
    bool isExtended() const;
    std::string getExtendedText() const;
    
    // Interactive management
    void setDisabled(bool is_disabled);
    bool isDisabled() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderCircular() const;
    std::string renderExtended() const;
};

/**
 * Floating Action Button with Sizes Component
 * FAB with different sizes
 */
struct FloatingActionButtonWithSizes {
    std::string id;
    std::string icon_name;
    ButtonColor color;
    ButtonSize size;
    
    // FAB properties
    bool disabled;
    bool extended;
    std::string extended_text;
    double fab_size;
    double icon_size;
    
    FloatingActionButtonWithSizes() : color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM), disabled(false), extended(false),
                                      fab_size(56.0), icon_size(24.0) {}
    FloatingActionButtonWithSizes(const std::string& button_id, const std::string& icon_name, ButtonColor button_color = ButtonColor::PRIMARY)
        : id(button_id), icon_name(icon_name), color(button_color), size(ButtonSize::MEDIUM), disabled(false), extended(false),
          fab_size(56.0), icon_size(24.0) {}
    
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
    double getFabSize() const;
    double getIconSize() const;
    
    // Extended management
    void setExtended(bool is_extended, const std::string& text = "");
    void setExtendedText(const std::string& text);
    bool isExtended() const;
    std::string getExtendedText() const;
    
    // Interactive management
    void setDisabled(bool is_disabled);
    bool isDisabled() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderSmallSize() const;
    std::string renderMediumSize() const;
    std::string renderLargeSize() const;
};

/**
 * Button Group with Variants Component
 * Button group with different variants
 */
struct ButtonGroupWithVariants {
    std::string id;
    std::vector<ButtonComponent> buttons;
    
    // Group properties
    ButtonVariant variant;
    ButtonColor color;
    ButtonSize size;
    std::string orientation;
    bool disabled;
    bool full_width;
    
    // Styling properties
    std::string background_color;
    std::string border_color;
    std::string shadow_level;
    std::string spacing_level;
    
    ButtonGroupWithVariants() : variant(ButtonVariant::OUTLINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
                                orientation("horizontal"), disabled(false), full_width(false) {}
    ButtonGroupWithVariants(const std::string& group_id)
        : id(group_id), variant(ButtonVariant::OUTLINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
          orientation("horizontal"), disabled(false), full_width(false) {}
    
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
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderOutlinedVariant() const;
    std::string renderContainedVariant() const;
    std::string renderTextVariant() const;
};

/**
 * Button Group with Sizes Component
 * Button group with different sizes
 */
struct ButtonGroupWithSizes {
    std::string id;
    std::vector<ButtonComponent> buttons;
    
    // Group properties
    ButtonVariant variant;
    ButtonColor color;
    ButtonSize size;
    std::string orientation;
    bool disabled;
    bool full_width;
    
    // Size properties
    double group_height;
    double button_padding;
    double spacing;
    
    ButtonGroupWithSizes() : variant(ButtonVariant::OUTLINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
                             orientation("horizontal"), disabled(false), full_width(false),
                             group_height(36.0), button_padding(16.0), spacing(8.0) {}
    ButtonGroupWithSizes(const std::string& group_id)
        : id(group_id), variant(ButtonVariant::OUTLINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
          orientation("horizontal"), disabled(false), full_width(false),
          group_height(36.0), button_padding(16.0), spacing(8.0) {}
    
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
    
    // Size properties
    double getGroupHeight() const;
    double getButtonPadding() const;
    double getSpacing() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderSmallSize() const;
    std::string renderMediumSize() const;
    std::string renderLargeSize() const;
};

/**
 * Vertical Button Group Component
 * Button group with vertical orientation
 */
struct VerticalButtonGroup {
    std::string id;
    std::vector<ButtonComponent> buttons;
    
    // Group properties
    ButtonVariant variant;
    ButtonColor color;
    ButtonSize size;
    bool disabled;
    bool full_width;
    
    // Vertical properties
    std::string background_color;
    std::string border_color;
    std::string shadow_level;
    std::string spacing_level;
    double vertical_spacing;
    
    VerticalButtonGroup() : variant(ButtonVariant::OUTLINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
                            disabled(false), full_width(false), vertical_spacing(8.0) {}
    VerticalButtonGroup(const std::string& group_id)
        : id(group_id), variant(ButtonVariant::OUTLINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
          disabled(false), full_width(false), vertical_spacing(8.0) {}
    
    // Button management
    void addButton(const ButtonComponent& button);
    void removeButton(const std::string& button_id);
    void clearButtons();
    std::vector<ButtonComponent> getButtons() const;
    
    // Group properties
    void setVariant(ButtonVariant group_variant);
    void setColor(ButtonColor group_color);
    void setSize(ButtonSize group_size);
    void setDisabled(bool is_disabled);
    void setFullWidth(bool is_full_width);
    ButtonVariant getVariant() const;
    ButtonColor getColor() const;
    ButtonSize getSize() const;
    bool isDisabled() const;
    bool isFullWidth() const;
    
    // Vertical properties
    void setVerticalSpacing(double spacing);
    double getVerticalSpacing() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderVerticalLayout() const;
};

/**
 * Button Group with Colors Component
 * Button group with different colors
 */
struct ButtonGroupWithColors {
    std::string id;
    std::vector<ButtonComponent> buttons;
    
    // Group properties
    ButtonVariant variant;
    ButtonColor color;
    ButtonSize size;
    std::string orientation;
    bool disabled;
    bool full_width;
    
    // Color properties
    std::string background_color;
    std::string border_color;
    std::string text_color;
    std::string shadow_level;
    
    ButtonGroupWithColors() : variant(ButtonVariant::OUTLINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
                              orientation("horizontal"), disabled(false), full_width(false) {}
    ButtonGroupWithColors(const std::string& group_id)
        : id(group_id), variant(ButtonVariant::OUTLINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
          orientation("horizontal"), disabled(false), full_width(false) {}
    
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
    
    // Color properties
    std::string getBackgroundColor() const;
    std::string getBorderColor() const;
    std::string getTextColor() const;
    std::string getShadowLevel() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderPrimaryColor() const;
    std::string renderSecondaryColor() const;
    std::string renderSuccessColor() const;
    std::string renderErrorColor() const;
    std::string renderWarningColor() const;
    std::string renderInfoColor() const;
};

/**
 * Button Factory
 * Factory for creating different types of buttons
 */
class ButtonFactory {
public:
    // Basic button creation
    static ButtonComponent createButton(const std::string& id, const std::string& text = "");
    static ContainedButton createContainedButton(const std::string& id, const std::string& text, ButtonColor color = ButtonColor::PRIMARY);
    static TextButton createTextButton(const std::string& id, const std::string& text, ButtonColor color = ButtonColor::PRIMARY);
    static OutlinedButton createOutlinedButton(const std::string& id, const std::string& text, ButtonColor color = ButtonColor::PRIMARY);
    static IconButton createIconButton(const std::string& id, const std::string& icon_name, ButtonColor color = ButtonColor::DEFAULT);
    static ButtonWithIcon createButtonWithIcon(const std::string& id, const std::string& text);
    static CustomIconButton createCustomIconButton(const std::string& id, const std::string& icon_name, ButtonColor color = ButtonColor::DEFAULT);
    static FloatingActionButton createFloatingActionButton(const std::string& id, const std::string& icon_name, ButtonColor color = ButtonColor::PRIMARY);
    static ButtonGroup createButtonGroup(const std::string& id);
    static SplitButton createSplitButton(const std::string& id, const ButtonComponent& main_button);
    
    // Advanced button creation
    static ButtonWithIconAndLabel createButtonWithIconAndLabel(const std::string& id, const std::string& text);
    static ButtonWithSizes createButtonWithSizes(const std::string& id, const std::string& text);
    static ButtonWithColors createButtonWithColors(const std::string& id, const std::string& text);
    static CustomizedButton createCustomizedButton(const std::string& id, const std::string& text);
    static FloatingActionButtonWithVariants createFloatingActionButtonWithVariants(const std::string& id, const std::string& icon_name, ButtonColor color = ButtonColor::PRIMARY);
    static FloatingActionButtonWithSizes createFloatingActionButtonWithSizes(const std::string& id, const std::string& icon_name, ButtonColor color = ButtonColor::PRIMARY);
    static ButtonGroupWithVariants createButtonGroupWithVariants(const std::string& id);
    static ButtonGroupWithSizes createButtonGroupWithSizes(const std::string& id);
    static VerticalButtonGroup createVerticalButtonGroup(const std::string& id);
    static ButtonGroupWithColors createButtonGroupWithColors(const std::string& id);
    
    // Added: Missing MUI features
    static IconButtonWithColors createIconButtonWithColors(const std::string& id, const std::string& icon_name, ButtonColor color = ButtonColor::DEFAULT);
    static IconButtonWithSizes createIconButtonWithSizes(const std::string& id, const std::string& icon_name, ButtonColor color = ButtonColor::DEFAULT);
    static FloatingActionButtonWithColor createFloatingActionButtonWithColor(const std::string& id, const std::string& icon_name, ButtonColor color = ButtonColor::PRIMARY);
    static BasicButtonGroup createBasicButtonGroup(const std::string& id);
    
    // Predefined variants
    static ButtonComponent createTextButton(const std::string& id, const std::string& text);
    static ButtonComponent createOutlinedButton(const std::string& id, const std::string& text);
    static ButtonComponent createContainedButton(const std::string& id, const std::string& text);
    static ButtonComponent createYorkshireButton(const std::string& id, const std::string& text);
    
    // Predefined colors
    static ButtonComponent createPrimaryButton(const std::string& id, const std::string& text);
    static ButtonComponent createSecondaryButton(const std::string& id, const std::string& text);
    static ButtonComponent createSuccessButton(const std::string& id, const std::string& text);
    static ButtonComponent createErrorButton(const std::string& id, const std::string& text);
    static ButtonComponent createWarningButton(const std::string& id, const std::string& text);
    static ButtonComponent createInfoButton(const std::string& id, const std::string& text);
    
    // Yorkshire Champion buttons
    static ButtonComponent createYorkshireContainedButton(const std::string& id, const std::string& text);
    static ButtonComponent createYorkshireTextButton(const std::string& id, const std::string& text);
    static ButtonComponent createYorkshireOutlinedButton(const std::string& id, const std::string& text);
    static ButtonComponent createYorkshireIconButton(const std::string& id, const std::string& icon_name);
    static ButtonComponent createYorkshireFloatingActionButton(const std::string& id, const std::string& icon_name);
    
    // Added: MUI-specific factory methods
    static IconButton createIconButtonWithColor(const std::string& id, const std::string& icon_name, ButtonColor color);
    static IconButton createIconButtonWithSize(const std::string& id, const std::string& icon_name, ButtonSize size);
    static FloatingActionButton createFloatingActionButtonWithVariant(const std::string& id, const std::string& icon_name, const std::string& variant);
    static FloatingActionButton createFloatingActionButtonWithSize(const std::string& id, const std::string& icon_name, ButtonSize size);
    static ButtonGroup createButtonGroupWithVariant(const std::string& id, ButtonVariant variant);
    static ButtonGroup createButtonGroupWithSize(const std::string& id, ButtonSize size);
    static ButtonGroup createVerticalButtonGroup(const std::string& id);
    static ButtonGroup createSplitButtonGroup(const std::string& id, const ButtonComponent& main_button);
    static ButtonGroup createButtonGroupWithColor(const std::string& id, ButtonColor color);
};

/**
 * Button Utilities
 * Utility functions for button operations
 */
namespace Utils {
    // Variant utilities
    std::string getVariantCSS(ButtonVariant variant);
    std::string getVariantHTML(ButtonVariant variant);
    std::string getVariantJSON(ButtonVariant variant);
    
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
    std::string getSizeHTML(ButtonSize size);
    std::string getSizeJSON(ButtonSize size);
    
    // Icon utilities
    std::string getDefaultButtonIcon();
    std::string getYorkshireButtonIcon();
    std::string getIconCSS(const std::string& icon_name, double size);
    std::string getIconHTML(const std::string& icon_name, double size);
    std::string getIconJSON(const std::string& icon_name, double size);
    
    // Position utilities
    std::string getIconPositionCSS(IconPosition position);
    std::string getIconPositionHTML(IconPosition position);
    std::string getIconPositionJSON(IconPosition position);
    
    // State utilities
    std::string getStateCSS(ButtonState state);
    std::string getStateHTML(ButtonState state);
    std::string getStateJSON(ButtonState state);
    
    // Validation utilities
    bool isValidButtonId(const std::string& id);
    bool isValidButtonComponent(const ButtonComponent& button);
    bool isValidButtonVariant(ButtonVariant variant);
    bool isValidButtonColor(ButtonColor color);
    bool isValidButtonSize(ButtonSize size);
    bool isValidIconName(const std::string& icon_name);
    bool isValidIconPosition(IconPosition position);
    
    // Platform utilities
    std::string detectPlatform();
    bool isPlatformSupported(const std::string& platform);
    std::string getPlatformButtonFormat(const std::string& platform);
    
    // Rendering utilities
    std::string generateButtonCSS(const std::string& variant, const std::string& color, const std::string& size);
    std::string generateButtonHTML(const std::string& variant, const std::string& color, const std::string& text);
    std::string generateButtonJSON(const std::string& variant, const std::string& color, const std::string& text);
}

/**
 * Button Examples
 * Example implementations for different button types
 */
namespace Examples {
    // Basic button examples
    ContainedButton createContainedButtonExample();
    TextButton createTextButtonExample();
    OutlinedButton createOutlinedButtonExample();
    IconButton createIconButtonExample();
    ButtonWithIcon createButtonWithIconExample();
    CustomIconButton createCustomIconButtonExample();
    FloatingActionButton createFloatingActionButtonExample();
    
    // Advanced button examples
    ButtonWithIconAndLabel createButtonWithIconAndLabelExample();
    ButtonWithSizes createButtonWithSizesExample();
    ButtonWithColors createButtonWithColorsExample();
    CustomizedButton createCustomizedButtonExample();
    FloatingActionButtonWithVariants createFloatingActionButtonWithVariantsExample();
    FloatingActionButtonWithSizes createFloatingActionButtonWithSizesExample();
    
    // Button group examples
    ButtonGroup createButtonGroupExample();
    ButtonGroupWithVariants createButtonGroupWithVariantsExample();
    ButtonGroupWithSizes createButtonGroupWithSizesExample();
    VerticalButtonGroup createVerticalButtonGroupExample();
    ButtonGroupWithColors createButtonGroupWithColorsExample();
    SplitButton createSplitButtonExample();
    
    // Yorkshire Champion examples
    ButtonComponent createYorkshireContainedButtonExample();
    ButtonComponent createYorkshireTextButtonExample();
    ButtonComponent createYorkshireOutlinedButtonExample();
    ButtonComponent createYorkshireIconButtonExample();
    ButtonComponent createYorkshireFloatingActionButtonExample();
    
    // Added: Missing MUI feature examples
    IconButtonWithColors createIconButtonWithColorsExample();
    IconButtonWithSizes createIconButtonWithSizesExample();
    FloatingActionButtonWithColor createFloatingActionButtonWithColorExample();
    BasicButtonGroup createBasicButtonGroupExample();
    
    // Added: MUI-specific examples
    IconButton createIconButtonWithColorExample();
    IconButton createIconButtonWithSizeExample();
    FloatingActionButton createFloatingActionButtonWithVariantExample();
    FloatingActionButton createFloatingActionButtonWithSizeExample();
    ButtonGroup createButtonGroupWithVariantExample();
    ButtonGroup createButtonGroupWithSizeExample();
    ButtonGroup createVerticalButtonGroupExample();
    ButtonGroup createSplitButtonGroupExample();
    ButtonGroup createButtonGroupWithColorExample();
}

/**
 * Icon Button with Colors Component
 * Icon button with different colors using color prop
 */
struct IconButtonWithColors {
    std::string id;
    std::string icon_name;
    ButtonColor color;
    ButtonSize size;
    
    // Icon properties
    bool disabled;
    double icon_size;
    double font_size;
    std::string icon_color;
    std::string background_color;
    
    IconButtonWithColors() : color(ButtonColor::DEFAULT), size(ButtonSize::MEDIUM), disabled(false), icon_size(24.0), font_size(24.0) {}
    IconButtonWithColors(const std::string& button_id, const std::string& icon_name, ButtonColor button_color = ButtonColor::DEFAULT)
        : id(button_id), icon_name(icon_name), color(button_color), size(ButtonSize::MEDIUM), disabled(false), icon_size(24.0), font_size(24.0) {}
    
    // Icon management
    void setIcon(const std::string& icon_name);
    void setIconSize(double size);
    void setFontSize(double size);
    std::string getIconName() const;
    double getIconSize() const;
    double getFontSize() const;
    
    // Color management
    void setColor(ButtonColor button_color);
    ButtonColor getColor() const;
    std::string getColorString() const;
    std::string getIconColor() const;
    std::string getBackgroundColor() const;
    
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
    std::string renderPrimaryColor() const;
    std::string renderSecondaryColor() const;
    std::string renderSuccessColor() const;
    std::string renderErrorColor() const;
    std::string renderWarningColor() const;
    std::string renderInfoColor() const;
};

/**
 * Icon Button with Sizes Component
 * Icon button with different sizes using size prop
 */
struct IconButtonWithSizes {
    std::string id;
    std::string icon_name;
    ButtonColor color;
    ButtonSize size;
    
    // Icon properties
    bool disabled;
    double icon_size;
    double font_size;
    
    IconButtonWithSizes() : color(ButtonColor::DEFAULT), size(ButtonSize::MEDIUM), disabled(false), icon_size(24.0), font_size(24.0) {}
    IconButtonWithSizes(const std::string& button_id, const std::string& icon_name, ButtonColor button_color = ButtonColor::DEFAULT)
        : id(button_id), icon_name(icon_name), color(button_color), size(ButtonSize::MEDIUM), disabled(false), icon_size(24.0), font_size(24.0) {}
    
    // Icon management
    void setIcon(const std::string& icon_name);
    void setIconSize(double size);
    void setFontSize(double size);
    std::string getIconName() const;
    double getIconSize() const;
    double getFontSize() const;
    
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
    std::string renderSmallSize() const;
    std::string renderMediumSize() const;
    std::string renderLargeSize() const;
};

/**
 * Floating Action Button with Color Component
 * FAB with different colors using color prop
 */
struct FloatingActionButtonWithColor {
    std::string id;
    std::string icon_name;
    ButtonColor color;
    ButtonSize size;
    
    // FAB properties
    bool disabled;
    bool extended;
    std::string extended_text;
    std::string variant;
    std::string background_color;
    std::string icon_color;
    std::string shadow_level;
    
    FloatingActionButtonWithColor() : color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM), disabled(false), extended(false), variant("circular") {}
    FloatingActionButtonWithColor(const std::string& button_id, const std::string& icon_name, ButtonColor button_color = ButtonColor::PRIMARY)
        : id(button_id), icon_name(icon_name), color(button_color), size(ButtonSize::MEDIUM), disabled(false), extended(false), variant("circular") {}
    
    // Icon management
    void setIcon(const std::string& icon_name);
    std::string getIconName() const;
    
    // Color management
    void setColor(ButtonColor button_color);
    ButtonColor getColor() const;
    std::string getColorString() const;
    std::string getBackgroundColor() const;
    std::string getIconColor() const;
    
    // Size management
    void setSize(ButtonSize button_size);
    ButtonSize getSize() const;
    std::string getSizeString() const;
    
    // Variant management
    void setVariant(const std::string& fab_variant);
    void setExtended(bool is_extended, const std::string& text = "");
    void setExtendedText(const std::string& text);
    std::string getVariant() const;
    bool isExtended() const;
    std::string getExtendedText() const;
    
    // Interactive management
    void setDisabled(bool is_disabled);
    bool isDisabled() const;
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderPrimaryColor() const;
    std::string renderSecondaryColor() const;
    std::string renderSuccessColor() const;
    std::string renderErrorColor() const;
    std::string renderWarningColor() const;
    std::string renderInfoColor() const;
};

/**
 * Basic Button Group Component
 * Button group with variant prop for button groups
 */
struct BasicButtonGroup {
    std::string id;
    std::vector<ButtonComponent> buttons;
    
    // Group properties
    ButtonVariant variant;
    ButtonColor color;
    ButtonSize size;
    std::string orientation;
    bool disabled;
    bool full_width;
    
    // Styling properties
    std::string background_color;
    std::string border_color;
    std::string shadow_level;
    std::string spacing_level;
    
    BasicButtonGroup() : variant(ButtonVariant::OUTLINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
                         orientation("horizontal"), disabled(false), full_width(false) {}
    BasicButtonGroup(const std::string& group_id)
        : id(group_id), variant(ButtonVariant::OUTLINED), color(ButtonColor::PRIMARY), size(ButtonSize::MEDIUM),
          orientation("horizontal"), disabled(false), full_width(false) {}
    
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
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderOutlinedVariant() const;
    std::string renderContainedVariant() const;
    std::string renderTextVariant() const;
};

} // namespace Button
} // namespace Components
} // namespace MedusaTheme 