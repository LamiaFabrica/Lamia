/**
 * System Information Module - MedusaServ Control Panel
 * @fileoverview Comprehensive system information gathering module
 * @version 0.3.0c-ULTIMATE
 * @security Yorkshire Champion Gold Standard
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <json/json.h>
#include <cstring>
#include <regex>

namespace MedusaCP {
    namespace SystemInfo {

        /**
         * Get CPU information from /proc/cpuinfo
         */
        Json::Value getCPUInfo() {
            Json::Value cpu_info;
            std::ifstream cpuinfo("/proc/cpuinfo");
            std::string line;
            int core_count = 0;
            std::string model_name;
            
            while (std::getline(cpuinfo, line)) {
                if (line.find("processor") != std::string::npos) {
                    core_count++;
                } else if (line.find("model name") != std::string::npos) {
                    size_t pos = line.find(":");
                    if (pos != std::string::npos && model_name.empty()) {
                        model_name = line.substr(pos + 2);
                    }
                }
            }
            
            cpu_info["cores"] = core_count;
            cpu_info["model"] = model_name;
            
            // Get load average
            struct sysinfo si;
            if (sysinfo(&si) == 0) {
                cpu_info["load_1min"] = static_cast<double>(si.loads[0]) / 65536.0;
                cpu_info["load_5min"] = static_cast<double>(si.loads[1]) / 65536.0;
                cpu_info["load_15min"] = static_cast<double>(si.loads[2]) / 65536.0;
            }
            
            return cpu_info;
        }

        /**
         * Get memory information
         */
        Json::Value getMemoryInfo() {
            Json::Value memory_info;
            struct sysinfo si;
            
            if (sysinfo(&si) == 0) {
                unsigned long total_ram = si.totalram * si.mem_unit;
                unsigned long free_ram = si.freeram * si.mem_unit;
                unsigned long used_ram = total_ram - free_ram;
                
                memory_info["total_bytes"] = static_cast<int64_t>(total_ram);
                memory_info["free_bytes"] = static_cast<int64_t>(free_ram);
                memory_info["used_bytes"] = static_cast<int64_t>(used_ram);
                memory_info["total_gb"] = static_cast<double>(total_ram) / (1024 * 1024 * 1024);
                memory_info["free_gb"] = static_cast<double>(free_ram) / (1024 * 1024 * 1024);
                memory_info["used_gb"] = static_cast<double>(used_ram) / (1024 * 1024 * 1024);
                memory_info["usage_percent"] = static_cast<double>(used_ram) / total_ram * 100.0;
                
                memory_info["total_swap"] = static_cast<int64_t>(si.totalswap * si.mem_unit);
                memory_info["free_swap"] = static_cast<int64_t>(si.freeswap * si.mem_unit);
            }
            
            return memory_info;
        }

        /**
         * Get disk information
         */
        Json::Value getDiskInfo() {
            Json::Value disk_info;
            Json::Value partitions(Json::arrayValue);
            
            std::vector<std::string> mount_points = {"/", "/home", "/var", "/tmp", "/boot"};
            
            for (const std::string& mount_point : mount_points) {
                struct statvfs stat;
                if (statvfs(mount_point.c_str()) == 0) {
                    Json::Value partition;
                    
                    unsigned long block_size = stat.f_frsize;
                    unsigned long total_blocks = stat.f_blocks;
                    unsigned long free_blocks = stat.f_bavail;
                    unsigned long used_blocks = total_blocks - free_blocks;
                    
                    partition["mount_point"] = mount_point;
                    partition["total_bytes"] = static_cast<int64_t>(total_blocks * block_size);
                    partition["free_bytes"] = static_cast<int64_t>(free_blocks * block_size);
                    partition["used_bytes"] = static_cast<int64_t>(used_blocks * block_size);
                    partition["total_gb"] = static_cast<double>(total_blocks * block_size) / (1024 * 1024 * 1024);
                    partition["free_gb"] = static_cast<double>(free_blocks * block_size) / (1024 * 1024 * 1024);
                    partition["used_gb"] = static_cast<double>(used_blocks * block_size) / (1024 * 1024 * 1024);
                    partition["usage_percent"] = total_blocks > 0 ? static_cast<double>(used_blocks) / total_blocks * 100.0 : 0.0;
                    
                    partitions.append(partition);
                }
            }
            
            disk_info["partitions"] = partitions;
            return disk_info;
        }

        /**
         * Get network interface information
         */
        Json::Value getNetworkInfo() {
            Json::Value network_info;
            Json::Value interfaces(Json::arrayValue);
            
            std::ifstream net_dev("/proc/net/dev");
            std::string line;
            std::getline(net_dev, line); // Skip header line 1
            std::getline(net_dev, line); // Skip header line 2
            
            while (std::getline(net_dev, line)) {
                std::istringstream iss(line);
                std::string interface;
                unsigned long rx_bytes, rx_packets, rx_errors, rx_dropped;
                unsigned long tx_bytes, tx_packets, tx_errors, tx_dropped;
                
                if (iss >> interface >> rx_bytes >> rx_packets >> rx_errors >> rx_dropped) {
                    // Skip to tx stats
                    for (int i = 0; i < 4; ++i) iss >> tx_bytes;
                    iss >> tx_bytes >> tx_packets >> tx_errors >> tx_dropped;
                    
                    // Remove colon from interface name
                    interface = interface.substr(0, interface.find(':'));
                    
                    Json::Value iface;
                    iface["name"] = interface;
                    iface["rx_bytes"] = static_cast<int64_t>(rx_bytes);
                    iface["tx_bytes"] = static_cast<int64_t>(tx_bytes);
                    iface["rx_packets"] = static_cast<int64_t>(rx_packets);
                    iface["tx_packets"] = static_cast<int64_t>(tx_packets);
                    iface["rx_errors"] = static_cast<int64_t>(rx_errors);
                    iface["tx_errors"] = static_cast<int64_t>(tx_errors);
                    
                    interfaces.append(iface);
                }
            }
            
            network_info["interfaces"] = interfaces;
            return network_info;
        }

        /**
         * Get system uptime and processes
         */
        Json::Value getSystemStats() {
            Json::Value system_stats;
            struct sysinfo si;
            
            if (sysinfo(&si) == 0) {
                system_stats["uptime_seconds"] = static_cast<int64_t>(si.uptime);
                system_stats["process_count"] = static_cast<int>(si.procs);
                
                // Convert uptime to human readable
                int days = si.uptime / 86400;
                int hours = (si.uptime % 86400) / 3600;
                int minutes = (si.uptime % 3600) / 60;
                
                std::ostringstream uptime_str;
                if (days > 0) uptime_str << days << "d ";
                if (hours > 0) uptime_str << hours << "h ";
                uptime_str << minutes << "m";
                
                system_stats["uptime_human"] = uptime_str.str();
            }
            
            // Get hostname
            char hostname[256];
            if (gethostname(hostname, sizeof(hostname)) == 0) {
                system_stats["hostname"] = std::string(hostname);
            }
            
            // Get system info
            struct utsname uts;
            if (uname(&uts) == 0) {
                system_stats["kernel_name"] = std::string(uts.sysname);
                system_stats["kernel_release"] = std::string(uts.release);
                system_stats["kernel_version"] = std::string(uts.version);
                system_stats["machine"] = std::string(uts.machine);
            }
            
            return system_stats;
        }

        /**
         * Get service status information
         */
        Json::Value getServiceStatus() {
            Json::Value services;
            Json::Value service_list(Json::arrayValue);
            
            std::vector<std::string> important_services = {
                "apache2", "nginx", "mysql", "postgresql", "ssh", "fail2ban",
                "ufw", "cron", "rsyslog", "systemd-resolved", "docker", "redis"
            };
            
            for (const std::string& service : important_services) {
                Json::Value service_info;
                service_info["name"] = service;
                
                // Check service status using systemctl
                std::string command = "systemctl is-active " + service + " 2>/dev/null";
                FILE* pipe = popen(command.c_str(), "r");
                if (pipe) {
                    char buffer[128];
                    std::string result = "";
                    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                        result += buffer;
                    }
                    pclose(pipe);
                    
                    // Remove newline
                    if (!result.empty() && result.back() == '\n') {
                        result.pop_back();
                    }
                    
                    service_info["status"] = result;
                    service_info["active"] = (result == "active");
                } else {
                    service_info["status"] = "unknown";
                    service_info["active"] = false;
                }
                
                service_list.append(service_info);
            }
            
            services["services"] = service_list;
            return services;
        }
    }
}

// C interface for the module
extern "C" {
    /**
     * Get comprehensive system information
     */
    char* get_system_info() {
        try {
            Json::Value system_info;
            
            system_info["cpu"] = MedusaCP::SystemInfo::getCPUInfo();
            system_info["memory"] = MedusaCP::SystemInfo::getMemoryInfo();
            system_info["disk"] = MedusaCP::SystemInfo::getDiskInfo();
            system_info["network"] = MedusaCP::SystemInfo::getNetworkInfo();
            system_info["system"] = MedusaCP::SystemInfo::getSystemStats();
            system_info["services"] = MedusaCP::SystemInfo::getServiceStatus();
            
            system_info["timestamp"] = static_cast<int64_t>(std::time(nullptr));
            system_info["medusaserv_version"] = "0.3.0c-ULTIMATE";
            system_info["module"] = "system_info";
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, system_info);
            
            char* result = (char*)malloc(json_string.length() + 1);
            strcpy(result, json_string.c_str());
            return result;
            
        } catch (const std::exception& e) {
            Json::Value error;
            error["error"] = "System info collection failed";
            error["details"] = e.what();
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, error);
            
            char* result = (char*)malloc(json_string.length() + 1);
            strcpy(result, json_string.c_str());
            return result;
        }
    }
    
    /**
     * Get CPU information only
     */
    char* get_cpu_info() {
        try {
            Json::Value cpu_info = MedusaCP::SystemInfo::getCPUInfo();
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, cpu_info);
            
            char* result = (char*)malloc(json_string.length() + 1);
            strcpy(result, json_string.c_str());
            return result;
            
        } catch (const std::exception& e) {
            Json::Value error;
            error["error"] = "CPU info collection failed";
            error["details"] = e.what();
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, error);
            
            char* result = (char*)malloc(json_string.length() + 1);
            strcpy(result, json_string.c_str());
            return result;
        }
    }
    
    /**
     * Get memory information only
     */
    char* get_memory_info() {
        try {
            Json::Value memory_info = MedusaCP::SystemInfo::getMemoryInfo();
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, memory_info);
            
            char* result = (char*)malloc(json_string.length() + 1);
            strcpy(result, json_string.c_str());
            return result;
            
        } catch (const std::exception& e) {
            Json::Value error;
            error["error"] = "Memory info collection failed";
            error["details"] = e.what();
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, error);
            
            char* result = (char*)malloc(json_string.length() + 1);
            strcpy(result, json_string.c_str());
            return result;
        }
    }
}