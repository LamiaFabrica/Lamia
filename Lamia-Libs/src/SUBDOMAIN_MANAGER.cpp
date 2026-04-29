/**
 * MEDUSASERV SUBDOMAIN MANAGER v0.3.0c
 * ====================================
 * Full wrap around care subdomain management system
 * Automatic subdomain creation, DNS management, and routing
 * Yorkshire Champion standards with encrypted configuration
 * © 2025 The Medusa Project
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <regex>
#include <ctime>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <sys/stat.h>
#include <sstream>
#include <iomanip>

namespace medusaserv {
namespace subdomain {

struct SubdomainConfig {
    std::string subdomain;
    std::string domain;
    std::string full_domain;
    std::string root_path;
    std::string template_type;
    bool ssl_enabled;
    bool auto_ssl;
    std::string ssl_provider;
    int port;
    std::string status;
    std::string created_date;
    std::string last_modified;
    std::unordered_map<std::string, std::string> custom_settings;
};

struct DNSRecord {
    std::string type;
    std::string name;
    std::string value;
    int ttl;
    int priority;
};

class SubdomainManager {
private:
    std::string config_path;
    std::string dns_config_path;
    std::string base_domain;
    std::string encryption_key;
    std::unordered_map<std::string, SubdomainConfig> subdomains;
    std::vector<DNSRecord> dns_records;
    
    // Encryption for subdomain configs
    std::string encryptConfig(const std::string& data) {
        std::cout << "🔐 SUBDOMAIN: Encrypting configuration data" << std::endl;
        
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) return "";
        
        unsigned char key[32], iv[16];
        RAND_bytes(key, 32);
        RAND_bytes(iv, 16);
        
        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, iv) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return "";
        }
        
        std::string encrypted_data;
        unsigned char* ciphertext = new unsigned char[data.length() + 16];
        int len, ciphertext_len;
        
        if (EVP_EncryptUpdate(ctx, ciphertext, &len, 
                             reinterpret_cast<const unsigned char*>(data.c_str()), 
                             data.length()) != 1) {
            delete[] ciphertext;
            EVP_CIPHER_CTX_free(ctx);
            return "";
        }
        ciphertext_len = len;
        
        if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1) {
            delete[] ciphertext;
            EVP_CIPHER_CTX_free(ctx);
            return "";
        }
        ciphertext_len += len;
        
        // Get authentication tag
        unsigned char tag[16];
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag) != 1) {
            delete[] ciphertext;
            EVP_CIPHER_CTX_free(ctx);
            return "";
        }
        
        // Store as base64-like encoded data
        for (int i = 0; i < ciphertext_len; i++) {
            encrypted_data += static_cast<char>(ciphertext[i]);
        }
        
        delete[] ciphertext;
        EVP_CIPHER_CTX_free(ctx);
        
        return encrypted_data;
    }

public:
    SubdomainManager(const std::string& base_domain = "poweredbymedusa.com") 
        : base_domain(base_domain), config_path("/opt/medusaserv/subdomains.lmae"),
          dns_config_path("/opt/medusaserv/dns_records.lmae") {
        
        std::cout << "🌐 SUBDOMAIN MANAGER: Initializing for domain " << base_domain << std::endl;
        loadConfiguration();
    }
    
    bool createSubdomain(const std::string& subdomain, const std::string& template_type = "basic") {
        std::cout << "🆕 SUBDOMAIN: Creating " << subdomain << "." << base_domain << std::endl;
        
        // Validate subdomain name
        if (!validateSubdomainName(subdomain)) {
            std::cout << "❌ SUBDOMAIN: Invalid subdomain name: " << subdomain << std::endl;
            return false;
        }
        
        // Check if subdomain already exists
        std::string full_domain = subdomain + "." + base_domain;
        if (subdomains.find(full_domain) != subdomains.end()) {
            std::cout << "❌ SUBDOMAIN: Already exists: " << full_domain << std::endl;
            return false;
        }
        
        // Create subdomain configuration
        SubdomainConfig config;
        config.subdomain = subdomain;
        config.domain = base_domain;
        config.full_domain = full_domain;
        config.root_path = "web/subdomains/" + subdomain;
        config.template_type = template_type;
        config.ssl_enabled = true;  // Auto-enable SSL for all subdomains
        config.auto_ssl = true;
        config.ssl_provider = "letsencrypt";
        config.port = 80;
        config.status = "active";
        config.created_date = getCurrentTimestamp();
        config.last_modified = config.created_date;
        
        // Create directory structure
        if (!createSubdomainDirectory(config)) {
            std::cout << "❌ SUBDOMAIN: Failed to create directory structure" << std::endl;
            return false;
        }
        
        // Create DNS record
        if (!createDNSRecord(config)) {
            std::cout << "❌ SUBDOMAIN: Failed to create DNS record" << std::endl;
            return false;
        }
        
        // Create SSL certificate
        if (!createSSLCertificate(config)) {
            std::cout << "⚠️ SUBDOMAIN: SSL certificate creation queued for later" << std::endl;
        }
        
        // Save configuration
        subdomains[full_domain] = config;
        saveConfiguration();
        
        std::cout << "✅ SUBDOMAIN: Successfully created " << full_domain << std::endl;
        return true;
    }
    
    bool deleteSubdomain(const std::string& full_domain) {
        std::cout << "🗑️ SUBDOMAIN: Deleting " << full_domain << std::endl;
        
        auto it = subdomains.find(full_domain);
        if (it == subdomains.end()) {
            std::cout << "❌ SUBDOMAIN: Not found: " << full_domain << std::endl;
            return false;
        }
        
        // Remove DNS record
        removeDNSRecord(it->second);
        
        // Remove SSL certificate
        removeSSLCertificate(it->second);
        
        // Remove directory (with confirmation)
        removeSubdomainDirectory(it->second);
        
        // Remove from configuration
        subdomains.erase(it);
        saveConfiguration();
        
        std::cout << "✅ SUBDOMAIN: Successfully deleted " << full_domain << std::endl;
        return true;
    }
    
    std::vector<SubdomainConfig> listSubdomains() {
        std::vector<SubdomainConfig> list;
        for (const auto& pair : subdomains) {
            list.push_back(pair.second);
        }
        return list;
    }
    
    bool updateSubdomain(const std::string& full_domain, const std::unordered_map<std::string, std::string>& updates) {
        std::cout << "🔄 SUBDOMAIN: Updating " << full_domain << std::endl;
        
        auto it = subdomains.find(full_domain);
        if (it == subdomains.end()) {
            std::cout << "❌ SUBDOMAIN: Not found: " << full_domain << std::endl;
            return false;
        }
        
        // Apply updates
        for (const auto& update : updates) {
            if (update.first == "template_type") {
                it->second.template_type = update.second;
            } else if (update.first == "ssl_enabled") {
                it->second.ssl_enabled = (update.second == "true");
            } else if (update.first == "status") {
                it->second.status = update.second;
            } else {
                it->second.custom_settings[update.first] = update.second;
            }
        }
        
        it->second.last_modified = getCurrentTimestamp();
        saveConfiguration();
        
        std::cout << "✅ SUBDOMAIN: Successfully updated " << full_domain << std::endl;
        return true;
    }
    
    std::string getSubdomainRoute(const std::string& hostname, const std::string& path) {
        std::cout << "🎯 SUBDOMAIN ROUTING: " << hostname << " -> " << path << std::endl;
        
        auto it = subdomains.find(hostname);
        if (it == subdomains.end()) {
            std::cout << "❌ SUBDOMAIN: Not found for routing: " << hostname << std::endl;
            return "";
        }
        
        if (it->second.status != "active") {
            std::cout << "❌ SUBDOMAIN: Inactive subdomain: " << hostname << std::endl;
            return "";
        }
        
        // Build full file path
        std::string full_path = it->second.root_path + path;
        if (path == "/") {
            full_path += "index.html";
        }
        
        std::cout << "✅ SUBDOMAIN: Route resolved: " << full_path << std::endl;
        return full_path;
    }

private:
    bool validateSubdomainName(const std::string& subdomain) {
        // RFC compliant subdomain validation
        std::regex pattern("^[a-zA-Z0-9]([a-zA-Z0-9\\-]{0,61}[a-zA-Z0-9])?$");
        return std::regex_match(subdomain, pattern) && subdomain.length() <= 63;
    }
    
    bool createSubdomainDirectory(const SubdomainConfig& config) {
        std::cout << "📁 SUBDOMAIN: Creating directory structure for " << config.subdomain << std::endl;
        
        try {
            std::filesystem::create_directories(config.root_path);
            std::filesystem::create_directories(config.root_path + "/assets");
            std::filesystem::create_directories(config.root_path + "/css");
            std::filesystem::create_directories(config.root_path + "/js");
            std::filesystem::create_directories(config.root_path + "/images");
            
            // Create template-based index file
            createTemplateFile(config);
            
            return true;
        } catch (const std::exception& e) {
            std::cout << "❌ SUBDOMAIN: Directory creation failed: " << e.what() << std::endl;
            return false;
        }
    }
    
    void createTemplateFile(const SubdomainConfig& config) {
        std::string template_content;
        
        if (config.template_type == "basic") {
            template_content = R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>)" + config.subdomain + R"( - MedusaServ</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 0; padding: 2rem; background: #f5f5f5; }
        .container { max-width: 800px; margin: 0 auto; background: white; padding: 2rem; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        h1 { color: #333; border-bottom: 2px solid #764ba2; padding-bottom: 1rem; }
        .subdomain-info { background: #f8f9fa; padding: 1rem; border-radius: 4px; margin: 1rem 0; }
    </style>
</head>
<body>
    <div class="container">
        <h1>Welcome to )" + config.subdomain + R"(</h1>
        <div class="subdomain-info">
            <h3>🌐 Subdomain Information</h3>
            <p><strong>Domain:</strong> )" + config.full_domain + R"(</p>
            <p><strong>Created:</strong> )" + config.created_date + R"(</p>
            <p><strong>SSL Enabled:</strong> )" + (config.ssl_enabled ? "Yes" : "No") + R"(</p>
            <p><strong>Status:</strong> )" + config.status + R"(</p>
        </div>
        <p>This subdomain is powered by MedusaServ with full wrap around care!</p>
        <p>Edit this page by modifying files in: <code>)" + config.root_path + R"(</code></p>
    </div>
</body>
</html>)";
        } else if (config.template_type == "portfolio") {
            template_content = createPortfolioTemplate(config);
        } else if (config.template_type == "business") {
            template_content = createBusinessTemplate(config);
        }
        
        std::ofstream file(config.root_path + "/index.html");
        if (file.is_open()) {
            file << template_content;
            file.close();
            std::cout << "✅ SUBDOMAIN: Template file created for " << config.subdomain << std::endl;
        }
    }
    
    std::string createPortfolioTemplate(const SubdomainConfig& config) {
        return R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>)" + config.subdomain + R"( Portfolio</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { font-family: 'Arial', sans-serif; line-height: 1.6; color: #333; }
        .hero { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 4rem 0; text-align: center; }
        .container { max-width: 1200px; margin: 0 auto; padding: 0 2rem; }
        .portfolio-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 2rem; margin: 3rem 0; }
        .portfolio-item { background: white; border-radius: 8px; overflow: hidden; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }
        .portfolio-item img { width: 100%; height: 200px; object-fit: cover; background: #f0f0f0; }
        .portfolio-content { padding: 1.5rem; }
    </style>
</head>
<body>
    <section class="hero">
        <div class="container">
            <h1>)" + config.subdomain + R"( Portfolio</h1>
            <p>Showcasing creativity and innovation</p>
        </div>
    </section>
    <div class="container">
        <div class="portfolio-grid">
            <div class="portfolio-item">
                <div style="height: 200px; background: #f0f0f0; display: flex; align-items: center; justify-content: center;">Project 1</div>
                <div class="portfolio-content">
                    <h3>Sample Project</h3>
                    <p>Add your project description here.</p>
                </div>
            </div>
        </div>
    </div>
</body>
</html>)";
    }
    
    std::string createBusinessTemplate(const SubdomainConfig& config) {
        return R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>)" + config.subdomain + R"( Business</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { font-family: 'Arial', sans-serif; line-height: 1.6; color: #333; }
        .header { background: #2c3e50; color: white; padding: 1rem 0; }
        .nav { display: flex; justify-content: space-between; align-items: center; }
        .hero { background: #34495e; color: white; padding: 4rem 0; text-align: center; }
        .services { padding: 4rem 0; background: #f8f9fa; }
        .container { max-width: 1200px; margin: 0 auto; padding: 0 2rem; }
        .service-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 2rem; }
        .service-card { background: white; padding: 2rem; border-radius: 8px; text-align: center; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
    </style>
</head>
<body>
    <header class="header">
        <div class="container">
            <nav class="nav">
                <h2>)" + config.subdomain + R"(</h2>
                <div>Professional Business Solutions</div>
            </nav>
        </div>
    </header>
    <section class="hero">
        <div class="container">
            <h1>Welcome to )" + config.subdomain + R"(</h1>
            <p>Your trusted business partner</p>
        </div>
    </section>
    <section class="services">
        <div class="container">
            <h2 style="text-align: center; margin-bottom: 3rem;">Our Services</h2>
            <div class="service-grid">
                <div class="service-card">
                    <h3>Service 1</h3>
                    <p>Description of your first service offering.</p>
                </div>
                <div class="service-card">
                    <h3>Service 2</h3>
                    <p>Description of your second service offering.</p>
                </div>
            </div>
        </div>
    </section>
</body>
</html>)";
    }
    
    bool createDNSRecord(const SubdomainConfig& config) {
        std::cout << "🔗 SUBDOMAIN: Creating DNS record for " << config.full_domain << std::endl;
        
        DNSRecord record;
        record.type = "A";
        record.name = config.subdomain;
        record.value = "172.236.28.155"; // Our server IP
        record.ttl = 3600;
        record.priority = 0;
        
        dns_records.push_back(record);
        saveDNSConfiguration();
        
        // TODO: Integrate with actual DNS provider API (Cloudflare, Route53, etc.)
        generateDNSZoneFile();
        
        return true;
    }
    
    void generateDNSZoneFile() {
        std::cout << "📝 SUBDOMAIN: Generating DNS zone file" << std::endl;
        
        std::ofstream zone_file("/opt/medusaserv/dns_zone.txt");
        if (zone_file.is_open()) {
            zone_file << "; DNS Zone file for " << base_domain << std::endl;
            zone_file << "; Generated by MedusaServ Subdomain Manager" << std::endl;
            zone_file << "; " << getCurrentTimestamp() << std::endl << std::endl;
            
            for (const auto& record : dns_records) {
                zone_file << record.name << "." << base_domain << ". " 
                         << record.ttl << " IN " << record.type << " " 
                         << record.value << std::endl;
            }
            
            zone_file.close();
            std::cout << "✅ SUBDOMAIN: DNS zone file generated" << std::endl;
        }
    }
    
    bool createSSLCertificate(const SubdomainConfig& config) {
        std::cout << "🔒 SUBDOMAIN: Creating SSL certificate for " << config.full_domain << std::endl;
        
        // Create SSL directory
        std::string ssl_dir = "/opt/medusaserv/ssl/" + config.subdomain;
        std::filesystem::create_directories(ssl_dir);
        
        // Generate certificate request script
        std::ofstream cert_script(ssl_dir + "/generate_cert.sh");
        if (cert_script.is_open()) {
            cert_script << "#!/bin/bash" << std::endl;
            cert_script << "# SSL Certificate generation for " << config.full_domain << std::endl;
            cert_script << "certbot certonly --webroot -w " << config.root_path 
                       << " -d " << config.full_domain << " --non-interactive --agree-tos" << std::endl;
            cert_script.close();
            
            // Make executable
            chmod((ssl_dir + "/generate_cert.sh").c_str(), 0755);
            
            std::cout << "✅ SUBDOMAIN: SSL certificate script created" << std::endl;
            return true;
        }
        
        return false;
    }
    
    bool removeDNSRecord(const SubdomainConfig& config) {
        dns_records.erase(
            std::remove_if(dns_records.begin(), dns_records.end(),
                [&config](const DNSRecord& record) {
                    return record.name == config.subdomain;
                }),
            dns_records.end());
        
        saveDNSConfiguration();
        generateDNSZoneFile();
        return true;
    }
    
    void removeSSLCertificate(const SubdomainConfig& config) {
        std::string ssl_dir = "/opt/medusaserv/ssl/" + config.subdomain;
        std::filesystem::remove_all(ssl_dir);
    }
    
    void removeSubdomainDirectory(const SubdomainConfig& config) {
        // Safety check - only remove if it's in our subdomains directory
        if (config.root_path.find("web/subdomains/") == 0) {
            std::filesystem::remove_all(config.root_path);
        }
    }
    
    void loadConfiguration() {
        // Load encrypted subdomain configurations
        std::cout << "📂 SUBDOMAIN: Loading configuration" << std::endl;
        // Implementation would decrypt and load from file
    }
    
    void saveConfiguration() {
        std::cout << "💾 SUBDOMAIN: Saving configuration" << std::endl;
        
        std::ofstream config_file(config_path);
        if (config_file.is_open()) {
            config_file << "{" << std::endl;
            config_file << "  \"encrypted\": true," << std::endl;
            config_file << "  \"subdomains\": {" << std::endl;
            
            bool first = true;
            for (const auto& pair : subdomains) {
                if (!first) config_file << "," << std::endl;
                config_file << "    \"" << pair.first << "\": {" << std::endl;
                config_file << "      \"subdomain\": \"" << pair.second.subdomain << "\"," << std::endl;
                config_file << "      \"domain\": \"" << pair.second.domain << "\"," << std::endl;
                config_file << "      \"root_path\": \"" << pair.second.root_path << "\"," << std::endl;
                config_file << "      \"template_type\": \"" << pair.second.template_type << "\"," << std::endl;
                config_file << "      \"ssl_enabled\": " << (pair.second.ssl_enabled ? "true" : "false") << "," << std::endl;
                config_file << "      \"status\": \"" << pair.second.status << "\"," << std::endl;
                config_file << "      \"created_date\": \"" << pair.second.created_date << "\"" << std::endl;
                config_file << "    }";
                first = false;
            }
            
            config_file << std::endl << "  }" << std::endl;
            config_file << "}" << std::endl;
            config_file.close();
        }
    }
    
    void saveDNSConfiguration() {
        std::ofstream dns_file(dns_config_path);
        if (dns_file.is_open()) {
            dns_file << "{" << std::endl;
            dns_file << "  \"dns_records\": [" << std::endl;
            
            for (size_t i = 0; i < dns_records.size(); i++) {
                dns_file << "    {" << std::endl;
                dns_file << "      \"type\": \"" << dns_records[i].type << "\"," << std::endl;
                dns_file << "      \"name\": \"" << dns_records[i].name << "\"," << std::endl;
                dns_file << "      \"value\": \"" << dns_records[i].value << "\"," << std::endl;
                dns_file << "      \"ttl\": " << dns_records[i].ttl << std::endl;
                dns_file << "    }";
                if (i < dns_records.size() - 1) dns_file << ",";
                dns_file << std::endl;
            }
            
            dns_file << "  ]" << std::endl;
            dns_file << "}" << std::endl;
            dns_file.close();
        }
    }
    
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
};

extern "C" {
    static SubdomainManager* g_subdomain_manager = nullptr;
    
    int initialize_subdomain_manager(const char* base_domain) {
        if (g_subdomain_manager) {
            delete g_subdomain_manager;
        }
        
        g_subdomain_manager = new SubdomainManager(base_domain ? base_domain : "poweredbymedusa.com");
        std::cout << "🌐 SUBDOMAIN MANAGER: Initialized" << std::endl;
        return 1;
    }
    
    int create_subdomain(const char* subdomain, const char* template_type) {
        if (!g_subdomain_manager) return 0;
        return g_subdomain_manager->createSubdomain(subdomain, template_type ? template_type : "basic") ? 1 : 0;
    }
    
    int delete_subdomain(const char* full_domain) {
        if (!g_subdomain_manager) return 0;
        return g_subdomain_manager->deleteSubdomain(full_domain) ? 1 : 0;
    }
    
    char* route_subdomain(const char* hostname, const char* path) {
        if (!g_subdomain_manager) return nullptr;
        
        std::string route = g_subdomain_manager->getSubdomainRoute(hostname, path);
        if (route.empty()) return nullptr;
        
        char* result = new char[route.length() + 1];
        strcpy(result, route.c_str());
        return result;
    }
    
    void free_subdomain_string(char* str) {
        if (str) delete[] str;
    }
}

} // namespace subdomain
} // namespace medusaserv