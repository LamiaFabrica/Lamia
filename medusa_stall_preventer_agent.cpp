#include "medusa_stall_preventer_agent.hpp"
#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#include <unistd.h>
#endif

using json = nlohmann::json;

namespace MedusaServ {

std::unique_ptr<StallPreventerAgent> StallPreventerAgent::s_instance;
std::mutex StallPreventerAgent::s_mutex;

StallPreventerAgent& StallPreventerAgent::Instance() {
    std::lock_guard<std::mutex> lock(s_mutex);
    if (!s_instance) {
        s_instance.reset(new StallPreventerAgent());
    }
    return *s_instance;
}

StallPreventerAgent::StallPreventerAgent() = default;

StallPreventerAgent::~StallPreventerAgent() {
    Stop();
}

void StallPreventerAgent::Initialize(const std::string& configPath) {
    // Simple logging since Logger might not be fully ready or we want to avoid circular deps
    std::cout << "[StallPreventer] Initializing from " << configPath << std::endl;
    if (!LoadConfig(configPath)) {
        std::cout << "[StallPreventer] Failed to load config, using defaults" << std::endl;
    }
    
    // Initialize activity map for monitored agents with current time
    std::lock_guard<std::mutex> lock(m_activityMutex);
    auto now = std::chrono::steady_clock::now();
    for (const auto& agent : m_config.monitored_agents) {
        m_agentLastActivity[agent] = now;
    }
}

bool StallPreventerAgent::LoadConfig(const std::string& path) {
    try {
        std::ifstream file(path);
        if (!file.is_open()) return false;
        
        json j;
        file >> j;
        
        if (j.contains("interval_seconds")) m_config.interval_seconds = j["interval_seconds"];
        if (j.contains("stall_threshold_seconds")) m_config.stall_threshold_seconds = j["stall_threshold_seconds"];
        if (j.contains("monitored_agents")) {
            m_config.monitored_agents.clear();
            for (const auto& agent : j["monitored_agents"]) {
                m_config.monitored_agents.push_back(agent);
            }
        }
        return true;
    } catch (...) {
        return false;
    }
}

void StallPreventerAgent::Start() {
    if (m_running) return;
    m_running = true;
    m_monitoringThread = std::jthread([this](std::stop_token st) {
        while (!st.stop_requested() && m_running) {
            CheckForStalls();
            std::this_thread::sleep_for(std::chrono::seconds(m_config.interval_seconds));
        }
    });
    std::cout << "[StallPreventer] Started monitoring loop" << std::endl;
}

void StallPreventerAgent::Stop() {
    m_running = false;
    if (m_monitoringThread.joinable()) {
        m_monitoringThread.request_stop();
        m_monitoringThread.join();
    }
}

void StallPreventerAgent::RegisterAgentActivity(const std::string& agentName) {
    std::lock_guard<std::mutex> lock(m_activityMutex);
    m_agentLastActivity[agentName] = std::chrono::steady_clock::now();
}

void StallPreventerAgent::CheckForStalls() {
    auto now = std::chrono::steady_clock::now();
    bool stallDetected = false;
    
    std::lock_guard<std::mutex> lock(m_activityMutex);
    for (const auto& [agent, lastTime] : m_agentLastActivity) {
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastTime).count();
        if (duration > m_config.stall_threshold_seconds) {
            std::cout << "[StallPreventer] STALL DETECTED: Agent '" << agent << "' stalled for " << duration << "s" << std::endl;
            stallDetected = true;
        }
    }
    
    if (stallDetected) {
        SimulateEnterKey();
        NotifyRecovery();
        
        // Reset timers to avoid repeated firing immediately
        auto resetTime = std::chrono::steady_clock::now();
        for (auto& [agent, lastTime] : m_agentLastActivity) {
            lastTime = resetTime;
        }
    }
}

void StallPreventerAgent::SimulateEnterKey() {
    std::cout << "[StallPreventer] Simulating ENTER key press to unstall..." << std::endl;
#ifdef _WIN32
    // Windows native API
    keybd_event(VK_RETURN, 0, 0, 0);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
#else
    // Linux native API fallback
    int ret = std::system("xdotool key Return");
    if (ret != 0) {
        std::cout << "\n" << std::endl; 
    }
#endif
}

void StallPreventerAgent::NotifyRecovery() {
    std::cout << "[StallPreventer] Recovery action taken. Notifying agents..." << std::endl;
}

} // namespace MedusaServ


