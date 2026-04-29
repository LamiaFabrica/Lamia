/**
 * Process Manager Module - MedusaServ Control Panel
 * @fileoverview Advanced process management and monitoring module
 * @version 0.3.0c-ULTIMATE
 * @security Yorkshire Champion Gold Standard
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <json/json.h>
#include <algorithm>
#include <regex>
#include <pwd.h>

namespace MedusaCP {
    namespace ProcessManager {

        /**
         * Structure to hold process information
         */
        struct ProcessInfo {
            int pid;
            std::string name;
            std::string state;
            std::string user;
            double cpu_percent;
            double memory_percent;
            unsigned long memory_kb;
            unsigned long virtual_memory_kb;
            std::string cmdline;
            int ppid;
            int threads;
            unsigned long start_time;
        };

        /**
         * Get user name from UID
         */
        std::string getUserName(uid_t uid) {
            struct passwd* pwd = getpwuid(uid);
            return pwd ? std::string(pwd->pw_name) : std::to_string(uid);
        }

        /**
         * Parse process information from /proc/[pid]/stat
         */
        ProcessInfo parseProcessStat(int pid) {
            ProcessInfo proc;
            proc.pid = pid;
            
            std::string stat_path = "/proc/" + std::to_string(pid) + "/stat";
            std::ifstream stat_file(stat_path);
            
            if (!stat_file.is_open()) {
                return proc;
            }
            
            std::string line;
            std::getline(stat_file, line);
            std::istringstream iss(line);
            
            std::string token;
            std::vector<std::string> tokens;
            
            while (iss >> token) {
                tokens.push_back(token);
            }
            
            if (tokens.size() >= 24) {
                // Extract process name (remove parentheses)
                proc.name = tokens[1];
                if (proc.name.length() >= 2) {
                    proc.name = proc.name.substr(1, proc.name.length() - 2);
                }
                
                // Process state
                proc.state = tokens[2];
                
                // Parent PID
                proc.ppid = std::stoi(tokens[3]);
                
                // Number of threads
                proc.threads = std::stoi(tokens[19]);
                
                // Start time
                proc.start_time = std::stoul(tokens[21]);
                
                // Virtual memory size (in bytes, convert to KB)
                proc.virtual_memory_kb = std::stoul(tokens[22]) / 1024;
                
                // Resident Set Size (in pages, convert to KB)
                long page_size = sysconf(_SC_PAGESIZE);
                proc.memory_kb = std::stoul(tokens[23]) * page_size / 1024;
            }
            
            // Get command line
            std::string cmdline_path = "/proc/" + std::to_string(pid) + "/cmdline";
            std::ifstream cmdline_file(cmdline_path);
            if (cmdline_file.is_open()) {
                std::getline(cmdline_file, proc.cmdline);
                // Replace null characters with spaces
                std::replace(proc.cmdline.begin(), proc.cmdline.end(), '\0', ' ');
                if (proc.cmdline.empty()) {
                    proc.cmdline = "[" + proc.name + "]";
                }
            }
            
            // Get process owner
            std::string status_path = "/proc/" + std::to_string(pid) + "/status";
            std::ifstream status_file(status_path);
            if (status_file.is_open()) {
                std::string status_line;
                while (std::getline(status_file, status_line)) {
                    if (status_line.find("Uid:") == 0) {
                        std::istringstream uid_stream(status_line);
                        std::string uid_label, uid_str;
                        uid_stream >> uid_label >> uid_str;
                        uid_t uid = std::stoi(uid_str);
                        proc.user = getUserName(uid);
                        break;
                    }
                }
            }
            
            return proc;
        }

        /**
         * Calculate CPU and memory percentages
         */
        void calculateResourceUsage(std::vector<ProcessInfo>& processes) {
            // Get total memory from /proc/meminfo
            unsigned long total_memory_kb = 0;
            std::ifstream meminfo("/proc/meminfo");
            std::string line;
            while (std::getline(meminfo, line)) {
                if (line.find("MemTotal:") == 0) {
                    std::istringstream iss(line);
                    std::string label, value, unit;
                    iss >> label >> value >> unit;
                    total_memory_kb = std::stoul(value);
                    break;
                }
            }
            
            // Calculate memory percentages
            for (auto& proc : processes) {
                if (total_memory_kb > 0) {
                    proc.memory_percent = static_cast<double>(proc.memory_kb) / total_memory_kb * 100.0;
                }
                // CPU percentage calculation would require multiple samples over time
                // For now, we'll set it to 0 or implement a simplified version
                proc.cpu_percent = 0.0;
            }
        }

        /**
         * Get list of all processes
         */
        std::vector<ProcessInfo> getAllProcesses() {
            std::vector<ProcessInfo> processes;
            DIR* proc_dir = opendir("/proc");
            
            if (!proc_dir) {
                return processes;
            }
            
            struct dirent* entry;
            while ((entry = readdir(proc_dir)) != nullptr) {
                // Check if directory name is numeric (PID)
                if (std::regex_match(entry->d_name, std::regex("^[0-9]+$"))) {
                    int pid = std::stoi(entry->d_name);
                    ProcessInfo proc = parseProcessStat(pid);
                    if (!proc.name.empty()) {
                        processes.push_back(proc);
                    }
                }
            }
            
            closedir(proc_dir);
            
            calculateResourceUsage(processes);
            
            // Sort by memory usage (descending)
            std::sort(processes.begin(), processes.end(), 
                [](const ProcessInfo& a, const ProcessInfo& b) {
                    return a.memory_kb > b.memory_kb;
                });
            
            return processes;
        }

        /**
         * Get top processes by memory usage
         */
        std::vector<ProcessInfo> getTopProcesses(int limit = 20) {
            std::vector<ProcessInfo> all_processes = getAllProcesses();
            
            if (all_processes.size() > static_cast<size_t>(limit)) {
                all_processes.resize(limit);
            }
            
            return all_processes;
        }

        /**
         * Kill a process by PID (with security checks)
         */
        bool killProcess(int pid, int signal = SIGTERM) {
            // Security check: don't allow killing system critical processes
            std::vector<int> protected_pids = {1, 2}; // init and kthreadd
            
            if (std::find(protected_pids.begin(), protected_pids.end(), pid) != protected_pids.end()) {
                return false;
            }
            
            // Check if process exists and we have permission
            if (kill(pid, 0) == 0) {
                return kill(pid, signal) == 0;
            }
            
            return false;
        }

        /**
         * Get process details by PID
         */
        ProcessInfo getProcessDetails(int pid) {
            return parseProcessStat(pid);
        }

        /**
         * Get system resource summary
         */
        Json::Value getResourceSummary() {
            Json::Value summary;
            std::vector<ProcessInfo> processes = getAllProcesses();
            
            summary["total_processes"] = static_cast<int>(processes.size());
            
            // Count processes by state
            std::unordered_map<std::string, int> state_counts;
            double total_memory_usage = 0.0;
            
            for (const auto& proc : processes) {
                state_counts[proc.state]++;
                total_memory_usage += proc.memory_percent;
            }
            
            Json::Value states;
            for (const auto& state : state_counts) {
                states[state.first] = state.second;
            }
            
            summary["process_states"] = states;
            summary["total_memory_usage_percent"] = total_memory_usage;
            
            // Get load average
            std::ifstream loadavg("/proc/loadavg");
            if (loadavg.is_open()) {
                double load1, load5, load15;
                loadavg >> load1 >> load5 >> load15;
                
                summary["load_average"]["1min"] = load1;
                summary["load_average"]["5min"] = load5;
                summary["load_average"]["15min"] = load15;
            }
            
            return summary;
        }
    }
}

// C interface for the module
extern "C" {
    /**
     * Get list of all processes as JSON
     */
    char* list_processes() {
        try {
            Json::Value result;
            std::vector<MedusaCP::ProcessManager::ProcessInfo> processes = 
                MedusaCP::ProcessManager::getAllProcesses();
            
            Json::Value process_array(Json::arrayValue);
            
            for (const auto& proc : processes) {
                Json::Value process_json;
                process_json["pid"] = proc.pid;
                process_json["name"] = proc.name;
                process_json["state"] = proc.state;
                process_json["user"] = proc.user;
                process_json["cpu_percent"] = proc.cpu_percent;
                process_json["memory_percent"] = proc.memory_percent;
                process_json["memory_kb"] = static_cast<int64_t>(proc.memory_kb);
                process_json["virtual_memory_kb"] = static_cast<int64_t>(proc.virtual_memory_kb);
                process_json["cmdline"] = proc.cmdline;
                process_json["ppid"] = proc.ppid;
                process_json["threads"] = proc.threads;
                process_json["start_time"] = static_cast<int64_t>(proc.start_time);
                
                process_array.append(process_json);
            }
            
            result["processes"] = process_array;
            result["total_count"] = static_cast<int>(processes.size());
            result["timestamp"] = static_cast<int64_t>(std::time(nullptr));
            result["module"] = "process_manager";
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, result);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
            
        } catch (const std::exception& e) {
            Json::Value error;
            error["error"] = "Process listing failed";
            error["details"] = e.what();
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, error);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
        }
    }
    
    /**
     * Get top processes by memory usage
     */
    char* get_top_processes(int limit) {
        try {
            Json::Value result;
            std::vector<MedusaCP::ProcessManager::ProcessInfo> processes = 
                MedusaCP::ProcessManager::getTopProcesses(limit);
            
            Json::Value process_array(Json::arrayValue);
            
            for (const auto& proc : processes) {
                Json::Value process_json;
                process_json["pid"] = proc.pid;
                process_json["name"] = proc.name;
                process_json["user"] = proc.user;
                process_json["memory_percent"] = proc.memory_percent;
                process_json["memory_kb"] = static_cast<int64_t>(proc.memory_kb);
                process_json["cmdline"] = proc.cmdline.substr(0, 100); // Truncate for display
                
                process_array.append(process_json);
            }
            
            result["top_processes"] = process_array;
            result["limit"] = limit;
            result["timestamp"] = static_cast<int64_t>(std::time(nullptr));
            result["module"] = "process_manager";
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, result);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
            
        } catch (const std::exception& e) {
            Json::Value error;
            error["error"] = "Top processes query failed";
            error["details"] = e.what();
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, error);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
        }
    }
    
    /**
     * Get resource summary
     */
    char* get_resource_summary() {
        try {
            Json::Value summary = MedusaCP::ProcessManager::getResourceSummary();
            summary["timestamp"] = static_cast<int64_t>(std::time(nullptr));
            summary["module"] = "process_manager";
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, summary);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
            
        } catch (const std::exception& e) {
            Json::Value error;
            error["error"] = "Resource summary failed";
            error["details"] = e.what();
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, error);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
        }
    }
    
    /**
     * Kill a process (with security validation)
     */
    char* kill_process(int pid, int signal) {
        try {
            Json::Value result;
            
            bool success = MedusaCP::ProcessManager::killProcess(pid, signal);
            
            result["success"] = success;
            result["pid"] = pid;
            result["signal"] = signal;
            result["message"] = success ? "Process terminated" : "Failed to terminate process";
            result["timestamp"] = static_cast<int64_t>(std::time(nullptr));
            result["module"] = "process_manager";
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, result);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
            
        } catch (const std::exception& e) {
            Json::Value error;
            error["error"] = "Process termination failed";
            error["details"] = e.what();
            error["pid"] = pid;
            
            Json::StreamWriterBuilder builder;
            std::string json_string = Json::writeString(builder, error);
            
            char* response = (char*)malloc(json_string.length() + 1);
            strcpy(response, json_string.c_str());
            return response;
        }
    }
}