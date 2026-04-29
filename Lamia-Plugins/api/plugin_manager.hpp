/**
 * LAMIA PLUGIN MANAGER
 * © 2025 The Medusa Project
 */

#pragma once
#include <string>
#include <vector>
#include <map>

namespace Lamia {
namespace Plugins {

class PluginManager {
public:
    bool install_plugin(const std::string& name);
    bool load_plugin(const std::string& name);
    bool unload_plugin(const std::string& name);
    std::vector<std::string> list_plugins();
    bool verify_plugin(const std::string& name);
    
private:
    std::map<std::string, void*> loaded_plugins_;
};

} // namespace Plugins
} // namespace Lamia
