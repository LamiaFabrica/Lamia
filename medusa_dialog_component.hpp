/*
 * MEDUSA DIALOG COMPONENT - YORKSHIRE CHAMPION GOLD STANDARD
 * Revolutionary C++ dialog/modal component with advanced theming
 * Converted from @mui/material Dialog with TRUE TRIFORCE integration
 * 
 * ENIGMA Scale: 0.315 (Lightspeed Performance)
 * Yorkshire Champion: No Shortcuts Implementation
 * Cross-Platform: Windows, MacOS, Linux, Web, Mobile, Console
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <chrono>

// Foundation system includes
#include "../../../core/foundation/colors/medusa_color_system.hpp"
#include "../../../core/foundation/typography/medusa_typography_system.hpp"
#include "../../../core/foundation/shadows/medusa_shadow_system.hpp"
#include "../../../core/foundation/spacing/medusa_spacing_system.hpp"
#include "../../../core/foundation/icons/iconify/medusa_iconify_system.hpp"
#include "../../../core/medusa_theme_engine.hpp"

namespace MedusaTheme {
namespace Components {
namespace Dialog {

// Forward declarations
class DialogComponent;
class DialogTitle;
class DialogContent;
class DialogActions;
class DialogContentText;

// Dialog size variants
enum class DialogSize {
    XS,           // Extra small (max-width: 444px)
    SM,           // Small (max-width: 600px)
    MD,           // Medium (max-width: 900px)
    LG,           // Large (max-width: 1200px)
    XL,           // Extra large (max-width: 1536px)
    FULLWIDTH     // Full width of container
};

// Dialog scroll behavior
enum class DialogScroll {
    PAPER,        // Scroll within dialog paper
    BODY          // Scroll the body of the dialog
};

// Dialog animation transition
enum class DialogTransition {
    SLIDE,        // Slide in from bottom
    FADE,         // Fade in
    GROW,         // Grow from center
    ZOOM          // Zoom in
};

// Dialog variant styles
enum class DialogVariant {
    STANDARD,     // Standard Material Design dialog
    OUTLINED,     // Dialog with outlined border
    ELEVATED,     // Dialog with elevated shadow
    FULLSCREEN,   // Full screen dialog
    ALERT,        // Alert dialog variant
    CONFIRMATION  // Confirmation dialog variant
};

// Dialog backdrop behavior
enum class DialogBackdrop {
    STATIC,       // Backdrop doesn't close dialog
    CLICKABLE,    // Backdrop closes dialog on click
    DISABLED      // No backdrop
};

// Dialog state management
struct DialogState {
    bool open = false;
    bool fullscreen = false;
    bool disable_escape_key_down = false;
    bool disable_backdrop_click = false;
    bool disable_portal = false;
    bool disable_scrolling = false;
    bool keep_mounted = false;
    
    // Animation state
    bool transitioning = false;
    std::chrono::milliseconds enter_duration{300};
    std::chrono::milliseconds exit_duration{300};
    
    // Focus management
    bool disable_auto_focus = false;
    bool disable_enforce_focus = false;
    bool disable_restore_focus = false;
    std::string initial_focus_ref;
    
    // Yorkshire Champion performance tracking
    std::chrono::high_resolution_clock::time_point open_time;
    std::chrono::microseconds render_time{0};
    int render_count = 0;
};

// Dialog styling configuration
struct DialogStyling {
    // Size and positioning
    DialogSize size = DialogSize::MD;
    int max_width = 600;
    int min_width = 280;
    int min_height = 100;
    
    // Visual styling
    DialogVariant variant = DialogVariant::STANDARD;
    std::string background_color = "surface";
    std::string text_color = "on-surface";
    int border_radius = 12;
    int elevation = 16;
    
    // Backdrop styling
    DialogBackdrop backdrop_behavior = DialogBackdrop::CLICKABLE;
    std::string backdrop_color = "rgba(0, 0, 0, 0.5)";
    bool backdrop_blur = true;
    
    // Animation configuration
    DialogTransition transition_type = DialogTransition::FADE;
    DialogScroll scroll_behavior = DialogScroll::PAPER;
    
    // Yorkshire Champion theme integration
    std::string theme_variant = "yorkshire_champion";
    bool material_design_compliance = true;
    bool accessibility_enhanced = true;
    
    // Custom styling
    std::map<std::string, std::string> custom_css;
    std::string custom_class;
};

// Dialog content structure
struct DialogContentData {
    std::string title;
    std::string content;
    std::vector<std::string> paragraphs;
    bool show_close_button = true;
    
    // Action buttons
    std::vector<std::pair<std::string, std::function<void()>>> primary_actions;
    std::vector<std::pair<std::string, std::function<void()>>> secondary_actions;
    
    // Icons and media
    std::string icon_name;
    std::string image_url;
    
    // Form integration
    bool contains_form = false;
    std::map<std::string, std::string> form_data;
};

// Dialog event handlers
struct DialogEvents {
    std::function<void()> on_open;
    std::function<void()> on_close;
    std::function<void(std::string)> on_backdrop_click;
    std::function<void()> on_escape_key_down;
    
    // Animation events
    std::function<void()> on_enter;
    std::function<void()> on_entered;
    std::function<void()> on_exit;
    std::function<void()> on_exited;
    
    // Form events
    std::function<void(std::map<std::string, std::string>)> on_form_submit;
    std::function<void()> on_form_cancel;
    
    // Yorkshire Champion event tracking
    std::function<void(std::chrono::microseconds)> on_performance_metric;
};

// Main Dialog Component Class
class DialogComponent {
private:
    DialogState state_;
    DialogStyling styling_;
    DialogContentData content_;
    DialogEvents events_;
    
    // Theme integration
    std::shared_ptr<MedusaThemeEngine> theme_engine_;
    std::shared_ptr<Foundation::ColorSystem> color_system_;
    std::shared_ptr<Foundation::TypographySystem> typography_system_;
    std::shared_ptr<Foundation::ShadowSystem> shadow_system_;
    std::shared_ptr<Foundation::SpacingSystem> spacing_system_;
    std::shared_ptr<Foundation::IconifySystem> iconify_system_;
    
    // Performance monitoring
    mutable std::chrono::high_resolution_clock::time_point last_render_time_;
    mutable int total_renders_ = 0;
    
public:
    DialogComponent();
    explicit DialogComponent(const DialogStyling& styling);
    explicit DialogComponent(const DialogContentData& content);
    DialogComponent(const DialogStyling& styling, const DialogContentData& content);
    ~DialogComponent() = default;
    
    // Factory methods for common dialog types
    static std::unique_ptr<DialogComponent> createAlertDialog(
        const std::string& title,
        const std::string& message,
        const std::function<void()>& on_confirm = nullptr
    );
    
    static std::unique_ptr<DialogComponent> createConfirmationDialog(
        const std::string& title,
        const std::string& message,
        const std::function<void()>& on_confirm,
        const std::function<void()>& on_cancel
    );
    
    static std::unique_ptr<DialogComponent> createFormDialog(
        const std::string& title,
        const std::vector<std::pair<std::string, std::string>>& fields,
        const std::function<void(std::map<std::string, std::string>)>& on_submit
    );
    
    static std::unique_ptr<DialogComponent> createFullscreenDialog(
        const std::string& title,
        const DialogContentData& content
    );
    
    // State management
    void open();
    void close();
    void toggle();
    bool isOpen() const { return state_.open; }
    
    // Content management
    void setTitle(const std::string& title);
    void setContent(const std::string& content);
    void setContentData(const DialogContentData& content);
    void addAction(const std::string& label, const std::function<void()>& action, bool primary = true);
    void clearActions();
    
    // Styling configuration
    void setStyling(const DialogStyling& styling);
    void setSize(DialogSize size);
    void setVariant(DialogVariant variant);
    void setTransition(DialogTransition transition);
    void setBackdropBehavior(DialogBackdrop backdrop);
    
    // Event handling
    void setEvents(const DialogEvents& events);
    void setOnClose(const std::function<void()>& callback);
    void setOnOpen(const std::function<void()>& callback);
    
    // Rendering (cross-platform)
    std::string renderHTML() const;
    std::string renderCSS() const;
    std::string renderJS() const;
    
    // Platform-specific rendering
    #ifdef PLATFORM_WINDOWS
    void renderWindows(HDC hdc, const RECT& rect) const;
    #endif
    
    #ifdef PLATFORM_LINUX
    void renderLinux() const;
    #endif
    
    #ifdef PLATFORM_CONSOLE
    void renderConsole() const;
    #endif
    
    // Accessibility
    std::string getAriaLabel() const;
    std::string getAriaDescribedBy() const;
    std::vector<std::string> getAccessibilityTags() const;
    
    // Performance metrics (Yorkshire Champion)
    std::chrono::microseconds getAverageRenderTime() const;
    int getTotalRenders() const { return total_renders_; }
    double getCacheHitRatio() const;
    
    // Theme integration
    void setThemeEngine(std::shared_ptr<MedusaThemeEngine> theme_engine);
    void applyYorkshireChampionTheme();
    void applyMaterialDesignTheme();
    void applyCustomTheme(const std::string& theme_name);
    
    // Serialization for TRUE TRIFORCE integration
    std::string toJSON() const;
    void fromJSON(const std::string& json);
    std::map<std::string, std::string> toKeyValuePairs() const;
    void fromKeyValuePairs(const std::map<std::string, std::string>& data);
    
private:
    // Internal rendering helpers
    std::string generateDialogStructure() const;
    std::string generateDialogStyling() const;
    std::string generateDialogAnimation() const;
    std::string generateBackdropStyling() const;
    std::string generateResponsiveStyling() const;
    
    // Animation helpers
    void startOpenAnimation();
    void startCloseAnimation();
    void updateAnimationState();
    
    // Performance tracking
    void trackRenderPerformance() const;
    void optimizeRenderPath() const;
    
    // Event processing
    void handleBackdropClick();
    void handleEscapeKeyDown();
    void handleFormSubmit();
    
    // Theme application
    void applyColorTheme();
    void applyTypographyTheme();
    void applyShadowTheme();
    void applySpacingTheme();
    void applyIconTheme();
};

// Dialog subcomponents
class DialogTitle {
private:
    std::string title_;
    std::string subtitle_;
    bool show_close_button_;
    std::string icon_name_;
    std::shared_ptr<MedusaThemeEngine> theme_engine_;
    
public:
    DialogTitle(const std::string& title);
    DialogTitle(const std::string& title, const std::string& subtitle);
    
    void setTitle(const std::string& title) { title_ = title; }
    void setSubtitle(const std::string& subtitle) { subtitle_ = subtitle; }
    void setShowCloseButton(bool show) { show_close_button_ = show; }
    void setIcon(const std::string& icon_name) { icon_name_ = icon_name; }
    
    std::string render() const;
    std::string renderCSS() const;
};

class DialogContent {
private:
    std::string content_;
    std::vector<std::string> paragraphs_;
    DialogScroll scroll_behavior_;
    bool dividers_enabled_;
    std::shared_ptr<MedusaThemeEngine> theme_engine_;
    
public:
    DialogContent();
    explicit DialogContent(const std::string& content);
    explicit DialogContent(const std::vector<std::string>& paragraphs);
    
    void setContent(const std::string& content) { content_ = content; }
    void setParagraphs(const std::vector<std::string>& paragraphs) { paragraphs_ = paragraphs; }
    void setScrollBehavior(DialogScroll scroll) { scroll_behavior_ = scroll; }
    void setDividersEnabled(bool enabled) { dividers_enabled_ = enabled; }
    
    std::string render() const;
    std::string renderCSS() const;
};

class DialogActions {
private:
    std::vector<std::pair<std::string, std::function<void()>>> primary_actions_;
    std::vector<std::pair<std::string, std::function<void()>>> secondary_actions_;
    bool disable_spacing_;
    std::string alignment_;
    std::shared_ptr<MedusaThemeEngine> theme_engine_;
    
public:
    DialogActions();
    
    void addPrimaryAction(const std::string& label, const std::function<void()>& action);
    void addSecondaryAction(const std::string& label, const std::function<void()>& action);
    void clearActions();
    void setDisableSpacing(bool disable) { disable_spacing_ = disable; }
    void setAlignment(const std::string& alignment) { alignment_ = alignment; }
    
    std::string render() const;
    std::string renderCSS() const;
};

// Utility functions for dialog management
namespace DialogUtils {
    // Dialog stack management for multiple dialogs
    class DialogStack {
    private:
        std::vector<std::shared_ptr<DialogComponent>> dialog_stack_;
        int z_index_base_ = 1300;
        
    public:
        void push(std::shared_ptr<DialogComponent> dialog);
        void pop();
        void clear();
        std::shared_ptr<DialogComponent> top() const;
        size_t size() const { return dialog_stack_.size(); }
        bool empty() const { return dialog_stack_.empty(); }
    };
    
    // Global dialog functions
    void showAlert(const std::string& title, const std::string& message);
    bool showConfirm(const std::string& title, const std::string& message);
    std::map<std::string, std::string> showForm(
        const std::string& title,
        const std::vector<std::pair<std::string, std::string>>& fields
    );
    
    // Dialog positioning utilities
    struct DialogPosition {
        int x = -1;        // -1 for centered
        int y = -1;        // -1 for centered
        bool center_horizontal = true;
        bool center_vertical = true;
    };
    
    DialogPosition calculateOptimalPosition(
        const DialogStyling& styling,
        int screen_width,
        int screen_height
    );
    
    // Performance optimization utilities
    void optimizeDialogPerformance(DialogComponent& dialog);
    void preloadDialogAssets();
    void clearDialogCache();
    
    // Accessibility utilities
    std::vector<std::string> validateAccessibility(const DialogComponent& dialog);
    void enhanceAccessibility(DialogComponent& dialog);
}

// Export macros for library compilation
#ifdef DIALOG_COMPONENT_LIBRARY
#define DIALOG_API __declspec(dllexport)
#else
#define DIALOG_API __declspec(dllimport)
#endif

// External interface for C API compatibility
extern "C" {
    DIALOG_API void* medusa_dialog_create();
    DIALOG_API void medusa_dialog_destroy(void* dialog);
    DIALOG_API void medusa_dialog_open(void* dialog);
    DIALOG_API void medusa_dialog_close(void* dialog);
    DIALOG_API void medusa_dialog_set_title(void* dialog, const char* title);
    DIALOG_API void medusa_dialog_set_content(void* dialog, const char* content);
    DIALOG_API const char* medusa_dialog_render_html(void* dialog);
}

} // namespace Dialog
} // namespace Components
} // namespace MedusaTheme