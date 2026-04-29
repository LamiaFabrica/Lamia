/*
 * MEDUSA MENU COMPONENT - YORKSHIRE CHAMPION GOLD STANDARD
 * Revolutionary C++ menu/dropdown component with Materio theme integration
 * Converted from @mui/material Menu with TRUE TRIFORCE integration
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
#include <optional>

// Foundation system includes
#include "../../../core/foundation/colors/medusa_color_system.hpp"
#include "../../../core/foundation/typography/medusa_typography_system.hpp"
#include "../../../core/foundation/shadows/medusa_shadow_system.hpp"
#include "../../../core/foundation/spacing/medusa_spacing_system.hpp"
#include "../../../core/foundation/icons/iconify/medusa_iconify_system.hpp"
#include "../../../core/medusa_theme_engine.hpp"

namespace MedusaTheme {
namespace Components {
namespace Menu {

// Forward declarations
class MenuComponent;
class MenuItem;
class MenuList;
class MenuDivider;
class MenuGroup;
class SubMenu;

// Menu anchor positions
enum class MenuAnchorOrigin {
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    CENTER_LEFT,
    CENTER_CENTER,
    CENTER_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT
};

// Menu transform origin
enum class MenuTransformOrigin {
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    CENTER_LEFT,
    CENTER_CENTER,
    CENTER_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT
};

// Menu variant styles
enum class MenuVariant {
    STANDARD,      // Standard menu
    SELECTEDMENU,  // Selected menu variant
    OUTLINED,      // Outlined menu
    FILLED,        // Filled menu
    CONTEXT,       // Context menu
    DROPDOWN       // Dropdown menu
};

// Menu transition types
enum class MenuTransition {
    GROW,          // Grow from anchor
    FADE,          // Fade in/out
    SLIDE,         // Slide from direction
    COLLAPSE       // Collapse animation
};

// Menu item types
enum class MenuItemType {
    STANDARD,      // Standard menu item
    CHECKBOX,      // Checkbox menu item
    RADIO,         // Radio menu item
    SWITCH,        // Switch menu item
    HEADER,        // Header item (non-clickable)
    DIVIDER        // Divider item
};

// Menu configuration
struct MenuConfig {
    MenuVariant variant = MenuVariant::STANDARD;
    MenuAnchorOrigin anchor_origin = MenuAnchorOrigin::TOP_LEFT;
    MenuTransformOrigin transform_origin = MenuTransformOrigin::TOP_LEFT;
    MenuTransition transition_type = MenuTransition::GROW;
    
    // Positioning
    int anchor_el_x = 0;
    int anchor_el_y = 0;
    int margin_threshold = 16;
    bool auto_focus = true;
    bool disable_auto_focus_item = false;
    bool disable_enforce_focus = false;
    bool disable_restore_focus = false;
    
    // Behavior
    bool keep_mounted = false;
    bool disable_portal = false;
    bool disable_scrolling = false;
    bool close_on_select = true;
    bool close_on_escape = true;
    
    // Styling
    int elevation = 8;
    int max_height = 400;
    int min_width = 200;
    std::string background_color = "surface";
    std::string text_color = "on-surface";
    int border_radius = 4;
    
    // Animation
    std::chrono::milliseconds enter_duration{225};
    std::chrono::milliseconds exit_duration{195};
    
    // Theme integration
    std::string theme_variant = "materio";
    bool material_design_compliance = true;
};

// Menu item configuration
struct MenuItemConfig {
    MenuItemType type = MenuItemType::STANDARD;
    std::string label;
    std::string value;
    std::string icon;
    std::string shortcut;
    bool selected = false;
    bool disabled = false;
    bool dense = false;
    bool divider = false;
    
    // Checkbox/Radio/Switch specific
    bool checked = false;
    std::string group_name; // For radio items
    
    // Nested menu
    bool has_submenu = false;
    std::shared_ptr<SubMenu> submenu;
    
    // Styling
    std::string custom_class;
    std::map<std::string, std::string> custom_styles;
    
    // Events
    std::function<void()> on_click;
    std::function<void(bool)> on_toggle; // For checkbox/switch
    std::function<void(std::string)> on_select; // For radio
};

// Menu state management
struct MenuState {
    bool open = false;
    int focused_index = -1;
    int hover_index = -1;
    std::vector<int> selected_indices;
    std::map<std::string, bool> checkbox_states;
    std::map<std::string, std::string> radio_selections;
    
    // Position tracking
    int menu_x = 0;
    int menu_y = 0;
    int actual_width = 0;
    int actual_height = 0;
    
    // Performance metrics
    std::chrono::high_resolution_clock::time_point open_time;
    std::chrono::microseconds total_render_time{0};
    int render_count = 0;
};

// Main Menu Component Class
class MenuComponent {
private:
    MenuConfig config_;
    MenuState state_;
    std::vector<std::shared_ptr<MenuItem>> items_;
    std::shared_ptr<MenuList> menu_list_;
    
    // Theme integration
    std::shared_ptr<MedusaThemeEngine> theme_engine_;
    std::shared_ptr<Foundation::ColorSystem> color_system_;
    std::shared_ptr<Foundation::TypographySystem> typography_system_;
    std::shared_ptr<Foundation::ShadowSystem> shadow_system_;
    std::shared_ptr<Foundation::SpacingSystem> spacing_system_;
    std::shared_ptr<Foundation::IconifySystem> iconify_system_;
    
    // Anchor element reference
    std::string anchor_element_id_;
    
public:
    MenuComponent();
    explicit MenuComponent(const MenuConfig& config);
    ~MenuComponent() = default;
    
    // Factory methods for common menu types
    static std::unique_ptr<MenuComponent> createContextMenu();
    static std::unique_ptr<MenuComponent> createDropdownMenu();
    static std::unique_ptr<MenuComponent> createSelectMenu();
    static std::unique_ptr<MenuComponent> createOptionsMenu();
    static std::unique_ptr<MenuComponent> createAccountMenu();
    
    // Menu control
    void open();
    void open(int anchor_x, int anchor_y);
    void openAt(const std::string& anchor_element_id);
    void close();
    void toggle();
    bool isOpen() const { return state_.open; }
    
    // Item management
    void addItem(std::shared_ptr<MenuItem> item);
    void insertItem(int index, std::shared_ptr<MenuItem> item);
    void removeItem(int index);
    void clearItems();
    std::shared_ptr<MenuItem> getItem(int index) const;
    size_t getItemCount() const { return items_.size(); }
    
    // Add convenience methods
    void addMenuItem(const std::string& label, std::function<void()> on_click = nullptr);
    void addCheckboxItem(const std::string& label, bool checked, std::function<void(bool)> on_toggle = nullptr);
    void addRadioItem(const std::string& label, const std::string& group, bool selected, std::function<void(std::string)> on_select = nullptr);
    void addDivider();
    void addHeader(const std::string& text);
    void addSubMenu(const std::string& label, std::shared_ptr<SubMenu> submenu);
    
    // Configuration
    void setConfig(const MenuConfig& config) { config_ = config; }
    MenuConfig getConfig() const { return config_; }
    void setVariant(MenuVariant variant) { config_.variant = variant; }
    void setAnchorOrigin(MenuAnchorOrigin origin) { config_.anchor_origin = origin; }
    void setTransformOrigin(MenuTransformOrigin origin) { config_.transform_origin = origin; }
    
    // Selection management
    void selectItem(int index);
    void deselectItem(int index);
    std::vector<int> getSelectedIndices() const { return state_.selected_indices; }
    std::string getSelectedValue() const;
    std::vector<std::string> getSelectedValues() const;
    
    // Checkbox/Radio state management
    void setCheckboxState(const std::string& item_id, bool checked);
    bool getCheckboxState(const std::string& item_id) const;
    void setRadioSelection(const std::string& group, const std::string& value);
    std::string getRadioSelection(const std::string& group) const;
    
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
    
    // Event handling
    void handleKeyDown(int key_code);
    void handleMouseMove(int x, int y);
    void handleMouseClick(int x, int y);
    
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
    void calculateMenuPosition();
    void adjustPositionToViewport();
    std::string generateMenuStructure() const;
    std::string generateMenuStyling() const;
    std::string generateMenuAnimation() const;
    void updateFocusedItem(int delta);
    void activateFocusedItem();
    void updateRenderMetrics() const;
};

// Menu Item Component
class MenuItem {
private:
    MenuItemConfig config_;
    std::vector<std::shared_ptr<MenuItem>> children_; // For nested items
    
public:
    MenuItem();
    explicit MenuItem(const MenuItemConfig& config);
    MenuItem(const std::string& label, std::function<void()> on_click = nullptr);
    
    // Configuration
    void setConfig(const MenuItemConfig& config) { config_ = config; }
    MenuItemConfig getConfig() const { return config_; }
    void setLabel(const std::string& label) { config_.label = label; }
    void setValue(const std::string& value) { config_.value = value; }
    void setIcon(const std::string& icon) { config_.icon = icon; }
    void setShortcut(const std::string& shortcut) { config_.shortcut = shortcut; }
    void setDisabled(bool disabled) { config_.disabled = disabled; }
    void setSelected(bool selected) { config_.selected = selected; }
    
    // Type-specific setters
    void setType(MenuItemType type) { config_.type = type; }
    void setChecked(bool checked) { config_.checked = checked; }
    void setGroupName(const std::string& group) { config_.group_name = group; }
    
    // Event handling
    void setOnClick(std::function<void()> handler) { config_.on_click = handler; }
    void setOnToggle(std::function<void(bool)> handler) { config_.on_toggle = handler; }
    void click();
    void toggle();
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
};

// Menu List Component
class MenuList {
private:
    std::vector<std::shared_ptr<MenuItem>> items_;
    bool auto_focus_ = true;
    bool auto_focus_item_ = true;
    bool disable_list_wrap_ = false;
    std::string variant_ = "selectedMenu";
    
public:
    MenuList();
    
    void addItem(std::shared_ptr<MenuItem> item) { items_.push_back(item); }
    void clearItems() { items_.clear(); }
    std::vector<std::shared_ptr<MenuItem>> getItems() const { return items_; }
    
    void setAutoFocus(bool auto_focus) { auto_focus_ = auto_focus; }
    void setAutoFocusItem(bool auto_focus) { auto_focus_item_ = auto_focus; }
    void setDisableListWrap(bool disable) { disable_list_wrap_ = disable; }
    void setVariant(const std::string& variant) { variant_ = variant; }
    
    std::string render() const;
};

// SubMenu Component
class SubMenu {
private:
    std::vector<std::shared_ptr<MenuItem>> items_;
    MenuConfig config_;
    bool open_ = false;
    std::string parent_id_;
    
public:
    SubMenu();
    explicit SubMenu(const std::string& parent_id);
    
    void addItem(std::shared_ptr<MenuItem> item) { items_.push_back(item); }
    void clearItems() { items_.clear(); }
    void setConfig(const MenuConfig& config) { config_ = config; }
    
    void open() { open_ = true; }
    void close() { open_ = false; }
    bool isOpen() const { return open_; }
    
    std::string render() const;
};

// Menu utilities
namespace MenuUtils {
    // Menu creation helpers
    std::unique_ptr<MenuComponent> createFromData(const std::vector<std::string>& items);
    std::unique_ptr<MenuComponent> createFromJSON(const std::string& json);
    
    // Position calculation
    struct MenuPosition {
        int x;
        int y;
        bool flip_horizontal = false;
        bool flip_vertical = false;
    };
    
    MenuPosition calculateOptimalPosition(
        const MenuConfig& config,
        int anchor_x, int anchor_y,
        int menu_width, int menu_height,
        int viewport_width, int viewport_height
    );
    
    // Keyboard navigation
    int getNextFocusableIndex(const std::vector<std::shared_ptr<MenuItem>>& items, int current_index, int direction);
    
    // Performance optimization
    void optimizeMenuPerformance(MenuComponent& menu);
    void preloadMenuAssets();
}

// Export macros for library compilation
#ifdef MENU_COMPONENT_LIBRARY
#define MENU_API __declspec(dllexport)
#else
#define MENU_API __declspec(dllimport)
#endif

// External C API for compatibility
extern "C" {
    MENU_API void* medusa_menu_create();
    MENU_API void medusa_menu_destroy(void* menu);
    MENU_API void medusa_menu_add_item(void* menu, const char* label);
    MENU_API void medusa_menu_open(void* menu, int x, int y);
    MENU_API void medusa_menu_close(void* menu);
    MENU_API const char* medusa_menu_render_html(void* menu);
}

} // namespace Menu
} // namespace Components
} // namespace MedusaTheme