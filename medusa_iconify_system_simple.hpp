#pragma once

#include <string>

namespace MedusaTheme {
namespace Foundation {
namespace Iconify {

class IconifySystem {
public:
    static IconifySystem& getInstance() {
        static IconifySystem instance;
        return instance;
    }
    
    std::string getIconSVG(const std::string& icon_name, int size = 24, const std::string& color = "#ffffff") {
        // Simple fallback implementation
        return "<span style='font-size: " + std::to_string(size) + "px; color: " + color + ";'>[" + icon_name + "]</span>";
    }
    
private:
    IconifySystem() = default;
};

} // namespace Iconify
} // namespace Foundation
} // namespace MedusaTheme