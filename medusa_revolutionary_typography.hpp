/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * MEDUSA REVOLUTIONARY TYPOGRAPHY v0.3.0c
 * =======================================
 * 
 * Revolutionary typography engine for Lamia WYSIWYG editor
 * Yorkshire Champion Standards implementation
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace MedusaServ::Typography {

    /**
     * @brief Font weight constants
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
     * @brief Font style enumeration
     */
    enum class FontStyle {
        NORMAL,
        ITALIC,
        OBLIQUE
    };

    /**
     * @brief Text alignment options
     */
    enum class TextAlignment {
        LEFT,
        CENTER,
        RIGHT,
        JUSTIFY,
        START,
        END
    };

    /**
     * @brief Typography configuration structure
     */
    struct TypographyConfig {
        std::string font_family;
        float font_size;
        FontWeight font_weight;
        FontStyle font_style;
        std::string color;
        float line_height;
        float letter_spacing;
        float word_spacing;
        TextAlignment text_align;
        std::string text_decoration;
        std::string text_transform;
        
        TypographyConfig() 
            : font_family("Arial, sans-serif")
            , font_size(16.0f)
            , font_weight(FontWeight::NORMAL)
            , font_style(FontStyle::NORMAL)
            , color("#000000")
            , line_height(1.5f)
            , letter_spacing(0.0f)
            , word_spacing(0.0f)
            , text_align(TextAlignment::LEFT)
            , text_decoration("none")
            , text_transform("none") {}
    };

    /**
     * @brief Revolutionary typography engine
     */
    class RevolutionaryTypographyEngine {
    private:
        std::unordered_map<std::string, TypographyConfig> style_presets_;
        std::vector<std::string> font_stack_;
        
    public:
        RevolutionaryTypographyEngine();
        
        // Style preset management
        void register_preset(const std::string& name, const TypographyConfig& config);
        TypographyConfig get_preset(const std::string& name) const;
        std::vector<std::string> list_presets() const;
        
        // Font management
        void add_font_to_stack(const std::string& font_family);
        std::string get_font_stack() const;
        bool is_font_available(const std::string& font_family) const;
        
        // CSS generation
        std::string generate_css(const TypographyConfig& config) const;
        std::string generate_css_class(const std::string& class_name, const TypographyConfig& config) const;
        
        // Typography calculations
        float calculate_line_height(float font_size, float multiplier = 1.5f) const;
        float calculate_character_width(char character, float font_size) const;
        int calculate_text_width(const std::string& text, const TypographyConfig& config) const;
        int calculate_text_height(const std::string& text, const TypographyConfig& config, int max_width) const;
        
        // Text processing
        std::string apply_text_transform(const std::string& text, const std::string& transform) const;
        std::vector<std::string> word_wrap(const std::string& text, int max_width, const TypographyConfig& config) const;
        
        // Accessibility
        bool check_contrast_ratio(const std::string& foreground, const std::string& background) const;
        std::string suggest_accessible_color(const std::string& background) const;
        
        // Revolutionary features
        std::string apply_quantum_kerning(const std::string& text, const TypographyConfig& config) const;
        std::string optimize_readability(const std::string& text, const TypographyConfig& config) const;
        TypographyConfig auto_adjust_for_content(const std::string& content_type) const;
    };

    /**
     * @brief Predefined typography presets
     */
    namespace Presets {
        TypographyConfig heading_1();
        TypographyConfig heading_2();
        TypographyConfig heading_3();
        TypographyConfig body_text();
        TypographyConfig caption();
        TypographyConfig code();
        TypographyConfig quote();
        TypographyConfig button();
        TypographyConfig navigation();
        TypographyConfig footer();
    }

} // namespace MedusaServ::Typography