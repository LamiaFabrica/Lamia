/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * MEDUSA WIDGET SYSTEM v0.3.0c
 * ============================
 * 
 * Revolutionary widget system for WYSIWYG editor
 * 166+ widgets with Yorkshire Champion Standards
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace MedusaServ::Widgets {

    /**
     * @brief Widget category enumeration
     */
    enum class WidgetCategory {
        BASIC,
        LAYOUT,
        TEXT,
        MEDIA,
        INTERACTIVE,
        FORM,
        DATA,
        ADVANCED,
        CUSTOM
    };

    /**
     * @brief Widget properties structure
     */
    struct WidgetProperties {
        std::string id;
        std::string class_name;
        std::unordered_map<std::string, std::string> attributes;
        std::unordered_map<std::string, std::string> styles;
        std::string content;
        bool visible = true;
        bool enabled = true;
        
        WidgetProperties(const std::string& widget_id = "") 
            : id(widget_id) {}
    };

    /**
     * @brief Base widget interface
     */
    class BaseWidget {
    protected:
        WidgetProperties properties_;
        WidgetCategory category_;
        std::vector<std::shared_ptr<BaseWidget>> children_;
        
    public:
        BaseWidget(WidgetCategory category, const std::string& id = "");
        virtual ~BaseWidget() = default;
        
        // Core methods
        virtual std::string render() const = 0;
        virtual std::string get_html() const = 0;
        virtual std::string get_css() const = 0;
        virtual std::string get_javascript() const { return ""; }
        
        // Property management
        void set_property(const std::string& key, const std::string& value);
        std::string get_property(const std::string& key) const;
        void set_style(const std::string& property, const std::string& value);
        std::string get_style(const std::string& property) const;
        
        // Hierarchy management
        void add_child(std::shared_ptr<BaseWidget> child);
        void remove_child(const std::string& id);
        std::shared_ptr<BaseWidget> find_child(const std::string& id) const;
        
        // Getters
        const std::string& get_id() const { return properties_.id; }
        WidgetCategory get_category() const { return category_; }
        const std::vector<std::shared_ptr<BaseWidget>>& get_children() const { return children_; }
    };

    /**
     * @brief Text widget
     */
    class TextWidget : public BaseWidget {
    public:
        TextWidget(const std::string& id = "", const std::string& text = "");
        std::string render() const override;
        std::string get_html() const override;
        std::string get_css() const override;
        
        void set_text(const std::string& text);
        std::string get_text() const;
    };

    /**
     * @brief Button widget
     */
    class ButtonWidget : public BaseWidget {
    private:
        std::function<void()> click_handler_;
        
    public:
        ButtonWidget(const std::string& id = "", const std::string& text = "Button");
        std::string render() const override;
        std::string get_html() const override;
        std::string get_css() const override;
        std::string get_javascript() const override;
        
        void set_click_handler(std::function<void()> handler);
        void set_button_text(const std::string& text);
    };

    /**
     * @brief Container widget
     */
    class ContainerWidget : public BaseWidget {
    public:
        ContainerWidget(const std::string& id = "");
        std::string render() const override;
        std::string get_html() const override;
        std::string get_css() const override;
    };

    /**
     * @brief Image widget
     */
    class ImageWidget : public BaseWidget {
    public:
        ImageWidget(const std::string& id = "", const std::string& src = "");
        std::string render() const override;
        std::string get_html() const override;
        std::string get_css() const override;
        
        void set_source(const std::string& src);
        void set_alt_text(const std::string& alt);
    };

    /**
     * @brief Revolutionary widget factory
     */
    class WidgetFactory {
    private:
        static std::unordered_map<std::string, std::function<std::shared_ptr<BaseWidget>(const std::string&)>> creators_;
        
    public:
        template<typename T>
        static void register_widget(const std::string& type_name) {
            creators_[type_name] = [](const std::string& id) -> std::shared_ptr<BaseWidget> {
                return std::make_shared<T>(id);
            };
        }
        
        static std::shared_ptr<BaseWidget> create_widget(const std::string& type, const std::string& id = "");
        static std::vector<std::string> get_available_types();
        
        // Initialize all 166+ widgets
        static void initialize_all_widgets();
    };

    /**
     * @brief Widget system manager
     */
    class RevolutionaryWidgetSystem {
    private:
        std::unordered_map<std::string, std::shared_ptr<BaseWidget>> widgets_;
        std::string root_widget_id_;
        
    public:
        RevolutionaryWidgetSystem();
        
        // Widget management
        std::string add_widget(const std::string& type, const std::string& parent_id = "");
        bool remove_widget(const std::string& id);
        std::shared_ptr<BaseWidget> get_widget(const std::string& id) const;
        
        // Layout management
        void set_root_widget(const std::string& id);
        std::string get_root_widget_id() const { return root_widget_id_; }
        
        // Rendering
        std::string render_all() const;
        std::string generate_html() const;
        std::string generate_css() const;
        std::string generate_javascript() const;
        
        // Serialization
        std::string serialize_to_json() const;
        bool deserialize_from_json(const std::string& json);
        
        // Statistics
        size_t get_widget_count() const { return widgets_.size(); }
        std::vector<std::string> get_widget_ids() const;
        std::unordered_map<WidgetCategory, int> get_category_stats() const;
    };

} // namespace MedusaServ::Widgets