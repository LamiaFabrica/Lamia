/*
 * MEDUSA ACCORDION IMPLEMENTATION - C++ Foundation
 * Complete accordion implementation with Basic, Controlled, Customized, and Additional Actions
 * Yorkshire Champion themed accordion implementation
 * 
 * Deep Links for Codebase Tracing:
 * - Component Header: src/theme/components/accordion/medusa_accordion_component.hpp
 * - Foundation Systems: src/theme/core/foundation/
 * - Theme Engine: src/theme/core/medusa_theme_engine.hpp
 */

#pragma once

#include "medusa_accordion_component.hpp"
#include <functional>
#include <variant>

namespace MedusaTheme {
namespace Components {
namespace Accordion {

/**
 * Accordion Summary Component
 * Represents the clickable header of an accordion item
 */
struct AccordionSummary {
    std::string id;
    std::string content;
    std::string icon_name;
    std::string icon_expanded_name;
    std::string icon_collapsed_name;
    
    // Summary properties
    bool expanded;
    bool disabled;
    bool focused;
    bool hovered;
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    // Event handlers
    std::function<void()> onClick;
    std::function<void()> onFocus;
    std::function<void()> onBlur;
    std::function<void()> onMouseEnter;
    std::function<void()> onMouseLeave;
    
    AccordionSummary() : expanded(false), disabled(false), focused(false), hovered(false) {}
    AccordionSummary(const std::string& summary_id, const std::string& summary_content)
        : id(summary_id), content(summary_content), expanded(false), disabled(false), focused(false), hovered(false) {}
    
    // Event handling
    void handleClick();
    void handleFocus();
    void handleBlur();
    void handleMouseEnter();
    void handleMouseLeave();
    
    // State management
    void setExpanded(bool is_expanded);
    void setDisabled(bool is_disabled);
    void setFocused(bool is_focused);
    void setHovered(bool is_hovered);
    
    // Property access
    std::string getProperty(const std::string& property_name) const;
    void setProperty(const std::string& property_name, const std::string& value);
    
    // Icon management
    std::string getCurrentIcon() const;
    void setIcon(const std::string& icon);
    void setExpandedIcon(const std::string& icon);
    void setCollapsedIcon(const std::string& icon);
};

/**
 * Accordion Details Component
 * Represents the expandable content of an accordion item
 */
struct AccordionDetails {
    std::string id;
    std::string content;
    
    // Details properties
    bool visible;
    bool animating;
    double animation_progress;
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    // Animation properties
    AccordionAnimationType animation_type;
    double animation_duration;
    std::string animation_easing;
    
    AccordionDetails() : visible(false), animating(false), animation_progress(0.0),
                         animation_type(AccordionAnimationType::SLIDE_DOWN),
                         animation_duration(0.3), animation_easing("ease-in-out") {}
    
    AccordionDetails(const std::string& details_id, const std::string& details_content)
        : id(details_id), content(details_content), visible(false), animating(false), animation_progress(0.0),
          animation_type(AccordionAnimationType::SLIDE_DOWN), animation_duration(0.3), animation_easing("ease-in-out") {}
    
    // Visibility management
    void show();
    void hide();
    void toggle();
    void setVisible(bool is_visible);
    
    // Animation management
    void startAnimation();
    void stopAnimation();
    void updateAnimationProgress(double progress);
    bool isAnimating() const;
    
    // Property access
    std::string getProperty(const std::string& property_name) const;
    void setProperty(const std::string& property_name, const std::string& value);
    
    // Animation utilities
    std::string getAnimationCSS() const;
    std::string getAnimationKeyframes() const;
    std::string getPlatformAnimation(const std::string& platform) const;
};

/**
 * Additional Action Component
 * Represents additional actions within accordion summary
 */
struct AdditionalAction {
    std::string id;
    std::string type;  // "checkbox", "button", "icon", "custom"
    std::string content;
    std::string icon_name;
    
    // Action properties
    bool checked;  // For checkboxes
    bool disabled;
    bool focused;
    bool hovered;
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    // Event handlers
    std::function<void()> onClick;
    std::function<void(bool)> onChange;  // For checkboxes
    std::function<void()> onFocus;
    std::function<void()> onBlur;
    
    AdditionalAction() : checked(false), disabled(false), focused(false), hovered(false) {}
    AdditionalAction(const std::string& action_id, const std::string& action_type, const std::string& action_content)
        : id(action_id), type(action_type), content(action_content), checked(false), disabled(false), focused(false), hovered(false) {}
    
    // Event handling
    void handleClick();
    void handleChange(bool new_value);
    void handleFocus();
    void handleBlur();
    
    // State management
    void setChecked(bool is_checked);
    void setDisabled(bool is_disabled);
    void setFocused(bool is_focused);
    void setHovered(bool is_hovered);
    
    // Property access
    std::string getProperty(const std::string& property_name) const;
    void setProperty(const std::string& property_name, const std::string& value);
    
    // Type-specific utilities
    bool isCheckbox() const;
    bool isButton() const;
    bool isIcon() const;
    bool isCustom() const;
};

/**
 * Basic Accordion Component
 * Simple accordion without state management
 */
struct BasicAccordion {
    std::string id;
    std::vector<AccordionItem> items;
    AccordionVariant variant;
    
    // Basic properties
    bool allow_multiple;
    bool allow_none;
    
    BasicAccordion() : variant(AccordionVariant::DEFAULT), allow_multiple(false), allow_none(true) {}
    BasicAccordion(const std::string& accordion_id)
        : id(accordion_id), variant(AccordionVariant::DEFAULT), allow_multiple(false), allow_none(true) {}
    
    // Item management
    void addItem(const AccordionItem& item);
    void removeItem(const std::string& item_id);
    void clearItems();
    
    // Basic operations
    void expandItem(const std::string& item_id);
    void collapseItem(const std::string& item_id);
    void toggleItem(const std::string& item_id);
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Controlled Accordion Component
 * Accordion with managed expanded state
 */
struct ControlledAccordion {
    std::string id;
    std::vector<AccordionItem> items;
    AccordionVariant variant;
    
    // Controlled properties
    std::vector<std::string> expanded_items;
    bool allow_multiple;
    bool allow_none;
    
    // State management callbacks
    std::function<void(const std::string&)> onItemExpand;
    std::function<void(const std::string&)> onItemCollapse;
    std::function<void(const std::string&)> onItemToggle;
    std::function<void(const std::vector<std::string>&)> onExpandedChange;
    
    ControlledAccordion() : variant(AccordionVariant::DEFAULT), allow_multiple(false), allow_none(true) {}
    ControlledAccordion(const std::string& accordion_id)
        : id(accordion_id), variant(AccordionVariant::DEFAULT), allow_multiple(false), allow_none(true) {}
    
    // Item management
    void addItem(const AccordionItem& item);
    void removeItem(const std::string& item_id);
    void clearItems();
    
    // State management
    void setExpandedItems(const std::vector<std::string>& items);
    std::vector<std::string> getExpandedItems() const;
    bool isItemExpanded(const std::string& item_id) const;
    
    // Controlled operations
    void expandItem(const std::string& item_id);
    void collapseItem(const std::string& item_id);
    void toggleItem(const std::string& item_id);
    void expandAll();
    void collapseAll();
    
    // Callback management
    void setOnItemExpand(std::function<void(const std::string&)> callback);
    void setOnItemCollapse(std::function<void(const std::string&)> callback);
    void setOnItemToggle(std::function<void(const std::string&)> callback);
    void setOnExpandedChange(std::function<void(const std::vector<std::string>&)> callback);
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Customized Accordion Component
 * Accordion with custom styling and behavior
 */
struct CustomizedAccordion {
    std::string id;
    std::vector<AccordionItem> items;
    AccordionVariant variant;
    
    // Custom styling
    std::map<std::string, std::string> custom_styles;
    std::map<std::string, std::string> custom_classes;
    std::map<std::string, std::string> custom_attributes;
    
    // Custom behavior
    std::function<bool(const std::string&)> canExpand;
    std::function<void(const std::string&)> onBeforeExpand;
    std::function<void(const std::string&)> onAfterExpand;
    std::function<void(const std::string&)> onBeforeCollapse;
    std::function<void(const std::string&)> onAfterCollapse;
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    CustomizedAccordion() : variant(AccordionVariant::DEFAULT) {}
    CustomizedAccordion(const std::string& accordion_id)
        : id(accordion_id), variant(AccordionVariant::DEFAULT) {}
    
    // Item management
    void addItem(const AccordionItem& item);
    void removeItem(const std::string& item_id);
    void clearItems();
    
    // Custom styling
    void setCustomStyle(const std::string& property, const std::string& value);
    void setCustomClass(const std::string& element, const std::string& class_name);
    void setCustomAttribute(const std::string& element, const std::string& attribute, const std::string& value);
    
    // Custom behavior
    void setCanExpand(std::function<bool(const std::string&)> callback);
    void setOnBeforeExpand(std::function<void(const std::string&)> callback);
    void setOnAfterExpand(std::function<void(const std::string&)> callback);
    void setOnBeforeCollapse(std::function<void(const std::string&)> callback);
    void setOnAfterCollapse(std::function<void(const std::string&)> callback);
    
    // Custom operations
    bool canExpandItem(const std::string& item_id) const;
    void expandItem(const std::string& item_id);
    void collapseItem(const std::string& item_id);
    void toggleItem(const std::string& item_id);
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderCustomStyles() const;
};

/**
 * Accordion with Additional Actions
 * Accordion that supports additional actions in summary
 */
struct AccordionWithActions {
    std::string id;
    std::vector<AccordionItem> items;
    AccordionVariant variant;
    
    // Additional actions mapping
    std::map<std::string, std::vector<AdditionalAction>> item_actions;
    
    // Action event handlers
    std::function<void(const std::string&, const std::string&)> onActionClick;
    std::function<void(const std::string&, const std::string&, bool)> onActionChange;
    
    AccordionWithActions() : variant(AccordionVariant::DEFAULT) {}
    AccordionWithActions(const std::string& accordion_id)
        : id(accordion_id), variant(AccordionVariant::DEFAULT) {}
    
    // Item management
    void addItem(const AccordionItem& item);
    void removeItem(const std::string& item_id);
    void clearItems();
    
    // Action management
    void addAction(const std::string& item_id, const AdditionalAction& action);
    void removeAction(const std::string& item_id, const std::string& action_id);
    void clearActions(const std::string& item_id);
    std::vector<AdditionalAction> getActions(const std::string& item_id) const;
    
    // Action event handling
    void handleActionClick(const std::string& item_id, const std::string& action_id);
    void handleActionChange(const std::string& item_id, const std::string& action_id, bool new_value);
    
    // Event propagation control
    void stopEventPropagation(const std::string& event_type);
    bool shouldPropagateEvent(const std::string& event_type) const;
    
    // Callback management
    void setOnActionClick(std::function<void(const std::string&, const std::string&)> callback);
    void setOnActionChange(std::function<void(const std::string&, const std::string&, bool)> callback);
    
    // Basic operations
    void expandItem(const std::string& item_id);
    void collapseItem(const std::string& item_id);
    void toggleItem(const std::string& item_id);
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderActions(const std::string& item_id) const;
};

/**
 * Accordion Factory
 * Factory for creating different types of accordions
 */
class AccordionFactory {
public:
    // Basic accordion creation
    static BasicAccordion createBasicAccordion(const std::string& id, const std::vector<AccordionItem>& items = {});
    
    // Controlled accordion creation
    static ControlledAccordion createControlledAccordion(const std::string& id, const std::vector<AccordionItem>& items = {});
    
    // Customized accordion creation
    static CustomizedAccordion createCustomizedAccordion(const std::string& id, const std::vector<AccordionItem>& items = {});
    
    // Accordion with actions creation
    static AccordionWithActions createAccordionWithActions(const std::string& id, const std::vector<AccordionItem>& items = {});
    
    // Predefined accordion templates
    static BasicAccordion createFAQAccordion(const std::string& id);
    static ControlledAccordion createSettingsAccordion(const std::string& id);
    static CustomizedAccordion createYorkshireAccordion(const std::string& id);
    static AccordionWithActions createMultiActionAccordion(const std::string& id);
};

/**
 * Accordion Utilities
 * Utility functions for accordion operations
 */
namespace Utils {
    // Event propagation utilities
    void stopEventPropagation(const std::string& event_type);
    bool shouldPropagateEvent(const std::string& event_type);
    
    // Action utilities
    AdditionalAction createCheckboxAction(const std::string& id, const std::string& label);
    AdditionalAction createButtonAction(const std::string& id, const std::string& label);
    AdditionalAction createIconAction(const std::string& id, const std::string& icon_name);
    AdditionalAction createCustomAction(const std::string& id, const std::string& content);
    
    // Validation utilities
    bool isValidAccordionId(const std::string& id);
    bool isValidActionId(const std::string& id);
    bool isValidEventType(const std::string& event_type);
    
    // Rendering utilities
    std::string generateAccordionCSS(const std::string& variant);
    std::string generateActionCSS(const std::string& action_type);
    std::string generateEventHandlers(const std::string& accordion_id);
}

} // namespace Accordion
} // namespace Components
} // namespace MedusaTheme 