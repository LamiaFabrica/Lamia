/*
 * MEDUSA THEME ENGINE - C++ Foundation
 * Unified cross-platform theme engine integrating all foundation systems
 * Yorkshire Champion themed engine for all OS/device interfaces
 * 
 * Deep Links for Codebase Tracing:
 * - Colors: src/theme/core/foundation/colors/medusa_color_system.hpp
 * - Typography: src/theme/core/foundation/typography/medusa_typography_system.hpp
 * - Shadows: src/theme/core/foundation/shadows/medusa_shadow_system.hpp
 * - Spacing: src/theme/core/foundation/spacing/medusa_spacing_system.hpp
 * - Icons: src/theme/core/foundation/icons/iconify/medusa_iconify_system.hpp
 * - Integration: src/theme/integrations/mui/
 * - Extensions: src/theme/extensions/materio/
 * - Components: src/theme/components/
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
#include "medusa_color_system.hpp"
#include "medusa_typography_system.hpp"
#include "medusa_shadow_system.hpp"
#include "medusa_spacing_system.hpp"
// #include "medusa_iconify_system.hpp"  // File doesn't exist

namespace MedusaTheme {
namespace Core {

/**
 * Theme Mode Enumeration
 * Different theme modes for different contexts
 */
enum class ThemeMode {
    LIGHT,
    DARK,
    BORDERED,
    YORKSHIRE_LIGHT,
    YORKSHIRE_DARK,
    YORKSHIRE_BORDERED,
    CUSTOM
};

/**
 * Theme Context Enumeration
 * Different contexts for theme application
 */
enum class ThemeContext {
    WEB,           // Web interface
    DESKTOP,       // Desktop application
    MOBILE,        // Mobile application
    EMBEDDED,      // Embedded systems
    CONSOLE,       // Console/Terminal
    CUSTOM         // Custom context
};

/**
 * Theme Component Structure
 * Represents a themed component with all its properties
 */
struct ThemeComponent {
    std::string name;
    std::string variant;
    std::string state;  // default, hover, active, disabled, etc.
    
    // Component properties
    std::string background_color;
    std::string text_color;
    std::string border_color;
    std::string shadow_level;
    std::string typography_variant;
    std::string spacing_level;
    std::string icon_name;
    
    // Custom properties
    std::map<std::string, std::string> custom_properties;
    
    ThemeComponent() = default;
    ThemeComponent(const std::string& component_name, const std::string& component_variant = "default")
        : name(component_name), variant(component_variant), state("default") {}
    
    // Property access
    std::string getProperty(const std::string& property_name) const;
    void setProperty(const std::string& property_name, const std::string& value);
    
    // Component manipulation
    ThemeComponent withState(const std::string& component_state) const;
    ThemeComponent withVariant(const std::string& component_variant) const;
    ThemeComponent withCustomProperty(const std::string& name, const std::string& value) const;
    
    // Convert to different formats
    std::string toCSSString() const;
    std::string toPlatformString(const std::string& platform) const;
    std::string toJSONString() const;
};

/**
 * Theme Definition Structure
 * Represents a complete theme definition
 */
struct ThemeDefinition {
    std::string name;
    std::string description;
    std::string author;
    std::string version;
    ThemeMode mode;
    ThemeContext context;
    
    // Theme systems
    std::map<std::string, std::string> color_scheme;
    std::map<std::string, std::string> typography_scale;
    std::map<std::string, std::string> shadow_scale;
    std::map<std::string, std::string> spacing_scale;
    std::map<std::string, std::string> icon_set;
    
    // Component definitions
    std::map<std::string, ThemeComponent> components;
    
    // Theme metadata
    std::chrono::system_clock::time_point created;
    std::chrono::system_clock::time_point modified;
    std::vector<std::string> tags;
    
    ThemeDefinition() : mode(ThemeMode::LIGHT), context(ThemeContext::WEB) {}
    ThemeDefinition(const std::string& theme_name, ThemeMode theme_mode = ThemeMode::LIGHT)
        : name(theme_name), mode(theme_mode), context(ThemeContext::WEB) {}
    
    // Component management
    void addComponent(const ThemeComponent& component);
    ThemeComponent getComponent(const std::string& name, const std::string& variant = "default") const;
    bool hasComponent(const std::string& name, const std::string& variant = "default") const;
    
    // Theme manipulation
    ThemeDefinition withMode(ThemeMode theme_mode) const;
    ThemeDefinition withContext(ThemeContext theme_context) const;
    ThemeDefinition withColorScheme(const std::map<std::string, std::string>& colors) const;
    ThemeDefinition withTypographyScale(const std::map<std::string, std::string>& typography) const;
    
    // Convert to different formats
    std::string toCSSString() const;
    std::string toPlatformString(const std::string& platform) const;
    std::string toJSONString() const;
    std::string toThemeFile() const;
};

/**
 * Theme Renderer
 * Handles theme rendering for different platforms
 */
class ThemeRenderer {
private:
    std::map<std::string, std::string> render_cache_;
    std::mutex render_mutex_;
    
public:
    ThemeRenderer();
    ~ThemeRenderer() = default;
    
    // Theme rendering
    std::string renderTheme(const ThemeDefinition& theme, const std::string& platform = "default");
    std::string renderComponent(const ThemeComponent& component, const std::string& platform = "default");
    std::string renderComponentWithStyle(const ThemeComponent& component, const std::map<std::string, std::string>& styles);
    
    // Platform-specific rendering
    std::string renderForWindows(const ThemeDefinition& theme);
    std::string renderForMacOS(const ThemeDefinition& theme);
    std::string renderForLinux(const ThemeDefinition& theme);
    std::string renderForWeb(const ThemeDefinition& theme);
    std::string renderForMobile(const ThemeDefinition& theme);
    std::string renderForConsole(const ThemeDefinition& theme);
    
    // Format-specific rendering
    std::string renderAsCSS(const ThemeDefinition& theme);
    std::string renderAsJSON(const ThemeDefinition& theme);
    std::string renderAsXML(const ThemeDefinition& theme);
    std::string renderAsYAML(const ThemeDefinition& theme);
    
    // Component rendering
    std::string renderComponentCSS(const ThemeComponent& component);
    std::string renderComponentJSON(const ThemeComponent& component);
    std::string renderComponentXML(const ThemeComponent& component);
    
    // Cache management
    void clearCache();
    size_t getCacheSize() const;
    double getCacheHitRate() const;
    
private:
    std::string generateCSSTheme(const ThemeDefinition& theme);
    std::string generateJSONTheme(const ThemeDefinition& theme);
    std::string generateXMLTheme(const ThemeDefinition& theme);
    std::string generateYAMLTheme(const ThemeDefinition& theme);
    std::string generatePlatformTheme(const ThemeDefinition& theme, const std::string& platform);
};

/**
 * Theme Cache Manager
 * Manages theme caching and optimization
 */
class ThemeCacheManager {
private:
    std::unordered_map<std::string, std::string> theme_cache_;
    std::unordered_map<std::string, ThemeDefinition> definition_cache_;
    std::unordered_map<std::string, std::string> component_cache_;
    std::mutex cache_mutex_;
    
    size_t max_cache_size_ = 1000;
    size_t cache_hits_ = 0;
    size_t cache_misses_ = 0;
    
public:
    ThemeCacheManager();
    ~ThemeCacheManager() = default;
    
    // Cache operations
    void cacheTheme(const std::string& key, const std::string& rendered_theme);
    void cacheDefinition(const std::string& key, const ThemeDefinition& definition);
    void cacheComponent(const std::string& key, const std::string& rendered_component);
    bool getCachedTheme(const std::string& key, std::string& rendered_theme);
    bool getCachedDefinition(const std::string& key, ThemeDefinition& definition);
    bool getCachedComponent(const std::string& key, std::string& rendered_component);
    
    // Cache management
    void setMaxCacheSize(size_t size);
    size_t getCacheSize() const;
    void cleanupCache();
    void clearCache();
    
    // Statistics
    double getHitRate() const;
    size_t getThemeCacheSize() const;
    size_t getDefinitionCacheSize() const;
    size_t getComponentCacheSize() const;
    
private:
    void evictOldestEntries();
    std::string generateCacheKey(const ThemeDefinition& theme, const std::string& platform);
    std::string generateComponentCacheKey(const ThemeComponent& component, const std::string& platform);
};

/**
 * Theme System
 * Main system for managing themes across all platforms
 */
class ThemeSystem {
private:
    static std::unique_ptr<ThemeSystem> instance_;
    static std::mutex instance_mutex_;
    
    // Foundation systems
    std::unique_ptr<Foundation::Colors::ColorSystem> color_system_;
    std::unique_ptr<Foundation::Typography::TypographySystem> typography_system_;
    std::unique_ptr<Foundation::Shadows::ShadowSystem> shadow_system_;
    std::unique_ptr<Foundation::Spacing::SpacingSystem> spacing_system_;
    std::unique_ptr<Foundation::Iconify::IconifySystem> iconify_system_;
    
    // Theme management
    std::unique_ptr<ThemeRenderer> renderer_;
    std::unique_ptr<ThemeCacheManager> cache_manager_;
    
    // Current state
    std::string current_theme_;
    ThemeMode current_mode_;
    ThemeContext current_context_;
    std::string current_platform_;
    bool initialized_ = false;
    
public:
    ThemeSystem();
    ~ThemeSystem() = default;
    
    // Singleton access
    static ThemeSystem& getInstance();
    static void initialize();
    static void shutdown();
    
    // System management
    bool initialize();
    void setTheme(const std::string& theme_name);
    void setMode(ThemeMode mode);
    void setContext(ThemeContext context);
    void setPlatform(const std::string& platform);
    
    // Theme access
    std::string getCurrentTheme() const;
    ThemeMode getCurrentMode() const;
    ThemeContext getCurrentContext() const;
    std::string getCurrentPlatform() const;
    
    // Foundation system access
    Foundation::Colors::ColorSystem& getColorSystem();
    Foundation::Typography::TypographySystem& getTypographySystem();
    Foundation::Shadows::ShadowSystem& getShadowSystem();
    Foundation::Spacing::SpacingSystem& getSpacingSystem();
    Foundation::Iconify::IconifySystem& getIconifySystem();
    
    // Theme operations
    ThemeDefinition getTheme(const std::string& theme_name) const;
    std::string renderTheme(const std::string& theme_name);
    std::string renderTheme(const ThemeDefinition& theme);
    std::string renderComponent(const std::string& component_name, const std::string& variant = "default");
    std::string renderComponent(const ThemeComponent& component);
    
    // Theme creation
    ThemeDefinition createTheme(const std::string& name, ThemeMode mode = ThemeMode::LIGHT);
    ThemeComponent createComponent(const std::string& name, const std::string& variant = "default");
    void addComponent(const std::string& theme_name, const ThemeComponent& component);
    
    // Theme management
    void loadTheme(const std::string& theme_file);
    void saveTheme(const std::string& theme_name, const std::string& theme_file);
    void exportTheme(const std::string& theme_name, const std::string& format = "css");
    void importTheme(const std::string& theme_file);
    
    // Built-in themes
    void createMaterioTheme();
    void createYorkshireTheme();
    void createDefaultTheme();
    void createDarkTheme();
    void createLightTheme();
    
    // Platform-specific operations
    std::string getPlatformSpecificTheme(const std::string& theme_name) const;
    ThemeDefinition getPlatformAdjustedTheme(const std::string& theme_name) const;
    
    // Theme utilities
    std::vector<std::string> getAvailableThemes() const;
    std::vector<ThemeComponent> getThemeComponents(const std::string& theme_name) const;
    bool hasTheme(const std::string& theme_name) const;
    
    // Performance and statistics
    double getCacheHitRate() const;
    size_t getMemoryUsage() const;
    void optimizeMemory();
    
    // Theme switching
    void switchToTheme(const std::string& theme_name);
    void switchToMode(ThemeMode mode);
    void switchToContext(ThemeContext context);
    void switchToPlatform(const std::string& platform);
    
private:
    void initializeFoundationSystems();
    void createDefaultThemes();
    void loadThemeDefinitions();
    void setupThemeDefaults();
    void validateTheme(const ThemeDefinition& theme);
};

/**
 * Theme Utilities
 * Helper functions for theme operations
 */
namespace Utils {
    // Theme utilities
    std::string themeModeToString(ThemeMode mode);
    ThemeMode stringToThemeMode(const std::string& mode);
    std::string themeContextToString(ThemeContext context);
    ThemeContext stringToThemeContext(const std::string& context);
    
    // File utilities
    bool saveThemeToFile(const ThemeDefinition& theme, const std::string& filename);
    ThemeDefinition loadThemeFromFile(const std::string& filename);
    std::string getThemeFileExtension(const std::string& format);
    
    // Validation utilities
    bool isValidThemeName(const std::string& name);
    bool isValidComponentName(const std::string& name);
    bool isValidThemeMode(ThemeMode mode);
    bool isValidThemeContext(ThemeContext context);
    
    // Platform utilities
    std::string detectPlatform();
    bool isPlatformSupported(const std::string& platform);
    std::string getPlatformThemeFormat(const std::string& platform);
    
    // Conversion utilities
    std::string themeToCSS(const ThemeDefinition& theme);
    std::string themeToJSON(const ThemeDefinition& theme);
    std::string themeToXML(const ThemeDefinition& theme);
    std::string themeToYAML(const ThemeDefinition& theme);
    ThemeDefinition cssToTheme(const std::string& css);
    ThemeDefinition jsonToTheme(const std::string& json);
    ThemeDefinition xmlToTheme(const std::string& xml);
    ThemeDefinition yamlToTheme(const std::string& yaml);
}

} // namespace Core
} // namespace MedusaTheme 