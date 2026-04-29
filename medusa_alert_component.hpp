/*
 * MEDUSA ALERT COMPONENT - C++ Foundation
 * Cross-platform alert component with advanced theming
 * Yorkshire Champion themed alert system
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
namespace Alert {

/**
 * Alert Severity Enumeration
 * Different severity levels for alerts
 */
enum class AlertSeverity {
    ERROR,
    WARNING,
    INFO,
    SUCCESS
};

/**
 * Alert Variant Enumeration
 * Different visual variants for alerts
 */
enum class AlertVariant {
    STANDARD,
    OUTLINED,
    FILLED,
    YORKSHIRE_CHAMPION
};

/**
 * Alert State Enumeration
 * Different states for alert components
 */
enum class AlertState {
    VISIBLE,
    HIDDEN,
    DISMISSING,
    DISMISSED,
    LOADING,
    ERROR
};

/**
 * Alert Action Type
 * Different types of actions for alerts
 */
enum class AlertActionType {
    NONE,
    BUTTON,
    CLOSE,
    CUSTOM,
    MULTIPLE
};

/**
 * Alert Action Structure
 * Represents an action within an alert
 */
struct AlertAction {
    std::string id;
    std::string type;  // "button", "close", "custom"
    std::string label;
    std::string icon_name;
    
    // Action properties
    bool disabled;
    bool focused;
    bool hovered;
    bool primary;
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    // Event handlers
    std::function<void()> onClick;
    std::function<void()> onFocus;
    std::function<void()> onBlur;
    std::function<void()> onMouseEnter;
    std::function<void()> onMouseLeave;
    
    AlertAction() : disabled(false), focused(false), hovered(false), primary(false) {}
    AlertAction(const std::string& action_id, const std::string& action_type, const std::string& action_label)
        : id(action_id), type(action_type), label(action_label), disabled(false), focused(false), hovered(false), primary(false) {}
    
    // Event handling
    void handleClick();
    void handleFocus();
    void handleBlur();
    void handleMouseEnter();
    void handleMouseLeave();
    
    // State management
    void setDisabled(bool is_disabled);
    void setFocused(bool is_focused);
    void setHovered(bool is_hovered);
    void setPrimary(bool is_primary);
    
    // Property access
    std::string getProperty(const std::string& property_name) const;
    void setProperty(const std::string& property_name, const std::string& value);
    
    // Type-specific utilities
    bool isButton() const;
    bool isClose() const;
    bool isCustom() const;
};

/**
 * Alert Component Structure
 * Represents a complete alert component
 */
struct AlertComponent {
    std::string id;
    std::string title;
    std::string message;
    std::string icon_name;
    
    // Alert properties
    AlertSeverity severity;
    AlertVariant variant;
    AlertState state;
    
    // Display properties
    bool show_icon;
    bool show_title;
    bool show_message;
    bool auto_dismiss;
    double auto_dismiss_delay;
    
    // Styling properties
    std::string background_color;
    std::string border_color;
    std::string text_color;
    std::string icon_color;
    std::string shadow_level;
    std::string typography_variant;
    std::string spacing_level;
    
    // Actions
    std::vector<AlertAction> actions;
    bool show_close_button;
    std::function<void()> onClose;
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    AlertComponent() : severity(AlertSeverity::INFO), variant(AlertVariant::STANDARD), state(AlertState::VISIBLE),
                       show_icon(true), show_title(true), show_message(true), auto_dismiss(false), auto_dismiss_delay(5000.0),
                       show_close_button(false) {}
    
    AlertComponent(const std::string& alert_id, const std::string& alert_message, AlertSeverity alert_severity = AlertSeverity::INFO)
        : id(alert_id), message(alert_message), severity(alert_severity), variant(AlertVariant::STANDARD), state(AlertState::VISIBLE),
          show_icon(true), show_title(true), show_message(true), auto_dismiss(false), auto_dismiss_delay(5000.0),
          show_close_button(false) {}
    
    // State management
    void show();
    void hide();
    void dismiss();
    void setState(AlertState alert_state);
    bool isVisible() const;
    bool isDismissed() const;
    
    // Severity management
    void setSeverity(AlertSeverity alert_severity);
    AlertSeverity getSeverity() const;
    std::string getSeverityString() const;
    
    // Variant management
    void setVariant(AlertVariant alert_variant);
    AlertVariant getVariant() const;
    std::string getVariantString() const;
    
    // Action management
    void addAction(const AlertAction& action);
    void removeAction(const std::string& action_id);
    void clearActions();
    std::vector<AlertAction> getActions() const;
    void setCloseButton(bool show_close);
    void setOnClose(std::function<void()> close_callback);
    
    // Auto-dismiss management
    void setAutoDismiss(bool auto_dismiss_enabled, double delay = 5000.0);
    void startAutoDismiss();
    void stopAutoDismiss();
    
    // Property access
    std::string getProperty(const std::string& property_name) const;
    void setProperty(const std::string& property_name, const std::string& value);
    
    // Component manipulation
    AlertComponent withSeverity(AlertSeverity alert_severity) const;
    AlertComponent withVariant(AlertVariant alert_variant) const;
    AlertComponent withAction(const AlertAction& action) const;
    AlertComponent withCloseButton(bool show_close) const;
    AlertComponent withAutoDismiss(bool auto_dismiss_enabled, double delay = 5000.0) const;
};

/**
 * Alert Renderer
 * Handles alert rendering for different platforms
 */
class AlertRenderer {
private:
    std::map<std::string, std::string> render_cache_;
    std::mutex render_mutex_;
    
public:
    AlertRenderer();
    ~AlertRenderer() = default;
    
    // Component rendering
    std::string renderAlert(const AlertComponent& alert, const std::string& platform = "default");
    std::string renderAlertWithStyle(const AlertComponent& alert, const std::map<std::string, std::string>& styles);
    std::string renderAlertAction(const AlertAction& action, const std::string& platform = "default");
    
    // Platform-specific rendering
    std::string renderForWindows(const AlertComponent& alert);
    std::string renderForMacOS(const AlertComponent& alert);
    std::string renderForLinux(const AlertComponent& alert);
    std::string renderForWeb(const AlertComponent& alert);
    std::string renderForMobile(const AlertComponent& alert);
    std::string renderForConsole(const AlertComponent& alert);
    
    // Severity-specific rendering
    std::string renderErrorAlert(const AlertComponent& alert);
    std::string renderWarningAlert(const AlertComponent& alert);
    std::string renderInfoAlert(const AlertComponent& alert);
    std::string renderSuccessAlert(const AlertComponent& alert);
    
    // Variant-specific rendering
    std::string renderStandardVariant(const AlertComponent& alert);
    std::string renderOutlinedVariant(const AlertComponent& alert);
    std::string renderFilledVariant(const AlertComponent& alert);
    std::string renderYorkshireVariant(const AlertComponent& alert);
    
    // Format-specific rendering
    std::string renderAsCSS(const AlertComponent& alert);
    std::string renderAsHTML(const AlertComponent& alert);
    std::string renderAsJSON(const AlertComponent& alert);
    std::string renderAsXML(const AlertComponent& alert);
    
    // Action rendering
    std::string renderActions(const AlertComponent& alert);
    std::string renderCloseButton(const AlertComponent& alert);
    std::string renderCustomAction(const AlertAction& action);
    
    // Cache management
    void clearCache();
    size_t getCacheSize() const;
    double getCacheHitRate() const;
    
private:
    std::string generateAlertCSS(const AlertComponent& alert);
    std::string generateAlertHTML(const AlertComponent& alert);
    std::string generateAlertJSON(const AlertComponent& alert);
    std::string generateAlertXML(const AlertComponent& alert);
    std::string generatePlatformAlert(const AlertComponent& alert, const std::string& platform);
    std::string generateActionCSS(const AlertAction& action, const std::string& variant);
    std::string generateSeverityCSS(AlertSeverity severity);
};

/**
 * Alert Cache Manager
 * Manages alert caching and optimization
 */
class AlertCacheManager {
private:
    std::unordered_map<std::string, std::string> alert_cache_;
    std::unordered_map<std::string, AlertComponent> component_cache_;
    std::unordered_map<std::string, std::string> action_cache_;
    std::mutex cache_mutex_;
    
    size_t max_cache_size_ = 1000;
    size_t cache_hits_ = 0;
    size_t cache_misses_ = 0;
    
public:
    AlertCacheManager();
    ~AlertCacheManager() = default;
    
    // Cache operations
    void cacheAlert(const std::string& key, const std::string& rendered_alert);
    void cacheComponent(const std::string& key, const AlertComponent& component);
    void cacheAction(const std::string& key, const std::string& rendered_action);
    bool getCachedAlert(const std::string& key, std::string& rendered_alert);
    bool getCachedComponent(const std::string& key, AlertComponent& component);
    bool getCachedAction(const std::string& key, std::string& rendered_action);
    
    // Cache management
    void setMaxCacheSize(size_t size);
    size_t getCacheSize() const;
    void cleanupCache();
    void clearCache();
    
    // Statistics
    double getHitRate() const;
    size_t getAlertCacheSize() const;
    size_t getComponentCacheSize() const;
    size_t getActionCacheSize() const;
    
private:
    void evictOldestEntries();
    std::string generateCacheKey(const AlertComponent& alert, const std::string& platform);
    std::string generateActionCacheKey(const AlertAction& action, const std::string& platform);
};

/**
 * Alert System
 * Main system for managing alert components across all platforms
 */
class AlertSystem {
private:
    static std::unique_ptr<AlertSystem> instance_;
    static std::mutex instance_mutex_;
    
    // Foundation systems
    std::unique_ptr<Foundation::Colors::ColorSystem> color_system_;
    std::unique_ptr<Foundation::Typography::TypographySystem> typography_system_;
    std::unique_ptr<Foundation::Shadows::ShadowSystem> shadow_system_;
    std::unique_ptr<Foundation::Spacing::SpacingSystem> spacing_system_;
    std::unique_ptr<Foundation::Iconify::IconifySystem> iconify_system_;
    
    // Alert management
    std::unique_ptr<AlertRenderer> renderer_;
    std::unique_ptr<AlertCacheManager> cache_manager_;
    
    // Current state
    std::string current_platform_;
    std::string current_theme_;
    bool initialized_ = false;
    
public:
    AlertSystem();
    ~AlertSystem() = default;
    
    // Singleton access
    static AlertSystem& getInstance();
    static void initialize();
    static void shutdown();
    
    // System management
    bool initialize();
    void setPlatform(const std::string& platform);
    void setTheme(const std::string& theme);
    std::string getCurrentPlatform() const;
    std::string getCurrentTheme() const;
    
    // Component operations
    std::string renderAlert(const AlertComponent& alert);
    std::string renderAlertAction(const AlertAction& action);
    std::string renderAlertWithStyle(const AlertComponent& alert, const std::map<std::string, std::string>& styles);
    
    // Component creation
    AlertComponent createAlert(const std::string& id, const std::string& message, AlertSeverity severity = AlertSeverity::INFO);
    AlertAction createAlertAction(const std::string& id, const std::string& type, const std::string& label);
    
    // Predefined alerts
    AlertComponent createErrorAlert(const std::string& id, const std::string& message);
    AlertComponent createWarningAlert(const std::string& id, const std::string& message);
    AlertComponent createInfoAlert(const std::string& id, const std::string& message);
    AlertComponent createSuccessAlert(const std::string& id, const std::string& message);
    
    // Predefined variants
    AlertComponent createOutlinedAlert(const std::string& id, const std::string& message, AlertSeverity severity = AlertSeverity::INFO);
    AlertComponent createFilledAlert(const std::string& id, const std::string& message, AlertSeverity severity = AlertSeverity::INFO);
    AlertComponent createYorkshireAlert(const std::string& id, const std::string& message, AlertSeverity severity = AlertSeverity::INFO);
    
    // Platform-specific operations
    std::string getPlatformSpecificAlert(const AlertComponent& alert) const;
    AlertComponent getPlatformAdjustedAlert(const AlertComponent& alert) const;
    
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
    void createDefaultAlerts();
    void setupAlertDefaults();
    void validateAlert(const AlertComponent& alert);
};

/**
 * Alert Utilities
 * Helper functions for alert operations
 */
namespace Utils {
    // Alert utilities
    std::string alertSeverityToString(AlertSeverity severity);
    AlertSeverity stringToAlertSeverity(const std::string& severity);
    std::string alertVariantToString(AlertVariant variant);
    AlertVariant stringToAlertVariant(const std::string& variant);
    std::string alertStateToString(AlertState state);
    AlertState stringToAlertState(const std::string& state);
    std::string alertActionTypeToString(AlertActionType type);
    AlertActionType stringToAlertActionType(const std::string& type);
    
    // Validation utilities
    bool isValidAlertId(const std::string& id);
    bool isValidAlertAction(const AlertAction& action);
    bool isValidAlertComponent(const AlertComponent& alert);
    bool isValidSeverity(AlertSeverity severity);
    bool isValidVariant(AlertVariant variant);
    
    // Platform utilities
    std::string detectPlatform();
    bool isPlatformSupported(const std::string& platform);
    std::string getPlatformAlertFormat(const std::string& platform);
    
    // Icon utilities
    std::string getSeverityIcon(AlertSeverity severity);
    std::string getCloseIcon();
    std::string getYorkshireSeverityIcon(AlertSeverity severity);
    std::string getYorkshireCloseIcon();
    
    // Color utilities
    std::string getSeverityBackgroundColor(AlertSeverity severity);
    std::string getSeverityBorderColor(AlertSeverity severity);
    std::string getSeverityTextColor(AlertSeverity severity);
    std::string getSeverityIconColor(AlertSeverity severity);
    std::string getYorkshireSeverityBackgroundColor(AlertSeverity severity);
    std::string getYorkshireSeverityBorderColor(AlertSeverity severity);
    std::string getYorkshireSeverityTextColor(AlertSeverity severity);
    std::string getYorkshireSeverityIconColor(AlertSeverity severity);
    
    // Action utilities
    AlertAction createButtonAction(const std::string& id, const std::string& label);
    AlertAction createCloseAction(const std::string& id);
    AlertAction createCustomAction(const std::string& id, const std::string& type, const std::string& label);
}

} // namespace Alert
} // namespace Components
} // namespace MedusaTheme 