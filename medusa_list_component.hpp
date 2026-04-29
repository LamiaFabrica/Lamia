/*
 * MEDUSA LIST COMPONENT - YORKSHIRE CHAMPION GOLD STANDARD
 * Revolutionary C++ list component with Materio theme integration
 * Converted from @mui/material List with TRUE TRIFORCE integration
 * 
 * ENIGMA Scale: 0.315 (Lightspeed Performance)
 * Cross-Platform: Windows, MacOS, Linux, Web, Mobile, Console
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <chrono>
#include <variant>

// Foundation system includes
#include "../../../core/foundation/colors/medusa_color_system.hpp"
#include "../../../core/foundation/typography/medusa_typography_system.hpp"
#include "../../../core/foundation/shadows/medusa_shadow_system.hpp"
#include "../../../core/foundation/spacing/medusa_spacing_system.hpp"
#include "../../../core/foundation/icons/iconify/medusa_iconify_system.hpp"
#include "../../../core/medusa_theme_engine.hpp"

namespace MedusaTheme {
namespace Components {
namespace List {

// Forward declarations
class ListComponent;
class ListItem;
class ListItemButton;
class ListItemIcon;
class ListItemText;
class ListItemAvatar;
class ListItemSecondaryAction;
class ListSubheader;
class ListDivider;

// List component variants
enum class ListVariant {
    STANDARD,      // Standard list variant
    OUTLINED,      // List with border
    CONTAINED,     // List with background
    NAVIGATION,    // Navigation list variant
    SELECTION      // Selection list variant
};

// List item alignment
enum class ListAlign {
    FLEX_START,    // Align items at the start
    CENTER,        // Center align items
    FLEX_END       // Align items at the end
};

// List density
enum class ListDensity {
    COMFORTABLE,   // Default spacing
    COMPACT,       // Reduced spacing
    EXPANDED       // Increased spacing
};

// List item button variants
enum class ListItemButtonVariant {
    TEXT,          // Text button
    CONTAINED,     // Contained button
    OUTLINED       // Outlined button
};

// List configuration
struct ListConfig {
    ListVariant variant = ListVariant::STANDARD;
    ListAlign align_items = ListAlign::CENTER;
    ListDensity density = ListDensity::COMFORTABLE;
    bool dense = false;
    bool disable_padding = false;
    bool disable_gutters = false;
    std::string subheader;
    
    // Styling
    std::string background_color = "surface";
    std::string text_color = "on-surface";
    int border_radius = 4;
    int elevation = 0;
    
    // Yorkshire Champion theme
    std::string theme_variant = "materio";
    bool material_design_compliance = true;
};

// List item configuration
struct ListItemConfig {
    bool button = false;
    bool selected = false;
    bool disabled = false;
    bool divider = false;
    bool disable_gutters = false;
    bool disable_padding = false;
    bool auto_focus = false;
    ListAlign align_items = ListAlign::CENTER;
    
    // Nested list
    bool has_nested = false;
    int nested_level = 0;
    bool nested_expanded = false;
    
    // Interactive states
    bool hover = false;
    bool focused = false;
    bool active = false;
    
    // Styling
    std::string background_color;
    std::string selected_background_color = "primary-container";
    std::string hover_background_color = "surface-variant";
    
    // Events
    std::function<void()> on_click;
    std::function<void()> on_double_click;
    std::function<void(bool)> on_toggle_nested;
};

// List item text configuration
struct ListItemTextConfig {
    std::string primary;
    std::string secondary;
    bool disable_typography = false;
    bool inline_secondary = false;
    
    // Typography variants
    std::string primary_typography_variant = "body1";
    std::string secondary_typography_variant = "body2";
    
    // Colors
    std::string primary_color = "on-surface";
    std::string secondary_color = "on-surface-variant";
};

// List state management
struct ListState {
    std::vector<int> selected_indices;
    std::vector<int> expanded_indices;
    int focused_index = -1;
    int hover_index = -1;
    
    // Performance tracking
    std::chrono::high_resolution_clock::time_point last_render;
    std::chrono::microseconds total_render_time{0};
    int render_count = 0;
    
    // Scroll state
    int scroll_position = 0;
    int visible_start_index = 0;
    int visible_end_index = 0;
};

// Main List Component Class
class ListComponent {
private:
    ListConfig config_;
    ListState state_;
    std::vector<std::shared_ptr<ListItem>> items_;
    std::shared_ptr<ListSubheader> subheader_;
    
    // Theme integration
    std::shared_ptr<MedusaThemeEngine> theme_engine_;
    std::shared_ptr<Foundation::ColorSystem> color_system_;
    std::shared_ptr<Foundation::TypographySystem> typography_system_;
    std::shared_ptr<Foundation::ShadowSystem> shadow_system_;
    std::shared_ptr<Foundation::SpacingSystem> spacing_system_;
    std::shared_ptr<Foundation::IconifySystem> iconify_system_;
    
public:
    ListComponent();
    explicit ListComponent(const ListConfig& config);
    ~ListComponent() = default;
    
    // Factory methods for common list types
    static std::unique_ptr<ListComponent> createNavigationList();
    static std::unique_ptr<ListComponent> createSelectionList(bool multi_select = false);
    static std::unique_ptr<ListComponent> createActionList();
    static std::unique_ptr<ListComponent> createNestedList();
    static std::unique_ptr<ListComponent> createVirtualizedList(int item_count);
    
    // Item management
    void addItem(std::shared_ptr<ListItem> item);
    void insertItem(int index, std::shared_ptr<ListItem> item);
    void removeItem(int index);
    void clearItems();
    std::shared_ptr<ListItem> getItem(int index) const;
    size_t getItemCount() const { return items_.size(); }
    
    // Configuration
    void setConfig(const ListConfig& config) { config_ = config; }
    ListConfig getConfig() const { return config_; }
    void setVariant(ListVariant variant) { config_.variant = variant; }
    void setDensity(ListDensity density) { config_.density = density; }
    void setSubheader(const std::string& subheader);
    
    // Selection management
    void selectItem(int index);
    void deselectItem(int index);
    void toggleItemSelection(int index);
    void clearSelection();
    std::vector<int> getSelectedIndices() const { return state_.selected_indices; }
    bool isItemSelected(int index) const;
    
    // Nested list management
    void expandItem(int index);
    void collapseItem(int index);
    void toggleItemExpansion(int index);
    bool isItemExpanded(int index) const;
    
    // Rendering
    std::string renderHTML() const;
    std::string renderCSS() const;
    std::string renderJS() const;
    
    // Platform-specific rendering
    #ifdef PLATFORM_WINDOWS
    void renderWindows(HDC hdc, const RECT& rect) const;
    #endif
    
    #ifdef PLATFORM_CONSOLE
    void renderConsole() const;
    #endif
    
    // Performance metrics (Yorkshire Champion)
    std::chrono::microseconds getAverageRenderTime() const;
    int getTotalRenders() const { return state_.render_count; }
    double getCacheHitRatio() const;
    
    // Theme integration
    void setThemeEngine(std::shared_ptr<MedusaThemeEngine> theme_engine);
    void applyMaterioTheme();
    void applyYorkshireChampionTheme();
    
    // Serialization
    std::string toJSON() const;
    void fromJSON(const std::string& json);
    
private:
    // Internal helpers
    std::string generateListStructure() const;
    std::string generateListStyling() const;
    std::string generateItemHTML(const ListItem& item, int index) const;
    void updateRenderMetrics() const;
    void optimizeRenderPath() const;
};

// List Item Component
class ListItem {
private:
    ListItemConfig config_;
    std::shared_ptr<ListItemIcon> icon_;
    std::shared_ptr<ListItemAvatar> avatar_;
    std::shared_ptr<ListItemText> text_;
    std::shared_ptr<ListItemSecondaryAction> secondary_action_;
    std::vector<std::shared_ptr<ListItem>> nested_items_;
    
public:
    ListItem();
    explicit ListItem(const ListItemConfig& config);
    explicit ListItem(const std::string& primary_text);
    ListItem(const std::string& primary_text, const std::string& secondary_text);
    
    // Content management
    void setIcon(std::shared_ptr<ListItemIcon> icon) { icon_ = icon; }
    void setAvatar(std::shared_ptr<ListItemAvatar> avatar) { avatar_ = avatar; }
    void setText(std::shared_ptr<ListItemText> text) { text_ = text; }
    void setSecondaryAction(std::shared_ptr<ListItemSecondaryAction> action) { secondary_action_ = action; }
    
    // Nested items
    void addNestedItem(std::shared_ptr<ListItem> item);
    void clearNestedItems();
    std::vector<std::shared_ptr<ListItem>> getNestedItems() const { return nested_items_; }
    
    // Configuration
    void setConfig(const ListItemConfig& config) { config_ = config; }
    ListItemConfig getConfig() const { return config_; }
    void setButton(bool button) { config_.button = button; }
    void setSelected(bool selected) { config_.selected = selected; }
    void setDisabled(bool disabled) { config_.disabled = disabled; }
    
    // Event handling
    void setOnClick(std::function<void()> handler) { config_.on_click = handler; }
    void click();
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
};

// List Item Button Component
class ListItemButton {
private:
    ListItemButtonVariant variant_;
    bool selected_ = false;
    bool disabled_ = false;
    std::string href_;
    std::function<void()> on_click_;
    
public:
    ListItemButton();
    explicit ListItemButton(ListItemButtonVariant variant);
    
    void setVariant(ListItemButtonVariant variant) { variant_ = variant; }
    void setSelected(bool selected) { selected_ = selected; }
    void setDisabled(bool disabled) { disabled_ = disabled; }
    void setHref(const std::string& href) { href_ = href; }
    void setOnClick(std::function<void()> handler) { on_click_ = handler; }
    
    std::string render() const;
};

// List Item Icon Component
class ListItemIcon {
private:
    std::string icon_name_;
    std::string color_;
    int size_ = 24;
    
public:
    ListItemIcon();
    explicit ListItemIcon(const std::string& icon_name);
    ListItemIcon(const std::string& icon_name, const std::string& color);
    
    void setIcon(const std::string& icon_name) { icon_name_ = icon_name; }
    void setColor(const std::string& color) { color_ = color; }
    void setSize(int size) { size_ = size; }
    
    std::string render() const;
};

// List Item Text Component
class ListItemText {
private:
    ListItemTextConfig config_;
    
public:
    ListItemText();
    explicit ListItemText(const std::string& primary);
    ListItemText(const std::string& primary, const std::string& secondary);
    explicit ListItemText(const ListItemTextConfig& config);
    
    void setPrimary(const std::string& primary) { config_.primary = primary; }
    void setSecondary(const std::string& secondary) { config_.secondary = secondary; }
    void setConfig(const ListItemTextConfig& config) { config_ = config; }
    
    std::string render() const;
};

// List Item Avatar Component
class ListItemAvatar {
private:
    std::string src_;
    std::string alt_;
    std::string initials_;
    std::string background_color_;
    int size_ = 40;
    
public:
    ListItemAvatar();
    explicit ListItemAvatar(const std::string& src);
    ListItemAvatar(const std::string& src, const std::string& alt);
    
    void setSrc(const std::string& src) { src_ = src; }
    void setAlt(const std::string& alt) { alt_ = alt; }
    void setInitials(const std::string& initials) { initials_ = initials; }
    void setBackgroundColor(const std::string& color) { background_color_ = color; }
    void setSize(int size) { size_ = size; }
    
    std::string render() const;
};

// List Subheader Component
class ListSubheader {
private:
    std::string text_;
    std::string color_ = "on-surface-variant";
    bool disable_gutters_ = false;
    bool disable_sticky_ = false;
    bool inset_ = false;
    
public:
    ListSubheader();
    explicit ListSubheader(const std::string& text);
    
    void setText(const std::string& text) { text_ = text; }
    void setColor(const std::string& color) { color_ = color; }
    void setDisableGutters(bool disable) { disable_gutters_ = disable; }
    void setDisableSticky(bool disable) { disable_sticky_ = disable; }
    void setInset(bool inset) { inset_ = inset; }
    
    std::string render() const;
};

// Utility functions
namespace ListUtils {
    // List creation helpers
    std::unique_ptr<ListComponent> createFromData(const std::vector<std::string>& items);
    std::unique_ptr<ListComponent> createFromJSON(const std::string& json);
    
    // List item creation helpers
    std::shared_ptr<ListItem> createSimpleItem(const std::string& text);
    std::shared_ptr<ListItem> createIconItem(const std::string& icon, const std::string& text);
    std::shared_ptr<ListItem> createAvatarItem(const std::string& avatar, const std::string& primary, const std::string& secondary);
    
    // Performance optimization
    void optimizeListPerformance(ListComponent& list);
    void enableVirtualization(ListComponent& list, int viewport_height);
}

// Export macros for library compilation
#ifdef LIST_COMPONENT_LIBRARY
#define LIST_API __declspec(dllexport)
#else
#define LIST_API __declspec(dllimport)
#endif

// External C API for compatibility
extern "C" {
    LIST_API void* medusa_list_create();
    LIST_API void medusa_list_destroy(void* list);
    LIST_API void medusa_list_add_item(void* list, const char* text);
    LIST_API void medusa_list_select_item(void* list, int index);
    LIST_API const char* medusa_list_render_html(void* list);
}

} // namespace List
} // namespace Components
} // namespace MedusaTheme