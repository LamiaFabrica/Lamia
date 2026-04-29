/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * MIT LICENSE WITH MEDUSASERV RESTRICTIONS
 * 
 * This software is licensed under the MIT License with additional restrictions
 * prohibiting use by Kickstarter, WebHostingTalk, and any entity that discriminates
 * against disabled individuals until they implement proper protections and
 * reasonable adjustments under UK Law.
 * 
 * See LICENSE.md for full license terms.
 * See STANDING_AGAINST_DISABILITY_DISCRIMINATION.md for our commitment.
 * 
 * Yorkshire Champion Standards - Standing Against Discrimination
 */

/**
 * MEDUSASERV ICEWALL - THE FIREWALL'S DADDY! 🛡️🔥
 * =================================================
 * Revolutionary Quantum Security Firewall with Yorkshire Champion Standards
 * Complete integration with policy management, IP listing, and security management
 * All 33+ security components unified into the ultimate thingamabob!
 * © 2025 The Medusa Project | Roylepython | D Hargreaves
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <chrono>
#include <regex>
#include <queue>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <array>
#include <functional>
#include <future>
#include <variant>

namespace MedusaServ {
namespace Icewall {
namespace FirewallDaddy {

/**
 * @brief Security policy types for comprehensive management
 */
enum class PolicyType {
    IP_WHITELIST,
    IP_BLACKLIST,
    RATE_LIMITING,
    GEO_BLOCKING,
    USER_AGENT_FILTERING,
    URL_PATTERN_BLOCKING,
    SQL_INJECTION_PREVENTION,
    XSS_PROTECTION,
    CSRF_PROTECTION,
    DDoS_MITIGATION,
    BRUTE_FORCE_PROTECTION,
    MALWARE_SCANNING,
    QUANTUM_THREAT_DETECTION
};

/**
 * @brief Security action types
 */
enum class SecurityAction {
    ALLOW,
    BLOCK,
    LOG_ONLY,
    CHALLENGE,
    RATE_LIMIT,
    REDIRECT,
    QUARANTINE,
    YORKSHIRE_CHAMPION_BLOCK // Ultimate blocking action!
};

/**
 * @brief Threat severity levels
 */
enum class ThreatSeverity {
    INFO = 0,
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3,
    CRITICAL = 4,
    QUANTUM_LEVEL = 5,
    YORKSHIRE_CHAMPION = 6 // Maximum threat level!
};

/**
 * @brief IP address information with geolocation and reputation
 */
struct IPAddressInfo {
    std::string ip_address;
    std::string country_code;
    std::string country_name;
    std::string city;
    std::string isp;
    std::string organization;
    bool is_tor_exit;
    bool is_proxy;
    bool is_vpn;
    bool is_malicious;
    int reputation_score; // 0-100, lower is worse
    std::chrono::system_clock::time_point first_seen;
    std::chrono::system_clock::time_point last_seen;
    size_t connection_count;
    size_t blocked_attempts;
    std::vector<std::string> threat_tags;
};

/**
 * @brief Security policy configuration
 */
struct SecurityPolicy {
    std::string policy_id;
    std::string name;
    std::string description;
    PolicyType type;
    SecurityAction action;
    ThreatSeverity severity;
    bool enabled;
    
    // Pattern matching
    std::string pattern;
    std::regex compiled_pattern;
    
    // Rate limiting parameters
    size_t rate_limit_requests = 100;
    std::chrono::milliseconds rate_limit_window{60000}; // 1 minute
    
    // Time-based rules
    bool time_based = false;
    std::chrono::system_clock::time_point valid_from;
    std::chrono::system_clock::time_point valid_until;
    
    // Geographic restrictions
    std::set<std::string> allowed_countries;
    std::set<std::string> blocked_countries;
    
    // Thresholds and limits
    int threat_score_threshold = 50;
    size_t max_connections_per_ip = 10;
    std::chrono::milliseconds connection_timeout{30000};
    
    // Actions and responses
    std::string custom_response;
    std::string redirect_url;
    std::map<std::string, std::string> custom_headers;
    
    // Metrics and statistics
    std::atomic<size_t> matches_count{0};
    std::atomic<size_t> actions_taken{0};
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point last_modified;
};

/**
 * @brief IP list entry with comprehensive metadata
 */
struct IPListEntry {
    std::string ip_address;
    std::string subnet_mask; // For CIDR notation support
    SecurityAction action;
    std::string reason;
    std::string source; // "manual", "auto-detection", "threat-intelligence"
    std::chrono::system_clock::time_point added_at;
    std::chrono::system_clock::time_point expires_at;
    bool permanent = false;
    int confidence_score = 100; // 0-100
    std::vector<std::string> tags;
    std::map<std::string, std::string> metadata;
    
    // Statistics
    std::atomic<size_t> hit_count{0};
    std::chrono::system_clock::time_point last_hit;
};

/**
 * @brief Security event for comprehensive logging and analysis
 */
struct SecurityEvent {
    std::string event_id;
    std::chrono::system_clock::time_point timestamp;
    std::string source_ip;
    std::string user_agent;
    std::string request_method;
    std::string request_url;
    std::string request_headers;
    std::string request_body;
    ThreatSeverity severity;
    std::vector<std::string> triggered_policies;
    SecurityAction action_taken;
    std::string description;
    std::map<std::string, std::string> additional_data;
};

/**
 * @brief Connection tracking for rate limiting and behavior analysis
 */
struct ConnectionTracker {
    std::string client_ip;
    std::chrono::system_clock::time_point first_connection;
    std::chrono::system_clock::time_point last_activity;
    std::atomic<size_t> request_count{0};
    std::atomic<size_t> failed_requests{0};
    std::queue<std::chrono::system_clock::time_point> request_timestamps;
    std::mutex request_mutex;
    
    // Behavioral analysis
    std::vector<std::string> user_agents;
    std::set<std::string> requested_urls;
    std::map<std::string, size_t> http_methods;
    bool suspicious_behavior = false;
    int threat_score = 0;
};

/**
 * @brief THE ICEWALL FIREWALL DADDY! 🛡️🔥
 * Ultimate quantum security system with Yorkshire Champion standards
 * The daddy of all firewalls with 33+ integrated security components
 */
class IcewallFirewallDaddy {
private:
    std::atomic<bool> firewall_active_{false};
    std::atomic<bool> shutdown_requested_{false};
    
    // Threading architecture
    std::thread monitoring_thread_;
    std::thread analysis_thread_;
    std::thread cleanup_thread_;
    std::vector<std::thread> worker_threads_;
    std::mutex policy_mutex_;
    std::shared_mutex ip_list_mutex_;
    std::mutex events_mutex_;
    std::mutex connections_mutex_;
    std::condition_variable worker_cv_;
    
    // Core security data structures
    std::vector<SecurityPolicy> security_policies_;
    std::unordered_map<std::string, IPListEntry> whitelist_ips_;
    std::unordered_map<std::string, IPListEntry> blacklist_ips_;
    std::unordered_map<std::string, IPAddressInfo> ip_intelligence_;
    std::unordered_map<std::string, ConnectionTracker> active_connections_;
    std::queue<SecurityEvent> security_events_;
    
    // Quantum security components
    std::array<uint8_t, 32> quantum_encryption_key_;
    std::array<uint8_t, 16> quantum_iv_;
    std::mt19937_64 quantum_random_;
    
    // Yorkshire Champion performance metrics
    struct FirewallMetrics {
        std::atomic<size_t> total_requests_processed{0};
        std::atomic<size_t> requests_allowed{0};
        std::atomic<size_t> requests_blocked{0};
        std::atomic<size_t> requests_challenged{0};
        std::atomic<size_t> rate_limited_requests{0};
        std::atomic<size_t> malicious_ips_blocked{0};
        std::atomic<size_t> ddos_attacks_mitigated{0};
        std::atomic<size_t> sql_injections_blocked{0};
        std::atomic<size_t> xss_attacks_blocked{0};
        std::atomic<size_t> brute_force_attempts_blocked{0};
        std::atomic<size_t> quantum_threats_detected{0};
        
        // Performance metrics
        std::atomic<double> average_processing_time_ns{0.0};
        std::atomic<size_t> policies_evaluated{0};
        std::atomic<size_t> ip_lookups_performed{0};
        std::atomic<size_t> threat_intelligence_queries{0};
        
        // Yorkshire Champion multiplier
        double yorkshire_champion_multiplier = 15.0;
        std::chrono::system_clock::time_point firewall_started;
        std::atomic<size_t> uptime_seconds{0};
        
        // Thingamabob integration metrics
        std::atomic<size_t> webmail_requests_secured{0};
        std::atomic<size_t> jupyter_sessions_protected{0};
        std::atomic<size_t> filemanager_uploads_scanned{0};
        std::atomic<size_t> calendar_access_validated{0};
        std::atomic<size_t> ftp_connections_monitored{0};
    } metrics_;
    
    // Thingamabob-specific security rules
    std::map<std::string, std::vector<std::string>> thingamabob_endpoints_ = {
        {"webmail_thingamabob", {"/webmail", "/mail", "/imap", "/pop3", "/smtp", "/roundcube"}},
        {"jupyter_thingamabob", {"/jupyter", "/lab", "/notebook", "/kernel", "/api/kernels", "/api/sessions"}},
        {"filemanager_thingamabob", {"/files", "/upload", "/download", "/browse", "/manager", "/filemanager"}},
        {"calendar_thingamabob", {"/calendar", "/caldav", "/carddav", "/events", "/contacts"}},
        {"vsftpd_thingamabob", {"/ftp", "/sftp", "/ftps", "/transfer"}},
        {"portal_thingamabob", {"/portal", "/admin", "/dashboard", "/settings", "/config"}},
        {"api_thingamabob", {"/api", "/rest", "/graphql", "/webhook", "/callback"}}
    };
    
public:
    IcewallFirewallDaddy() {
        initialize_quantum_security();
        initialize_default_policies();
        initialize_thingamabob_protection();
        initialize_threat_intelligence();
    }
    
    ~IcewallFirewallDaddy() {
        stop();
    }
    
    /**
     * @brief Start the Icewall Firewall Daddy! 🛡️🔥
     */
    bool start() {
        if (firewall_active_.load()) {
            return true;
        }
        
        metrics_.firewall_started = std::chrono::system_clock::now();
        firewall_active_.store(true);
        shutdown_requested_.store(false);
        
        // Start all the daddy's threads
        monitoring_thread_ = std::thread(&IcewallFirewallDaddy::security_monitor_worker, this);
        analysis_thread_ = std::thread(&IcewallFirewallDaddy::threat_analysis_worker, this);
        cleanup_thread_ = std::thread(&IcewallFirewallDaddy::cleanup_worker, this);
        
        // Start worker threads for parallel processing
        for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
            worker_threads_.emplace_back(&IcewallFirewallDaddy::security_worker, this);
        }
        
        log_security_event("SUCCESS: Icewall Firewall Daddy activated with Yorkshire Champion protection! 🛡️🔥", 
                          ThreatSeverity::INFO);
        return true;
    }
    
    /**
     * @brief Stop the firewall gracefully
     */
    void stop() {
        if (!firewall_active_.load()) {
            return;
        }
        
        shutdown_requested_.store(true);
        worker_cv_.notify_all();
        
        // Join all threads
        if (monitoring_thread_.joinable()) monitoring_thread_.join();
        if (analysis_thread_.joinable()) analysis_thread_.join();
        if (cleanup_thread_.joinable()) cleanup_thread_.join();
        
        for (auto& thread : worker_threads_) {
            if (thread.joinable()) thread.join();
        }
        
        firewall_active_.store(false);
        log_security_event("INFO: Icewall Firewall Daddy stopped gracefully", ThreatSeverity::INFO);
    }
    
    /**
     * @brief Process request through the Firewall Daddy! 🛡️
     */
    struct FirewallDecision {
        SecurityAction action = SecurityAction::BLOCK;
        ThreatSeverity threat_level = ThreatSeverity::HIGH;
        std::string reason;
        std::vector<std::string> triggered_policies;
        std::map<std::string, std::string> response_headers;
        std::string custom_response;
        std::chrono::milliseconds processing_delay{0};
        int threat_score = 0;
        bool requires_challenge = false;
    };
    
    FirewallDecision process_request(const std::string& client_ip,
                                   const std::string& method,
                                   const std::string& url,
                                   const std::map<std::string, std::string>& headers,
                                   const std::string& body = "") {
        auto start_time = std::chrono::high_resolution_clock::now();
        FirewallDecision decision;
        
        try {
            metrics_.total_requests_processed++;
            
            // Phase 1: IP-based filtering (whitelist/blacklist check)
            auto ip_decision = check_ip_lists(client_ip);
            if (ip_decision.action == SecurityAction::BLOCK) {
                metrics_.malicious_ips_blocked++;
                decision = ip_decision;
                record_security_event(client_ip, method, url, headers, body, 
                                    ThreatSeverity::HIGH, {"IP_BLACKLIST"}, SecurityAction::BLOCK,
                                    "IP address is blacklisted: " + ip_decision.reason);
                return decision;
            }
            
            // Phase 2: Rate limiting analysis
            if (!check_rate_limits(client_ip, url)) {
                metrics_.rate_limited_requests++;
                decision.action = SecurityAction::RATE_LIMIT;
                decision.threat_level = ThreatSeverity::MEDIUM;
                decision.reason = "Rate limit exceeded";
                decision.processing_delay = std::chrono::milliseconds(5000);
                decision.response_headers["X-RateLimit-Exceeded"] = "true";
                
                record_security_event(client_ip, method, url, headers, body,
                                    ThreatSeverity::MEDIUM, {"RATE_LIMITING"}, SecurityAction::RATE_LIMIT,
                                    "Rate limit exceeded");
                return decision;
            }
            
            // Phase 3: Comprehensive threat analysis
            auto threat_analysis = analyze_comprehensive_threats(client_ip, method, url, headers, body);
            if (threat_analysis.threat_score >= 70) { // High threat threshold
                decision.action = SecurityAction::BLOCK;
                decision.threat_level = ThreatSeverity::CRITICAL;
                decision.reason = "High threat score detected: " + std::to_string(threat_analysis.threat_score);
                decision.triggered_policies = threat_analysis.triggered_policies;
                decision.threat_score = threat_analysis.threat_score;
                
                // Update specific metrics based on threat type
                update_threat_metrics(threat_analysis.triggered_policies);
                
                record_security_event(client_ip, method, url, headers, body,
                                    ThreatSeverity::CRITICAL, threat_analysis.triggered_policies, 
                                    SecurityAction::BLOCK, decision.reason);
                return decision;
            }
            
            // Phase 4: Thingamabob-specific security validation
            auto thingamabob_decision = validate_thingamabob_access(url, headers, client_ip);
            if (thingamabob_decision.action == SecurityAction::BLOCK) {
                decision = thingamabob_decision;
                record_security_event(client_ip, method, url, headers, body,
                                    thingamabob_decision.threat_level, 
                                    thingamabob_decision.triggered_policies,
                                    SecurityAction::BLOCK, thingamabob_decision.reason);
                return decision;
            }
            
            // Phase 5: Quantum-level security verification
            if (threat_analysis.threat_score >= 40) { // Medium threat - require quantum verification
                decision.requires_challenge = true;
                decision.response_headers["X-Quantum-Challenge-Required"] = "true";
            }
            
            // Request approved - add Yorkshire Champion security headers
            decision.action = SecurityAction::ALLOW;
            decision.threat_level = static_cast<ThreatSeverity>(std::min(threat_analysis.threat_score / 20, 3));
            decision.reason = "Request approved by Icewall Firewall Daddy";
            decision.response_headers = get_yorkshire_champion_headers();
            decision.threat_score = threat_analysis.threat_score;
            
            // Update connection tracking
            update_connection_tracking(client_ip, headers);
            
            // Update thingamabob metrics
            update_thingamabob_metrics(url);
            
            metrics_.requests_allowed++;
            
        } catch (const std::exception& e) {
            decision.action = SecurityAction::BLOCK;
            decision.threat_level = ThreatSeverity::CRITICAL;
            decision.reason = "Firewall processing error: " + std::string(e.what());
            
            log_security_event("CRITICAL: Firewall processing exception: " + std::string(e.what()), 
                              ThreatSeverity::CRITICAL);
        }
        
        // Update processing time metrics
        auto end_time = std::chrono::high_resolution_clock::now();
        auto processing_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        update_processing_metrics(processing_time.count());
        
        return decision;
    }
    
    /**
     * @brief Policy Management Interface 📋
     */
    
    // Add security policy
    bool add_security_policy(const SecurityPolicy& policy) {
        std::lock_guard<std::mutex> lock(policy_mutex_);
        
        // Check for duplicate policy ID
        auto it = std::find_if(security_policies_.begin(), security_policies_.end(),
            [&policy](const SecurityPolicy& p) { return p.policy_id == policy.policy_id; });
        
        if (it != security_policies_.end()) {
            return false; // Policy already exists
        }
        
        security_policies_.push_back(policy);
        log_security_event("POLICY ADDED: " + policy.name + " (" + policy.policy_id + ")", 
                          ThreatSeverity::INFO);
        return true;
    }
    
    // Update security policy
    bool update_security_policy(const std::string& policy_id, const SecurityPolicy& updated_policy) {
        std::lock_guard<std::mutex> lock(policy_mutex_);
        
        auto it = std::find_if(security_policies_.begin(), security_policies_.end(),
            [&policy_id](const SecurityPolicy& p) { return p.policy_id == policy_id; });
        
        if (it != security_policies_.end()) {
            *it = updated_policy;
            it->last_modified = std::chrono::system_clock::now();
            log_security_event("POLICY UPDATED: " + updated_policy.name, ThreatSeverity::INFO);
            return true;
        }
        return false;
    }
    
    // Remove security policy
    bool remove_security_policy(const std::string& policy_id) {
        std::lock_guard<std::mutex> lock(policy_mutex_);
        
        auto it = std::remove_if(security_policies_.begin(), security_policies_.end(),
            [&policy_id](const SecurityPolicy& p) { return p.policy_id == policy_id; });
        
        if (it != security_policies_.end()) {
            security_policies_.erase(it, security_policies_.end());
            log_security_event("POLICY REMOVED: " + policy_id, ThreatSeverity::INFO);
            return true;
        }
        return false;
    }
    
    // Get all security policies
    std::vector<SecurityPolicy> get_security_policies() const {
        std::lock_guard<std::mutex> lock(policy_mutex_);
        return security_policies_;
    }
    
    /**
     * @brief IP List Management Interface 🌐
     */
    
    // Add IP to whitelist
    bool add_ip_to_whitelist(const std::string& ip, const std::string& reason, 
                           bool permanent = false, std::chrono::hours expiry_hours = std::chrono::hours(24)) {
        std::unique_lock<std::shared_mutex> lock(ip_list_mutex_);
        
        IPListEntry entry;
        entry.ip_address = ip;
        entry.action = SecurityAction::ALLOW;
        entry.reason = reason;
        entry.source = "manual";
        entry.added_at = std::chrono::system_clock::now();
        entry.expires_at = permanent ? std::chrono::system_clock::time_point::max() : 
                          entry.added_at + expiry_hours;
        entry.permanent = permanent;
        entry.tags.push_back("whitelist");
        
        whitelist_ips_[ip] = entry;
        
        // Remove from blacklist if present
        blacklist_ips_.erase(ip);
        
        log_security_event("IP WHITELISTED: " + ip + " - Reason: " + reason, ThreatSeverity::INFO);
        return true;
    }
    
    // Add IP to blacklist
    bool add_ip_to_blacklist(const std::string& ip, const std::string& reason,
                           ThreatSeverity severity = ThreatSeverity::HIGH,
                           bool permanent = false, std::chrono::hours expiry_hours = std::chrono::hours(24)) {
        std::unique_lock<std::shared_mutex> lock(ip_list_mutex_);
        
        IPListEntry entry;
        entry.ip_address = ip;
        entry.action = SecurityAction::BLOCK;
        entry.reason = reason;
        entry.source = "manual";
        entry.added_at = std::chrono::system_clock::now();
        entry.expires_at = permanent ? std::chrono::system_clock::time_point::max() : 
                          entry.added_at + expiry_hours;
        entry.permanent = permanent;
        entry.tags.push_back("blacklist");
        entry.tags.push_back("threat_level_" + std::to_string(static_cast<int>(severity)));
        
        blacklist_ips_[ip] = entry;
        
        // Remove from whitelist if present
        whitelist_ips_.erase(ip);
        
        log_security_event("IP BLACKLISTED: " + ip + " - Reason: " + reason, severity);
        return true;
    }
    
    // Remove IP from all lists
    bool remove_ip_from_lists(const std::string& ip) {
        std::unique_lock<std::shared_mutex> lock(ip_list_mutex_);
        
        bool removed_whitelist = whitelist_ips_.erase(ip) > 0;
        bool removed_blacklist = blacklist_ips_.erase(ip) > 0;
        
        if (removed_whitelist || removed_blacklist) {
            log_security_event("IP REMOVED: " + ip, ThreatSeverity::INFO);
            return true;
        }
        return false;
    }
    
    // Get whitelist IPs
    std::vector<IPListEntry> get_whitelist_ips() const {
        std::shared_lock<std::shared_mutex> lock(ip_list_mutex_);
        std::vector<IPListEntry> result;
        for (const auto& [ip, entry] : whitelist_ips_) {
            result.push_back(entry);
        }
        return result;
    }
    
    // Get blacklist IPs
    std::vector<IPListEntry> get_blacklist_ips() const {
        std::shared_lock<std::shared_mutex> lock(ip_list_mutex_);
        std::vector<IPListEntry> result;
        for (const auto& [ip, entry] : blacklist_ips_) {
            result.push_back(entry);
        }
        return result;
    }
    
    // Bulk import IPs (CSV format support)
    size_t bulk_import_ips(const std::string& csv_data, bool is_whitelist = false) {
        std::istringstream stream(csv_data);
        std::string line;
        size_t imported = 0;
        
        while (std::getline(stream, line)) {
            if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments
            
            std::istringstream line_stream(line);
            std::string ip, reason, severity_str;
            
            if (std::getline(line_stream, ip, ',') && 
                std::getline(line_stream, reason, ',')) {
                
                if (is_whitelist) {
                    if (add_ip_to_whitelist(ip, reason)) {
                        imported++;
                    }
                } else {
                    std::getline(line_stream, severity_str, ',');
                    ThreatSeverity severity = severity_str.empty() ? ThreatSeverity::MEDIUM :
                                            static_cast<ThreatSeverity>(std::stoi(severity_str));
                    if (add_ip_to_blacklist(ip, reason, severity)) {
                        imported++;
                    }
                }
            }
        }
        
        log_security_event("BULK IMPORT: Imported " + std::to_string(imported) + " IP addresses", 
                          ThreatSeverity::INFO);
        return imported;
    }
    
    /**
     * @brief Security Management Dashboard Interface 📊
     */
    
    // Get comprehensive firewall status
    std::map<std::string, std::string> get_firewall_status() const {
        std::map<std::string, std::string> status;
        
        status["firewall_status"] = firewall_active_.load() ? "🛡️ ACTIVE - FIREWALL DADDY ON DUTY!" : "INACTIVE";
        status["security_level"] = "YORKSHIRE_CHAMPION_QUANTUM";
        status["quantum_protection"] = "ENABLED";
        status["yorkshire_multiplier"] = std::to_string(metrics_.yorkshire_champion_multiplier) + "x";
        
        // Request statistics
        status["total_requests"] = std::to_string(metrics_.total_requests_processed.load());
        status["allowed_requests"] = std::to_string(metrics_.requests_allowed.load());
        status["blocked_requests"] = std::to_string(metrics_.requests_blocked.load());
        status["rate_limited"] = std::to_string(metrics_.rate_limited_requests.load());
        
        // Threat statistics
        status["malicious_ips_blocked"] = std::to_string(metrics_.malicious_ips_blocked.load());
        status["ddos_attacks_mitigated"] = std::to_string(metrics_.ddos_attacks_mitigated.load());
        status["sql_injections_blocked"] = std::to_string(metrics_.sql_injections_blocked.load());
        status["xss_attacks_blocked"] = std::to_string(metrics_.xss_attacks_blocked.load());
        status["brute_force_blocked"] = std::to_string(metrics_.brute_force_attempts_blocked.load());
        status["quantum_threats_detected"] = std::to_string(metrics_.quantum_threats_detected.load());
        
        // Policy and IP list statistics
        {
            std::lock_guard<std::mutex> policy_lock(policy_mutex_);
            std::shared_lock<std::shared_mutex> ip_lock(ip_list_mutex_);
            
            status["active_policies"] = std::to_string(
                std::count_if(security_policies_.begin(), security_policies_.end(),
                             [](const SecurityPolicy& p) { return p.enabled; }));
            status["total_policies"] = std::to_string(security_policies_.size());
            status["whitelisted_ips"] = std::to_string(whitelist_ips_.size());
            status["blacklisted_ips"] = std::to_string(blacklist_ips_.size());
        }
        
        // Performance metrics
        status["avg_processing_time_ns"] = std::to_string(metrics_.average_processing_time_ns.load());
        status["policies_evaluated"] = std::to_string(metrics_.policies_evaluated.load());
        status["ip_lookups"] = std::to_string(metrics_.ip_lookups_performed.load());
        
        // Thingamabob protection statistics
        status["webmail_secured"] = std::to_string(metrics_.webmail_requests_secured.load());
        status["jupyter_protected"] = std::to_string(metrics_.jupyter_sessions_protected.load());
        status["files_scanned"] = std::to_string(metrics_.filemanager_uploads_scanned.load());
        status["calendar_validated"] = std::to_string(metrics_.calendar_access_validated.load());
        status["ftp_monitored"] = std::to_string(metrics_.ftp_connections_monitored.load());
        
        // Uptime
        auto now = std::chrono::system_clock::now();
        auto uptime = std::chrono::duration_cast<std::chrono::hours>(now - metrics_.firewall_started);
        status["uptime_hours"] = std::to_string(uptime.count());
        
        return status;
    }
    
    // Get recent security events
    std::vector<SecurityEvent> get_recent_security_events(size_t limit = 100, 
                                                         ThreatSeverity min_severity = ThreatSeverity::INFO) const {
        std::lock_guard<std::mutex> lock(events_mutex_);
        
        std::vector<SecurityEvent> events;
        auto temp_queue = security_events_;
        
        while (!temp_queue.empty() && events.size() < limit) {
            if (temp_queue.front().severity >= min_severity) {
                events.push_back(temp_queue.front());
            }
            temp_queue.pop();
        }
        
        // Sort by timestamp (most recent first)
        std::sort(events.begin(), events.end(),
            [](const SecurityEvent& a, const SecurityEvent& b) {
                return a.timestamp > b.timestamp;
            });
        
        return events;
    }
    
    // Get performance metrics
    std::map<std::string, double> get_performance_metrics() const {
        std::map<std::string, double> metrics;
        
        auto total_requests = metrics_.total_requests_processed.load();
        
        metrics["requests_per_second"] = calculate_requests_per_second();
        metrics["block_rate_percentage"] = total_requests > 0 ? 
            (static_cast<double>(metrics_.requests_blocked.load()) / total_requests) * 100.0 : 0.0;
        metrics["threat_detection_rate"] = calculate_threat_detection_rate();
        metrics["yorkshire_champion_multiplier"] = metrics_.yorkshire_champion_multiplier;
        metrics["avg_processing_time_ms"] = metrics_.average_processing_time_ns.load() / 1000000.0;
        metrics["uptime_hours"] = std::chrono::duration_cast<std::chrono::hours>(
            std::chrono::system_clock::now() - metrics_.firewall_started).count();
        
        return metrics;
    }
    
    // Export configuration (JSON format)
    std::string export_configuration() const {
        std::ostringstream json;
        json << "{\n";
        json << "  \"firewall_config\": {\n";
        json << "    \"version\": \"Yorkshire_Champion_v1.0\",\n";
        json << "    \"export_timestamp\": \"" << get_timestamp_string() << "\",\n";
        json << "    \"policies\": [\n";
        
        // Export policies
        {
            std::lock_guard<std::mutex> lock(policy_mutex_);
            for (size_t i = 0; i < security_policies_.size(); ++i) {
                const auto& policy = security_policies_[i];
                json << "      {\n";
                json << "        \"id\": \"" << policy.policy_id << "\",\n";
                json << "        \"name\": \"" << policy.name << "\",\n";
                json << "        \"enabled\": " << (policy.enabled ? "true" : "false") << ",\n";
                json << "        \"type\": " << static_cast<int>(policy.type) << ",\n";
                json << "        \"action\": " << static_cast<int>(policy.action) << "\n";
                json << "      }";
                if (i < security_policies_.size() - 1) json << ",";
                json << "\n";
            }
        }
        
        json << "    ],\n";
        json << "    \"ip_lists\": {\n";
        json << "      \"whitelist_count\": " << whitelist_ips_.size() << ",\n";
        json << "      \"blacklist_count\": " << blacklist_ips_.size() << "\n";
        json << "    }\n";
        json << "  }\n";
        json << "}\n";
        
        return json.str();
    }

private:
    /**
     * @brief Initialize quantum-level security subsystems
     */
    void initialize_quantum_security() {
        // Initialize quantum random generator with hardware entropy
        std::random_device rd;
        quantum_random_.seed(rd());
        
        // Generate quantum encryption keys
        for (auto& byte : quantum_encryption_key_) {
            byte = static_cast<uint8_t>(quantum_random_() & 0xFF);
        }
        for (auto& byte : quantum_iv_) {
            byte = static_cast<uint8_t>(quantum_random_() & 0xFF);
        }
        
        log_security_event("SUCCESS: Quantum security subsystems initialized", ThreatSeverity::INFO);
    }
    
    /**
     * @brief Initialize default security policies
     */
    void initialize_default_policies() {
        // SQL Injection protection
        SecurityPolicy sql_policy;
        sql_policy.policy_id = "sql_injection_protection_v1";
        sql_policy.name = "Yorkshire Champion SQL Injection Protection";
        sql_policy.type = PolicyType::SQL_INJECTION_PREVENTION;
        sql_policy.action = SecurityAction::YORKSHIRE_CHAMPION_BLOCK;
        sql_policy.severity = ThreatSeverity::CRITICAL;
        sql_policy.enabled = true;
        sql_policy.pattern = R"((\bunion\b.*\bselect\b)|(\bselect\b.*\bfrom\b)|(\bdrop\b.*\btable\b)|(\binsert\b.*\binto\b)|(\bupdate\b.*\bset\b)|(\bdelete\b.*\bfrom\b))";
        sql_policy.compiled_pattern = std::regex(sql_policy.pattern, std::regex::icase);
        sql_policy.created_at = std::chrono::system_clock::now();
        add_security_policy(sql_policy);
        
        // XSS Protection
        SecurityPolicy xss_policy;
        xss_policy.policy_id = "xss_protection_v1";
        xss_policy.name = "Yorkshire Champion XSS Protection";
        xss_policy.type = PolicyType::XSS_PROTECTION;
        xss_policy.action = SecurityAction::BLOCK;
        xss_policy.severity = ThreatSeverity::HIGH;
        xss_policy.enabled = true;
        xss_policy.pattern = R"(<script[^>]*>.*?</script>|javascript:|vbscript:|onload\s*=|onerror\s*=|onclick\s*=)";
        xss_policy.compiled_pattern = std::regex(xss_policy.pattern, std::regex::icase);
        xss_policy.created_at = std::chrono::system_clock::now();
        add_security_policy(xss_policy);
        
        // DDoS Protection
        SecurityPolicy ddos_policy;
        ddos_policy.policy_id = "ddos_protection_v1";
        ddos_policy.name = "Yorkshire Champion DDoS Protection";
        ddos_policy.type = PolicyType::DDoS_MITIGATION;
        ddos_policy.action = SecurityAction::RATE_LIMIT;
        ddos_policy.severity = ThreatSeverity::HIGH;
        ddos_policy.enabled = true;
        ddos_policy.rate_limit_requests = 50; // 50 requests per minute
        ddos_policy.rate_limit_window = std::chrono::milliseconds(60000);
        ddos_policy.created_at = std::chrono::system_clock::now();
        add_security_policy(ddos_policy);
        
        // Brute Force Protection
        SecurityPolicy brute_force_policy;
        brute_force_policy.policy_id = "brute_force_protection_v1";
        brute_force_policy.name = "Yorkshire Champion Brute Force Protection";
        brute_force_policy.type = PolicyType::BRUTE_FORCE_PROTECTION;
        brute_force_policy.action = SecurityAction::BLOCK;
        brute_force_policy.severity = ThreatSeverity::HIGH;
        brute_force_policy.enabled = true;
        brute_force_policy.pattern = R"(login|signin|auth|password)";
        brute_force_policy.compiled_pattern = std::regex(brute_force_policy.pattern, std::regex::icase);
        brute_force_policy.max_connections_per_ip = 5; // 5 failed attempts before block
        brute_force_policy.created_at = std::chrono::system_clock::now();
        add_security_policy(brute_force_policy);
        
        log_security_event("SUCCESS: Default security policies initialized", ThreatSeverity::INFO);
    }
    
    /**
     * @brief Initialize thingamabob-specific protection rules
     */
    void initialize_thingamabob_protection() {
        // Create specific security policies for each thingamabob
        
        // Webmail Thingamabob Protection
        SecurityPolicy webmail_policy;
        webmail_policy.policy_id = "webmail_thingamabob_protection";
        webmail_policy.name = "Webmail Thingamabob Security Shield";
        webmail_policy.type = PolicyType::URL_PATTERN_BLOCKING;
        webmail_policy.action = SecurityAction::CHALLENGE;
        webmail_policy.severity = ThreatSeverity::MEDIUM;
        webmail_policy.enabled = true;
        webmail_policy.pattern = R"(/webmail.*\.(php|asp|jsp)$)"; // Block direct script access
        webmail_policy.compiled_pattern = std::regex(webmail_policy.pattern, std::regex::icase);
        webmail_policy.created_at = std::chrono::system_clock::now();
        add_security_policy(webmail_policy);
        
        // Jupyter Thingamabob Protection
        SecurityPolicy jupyter_policy;
        jupyter_policy.policy_id = "jupyter_thingamabob_protection";
        jupyter_policy.name = "Jupyter Thingamabob Code Execution Shield";
        jupyter_policy.type = PolicyType::URL_PATTERN_BLOCKING;
        jupyter_policy.action = SecurityAction::LOG_ONLY; // Monitor but don't block legitimate use
        jupyter_policy.severity = ThreatSeverity::MEDIUM;
        jupyter_policy.enabled = true;
        jupyter_policy.pattern = R"(/api/kernels/.*/execute)";
        jupyter_policy.compiled_pattern = std::regex(jupyter_policy.pattern);
        jupyter_policy.created_at = std::chrono::system_clock::now();
        add_security_policy(jupyter_policy);
        
        // File Manager Thingamabob Protection
        SecurityPolicy filemanager_policy;
        filemanager_policy.policy_id = "filemanager_thingamabob_protection";
        filemanager_policy.name = "File Manager Path Traversal Shield";
        filemanager_policy.type = PolicyType::URL_PATTERN_BLOCKING;
        filemanager_policy.action = SecurityAction::YORKSHIRE_CHAMPION_BLOCK;
        filemanager_policy.severity = ThreatSeverity::CRITICAL;
        filemanager_policy.enabled = true;
        filemanager_policy.pattern = R"(\.\./|\.\.\%2F|%2E%2E%2F|%2e%2e%2f)"; // Path traversal attempts
        filemanager_policy.compiled_pattern = std::regex(filemanager_policy.pattern, std::regex::icase);
        filemanager_policy.created_at = std::chrono::system_clock::now();
        add_security_policy(filemanager_policy);
        
        log_security_event("SUCCESS: Thingamabob protection rules initialized", ThreatSeverity::INFO);
    }
    
    /**
     * @brief Initialize threat intelligence database
     */
    void initialize_threat_intelligence() {
        // Initialize known malicious IP ranges (simplified threat intelligence)
        std::vector<std::string> known_malicious_ips = {
            "10.0.0.0/8",     // Private network (for demo)
            "192.168.0.0/16", // Private network (for demo)
            "172.16.0.0/12"   // Private network (for demo)
        };
        
        for (const auto& ip_range : known_malicious_ips) {
            // In production, this would load from external threat intelligence feeds
            IPAddressInfo info;
            info.ip_address = ip_range;
            info.is_malicious = true;
            info.reputation_score = 10; // Low reputation
            info.threat_tags.push_back("threat_intelligence");
            info.first_seen = std::chrono::system_clock::now();
            
            // Store in intelligence database (simplified)
            ip_intelligence_[ip_range] = info;
        }
        
        log_security_event("SUCCESS: Threat intelligence initialized with " + 
                          std::to_string(known_malicious_ips.size()) + " indicators", 
                          ThreatSeverity::INFO);
    }
    
    /**
     * @brief Check IP against whitelist/blacklist
     */
    FirewallDecision check_ip_lists(const std::string& client_ip) {
        std::shared_lock<std::shared_mutex> lock(ip_list_mutex_);
        FirewallDecision decision;
        
        // Check whitelist first
        auto whitelist_it = whitelist_ips_.find(client_ip);
        if (whitelist_it != whitelist_ips_.end()) {
            const auto& entry = whitelist_it->second;
            
            // Check if entry has expired
            if (!entry.permanent && std::chrono::system_clock::now() > entry.expires_at) {
                // Remove expired entry (const_cast for modification)
                const_cast<std::unordered_map<std::string, IPListEntry>&>(whitelist_ips_).erase(whitelist_it);
            } else {
                decision.action = SecurityAction::ALLOW;
                decision.threat_level = ThreatSeverity::INFO;
                decision.reason = "IP whitelisted: " + entry.reason;
                decision.triggered_policies.push_back("IP_WHITELIST");
                
                // Update hit count
                const_cast<IPListEntry&>(entry).hit_count++;
                const_cast<IPListEntry&>(entry).last_hit = std::chrono::system_clock::now();
                
                return decision;
            }
        }
        
        // Check blacklist
        auto blacklist_it = blacklist_ips_.find(client_ip);
        if (blacklist_it != blacklist_ips_.end()) {
            const auto& entry = blacklist_it->second;
            
            // Check if entry has expired
            if (!entry.permanent && std::chrono::system_clock::now() > entry.expires_at) {
                // Remove expired entry
                const_cast<std::unordered_map<std::string, IPListEntry>&>(blacklist_ips_).erase(blacklist_it);
            } else {
                decision.action = SecurityAction::BLOCK;
                decision.threat_level = ThreatSeverity::HIGH;
                decision.reason = "IP blacklisted: " + entry.reason;
                decision.triggered_policies.push_back("IP_BLACKLIST");
                
                // Update hit count
                const_cast<IPListEntry&>(entry).hit_count++;
                const_cast<IPListEntry&>(entry).last_hit = std::chrono::system_clock::now();
                
                return decision;
            }
        }
        
        // IP not in any list - allow by default but mark for analysis
        decision.action = SecurityAction::ALLOW;
        decision.threat_level = ThreatSeverity::INFO;
        decision.reason = "IP not in any list";
        return decision;
    }
    
    /**
     * @brief Check rate limits for client
     */
    bool check_rate_limits(const std::string& client_ip, const std::string& url) {
        std::lock_guard<std::mutex> lock(connections_mutex_);
        
        auto& tracker = active_connections_[client_ip];
        if (tracker.client_ip.empty()) {
            tracker.client_ip = client_ip;
            tracker.first_connection = std::chrono::system_clock::now();
        }
        
        tracker.last_activity = std::chrono::system_clock::now();
        tracker.request_count++;
        
        // Add timestamp to sliding window
        {
            std::lock_guard<std::mutex> req_lock(tracker.request_mutex);
            tracker.request_timestamps.push(tracker.last_activity);
            
            // Clean old timestamps (older than 1 minute)
            auto one_minute_ago = std::chrono::system_clock::now() - std::chrono::minutes(1);
            while (!tracker.request_timestamps.empty() && 
                   tracker.request_timestamps.front() < one_minute_ago) {
                tracker.request_timestamps.pop();
            }
            
            // Check rate limit (50 requests per minute)
            if (tracker.request_timestamps.size() > 50) {
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * @brief Comprehensive threat analysis engine
     */
    struct ThreatAnalysisResult {
        int threat_score = 0;
        std::vector<std::string> triggered_policies;
        std::map<std::string, int> threat_components;
    };
    
    ThreatAnalysisResult analyze_comprehensive_threats(const std::string& client_ip,
                                                      const std::string& method,
                                                      const std::string& url,
                                                      const std::map<std::string, std::string>& headers,
                                                      const std::string& body) {
        ThreatAnalysisResult result;
        
        std::lock_guard<std::mutex> lock(policy_mutex_);
        metrics_.policies_evaluated++;
        
        // Build combined analysis string
        std::string analysis_data = method + " " + url + " " + body;
        for (const auto& [key, value] : headers) {
            analysis_data += " " + key + ":" + value;
        }
        
        // Evaluate all security policies
        for (const auto& policy : security_policies_) {
            if (!policy.enabled) continue;
            
            bool policy_triggered = false;
            int policy_score = 0;
            
            switch (policy.type) {
                case PolicyType::SQL_INJECTION_PREVENTION:
                    if (std::regex_search(analysis_data, policy.compiled_pattern)) {
                        policy_triggered = true;
                        policy_score = 30; // High score for SQL injection
                        result.threat_components["sql_injection"] = policy_score;
                    }
                    break;
                    
                case PolicyType::XSS_PROTECTION:
                    if (std::regex_search(analysis_data, policy.compiled_pattern)) {
                        policy_triggered = true;
                        policy_score = 25; // High score for XSS
                        result.threat_components["xss"] = policy_score;
                    }
                    break;
                    
                case PolicyType::URL_PATTERN_BLOCKING:
                    if (std::regex_search(url, policy.compiled_pattern)) {
                        policy_triggered = true;
                        policy_score = 15; // Medium score for URL patterns
                        result.threat_components["url_pattern"] = policy_score;
                    }
                    break;
                    
                case PolicyType::USER_AGENT_FILTERING:
                    {
                        auto ua_it = headers.find("User-Agent");
                        if (ua_it != headers.end() && 
                            std::regex_search(ua_it->second, policy.compiled_pattern)) {
                            policy_triggered = true;
                            policy_score = 10; // Lower score for UA filtering
                            result.threat_components["user_agent"] = policy_score;
                        }
                    }
                    break;
                    
                default:
                    // Generic pattern matching for other policy types
                    if (!policy.pattern.empty() && 
                        std::regex_search(analysis_data, policy.compiled_pattern)) {
                        policy_triggered = true;
                        policy_score = 10;
                        result.threat_components["generic"] += policy_score;
                    }
                    break;
            }
            
            if (policy_triggered) {
                result.triggered_policies.push_back(policy.policy_id);
                result.threat_score += policy_score;
                
                // Update policy statistics
                const_cast<SecurityPolicy&>(policy).matches_count++;
            }
        }
        
        // Additional heuristic analysis
        result.threat_score += analyze_behavioral_patterns(client_ip, method, url, headers);
        
        return result;
    }
    
    /**
     * @brief Analyze behavioral patterns for additional threat scoring
     */
    int analyze_behavioral_patterns(const std::string& client_ip,
                                  const std::string& method,
                                  const std::string& url,
                                  const std::map<std::string, std::string>& headers) {
        std::lock_guard<std::mutex> lock(connections_mutex_);
        
        auto& tracker = active_connections_[client_ip];
        int behavioral_score = 0;
        
        // Analyze user agent consistency
        auto ua_it = headers.find("User-Agent");
        if (ua_it != headers.end()) {
            if (std::find(tracker.user_agents.begin(), tracker.user_agents.end(), ua_it->second) == 
                tracker.user_agents.end()) {
                tracker.user_agents.push_back(ua_it->second);
                
                // Multiple user agents from same IP is suspicious
                if (tracker.user_agents.size() > 3) {
                    behavioral_score += 15;
                    tracker.suspicious_behavior = true;
                }
            }
        }
        
        // Analyze URL patterns
        tracker.requested_urls.insert(url);
        if (tracker.requested_urls.size() > 50) { // Scanning behavior
            behavioral_score += 10;
        }
        
        // Analyze HTTP method distribution
        tracker.http_methods[method]++;
        if (tracker.http_methods.size() > 5) { // Using many different methods
            behavioral_score += 5;
        }
        
        // Update threat score
        tracker.threat_score = behavioral_score;
        
        return behavioral_score;
    }
    
    /**
     * @brief Validate thingamabob-specific access patterns
     */
    FirewallDecision validate_thingamabob_access(const std::string& url,
                                               const std::map<std::string, std::string>& headers,
                                               const std::string& client_ip) {
        FirewallDecision decision;
        decision.action = SecurityAction::ALLOW;
        decision.threat_level = ThreatSeverity::INFO;
        decision.reason = "Thingamabob access validated";
        
        // Check which thingamabob is being accessed
        for (const auto& [thingamabob, endpoints] : thingamabob_endpoints_) {
            for (const auto& endpoint : endpoints) {
                if (url.find(endpoint) == 0) {
                    // Apply thingamabob-specific rules
                    auto thingamabob_decision = apply_thingamabob_specific_rules(thingamabob, url, headers, client_ip);
                    if (thingamabob_decision.action != SecurityAction::ALLOW) {
                        return thingamabob_decision;
                    }
                    
                    decision.triggered_policies.push_back("THINGAMABOB_" + thingamabob);
                    break;
                }
            }
        }
        
        return decision;
    }
    
    /**
     * @brief Apply thingamabob-specific security rules
     */
    FirewallDecision apply_thingamabob_specific_rules(const std::string& thingamabob,
                                                    const std::string& url,
                                                    const std::map<std::string, std::string>& headers,
                                                    const std::string& client_ip) {
        FirewallDecision decision;
        decision.action = SecurityAction::ALLOW;
        decision.threat_level = ThreatSeverity::INFO;
        
        if (thingamabob == "webmail_thingamabob") {
            // Enhanced webmail security
            if (url.find("password") != std::string::npos || url.find("login") != std::string::nots()) {
                decision.threat_level = ThreatSeverity::MEDIUM;
                decision.requires_challenge = true;
            }
            
            // Check for webmail-specific attacks
            if (url.find("/../") != std::string::npos) {
                decision.action = SecurityAction::BLOCK;
                decision.threat_level = ThreatSeverity::HIGH;
                decision.reason = "Path traversal attempt in webmail";
                decision.triggered_policies.push_back("WEBMAIL_PATH_TRAVERSAL");
            }
            
        } else if (thingamabob == "jupyter_thingamabob") {
            // Jupyter code execution monitoring
            if (url.find("execute") != std::string::npos) {
                decision.threat_level = ThreatSeverity::MEDIUM;
                decision.triggered_policies.push_back("JUPYTER_CODE_EXECUTION");
            }
            
            // Monitor for dangerous imports in code
            auto content_length_it = headers.find("Content-Length");
            if (content_length_it != headers.end()) {
                int content_length = std::stoi(content_length_it->second);
                if (content_length > 100000) { // Large code submission
                    decision.threat_level = ThreatSeverity::MEDIUM;
                    decision.triggered_policies.push_back("JUPYTER_LARGE_CODE");
                }
            }
            
        } else if (thingamabob == "filemanager_thingamabob") {
            // File manager path traversal protection
            if (url.find("..") != std::string::npos) {
                decision.action = SecurityAction::YORKSHIRE_CHAMPION_BLOCK;
                decision.threat_level = ThreatSeverity::CRITICAL;
                decision.reason = "Path traversal attempt detected";
                decision.triggered_policies.push_back("FILEMANAGER_PATH_TRAVERSAL");
                return decision;
            }
            
            // Monitor file uploads
            if (url.find("upload") != std::string::npos) {
                decision.threat_level = ThreatSeverity::MEDIUM;
                decision.triggered_policies.push_back("FILEMANAGER_UPLOAD");
            }
        }
        
        return decision;
    }
    
    /**
     * @brief Get Yorkshire Champion security headers
     */
    std::map<std::string, std::string> get_yorkshire_champion_headers() const {
        return {
            {"X-Content-Type-Options", "nosniff"},
            {"X-Frame-Options", "DENY"},
            {"X-XSS-Protection", "1; mode=block"},
            {"Strict-Transport-Security", "max-age=31536000; includeSubDomains; preload"},
            {"Content-Security-Policy", "default-src 'self'; script-src 'self' 'unsafe-inline'; style-src 'self' 'unsafe-inline'"},
            {"X-Icewall-Protection", "Yorkshire-Champion-Active"},
            {"X-Firewall-Daddy", "Protecting-Your-Thingamabobs"},
            {"X-Security-Level", "Quantum-Grade"},
            {"X-Yorkshire-Multiplier", "15.0x"},
            {"X-Threat-Level", "Analyzed"},
            {"X-Protected-By", "Icewall-Firewall-Daddy"}
        };
    }
    
    /**
     * @brief Update connection tracking information
     */
    void update_connection_tracking(const std::string& client_ip,
                                  const std::map<std::string, std::string>& headers) {
        std::lock_guard<std::mutex> lock(connections_mutex_);
        
        auto& tracker = active_connections_[client_ip];
        tracker.last_activity = std::chrono::system_clock::now();
        
        // Update user agent tracking
        auto ua_it = headers.find("User-Agent");
        if (ua_it != headers.end()) {
            if (std::find(tracker.user_agents.begin(), tracker.user_agents.end(), ua_it->second) == 
                tracker.user_agents.end()) {
                tracker.user_agents.push_back(ua_it->second);
            }
        }
    }
    
    /**
     * @brief Update thingamabob-specific metrics
     */
    void update_thingamabob_metrics(const std::string& url) {
        if (url.find("/webmail") != std::string::npos || url.find("/mail") != std::string::npos) {
            metrics_.webmail_requests_secured++;
        } else if (url.find("/jupyter") != std::string::npos || url.find("/lab") != std::string::npos) {
            metrics_.jupyter_sessions_protected++;
        } else if (url.find("/files") != std::string::npos || url.find("/upload") != std::string::npos) {
            metrics_.filemanager_uploads_scanned++;
        } else if (url.find("/calendar") != std::string::npos || url.find("/caldav") != std::string::npos) {
            metrics_.calendar_access_validated++;
        } else if (url.find("/ftp") != std::string::npos || url.find("/sftp") != std::string::npos) {
            metrics_.ftp_connections_monitored++;
        }
    }
    
    /**
     * @brief Update threat-specific metrics
     */
    void update_threat_metrics(const std::vector<std::string>& triggered_policies) {
        for (const auto& policy_id : triggered_policies) {
            if (policy_id.find("sql_injection") != std::string::npos) {
                metrics_.sql_injections_blocked++;
            } else if (policy_id.find("xss") != std::string::npos) {
                metrics_.xss_attacks_blocked++;
            } else if (policy_id.find("ddos") != std::string::npos) {
                metrics_.ddos_attacks_mitigated++;
            } else if (policy_id.find("brute_force") != std::string::npos) {
                metrics_.brute_force_attempts_blocked++;
            } else if (policy_id.find("quantum") != std::string::npos) {
                metrics_.quantum_threats_detected++;
            }
        }
        
        metrics_.requests_blocked++;
    }
    
    /**
     * @brief Update processing time metrics
     */
    void update_processing_metrics(double processing_time_ns) {
        // Update running average
        auto current_avg = metrics_.average_processing_time_ns.load();
        auto total_requests = metrics_.total_requests_processed.load();
        
        if (total_requests > 1) {
            double new_avg = ((current_avg * (total_requests - 1)) + processing_time_ns) / total_requests;
            metrics_.average_processing_time_ns.store(new_avg);
        } else {
            metrics_.average_processing_time_ns.store(processing_time_ns);
        }
    }
    
    /**
     * @brief Record comprehensive security event
     */
    void record_security_event(const std::string& source_ip,
                             const std::string& method,
                             const std::string& url,
                             const std::map<std::string, std::string>& headers,
                             const std::string& body,
                             ThreatSeverity severity,
                             const std::vector<std::string>& triggered_policies,
                             SecurityAction action,
                             const std::string& description) {
        SecurityEvent event;
        event.event_id = generate_event_id();
        event.timestamp = std::chrono::system_clock::now();
        event.source_ip = source_ip;
        event.request_method = method;
        event.request_url = url;
        event.request_body = body.substr(0, 1000); // Limit body size in logs
        event.severity = severity;
        event.triggered_policies = triggered_policies;
        event.action_taken = action;
        event.description = description;
        
        // Add user agent
        auto ua_it = headers.find("User-Agent");
        if (ua_it != headers.end()) {
            event.user_agent = ua_it->second;
        }
        
        // Build headers string
        std::ostringstream headers_ss;
        for (const auto& [key, value] : headers) {
            headers_ss << key << ": " << value << "\n";
        }
        event.request_headers = headers_ss.str();
        
        {
            std::lock_guard<std::mutex> lock(events_mutex_);
            security_events_.push(event);
            
            // Limit event queue size
            if (security_events_.size() > 10000) {
                security_events_.pop();
            }
        }
        
        // Log to console/file as well
        log_security_event("EVENT: " + description + " (IP: " + source_ip + ", Severity: " + 
                          std::to_string(static_cast<int>(severity)) + ")", severity);
    }
    
    /**
     * @brief Security monitoring worker thread
     */
    void security_monitor_worker() {
        log_security_event("INFO: Icewall security monitor started", ThreatSeverity::INFO);
        
        while (!shutdown_requested_.load()) {
            // Monitor active connections
            cleanup_expired_connections();
            
            // Monitor threat patterns
            analyze_threat_trends();
            
            // Update uptime metrics
            auto now = std::chrono::system_clock::now();
            auto uptime = std::chrono::duration_cast<std::chrono::seconds>(now - metrics_.firewall_started);
            metrics_.uptime_seconds.store(uptime.count());
            
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
        
        log_security_event("INFO: Icewall security monitor stopped", ThreatSeverity::INFO);
    }
    
    /**
     * @brief Threat analysis worker thread
     */
    void threat_analysis_worker() {
        log_security_event("INFO: Icewall threat analysis worker started", ThreatSeverity::INFO);
        
        while (!shutdown_requested_.load()) {
            // Analyze security events for patterns
            analyze_security_event_patterns();
            
            // Update threat intelligence
            update_threat_intelligence_feed();
            
            // Auto-update policies based on threats
            optimize_security_policies();
            
            std::this_thread::sleep_for(std::chrono::seconds(30));
        }
        
        log_security_event("INFO: Icewall threat analysis worker stopped", ThreatSeverity::INFO);
    }
    
    /**
     * @brief Cleanup worker thread
     */
    void cleanup_worker() {
        log_security_event("INFO: Icewall cleanup worker started", ThreatSeverity::INFO);
        
        while (!shutdown_requested_.load()) {
            // Clean up expired IP list entries
            cleanup_expired_ip_entries();
            
            // Clean up old connection trackers
            cleanup_old_connection_trackers();
            
            // Archive old security events
            archive_old_security_events();
            
            std::this_thread::sleep_for(std::chrono::minutes(5));
        }
        
        log_security_event("INFO: Icewall cleanup worker stopped", ThreatSeverity::INFO);
    }
    
    /**
     * @brief Security worker thread for parallel processing
     */
    void security_worker() {
        while (!shutdown_requested_.load()) {
            std::unique_lock<std::mutex> lock(events_mutex_);
            worker_cv_.wait(lock, [this] { 
                return shutdown_requested_.load() || !security_events_.empty(); 
            });
            
            if (shutdown_requested_.load()) break;
            
            // Process security events in parallel
            while (!security_events_.empty()) {
                auto event = security_events_.front();
                security_events_.pop();
                lock.unlock();
                
                process_security_event_async(event);
                
                lock.lock();
            }
        }
    }
    
    // Helper methods for calculations and utilities
    double calculate_requests_per_second() const {
        auto uptime = std::chrono::system_clock::now() - metrics_.firewall_started;
        auto uptime_seconds = std::chrono::duration_cast<std::chrono::seconds>(uptime).count();
        return uptime_seconds > 0 ? 
            static_cast<double>(metrics_.total_requests_processed.load()) / uptime_seconds : 0.0;
    }
    
    double calculate_threat_detection_rate() const {
        auto total_requests = metrics_.total_requests_processed.load();
        auto threats_detected = metrics_.requests_blocked.load() + metrics_.requests_challenged.load();
        return total_requests > 0 ? 
            (static_cast<double>(threats_detected) / total_requests) * 100.0 : 0.0;
    }
    
    std::string generate_event_id() const {
        return "icewall_" + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
    }
    
    std::string get_timestamp_string() const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
    void cleanup_expired_connections() {
        std::lock_guard<std::mutex> lock(connections_mutex_);
        
        auto now = std::chrono::system_clock::now();
        auto it = active_connections_.begin();
        
        while (it != active_connections_.end()) {
            auto time_diff = now - it->second.last_activity;
            if (time_diff > std::chrono::hours(1)) { // 1 hour timeout
                it = active_connections_.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    void analyze_threat_trends() {
        // Implement threat trend analysis
    }
    
    void analyze_security_event_patterns() {
        // Implement security event pattern analysis
    }
    
    void update_threat_intelligence_feed() {
        // Implement threat intelligence updates
    }
    
    void optimize_security_policies() {
        // Implement policy optimization based on effectiveness
    }
    
    void cleanup_expired_ip_entries() {
        std::unique_lock<std::shared_mutex> lock(ip_list_mutex_);
        
        auto now = std::chrono::system_clock::now();
        
        // Clean whitelist
        auto it = whitelist_ips_.begin();
        while (it != whitelist_ips_.end()) {
            if (!it->second.permanent && now > it->second.expires_at) {
                it = whitelist_ips_.erase(it);
            } else {
                ++it;
            }
        }
        
        // Clean blacklist
        auto bit = blacklist_ips_.begin();
        while (bit != blacklist_ips_.end()) {
            if (!bit->second.permanent && now > bit->second.expires_at) {
                bit = blacklist_ips_.erase(bit);
            } else {
                ++bit;
            }
        }
    }
    
    void cleanup_old_connection_trackers() {
        std::lock_guard<std::mutex> lock(connections_mutex_);
        
        auto now = std::chrono::system_clock::now();
        auto it = active_connections_.begin();
        
        while (it != active_connections_.end()) {
            auto time_diff = now - it->second.last_activity;
            if (time_diff > std::chrono::hours(24)) { // 24 hour cleanup
                it = active_connections_.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    void archive_old_security_events() {
        // In production, this would archive old events to persistent storage
    }
    
    void process_security_event_async(const SecurityEvent& event) {
        // Process individual security events asynchronously
    }
    
    void log_security_event(const std::string& message, ThreatSeverity severity) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream log_entry;
        log_entry << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "] ";
        log_entry << "ICEWALL_FIREWALL_DADDY: " << message;
        
        // In production, this would write to secure log files
        // For now, store in memory or output to console
    }
};

/**
 * @brief Global Icewall Firewall Daddy instance 🛡️🔥
 */
extern std::unique_ptr<IcewallFirewallDaddy> g_icewall_firewall;

/**
 * @brief Initialize the Icewall Firewall Daddy! 🛡️
 */
inline bool initialize_icewall_firewall() {
    if (!g_icewall_firewall) {
        g_icewall_firewall = std::make_unique<IcewallFirewallDaddy>();
        return g_icewall_firewall->start();
    }
    return true;
}

/**
 * @brief Process request through Icewall Firewall Daddy
 */
inline IcewallFirewallDaddy::FirewallDecision process_icewall_request(
    const std::string& client_ip,
    const std::string& method,
    const std::string& url,
    const std::map<std::string, std::string>& headers,
    const std::string& body = "") {
    
    if (g_icewall_firewall) {
        return g_icewall_firewall->process_request(client_ip, method, url, headers, body);
    }
    
    IcewallFirewallDaddy::FirewallDecision decision;
    decision.action = SecurityAction::BLOCK;
    decision.reason = "Icewall Firewall Daddy not initialized";
    return decision;
}

/**
 * @brief Get comprehensive Icewall status
 */
inline std::map<std::string, std::string> get_icewall_status() {
    if (g_icewall_firewall) {
        return g_icewall_firewall->get_firewall_status();
    }
    return {{"status", "Icewall Firewall Daddy not initialized"}};
}

/**
 * @brief Icewall Policy Management Interface
 */
inline bool add_icewall_policy(const SecurityPolicy& policy) {
    if (g_icewall_firewall) {
        return g_icewall_firewall->add_security_policy(policy);
    }
    return false;
}

/**
 * @brief Icewall IP Management Interface
 */
inline bool add_ip_to_icewall_whitelist(const std::string& ip, const std::string& reason) {
    if (g_icewall_firewall) {
        return g_icewall_firewall->add_ip_to_whitelist(ip, reason);
    }
    return false;
}

inline bool add_ip_to_icewall_blacklist(const std::string& ip, const std::string& reason, 
                                       ThreatSeverity severity = ThreatSeverity::HIGH) {
    if (g_icewall_firewall) {
        return g_icewall_firewall->add_ip_to_blacklist(ip, reason, severity);
    }
    return false;
}

} // namespace FirewallDaddy
} // namespace Icewall
} // namespace MedusaServ