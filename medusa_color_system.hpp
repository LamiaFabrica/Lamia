/*
 * MEDUSA COLOR SYSTEM - C++ Foundation
 * Converts Materio color schemes to C++ for window styling
 * Yorkshire Champion themed color system
 * 
 * Deep Links for Codebase Tracing:
 * - Source: inc/docs/materio/src/@core/theme/colorSchemes.js
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

namespace MedusaTheme {
namespace Foundation {

/**
 * Color Structure - RGB with Alpha
 * Represents colors in RGBA format for C++ window styling
 */
struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;
    
    Color() = default;
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}
    
    // Convert from hex string
    static Color fromHex(const std::string& hex);
    
    // Convert to hex string
    std::string toHex() const;
    
    // Convert to RGB string
    std::string toRGB() const;
    
    // Convert to RGBA string
    std::string toRGBA() const;
    
    // Color manipulation
    Color lighten(double amount) const;
    Color darken(double amount) const;
    Color withAlpha(uint8_t alpha) const;
    
    // Predefined colors
    static const Color WHITE;
    static const Color BLACK;
    static const Color TRANSPARENT;
};

/**
 * Color Palette Structure
 * Represents a complete color palette for a theme
 */
struct ColorPalette {
    // Primary colors
    Color primary_main;
    Color primary_light;
    Color primary_dark;
    Color primary_lighter_opacity;
    Color primary_light_opacity;
    Color primary_main_opacity;
    Color primary_dark_opacity;
    Color primary_darker_opacity;
    
    // Secondary colors
    Color secondary_main;
    Color secondary_light;
    Color secondary_dark;
    Color secondary_contrast_text;
    Color secondary_lighter_opacity;
    Color secondary_light_opacity;
    Color secondary_main_opacity;
    Color secondary_dark_opacity;
    Color secondary_darker_opacity;
    
    // Semantic colors
    Color error_main;
    Color error_light;
    Color error_dark;
    Color error_contrast_text;
    Color error_lighter_opacity;
    Color error_light_opacity;
    Color error_main_opacity;
    Color error_dark_opacity;
    Color error_darker_opacity;
    
    Color warning_main;
    Color warning_light;
    Color warning_dark;
    Color warning_contrast_text;
    Color warning_lighter_opacity;
    Color warning_light_opacity;
    Color warning_main_opacity;
    Color warning_dark_opacity;
    Color warning_darker_opacity;
    
    Color info_main;
    Color info_light;
    Color info_dark;
    Color info_contrast_text;
    Color info_lighter_opacity;
    Color info_light_opacity;
    Color info_main_opacity;
    Color info_dark_opacity;
    Color info_darker_opacity;
    
    Color success_main;
    Color success_light;
    Color success_dark;
    Color success_contrast_text;
    Color success_lighter_opacity;
    Color success_light_opacity;
    Color success_main_opacity;
    Color success_dark_opacity;
    Color success_darker_opacity;
    
    // Text colors
    Color text_primary;
    Color text_secondary;
    Color text_disabled;
    Color text_primary_channel;
    Color text_secondary_channel;
    
    // Background colors
    Color background_default;
    Color background_paper;
    
    // Action colors
    Color action_active;
    Color action_hover;
    Color action_selected;
    Color action_disabled;
    Color action_disabled_background;
    Color action_focus;
    Color action_active_channel;
    Color action_selected_channel;
    
    // Divider colors
    Color divider;
    Color divider_channel;
    
    // Alert colors
    Color alert_error;
    Color alert_warning;
    Color alert_info;
    Color alert_success;
    Color alert_error_standard_bg;
    Color alert_warning_standard_bg;
    Color alert_info_standard_bg;
    Color alert_success_standard_bg;
};

/**
 * Color Scheme Manager
 * Manages light and dark color schemes
 */
class ColorSchemeManager {
private:
    std::map<std::string, ColorPalette> schemes_;
    std::string current_scheme_;
    std::string current_skin_;
    
public:
    ColorSchemeManager();
    ~ColorSchemeManager() = default;
    
    // Scheme management
    void setScheme(const std::string& scheme_name);
    void setSkin(const std::string& skin_name);
    std::string getCurrentScheme() const;
    std::string getCurrentSkin() const;
    
    // Color access
    Color getColor(const std::string& color_name) const;
    ColorPalette getCurrentPalette() const;
    
    // Scheme creation
    void createLightScheme(const std::string& skin = "default");
    void createDarkScheme(const std::string& skin = "default");
    void createBorderedScheme(const std::string& skin = "default");
    
    // Yorkshire Champion color schemes
    void createYorkshireLightScheme();
    void createYorkshireDarkScheme();
    void createYorkshireBorderedScheme();
    
    // Color utilities
    static Color hexToColor(const std::string& hex);
    static std::string colorToHex(const Color& color);
    static Color rgbToColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    static std::string colorToRGB(const Color& color);
    static std::string colorToRGBA(const Color& color);
    
    // Color manipulation
    static Color lightenColor(const Color& color, double amount);
    static Color darkenColor(const Color& color, double amount);
    static Color blendColors(const Color& color1, const Color& color2, double ratio);
    static Color createOpacityColor(const Color& color, double opacity);
    
private:
    void initializeDefaultSchemes();
    void initializeYorkshireSchemes();
    ColorPalette createMaterioLightPalette(const std::string& skin);
    ColorPalette createMaterioDarkPalette(const std::string& skin);
    ColorPalette createYorkshireLightPalette();
    ColorPalette createYorkshireDarkPalette();
};

/**
 * Color System Singleton
 * Global access to the color system
 */
class ColorSystem {
private:
    static std::unique_ptr<ColorSchemeManager> instance_;
    static std::mutex instance_mutex_;
    
public:
    static ColorSchemeManager& getInstance();
    static void initialize();
    static void shutdown();
    
    // Direct color access
    static Color getPrimary();
    static Color getSecondary();
    static Color getError();
    static Color getWarning();
    static Color getInfo();
    static Color getSuccess();
    static Color getBackground();
    static Color getSurface();
    static Color getText();
    static Color getDivider();
    
    // Scheme switching
    static void switchToLight();
    static void switchToDark();
    static void switchToBordered();
    static void switchToYorkshire();
    
    // Color generation
    static Color generateColor(const std::string& name, double opacity = 1.0);
    static std::vector<Color> generateColorGradient(const Color& start, const Color& end, int steps);
    static Color generateContrastColor(const Color& background);
};

} // namespace Foundation
} // namespace MedusaTheme 