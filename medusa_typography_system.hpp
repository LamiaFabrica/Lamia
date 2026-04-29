/*
 * MEDUSA TYPOGRAPHY SYSTEM - C++ Foundation
 * Cross-platform typography system for all OS/device interfaces
 * Yorkshire Champion themed typography system
 * 
 * Deep Links for Codebase Tracing:
 * - Source: inc/docs/materio/src/@core/theme/typography.js
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

namespace MedusaTheme {
namespace Foundation {
namespace Typography {

/**
 * Font Weight Enumeration
 * Cross-platform font weight definitions
 */
enum class FontWeight {
    THIN = 100,
    EXTRA_LIGHT = 200,
    LIGHT = 300,
    NORMAL = 400,
    MEDIUM = 500,
    SEMI_BOLD = 600,
    BOLD = 700,
    EXTRA_BOLD = 800,
    BLACK = 900
};

/**
 * Font Style Enumeration
 * Cross-platform font style definitions
 */
enum class FontStyle {
    NORMAL,
    ITALIC,
    OBLIQUE
};

/**
 * Text Transform Enumeration
 * Text transformation options
 */
enum class TextTransform {
    NONE,
    UPPERCASE,
    LOWERCASE,
    CAPITALIZE
};

/**
 * Font Family Structure
 * Represents a font family with fallbacks for different platforms
 */
struct FontFamily {
    std::string primary;
    std::vector<std::string> fallbacks;
    std::string platform_specific;  // Platform-specific font name
    
    FontFamily() = default;
    FontFamily(const std::string& primary_font, const std::vector<std::string>& fallback_fonts = {})
        : primary(primary_font), fallbacks(fallback_fonts) {}
    
    // Get font string for specific platform
    std::string getFontString(const std::string& platform = "default") const;
    
    // Platform-specific font resolution
    std::string resolveForPlatform(const std::string& platform) const;
};

/**
 * Typography Variant Structure
 * Represents a typography variant (h1, h2, body1, etc.)
 */
struct TypographyVariant {
    std::string name;
    double font_size;           // Base font size in points
    FontWeight font_weight;
    FontStyle font_style;
    double line_height;         // Line height multiplier
    double letter_spacing;      // Letter spacing in points
    TextTransform text_transform;
    std::string font_family;
    
    // Responsive sizing
    std::map<std::string, double> responsive_sizes;  // breakpoint -> size
    
    // Platform-specific adjustments
    std::map<std::string, double> platform_adjustments;  // platform -> adjustment
    
    TypographyVariant() = default;
    TypographyVariant(const std::string& variant_name, double size, FontWeight weight = FontWeight::NORMAL)
        : name(variant_name), font_size(size), font_weight(weight), font_style(FontStyle::NORMAL),
          line_height(1.0), letter_spacing(0.0), text_transform(TextTransform::NONE) {}
    
    // Get adjusted size for platform
    double getAdjustedSize(const std::string& platform = "default") const;
    
    // Get responsive size for breakpoint
    double getResponsiveSize(const std::string& breakpoint = "default") const;
    
    // Convert to CSS-like string
    std::string toCSSString() const;
    
    // Convert to platform-specific format
    std::string toPlatformString(const std::string& platform) const;
};

/**
 * Typography Scale Structure
 * Represents the complete typography scale
 */
struct TypographyScale {
    std::map<std::string, TypographyVariant> variants;
    double base_font_size;
    double scale_factor;
    
    TypographyScale() : base_font_size(13.125), scale_factor(1.25) {}
    
    // Add variant
    void addVariant(const TypographyVariant& variant);
    
    // Get variant
    TypographyVariant getVariant(const std::string& name) const;
    
    // Check if variant exists
    bool hasVariant(const std::string& name) const;
    
    // Generate scale variants
    void generateScale(double base_size, double factor);
};

/**
 * Platform Font Manager
 * Manages font availability and loading across different platforms
 */
class PlatformFontManager {
private:
    std::map<std::string, std::vector<std::string>> platform_fonts_;
    std::map<std::string, bool> font_availability_;
    std::mutex font_mutex_;
    
public:
    PlatformFontManager();
    ~PlatformFontManager() = default;
    
    // Platform detection
    std::string detectPlatform() const;
    std::string detectOS() const;
    std::string detectDevice() const;
    
    // Font availability
    bool isFontAvailable(const std::string& font_name, const std::string& platform = "default") const;
    std::vector<std::string> getAvailableFonts(const std::string& platform = "default") const;
    
    // Font loading
    bool loadFont(const std::string& font_path, const std::string& font_name);
    bool loadSystemFont(const std::string& font_name, const std::string& platform = "default");
    
    // Font resolution
    std::string resolveFont(const FontFamily& family, const std::string& platform = "default") const;
    std::string getBestMatch(const std::vector<std::string>& fonts, const std::string& platform = "default") const;
    
private:
    void initializePlatformFonts();
    void detectSystemFonts();
    std::vector<std::string> getSystemFonts(const std::string& platform) const;
};

/**
 * Typography Renderer
 * Handles typography rendering for different platforms
 */
class TypographyRenderer {
private:
    std::map<std::string, std::string> render_cache_;
    std::mutex render_mutex_;
    
public:
    TypographyRenderer();
    ~TypographyRenderer() = default;
    
    // Text rendering
    std::string renderText(const std::string& text, const TypographyVariant& variant, const std::string& platform = "default");
    std::string renderTextWithStyle(const std::string& text, const TypographyVariant& variant, const std::map<std::string, std::string>& styles);
    
    // Platform-specific rendering
    std::string renderForWindows(const std::string& text, const TypographyVariant& variant);
    std::string renderForMacOS(const std::string& text, const TypographyVariant& variant);
    std::string renderForLinux(const std::string& text, const TypographyVariant& variant);
    std::string renderForWeb(const std::string& text, const TypographyVariant& variant);
    std::string renderForMobile(const std::string& text, const TypographyVariant& variant);
    
    // Text measurement
    struct TextMetrics {
        double width;
        double height;
        double baseline;
        double ascent;
        double descent;
    };
    
    TextMetrics measureText(const std::string& text, const TypographyVariant& variant, const std::string& platform = "default");
    
    // Text layout
    std::vector<std::string> wrapText(const std::string& text, double max_width, const TypographyVariant& variant);
    std::string truncateText(const std::string& text, double max_width, const TypographyVariant& variant, const std::string& ellipsis = "...");
    
private:
    std::string applyTextTransform(const std::string& text, TextTransform transform);
    double calculateLineHeight(double font_size, double line_height_multiplier);
    double calculateLetterSpacing(double font_size, double letter_spacing_points);
};

/**
 * Typography System
 * Main system for managing typography across all platforms
 */
class TypographySystem {
private:
    static std::unique_ptr<TypographySystem> instance_;
    static std::mutex instance_mutex_;
    
    std::unique_ptr<TypographyScale> scale_;
    std::unique_ptr<PlatformFontManager> font_manager_;
    std::unique_ptr<TypographyRenderer> renderer_;
    
    std::string current_platform_;
    std::string current_theme_;
    bool initialized_ = false;
    
public:
    TypographySystem();
    ~TypographySystem() = default;
    
    // Singleton access
    static TypographySystem& getInstance();
    static void initialize();
    static void shutdown();
    
    // System management
    bool initialize();
    void setPlatform(const std::string& platform);
    void setTheme(const std::string& theme);
    std::string getCurrentPlatform() const;
    std::string getCurrentTheme() const;
    
    // Typography access
    TypographyVariant getVariant(const std::string& name) const;
    std::string renderText(const std::string& text, const std::string& variant_name);
    std::string renderText(const std::string& text, const TypographyVariant& variant);
    
    // Font management
    FontFamily getFontFamily(const std::string& name = "default") const;
    bool loadFont(const std::string& font_path, const std::string& font_name);
    std::vector<std::string> getAvailableFonts() const;
    
    // Scale management
    void setBaseFontSize(double size);
    void setScaleFactor(double factor);
    void generateScale();
    void addCustomVariant(const TypographyVariant& variant);
    
    // Platform-specific operations
    std::string getPlatformSpecificFont(const std::string& font_name) const;
    TypographyVariant getPlatformAdjustedVariant(const std::string& variant_name) const;
    
    // Text utilities
    TypographyRenderer::TextMetrics measureText(const std::string& text, const std::string& variant_name);
    std::vector<std::string> wrapText(const std::string& text, double max_width, const std::string& variant_name);
    std::string truncateText(const std::string& text, double max_width, const std::string& variant_name);
    
    // Theme integration
    void applyTheme(const std::string& theme_name);
    void createYorkshireTheme();
    void createMaterioTheme();
    
private:
    void initializeDefaultScale();
    void initializePlatformFonts();
    void createDefaultVariants();
    void createMaterioVariants();
    void createYorkshireVariants();
};

/**
 * Typography Utilities
 * Helper functions for typography operations
 */
namespace Utils {
    // Font utilities
    std::string fontWeightToString(FontWeight weight);
    FontWeight stringToFontWeight(const std::string& weight);
    std::string fontStyleToString(FontStyle style);
    FontStyle stringToFontStyle(const std::string& style);
    std::string textTransformToString(TextTransform transform);
    TextTransform stringToTextTransform(const std::string& transform);
    
    // Size utilities
    double remToPoints(double rem, double base_size = 16.0);
    double pointsToRem(double points, double base_size = 16.0);
    double pixelsToPoints(double pixels, double dpi = 96.0);
    double pointsToPixels(double points, double dpi = 96.0);
    
    // Text utilities
    std::string sanitizeText(const std::string& text);
    std::string normalizeWhitespace(const std::string& text);
    std::vector<std::string> splitText(const std::string& text, char delimiter = ' ');
    
    // Platform utilities
    std::string detectPlatform();
    std::string getPlatformFont(const std::string& font_name);
    bool isPlatformSupported(const std::string& platform);
    
    // Validation utilities
    bool isValidFontSize(double size);
    bool isValidLineHeight(double height);
    bool isValidLetterSpacing(double spacing);
    bool isValidFontWeight(FontWeight weight);
}

} // namespace Typography
} // namespace Foundation
} // namespace MedusaTheme 