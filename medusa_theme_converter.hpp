/*
 * MEDUSA THEME CONVERTER
 * Transforms MUI/Materio themes to Medusa-Theme components
 * Yorkshire Champion themed Material Design system
 */

#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <functional>

namespace MedusaServer {

struct MUIThemeConfig {
    struct Palette {
        std::string primary_main = "#1976d2";
        std::string primary_light = "#42a5f5";
        std::string primary_dark = "#1565c0";
        std::string secondary_main = "#dc004e";
        std::string secondary_light = "#ff5983";
        std::string secondary_dark = "#9a0036";
        std::string background_default = "#fafafa";
        std::string background_paper = "#ffffff";
        std::string text_primary = "rgba(0, 0, 0, 0.87)";
        std::string text_secondary = "rgba(0, 0, 0, 0.6)";
    } palette;
    
    struct Typography {
        std::string font_family = "\"Roboto\", \"Helvetica\", \"Arial\", sans-serif";
        double h1_size = 6.0; // rem
        double h2_size = 3.75;
        double h3_size = 3.0;
        double h4_size = 2.125;
        double h5_size = 1.5;
        double h6_size = 1.25;
        double body1_size = 1.0;
        double body2_size = 0.875;
    } typography;
    
    struct Spacing {
        int base_unit = 8; // px
    } spacing;
    
    struct Shape {
        int border_radius = 4; // px
    } shape;
};

struct MedusaThemeComponent {
    std::string component_name;
    std::string css_class;
    std::map<std::string, std::string> css_properties;
    std::map<std::string, std::string> css_variables;
    std::vector<std::string> child_selectors;
    std::map<std::string, std::map<std::string, std::string>> states; // hover, active, focus, disabled
};

class MedusaThemeConverter {
private:
    MUIThemeConfig mui_theme_;
    std::map<std::string, MedusaThemeComponent> medusa_components_;
    std::map<std::string, std::string> yorkshire_color_mappings_;
    std::string output_directory_;
    
public:
    MedusaThemeConverter(const std::string& output_dir = "/home/medusa/themes/medusa-components");
    ~MedusaThemeConverter() = default;
    
    // Configuration
    bool loadMUITheme(const MUIThemeConfig& theme);
    bool loadMaterioTheme(const std::string& materio_css_path);
    void setupYorkshireColorMappings();
    
    // Component conversion
    bool convertButton();
    bool convertCard();
    bool convertTextField();
    bool convertAppBar();
    bool convertDrawer();
    bool convertDialog();
    bool convertDataGrid();
    bool convertChip();
    bool convertFab();
    bool convertPaper();
    bool convertTypography();
    bool convertTabs();
    bool convertMenu();
    bool convertAccordion();
    bool convertProgress();
    
    // Theme generation
    std::string generateMedusaCSS();
    std::string generateMedusaVariables();
    std::string generateComponentCSS(const std::string& component_name);
    bool exportComponents();
    bool exportThemeBundle();
    
    // Utility methods
    std::string convertMUIColorToYorkshire(const std::string& mui_color);
    std::string convertMUISpacing(int spacing_multiplier);
    std::string convertMUITypography(const std::string& variant);
    std::string convertMUIElevation(int elevation);
    std::string generateResponsiveBreakpoints();
    
    // Validation
    bool validateConversion();
    std::vector<std::string> getConversionWarnings();
    std::vector<std::string> getComponentList();
    
private:
    // Internal conversion helpers
    MedusaThemeComponent createButtonComponent();
    MedusaThemeComponent createCardComponent();
    MedusaThemeComponent createTextFieldComponent();
    MedusaThemeComponent createAppBarComponent();
    MedusaThemeComponent createDrawerComponent();
    
    // CSS generation helpers
    std::string generateCSSRule(const std::string& selector, const std::map<std::string, std::string>& properties);
    std::string generateStateRules(const std::string& base_selector, const std::map<std::string, std::map<std::string, std::string>>& states);
    std::string generateVariableDeclarations(const std::map<std::string, std::string>& variables);
    
    // Color conversion helpers
    std::string hexToRgb(const std::string& hex);
    std::string rgbToHsl(int r, int g, int b);
    std::string lighten(const std::string& color, double amount);
    std::string darken(const std::string& color, double amount);
    
    // File operations
    bool writeComponentFile(const std::string& component_name, const std::string& css_content);
    bool createDirectoryStructure();
};

// Materio to Medusa component mappings
inline std::map<std::string, std::string> getMaterioToMedusaMapping() {
    return {
        // Buttons
        {"btn", "medusa-button"},
        {"btn-primary", "medusa-button-primary"},
        {"btn-secondary", "medusa-button-secondary"},
        {"btn-outline", "medusa-button-outline"},
        {"btn-fab", "medusa-fab"},
        
        // Cards
        {"card", "medusa-card"},
        {"card-header", "medusa-card-header"},
        {"card-body", "medusa-card-body"},
        {"card-footer", "medusa-card-footer"},
        
        // Forms
        {"form-control", "medusa-textfield"},
        {"form-label", "medusa-label"},
        {"form-check", "medusa-checkbox"},
        {"form-switch", "medusa-switch"},
        
        // Navigation
        {"navbar", "medusa-appbar"},
        {"nav-link", "medusa-nav-link"},
        {"sidebar", "medusa-drawer"},
        {"breadcrumb", "medusa-breadcrumb"},
        
        // Layout
        {"container", "medusa-container"},
        {"row", "medusa-row"},
        {"col", "medusa-column"},
        {"paper", "medusa-paper"},
        
        // Data Display
        {"table", "medusa-table"},
        {"badge", "medusa-badge"},
        {"chip", "medusa-chip"},
        {"avatar", "medusa-avatar"},
        
        // Feedback
        {"modal", "medusa-dialog"},
        {"alert", "medusa-alert"},
        {"toast", "medusa-snackbar"},
        {"progress", "medusa-progress"},
        
        // Surfaces
        {"accordion", "medusa-accordion"},
        {"tab-pane", "medusa-tab-panel"},
        {"dropdown", "medusa-menu"}
    };
}

// Yorkshire Champion color scheme for conversion
inline std::map<std::string, std::string> getYorkshireColorScheme() {
    return {
        // Primary Yorkshire colors
        {"primary", "#1a472a"},      // Yorkshire Green
        {"primary-light", "#2d5a3d"},
        {"primary-dark", "#0f2e1a"},
        
        {"secondary", "#8b4513"},    // Yorkshire Brown
        {"secondary-light", "#a0522d"},
        {"secondary-dark", "#654321"},
        
        {"accent", "#ffd700"},       // Yorkshire Gold
        {"accent-light", "#ffed4e"},
        {"accent-dark", "#b8860b"},
        
        // Neutral colors
        {"background", "#f5f5dc"},   // Yorkshire Cream
        {"surface", "#ffffff"},
        {"surface-variant", "#f8f8f8"},
        
        // Text colors
        {"text-primary", "#1a1a1a"},
        {"text-secondary", "#666666"},
        {"text-disabled", "#999999"},
        
        // State colors
        {"error", "#d32f2f"},
        {"warning", "#ff9800"},
        {"info", "#2196f3"},
        {"success", "#4caf50"},
        
        // Dark mode variants
        {"dark-primary", "#2d5a3d"},
        {"dark-secondary", "#a0522d"},
        {"dark-background", "#1a1a2e"},
        {"dark-surface", "#16213e"}
    };
}

// Component factory for creating Medusa components
class MedusaComponentFactory {
public:
    static MedusaThemeComponent createButton(const MUIThemeConfig& mui_theme);
    static MedusaThemeComponent createCard(const MUIThemeConfig& mui_theme);
    static MedusaThemeComponent createTextField(const MUIThemeConfig& mui_theme);
    static MedusaThemeComponent createAppBar(const MUIThemeConfig& mui_theme);
    static MedusaThemeComponent createDrawer(const MUIThemeConfig& mui_theme);
    static MedusaThemeComponent createDialog(const MUIThemeConfig& mui_theme);
    static MedusaThemeComponent createDataGrid(const MUIThemeConfig& mui_theme);
    static MedusaThemeComponent createChip(const MUIThemeConfig& mui_theme);
    static MedusaThemeComponent createFab(const MUIThemeConfig& mui_theme);
    static MedusaThemeComponent createPaper(const MUIThemeConfig& mui_theme);
    
private:
    static std::string applyYorkshireColor(const std::string& mui_color);
    static std::string convertSpacing(int spacing_units);
    static std::string convertElevation(int elevation);
};

} // namespace MedusaServer