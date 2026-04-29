/*
 * MEDUSA YORKSHIRE THEME SYSTEM - UBUNTU 24.04
 * Yorkshire Champion themed UI with Material Design elements
 */

#pragma once

#include <string>
#include <map>
#include <vector>

namespace MedusaServer {

struct ThemeConfig {
    std::string name = "yorkshire_material";
    bool dark_mode = true;
    
    // Yorkshire colors
    struct Colors {
        std::string primary = "#1a472a";      // Yorkshire Green
        std::string secondary = "#8b4513";    // Yorkshire Brown
        std::string accent = "#ffd700";       // Yorkshire Gold
        std::string background = "#f5f5dc";   // Yorkshire Cream
        std::string surface = "#ffffff";      // White
        std::string error = "#d32f2f";        // Red
        std::string warning = "#ff9800";      // Orange
        std::string info = "#2196f3";         // Blue
        std::string success = "#4caf50";      // Green
        
        // Dark mode variants
        std::string dark_primary = "#2d5a3d";
        std::string dark_secondary = "#a0522d";
        std::string dark_background = "#1a1a2e";
        std::string dark_surface = "#16213e";
    } colors;
    
    // Typography
    struct Typography {
        std::string font_family = "'Segoe UI', 'Roboto', sans-serif";
        std::string heading_font = "'Yorkshire', serif";
        double base_size = 16.0;
        double scale_ratio = 1.25;
    } typography;
    
    // Spacing
    struct Spacing {
        int base_unit = 8;
        int small = 4;
        int medium = 16;
        int large = 24;
        int xlarge = 32;
    } spacing;
};

class ThemeManager {
private:
    ThemeConfig config_;
    std::map<std::string, std::string> css_variables_;
    std::map<std::string, std::string> custom_styles_;
    
public:
    ThemeManager();
    ~ThemeManager() = default;
    
    // Initialization
    bool initialize(const ThemeConfig& config);
    void setDarkMode(bool dark_mode);
    
    // CSS Generation
    std::string generateMainCSS();
    std::string generateVariables();
    std::string generateComponents();
    std::string generateLayoutCSS();
    std::string generateResponsiveCSS();
    
    // Component styles
    std::string getHeaderStyles();
    std::string getSidebarStyles();
    std::string getDashboardStyles();
    std::string getButtonStyles();
    std::string getFormStyles();
    std::string getTableStyles();
    std::string getCardStyles();
    
    // Theme management
    void addCustomStyle(const std::string& selector, const std::string& styles);
    std::string getCustomStyle(const std::string& selector);
    void setThemeVariable(const std::string& name, const std::string& value);
    std::string getThemeVariable(const std::string& name);
    
    // Utilities
    std::string getColorScheme();
    std::string getTypographyCSS();
    std::string getSpacingCSS();
    
private:
    void initializeVariables();
    void initializeComponents();
    std::string lighten(const std::string& color, double amount);
    std::string darken(const std::string& color, double amount);
    std::string hexToRgb(const std::string& hex);
};

// Inline implementation for core CSS generation
inline std::string ThemeManager::generateMainCSS() {
    return R"(
/* MEDUSA YORKSHIRE THEME - MAIN STYLES */
:root {
)" + generateVariables() + R"(
}

/* Reset and base styles */
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

html {
    font-size: )" + std::to_string(config_.typography.base_size) + R"(px;
    scroll-behavior: smooth;
}

body {
    font-family: )" + config_.typography.font_family + R"(;
    background: var(--bg-primary);
    color: var(--text-primary);
    line-height: 1.6;
    overflow-x: hidden;
}

/* Yorkshire Theme Components */
.yorkshire-theme {
    --yorkshire-green: )" + config_.colors.primary + R"(;
    --yorkshire-brown: )" + config_.colors.secondary + R"(;
    --yorkshire-gold: )" + config_.colors.accent + R"(;
    --yorkshire-cream: )" + config_.colors.background + R"(;
}

/* App Container */
.app-container {
    display: grid;
    grid-template-areas: 
        "header header"
        "sidebar main"
        "footer footer";
    grid-template-rows: 60px 1fr auto;
    grid-template-columns: 250px 1fr;
    min-height: 100vh;
    transition: all 0.3s ease;
}

.app-container.sidebar-collapsed {
    grid-template-columns: 60px 1fr;
}

/* Header Styles */
.main-header {
    grid-area: header;
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 0 var(--spacing-medium);
    background: var(--surface-primary);
    border-bottom: 2px solid var(--yorkshire-green);
    box-shadow: 0 2px 4px rgba(0,0,0,0.1);
    z-index: 1000;
}

.header-left {
    display: flex;
    align-items: center;
    gap: var(--spacing-medium);
}

.logo {
    height: 40px;
    width: auto;
    border-radius: 4px;
    box-shadow: 0 2px 8px rgba(26, 71, 42, 0.3);
}

.app-title {
    font-family: )" + config_.typography.heading_font + R"(;
    color: var(--yorkshire-green);
    font-size: 1.5rem;
    font-weight: bold;
    margin: 0;
}

.header-center {
    flex: 1;
    display: flex;
    justify-content: center;
}

.breadcrumb {
    display: flex;
    align-items: center;
    gap: 8px;
    color: var(--text-secondary);
    font-size: 0.9rem;
}

.header-right {
    display: flex;
    align-items: center;
    gap: var(--spacing-medium);
}

.user-profile {
    display: flex;
    align-items: center;
    gap: 8px;
    padding: 8px 12px;
    border-radius: 20px;
    background: var(--bg-secondary);
    cursor: pointer;
    transition: background 0.3s ease;
    position: relative;
}

.user-profile:hover {
    background: var(--yorkshire-green);
    color: white;
}

.user-avatar {
    width: 32px;
    height: 32px;
    border-radius: 50%;
    border: 2px solid var(--yorkshire-gold);
}

.theme-toggle {
    padding: 8px 12px;
    background: var(--surface-secondary);
    border: 1px solid var(--border-color);
    border-radius: 6px;
    cursor: pointer;
    transition: all 0.3s ease;
}

.theme-toggle:hover {
    background: var(--yorkshire-gold);
    transform: scale(1.05);
}

/* Sidebar Styles */
.sidebar {
    grid-area: sidebar;
    background: var(--surface-primary);
    border-right: 2px solid var(--yorkshire-green);
    overflow-y: auto;
    transition: width 0.3s ease;
}

.sidebar-header {
    padding: var(--spacing-medium);
    border-bottom: 1px solid var(--border-color);
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.sidebar-title {
    font-weight: 600;
    color: var(--yorkshire-green);
    font-size: 1.1rem;
}

.sidebar-nav {
    padding: var(--spacing-small) 0;
}

.nav-menu {
    list-style: none;
}

.nav-menu li {
    margin: 0;
}

.nav-menu a {
    display: flex;
    align-items: center;
    padding: 12px var(--spacing-medium);
    color: var(--text-primary);
    text-decoration: none;
    transition: all 0.3s ease;
    border-left: 3px solid transparent;
}

.nav-menu a:hover,
.nav-menu a.active {
    background: linear-gradient(90deg, var(--yorkshire-green), transparent);
    color: white;
    border-left-color: var(--yorkshire-gold);
}

.nav-menu .icon {
    margin-right: 12px;
    font-size: 1.2rem;
    width: 20px;
    text-align: center;
}

/* Main Content */
.main-content {
    grid-area: main;
    padding: var(--spacing-large);
    background: var(--bg-primary);
    overflow-y: auto;
}

.content-wrapper {
    max-width: 1200px;
    margin: 0 auto;
}

/* Dashboard Grid */
.dashboard-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: var(--spacing-large);
    margin-bottom: var(--spacing-large);
}

.dashboard-card {
    background: var(--surface-primary);
    border-radius: 8px;
    padding: var(--spacing-large);
    box-shadow: 0 4px 12px rgba(0,0,0,0.1);
    border-top: 4px solid var(--yorkshire-green);
    transition: transform 0.3s ease, box-shadow 0.3s ease;
}

.dashboard-card:hover {
    transform: translateY(-2px);
    box-shadow: 0 8px 20px rgba(26, 71, 42, 0.2);
}

.card-header {
    display: flex;
    align-items: center;
    justify-content: space-between;
    margin-bottom: var(--spacing-medium);
}

.card-title {
    font-family: )" + config_.typography.heading_font + R"(;
    color: var(--yorkshire-green);
    font-size: 1.25rem;
    font-weight: 600;
}

/* Footer */
.main-footer {
    grid-area: footer;
    background: var(--surface-primary);
    border-top: 1px solid var(--border-color);
    padding: var(--spacing-medium);
    text-align: center;
    color: var(--text-secondary);
    font-size: 0.9rem;
}

/* Dark mode styles */
.dark {
    --bg-primary: )" + config_.colors.dark_background + R"(;
    --bg-secondary: )" + config_.colors.dark_surface + R"(;
    --surface-primary: )" + config_.colors.dark_surface + R"(;
    --text-primary: #ffffff;
    --text-secondary: #b0b0b0;
    --border-color: #404040;
}

/* Responsive design */
@media (max-width: 768px) {
    .app-container {
        grid-template-areas: 
            "header"
            "main"
            "footer";
        grid-template-columns: 1fr;
        grid-template-rows: 60px 1fr auto;
    }
    
    .sidebar {
        position: fixed;
        left: -250px;
        top: 60px;
        bottom: 0;
        width: 250px;
        z-index: 1000;
        transition: left 0.3s ease;
    }
    
    .sidebar.open {
        left: 0;
    }
    
    .header-left .app-title {
        display: none;
    }
}

/* Animation and transitions */
@keyframes fadeIn {
    from { opacity: 0; transform: translateY(10px); }
    to { opacity: 1; transform: translateY(0); }
}

.fade-in {
    animation: fadeIn 0.3s ease;
}

/* Yorkshire Champion accent colors */
.accent-gold { color: var(--yorkshire-gold) !important; }
.accent-green { color: var(--yorkshire-green) !important; }
.accent-brown { color: var(--yorkshire-brown) !important; }

.bg-accent-gold { background-color: var(--yorkshire-gold) !important; }
.bg-accent-green { background-color: var(--yorkshire-green) !important; }
.bg-accent-brown { background-color: var(--yorkshire-brown) !important; }
)";
}

} // namespace MedusaServer