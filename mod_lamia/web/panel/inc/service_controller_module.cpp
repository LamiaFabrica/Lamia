/**
 * Service Controller Module - MedusaServ Control Panel
 * @fileoverview Enterprise service management and monitoring module
 * @version 0.3.0c-ULTIMATE
 * @security Yorkshire Champion Gold Standard
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <json/json.h>
#include <regex>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>

namespace MedusaCP {
    namespace ServiceController {

        /**
         * Structure to hold service information
         */
        struct ServiceInfo {
            std::string name;
            std::string status;
            std::string sub_status;
            bool is_active;
            bool is_enabled;
            std::string description;
            std::string load_state;
            std::string active_state;
            std::string sub_state;
            std::string main_pid;
            std::string memory_usage;
            std::string cpu_usage;
            std::vector<std::string> dependencies;
        };

        /**
         * Execute system command and return output
         */
        std::string executeCommand(const std::string& command) {
            std::string result;
            FILE* pipe = popen(command.c_str(), "r");
            
            if (!pipe) {
                return "";
            }
            
            char buffer[128];
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                result += buffer;
            }
            
            pclose(pipe);
            
            // Remove trailing newline
            if (!result.empty() && result.back() == '\n') {
                result.pop_back();
            }
            
            return result;
        }

        /**
         * Check if systemctl is available
         */
        bool isSystemdAvailable() {
            return system("which systemctl >/dev/null 2>&1") == 0;
        }

        /**
         * Get service status using systemctl
         */
        ServiceInfo getServiceInfo(const std::string& service_name) {
            ServiceInfo info;
            info.name = service_name;
            
            if (!isSystemdAvailable()) {
                info.status = "systemd_unavailable";
                return info;
            }
            
            // Get basic service status
            std::string status_cmd = "systemctl is-active " + service_name + " 2>/dev/null";
            info.status = executeCommand(status_cmd);
            info.is_active = (info.status == "active");
            
            // Get enabled status
            std::string enabled_cmd = "systemctl is-enabled " + service_name + " 2>/dev/null";
            std::string enabled_status = executeCommand(enabled_cmd);
            info.is_enabled = (enabled_status == "enabled");
            
            // Get detailed service information
            std::string show_cmd = "systemctl show " + service_name + " --no-page 2>/dev/null";
            std::string show_output = executeCommand(show_cmd);
            
            std::istringstream ss(show_output);
            std::string line;
            
            while (std::getline(ss, line)) {
                size_t eq_pos = line.find('=');
                if (eq_pos != std::string::npos) {
                    std::string key = line.substr(0, eq_pos);
                    std::string value = line.substr(eq_pos + 1);
                    
                    if (key == "Description") {
                        info.description = value;
                    } else if (key == "LoadState") {
                        info.load_state = value;
                    } else if (key == "ActiveState") {
                        info.active_state = value;
                    } else if (key == "SubState") {
                        info.sub_state = value;
                    } else if (key == "MainPID") {
                        info.main_pid = value;
                    } else if (key == "MemoryCurrent") {
                        info.memory_usage = value;
                    }
                }
            }
            
            return info;
        }

        /**
         * Get list of important system services
         */
        std::vector<std::string> getImportantServices() {
            return {
                "apache2", "nginx", "httpd",
                "mysql", "mariadb", "postgresql",
                "ssh", "sshd",
                "fail2ban",
                "ufw", "iptables",
                "cron", "crond",
                "rsyslog", "syslog",
                "systemd-resolved",
                "docker", "containerd",
                "redis-server", "redis",
                "memcached",
                "postfix", "sendmail",
                "vsftpd", "proftpd",
                "named", "bind9",
                "dhcpd", "dhcp",
                "ntp", "chronyd",
                "snmpd"
            };
        }

        /**
         * Start a service
         */
        bool startService(const std::string& service_name) {
            if (!isSystemdAvailable()) {
                return false;
            }
            
            // Security check: validate service name
            if (!std::regex_match(service_name, std::regex("^[a-zA-Z0-9._-]+$"))) {
                return false;
            }
            
            std::string command = "systemctl start " + service_name + " 2>/dev/null";
            return system(command.c_str()) == 0;
        }

        /**
         * Stop a service
         */
        bool stopService(const std::string& service_name) {
            if (!isSystemdAvailable()) {
                return false;
            }
            
            // Security check: validate service name
            if (!std::regex_match(service_name, std::regex("^[a-zA-Z0-9._-]+$"))) {
                return false;
            }
            
            std::string command = "systemctl stop " + service_name + " 2>/dev/null";
            return system(command.c_str()) == 0;
        }

        /**
         * Restart a service
         */
        bool restartService(const std::string& service_name) {
            if (!isSystemdAvailable()) {
                return false;
            }
            
            // Security check: validate service name
            if (!std::regex_match(service_name, std::regex("^[a-zA-Z0-9._-]+$"))) {
                return false;
            }
            
            std::string command = "systemctl restart " + service_name + " 2>/dev/null";
            return system(command.c_str()) == 0;
        }

        /**
         * Enable a service
         */
        bool enableService(const std::string& service_name) {
            if (!isSystemdAvailable()) {
                return false;
            }
            
            // Security check: validate service name
            if (!std::regex_match(service_name, std::regex("^[a-zA-Z0-9._-]+$"))) {
                return false;
            }
            
            std::string command = "systemctl enable " + service_name + " 2>/dev/null";
            return system(command.c_str()) == 0;
        }

        /**
         * Disable a service
         */
        bool disableService(const std::string& service_name) {
            if (!isSystemdAvailable()) {
                return false;
            }
            
            // Security check: validate service name
            if (!std::regex_match(service_name, std::regex("^[a-zA-Z0-9._-]+$"))) {
                return false;
            }
            
            std::string command = "systemctl disable " + service_name + " 2>/dev/null";
            return system(command.c_str()) == 0;
        }

        /**
         * Get service logs
         */
        std::string getServiceLogs(const std::string& service_name, int lines = 50) {
            if (!isSystemdAvailable()) {
                return "systemd unavailable";
            }
            
            // Security check: validate service name
            if (!std::regex_match(service_name, std::regex("^[a-zA-Z0-9._-]+$"))) {
                return "Invalid service name";
            }
            
            std::string command = "journalctl -u " + service_name + " -n " + std::to_string(lines) + " --no-pager 2>/dev/null";
            return executeCommand(command);
        }

        /**
         * Get all available services
         */
        std::vector<std::string> getAllServices() {
            std::vector<std::string> services;
            
            if (!isSystemdAvailable()) {
                return services;
            }
            
            std::string command = "systemctl list-unit-files --type=service --no-pager --no-legend 2>/dev/null | awk '{print $1}' | sed 's/.service$//'";
            std::string output = executeCommand(command);
            
            std::istringstream ss(output);
            std::string line;
            
            while (std::getline(ss, line)) {
                if (!line.empty() && line.find('@') == std::string::npos) {
                    services.push_back(line);
                }
            }
            
            return services;
        }

        /**
         * Get system service statistics
         */
        Json::Value getServiceStatistics() {
            Json::Value stats;
            
            if (!isSystemdAvailable()) {
                stats["error"] = "systemd unavailable";
                return stats;
            }
            
            // Count services by state
            std::string command = "systemctl list-units --type=service --no-pager --no-legend 2>/dev/null";
            std::string output = executeCommand(command);
            
            int total = 0, active = 0, failed = 0, inactive = 0;
            
            std::istringstream ss(output);
            std::string line;
            
            while (std::getline(ss, line)) {
                if (!line.empty()) {
                    total++;
                    if (line.find(" active ") != std::string::npos) {
                        active++;
                    } else if (line.find(" failed ") != std::string::npos) {
                        failed++;
                    } else {
                        inactive++;
                    }
                }
            }
            
            stats["total_services"] = total;
            stats["active_services"] = active;
            stats["failed_services"] = failed;
            stats["inactive_services"] = inactive;
            
            return stats;
        }
    }
}

// C interface for the module
extern "C" {
    /**
     * Get service status
     */
    char* get_service_status(const char* service_name) {
        try {
            Json::Value result;
            MedusaCP::ServiceController::ServiceInfo info = 
                MedusaCP::ServiceController::getServiceInfo(std::string(service_name));
            
            result["name"] = info.name;
            result["status"] = info.status;
            result["is_active"] = info.is_active;
            result["is_enabled"] = info.is_enabled;
            result["description"] = info.description;
            result["load_state"] = info.load_state;
            result["active_state"] = info.active_state;
            result["sub_state"] = info.sub_state;
            result["main_pid"] = info.main_pid;
            result["memory_usage"] = info.memory_usage;
            result["timestamp"] = static_cast<int64_t>(std::time(nullptr));
            result["module"] = "service_controller";
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, result);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
            
        } catch (const std::exception& e) {
            Json::Value error;
            error["error"] = "Service status query failed";
            error["details"] = e.what();
            error["service"] = service_name;
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, error);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
        }
    }
    
    /**
     * Get all important services status
     */
    char* get_all_services_status() {
        try {
            Json::Value result;
            Json::Value services(Json::arrayValue);
            
            std::vector<std::string> important_services = 
                MedusaCP::ServiceController::getImportantServices();
            
            for (const std::string& service : important_services) {
                MedusaCP::ServiceController::ServiceInfo info = 
                    MedusaCP::ServiceController::getServiceInfo(service);
                
                Json::Value service_json;
                service_json["name"] = info.name;
                service_json["status"] = info.status;
                service_json["is_active"] = info.is_active;
                service_json["is_enabled"] = info.is_enabled;
                service_json["description"] = info.description;
                
                services.append(service_json);
            }
            
            result["services"] = services;
            result["statistics"] = MedusaCP::ServiceController::getServiceStatistics();
            result["timestamp"] = static_cast<int64_t>(std::time(nullptr));
            result["module"] = "service_controller";
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, result);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
            
        } catch (const std::exception& e) {
            Json::Value error;
            error["error"] = "Services status query failed";
            error["details"] = e.what();
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, error);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
        }
    }
    
    /**
     * Control service (start/stop/restart/enable/disable)
     */
    char* control_service(const char* service_name, const char* action) {
        try {
            Json::Value result;
            std::string service_str(service_name);
            std::string action_str(action);
            bool success = false;
            
            if (action_str == "start") {
                success = MedusaCP::ServiceController::startService(service_str);
            } else if (action_str == "stop") {
                success = MedusaCP::ServiceController::stopService(service_str);
            } else if (action_str == "restart") {
                success = MedusaCP::ServiceController::restartService(service_str);
            } else if (action_str == "enable") {
                success = MedusaCP::ServiceController::enableService(service_str);
            } else if (action_str == "disable") {
                success = MedusaCP::ServiceController::disableService(service_str);
            } else {
                result["error"] = "Invalid action";
                result["valid_actions"] = Json::Value(Json::arrayValue);
                result["valid_actions"].append("start");
                result["valid_actions"].append("stop");
                result["valid_actions"].append("restart");
                result["valid_actions"].append("enable");
                result["valid_actions"].append("disable");
            }
            
            if (result.isMember("error")) {
                // Error already set above
            } else {
                result["success"] = success;
                result["service"] = service_str;
                result["action"] = action_str;
                result["message"] = success ? ("Service " + action_str + " successful") : ("Service " + action_str + " failed");
            }
            
            result["timestamp"] = static_cast<int64_t>(std::time(nullptr));
            result["module"] = "service_controller";
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, result);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
            
        } catch (const std::exception& e) {
            Json::Value error;
            error["error"] = "Service control failed";
            error["details"] = e.what();
            error["service"] = service_name;
            error["action"] = action;
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, error);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
        }
    }
    
    /**
     * Get service logs
     */
    char* get_service_logs(const char* service_name, int lines) {
        try {
            Json::Value result;
            std::string logs = MedusaCP::ServiceController::getServiceLogs(std::string(service_name), lines);
            
            result["service"] = service_name;
            result["lines_requested"] = lines;
            result["logs"] = logs;
            result["timestamp"] = static_cast<int64_t>(std::time(nullptr));
            result["module"] = "service_controller";
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, result);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
            
        } catch (const std::exception& e) {
            Json::Value error;
            error["error"] = "Service logs query failed";
            error["details"] = e.what();
            error["service"] = service_name;
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, error);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
        }
    }
}