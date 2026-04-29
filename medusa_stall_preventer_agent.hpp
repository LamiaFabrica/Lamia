#pragma once

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>
#include <map>
#include <mutex>
#include "medusa_logger_core.hpp"
#include "nlohmann/json.hpp"

namespace MedusaServ {

class StallPreventerAgent {
public:
    struct Config {
        int interval_seconds = 1;
        int stall_threshold_seconds = 3;
        std::vector<std::string> monitored_agents;
    };

    static StallPreventerAgent& Instance();

    void Initialize(const std::string& configPath = "audits/stall_config.json");
    void Start();
    void Stop();
    
    // Agent API
    void RegisterAgentActivity(const std::string& agentName);
    void NotifyRecovery(); // Call all agents post-fix

    ~StallPreventerAgent();

private:
    StallPreventerAgent();
    StallPreventerAgent(const StallPreventerAgent&) = delete;
    StallPreventerAgent& operator=(const StallPreventerAgent&) = delete;

    bool LoadConfig(const std::string& path);
    void MonitoringLoop();
    void SimulateEnterKey();
    void CheckForStalls();

    Config m_config;
    std::atomic<bool> m_running{false};
    std::jthread m_monitoringThread;
    
    std::mutex m_activityMutex;
    std::map<std::string, std::chrono::steady_clock::time_point> m_agentLastActivity;
    
    static std::unique_ptr<StallPreventerAgent> s_instance;
    static std::mutex s_mutex;
};

} // namespace MedusaServ

