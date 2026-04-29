/*
 * MEDUSA ALERT IMPLEMENTATION - C++ Foundation
 * Complete alert implementation with Basic, Outlined, Filled, and Action alerts
 * Yorkshire Champion themed alert implementation
 * 
 * Deep Links for Codebase Tracing:
 * - Component Header: src/theme/components/alert/medusa_alert_component.hpp
 * - Foundation Systems: src/theme/core/foundation/
 * - Theme Engine: src/theme/core/medusa_theme_engine.hpp
 */

#pragma once

#include "medusa_alert_component.hpp"
#include <functional>
#include <variant>

namespace MedusaTheme {
namespace Components {
namespace Alert {

/**
 * Basic Alert Component
 * Simple alert with severity prop for different colored alerts
 */
struct BasicAlert {
    std::string id;
    std::string message;
    AlertSeverity severity;
    
    // Basic properties
    bool show_icon;
    bool show_title;
    bool show_message;
    
    BasicAlert() : severity(AlertSeverity::INFO), show_icon(true), show_title(true), show_message(true) {}
    BasicAlert(const std::string& alert_id, const std::string& alert_message, AlertSeverity alert_severity = AlertSeverity::INFO)
        : id(alert_id), message(alert_message), severity(alert_severity), show_icon(true), show_title(true), show_message(true) {}
    
    // Severity management
    void setSeverity(AlertSeverity alert_severity);
    AlertSeverity getSeverity() const;
    std::string getSeverityString() const;
    
    // Display management
    void setShowIcon(bool show);
    void setShowTitle(bool show);
    void setShowMessage(bool show);
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Outlined Alert Component
 * Alert with variant='outlined' prop for outlined alerts
 */
struct OutlinedAlert {
    std::string id;
    std::string message;
    AlertSeverity severity;
    
    // Outlined properties
    bool show_icon;
    bool show_title;
    bool show_message;
    std::string border_style;
    std::string border_width;
    
    OutlinedAlert() : severity(AlertSeverity::INFO), show_icon(true), show_title(true), show_message(true),
                      border_style("solid"), border_width("1px") {}
    OutlinedAlert(const std::string& alert_id, const std::string& alert_message, AlertSeverity alert_severity = AlertSeverity::INFO)
        : id(alert_id), message(alert_message), severity(alert_severity), show_icon(true), show_title(true), show_message(true),
          border_style("solid"), border_width("1px") {}
    
    // Severity management
    void setSeverity(AlertSeverity alert_severity);
    AlertSeverity getSeverity() const;
    std::string getSeverityString() const;
    
    // Border management
    void setBorderStyle(const std::string& style);
    void setBorderWidth(const std::string& width);
    std::string getBorderStyle() const;
    std::string getBorderWidth() const;
    
    // Display management
    void setShowIcon(bool show);
    void setShowTitle(bool show);
    void setShowMessage(bool show);
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Filled Alert Component
 * Alert with variant='filled' prop for filled alerts
 */
struct FilledAlert {
    std::string id;
    std::string message;
    AlertSeverity severity;
    
    // Filled properties
    bool show_icon;
    bool show_title;
    bool show_message;
    double opacity;
    std::string fill_pattern;
    
    FilledAlert() : severity(AlertSeverity::INFO), show_icon(true), show_title(true), show_message(true),
                    opacity(1.0), fill_pattern("solid") {}
    FilledAlert(const std::string& alert_id, const std::string& alert_message, AlertSeverity alert_severity = AlertSeverity::INFO)
        : id(alert_id), message(alert_message), severity(alert_severity), show_icon(true), show_title(true), show_message(true),
          opacity(1.0), fill_pattern("solid") {}
    
    // Severity management
    void setSeverity(AlertSeverity alert_severity);
    AlertSeverity getSeverity() const;
    std::string getSeverityString() const;
    
    // Fill management
    void setOpacity(double alert_opacity);
    void setFillPattern(const std::string& pattern);
    double getOpacity() const;
    std::string getFillPattern() const;
    
    // Display management
    void setShowIcon(bool show);
    void setShowTitle(bool show);
    void setShowMessage(bool show);
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
};

/**
 * Alert with Action Component
 * Alert that supports action prop with buttons and onClose prop
 */
struct AlertWithAction {
    std::string id;
    std::string message;
    AlertSeverity severity;
    AlertVariant variant;
    
    // Action properties
    std::vector<AlertAction> actions;
    bool show_close_button;
    std::function<void()> onClose;
    
    // Display properties
    bool show_icon;
    bool show_title;
    bool show_message;
    
    AlertWithAction() : severity(AlertSeverity::INFO), variant(AlertVariant::STANDARD),
                        show_close_button(false), show_icon(true), show_title(true), show_message(true) {}
    AlertWithAction(const std::string& alert_id, const std::string& alert_message, AlertSeverity alert_severity = AlertSeverity::INFO)
        : id(alert_id), message(alert_message), severity(alert_severity), variant(AlertVariant::STANDARD),
          show_close_button(false), show_icon(true), show_title(true), show_message(true) {}
    
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
    
    // Action event handling
    void handleActionClick(const std::string& action_id);
    void handleClose();
    
    // Display management
    void setShowIcon(bool show);
    void setShowTitle(bool show);
    void setShowMessage(bool show);
    
    // Rendering
    std::string render() const;
    std::string renderCSS() const;
    std::string renderHTML() const;
    std::string renderActions() const;
    std::string renderCloseButton() const;
};

/**
 * Alert Factory
 * Factory for creating different types of alerts
 */
class AlertFactory {
public:
    // Basic alert creation
    static BasicAlert createBasicAlert(const std::string& id, const std::string& message, AlertSeverity severity = AlertSeverity::INFO);
    
    // Outlined alert creation
    static OutlinedAlert createOutlinedAlert(const std::string& id, const std::string& message, AlertSeverity severity = AlertSeverity::INFO);
    
    // Filled alert creation
    static FilledAlert createFilledAlert(const std::string& id, const std::string& message, AlertSeverity severity = AlertSeverity::INFO);
    
    // Alert with action creation
    static AlertWithAction createAlertWithAction(const std::string& id, const std::string& message, AlertSeverity severity = AlertSeverity::INFO);
    
    // Predefined alert templates
    static BasicAlert createErrorAlert(const std::string& id, const std::string& message);
    static BasicAlert createWarningAlert(const std::string& id, const std::string& message);
    static BasicAlert createInfoAlert(const std::string& id, const std::string& message);
    static BasicAlert createSuccessAlert(const std::string& id, const std::string& message);
    
    // Predefined outlined alerts
    static OutlinedAlert createOutlinedErrorAlert(const std::string& id, const std::string& message);
    static OutlinedAlert createOutlinedWarningAlert(const std::string& id, const std::string& message);
    static OutlinedAlert createOutlinedInfoAlert(const std::string& id, const std::string& message);
    static OutlinedAlert createOutlinedSuccessAlert(const std::string& id, const std::string& message);
    
    // Predefined filled alerts
    static FilledAlert createFilledErrorAlert(const std::string& id, const std::string& message);
    static FilledAlert createFilledWarningAlert(const std::string& id, const std::string& message);
    static FilledAlert createFilledInfoAlert(const std::string& id, const std::string& message);
    static FilledAlert createFilledSuccessAlert(const std::string& id, const std::string& message);
    
    // Yorkshire Champion alerts
    static BasicAlert createYorkshireErrorAlert(const std::string& id, const std::string& message);
    static BasicAlert createYorkshireWarningAlert(const std::string& id, const std::string& message);
    static BasicAlert createYorkshireInfoAlert(const std::string& id, const std::string& message);
    static BasicAlert createYorkshireSuccessAlert(const std::string& id, const std::string& message);
};

/**
 * Alert Utilities
 * Utility functions for alert operations
 */
namespace Utils {
    // Severity utilities
    std::string getSeverityColor(AlertSeverity severity);
    std::string getSeverityBackgroundColor(AlertSeverity severity);
    std::string getSeverityBorderColor(AlertSeverity severity);
    std::string getSeverityTextColor(AlertSeverity severity);
    std::string getSeverityIconColor(AlertSeverity severity);
    
    // Yorkshire Champion severity utilities
    std::string getYorkshireSeverityColor(AlertSeverity severity);
    std::string getYorkshireSeverityBackgroundColor(AlertSeverity severity);
    std::string getYorkshireSeverityBorderColor(AlertSeverity severity);
    std::string getYorkshireSeverityTextColor(AlertSeverity severity);
    std::string getYorkshireSeverityIconColor(AlertSeverity severity);
    
    // Icon utilities
    std::string getSeverityIcon(AlertSeverity severity);
    std::string getCloseIcon();
    std::string getYorkshireSeverityIcon(AlertSeverity severity);
    std::string getYorkshireCloseIcon();
    
    // Action utilities
    AlertAction createButtonAction(const std::string& id, const std::string& label);
    AlertAction createCloseAction(const std::string& id);
    AlertAction createCustomAction(const std::string& id, const std::string& type, const std::string& label);
    
    // Validation utilities
    bool isValidAlertId(const std::string& id);
    bool isValidAlertAction(const AlertAction& action);
    bool isValidSeverity(AlertSeverity severity);
    bool isValidVariant(AlertVariant variant);
    
    // Platform utilities
    std::string detectPlatform();
    bool isPlatformSupported(const std::string& platform);
    std::string getPlatformAlertFormat(const std::string& platform);
    
    // Rendering utilities
    std::string generateAlertCSS(const std::string& severity, const std::string& variant);
    std::string generateActionCSS(const std::string& action_type);
    std::string generateSeverityCSS(AlertSeverity severity);
    std::string generateVariantCSS(AlertVariant variant);
}

/**
 * Alert Examples
 * Example implementations for different alert types
 */
namespace Examples {
    // Basic alert examples
    BasicAlert createBasicErrorExample();
    BasicAlert createBasicWarningExample();
    BasicAlert createBasicInfoExample();
    BasicAlert createBasicSuccessExample();
    
    // Outlined alert examples
    OutlinedAlert createOutlinedErrorExample();
    OutlinedAlert createOutlinedWarningExample();
    OutlinedAlert createOutlinedInfoExample();
    OutlinedAlert createOutlinedSuccessExample();
    
    // Filled alert examples
    FilledAlert createFilledErrorExample();
    FilledAlert createFilledWarningExample();
    FilledAlert createFilledInfoExample();
    FilledAlert createFilledSuccessExample();
    
    // Action alert examples
    AlertWithAction createActionErrorExample();
    AlertWithAction createActionWarningExample();
    AlertWithAction createActionInfoExample();
    AlertWithAction createActionSuccessExample();
    
    // Yorkshire Champion examples
    BasicAlert createYorkshireErrorExample();
    BasicAlert createYorkshireWarningExample();
    BasicAlert createYorkshireInfoExample();
    BasicAlert createYorkshireSuccessExample();
}

} // namespace Alert
} // namespace Components
} // namespace MedusaTheme 