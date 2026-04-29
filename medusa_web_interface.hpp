/*
 * MEDUSA WEB INTERFACE - UBUNTU 24.04
 * UI Layout: Logo top-left, sidebar menu, Yorkshire theme
 * Combines both projects' UI systems
 */

#pragma once

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <memory>

namespace MedusaServer {

struct MenuItem {
    std::string id;
    std::string title;
    std::string icon;
    std::string url;
    std::string permission_required;
    std::vector<MenuItem> submenu;
    bool separator_after = false;
};

struct DashboardWidget {
    std::string id;
    std::string title;
    std::string type; // "chart", "metric", "table", "text"
    std::string data_endpoint;
    std::map<std::string, std::string> config;
    int grid_x = 0, grid_y = 0, grid_w = 1, grid_h = 1;
};

struct UserProfile {
    std::string user_id;
    std::string username;
    std::string display_name;
    std::string role;
    std::string avatar_url;
    std::vector<std::string> permissions;
    std::map<std::string, std::string> preferences;
};

class WebInterface {
private:
    std::string theme_name_;
    bool dark_mode_;
    std::vector<MenuItem> menu_items_;
    std::vector<DashboardWidget> dashboard_widgets_;
    std::map<std::string, std::string> static_pages_;
    
public:
    WebInterface();
    ~WebInterface() = default;
    
    // Initialization
    bool initialize(const std::string& theme_name, bool dark_mode);
    void loadMenuStructure();
    void loadDashboardWidgets();
    
    // HTML Generation
    std::string generateMainPage(const UserProfile& user);
    std::string generateDashboard(const UserProfile& user);
    std::string generateLoginPage();
    std::string generateErrorPage(int error_code, const std::string& message);
    
    // Component generation
    std::string generateHeader(const UserProfile& user);
    std::string generateSidebar(const UserProfile& user);
    std::string generateFooter();
    std::string generateDashboardGrid(const UserProfile& user);
    
    // Theme and styling
    std::string getCSS();
    std::string getJavaScript();
    std::string getThemeVariables();
    
    // Menu management
    void addMenuItem(const MenuItem& item);
    void removeMenuItem(const std::string& id);
    std::vector<MenuItem> getMenuForUser(const UserProfile& user);
    
    // Dashboard management
    void addWidget(const DashboardWidget& widget);
    void removeWidget(const std::string& id);
    std::vector<DashboardWidget> getWidgetsForUser(const UserProfile& user);
    
    // Static content
    void addStaticPage(const std::string& path, const std::string& content);
    std::string getStaticPage(const std::string& path);
    
private:
    // HTML generation helpers
    std::string generateHTML5Header(const std::string& title);
    std::string generateMetaTags();
    std::string generateStylesheets();
    std::string generateScriptTags();
    std::string generateMenuHTML(const std::vector<MenuItem>& items, int level = 0);
    std::string generateWidgetHTML(const DashboardWidget& widget);
    
    // Theme helpers
    std::string getYorkshireThemeCSS();
    std::string getMaterialDesignCSS();
    std::string getResponsiveCSS();
    std::string getDarkModeCSS();
    
    // JavaScript helpers
    std::string getMenuInteractionJS();
    std::string getDashboardJS();
    std::string getThemeToggleJS();
    std::string getAjaxHelperJS();
    
    // Utility methods
    bool userHasPermission(const UserProfile& user, const std::string& permission);
    std::string sanitizeHTML(const std::string& input);
    std::string formatTimestamp(const std::chrono::system_clock::time_point& time);
};

// Inline implementation for critical methods

inline std::string WebInterface::generateMainPage(const UserProfile& user) {
    return R"(<!DOCTYPE html>
<html lang="en" class=")" + (dark_mode_ ? "dark" : "light") + R"(">
<head>
)" + generateHTML5Header("Medusa Server Dashboard") + R"(
</head>
<body class="medusa-server yorkshire-theme">
    <div class="app-container">
        <!-- Header with logo top-left -->
        <header class="main-header">
            <div class="header-left">
                <img src="/assets/images/medusa-logo.png" alt="Medusa" class="logo" />
                <h1 class="app-title">Medusa Server</h1>
            </div>
            <div class="header-center">
                <nav class="breadcrumb">
                    <span class="breadcrumb-item active">Dashboard</span>
                </nav>
            </div>
            <div class="header-right">
                <div class="user-profile">
                    <img src=")" + user.avatar_url + R"(" alt=")" + user.display_name + R"(" class="user-avatar" />
                    <span class="user-name">)" + user.display_name + R"(</span>
                    <div class="user-dropdown">
                        <a href="/profile">Profile</a>
                        <a href="/settings">Settings</a>
                        <a href="/logout">Logout</a>
                    </div>
                </div>
                <button class="theme-toggle" onclick="toggleTheme()">🌓</button>
            </div>
        </header>
        
        <!-- Sidebar menu -->
        <aside class="sidebar">
)" + generateSidebar(user) + R"(
        </aside>
        
        <!-- Main content area -->
        <main class="main-content">
            <div class="content-wrapper">
)" + generateDashboardGrid(user) + R"(
            </div>
        </main>
        
        <!-- Footer -->
        <footer class="main-footer">
)" + generateFooter() + R"(
        </footer>
    </div>
    
    <script>
)" + getJavaScript() + R"(
    </script>
</body>
</html>)";
}

inline std::string WebInterface::generateSidebar(const UserProfile& user) {
    std::string sidebar_html = R"(
        <div class="sidebar-header">
            <div class="sidebar-title">Navigation</div>
            <button class="sidebar-toggle" onclick="toggleSidebar()">☰</button>
        </div>
        <nav class="sidebar-nav">
            <ul class="nav-menu">
)";
    
    auto menu_items = getMenuForUser(user);
    sidebar_html += generateMenuHTML(menu_items);
    
    sidebar_html += R"(
            </ul>
        </nav>
        <div class="sidebar-footer">
            <div class="server-status">
                <span class="status-indicator online"></span>
                <span class="status-text">Online</span>
            </div>
        </div>
    )";
    
    return sidebar_html;
}

} // namespace MedusaServer