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
 * MEDUSA THINGAMABOB COMMUNICATION PROTOCOL - Yorkshire Champion Implementation
 * ==========================================================================
 * Revolutionary inter-component communication system with quantum messaging
 * 
 * DESCRIPTION: High-speed, secure, and intelligent communication protocol for all thingamabobs
 * CATEGORY: communication
 * COMPLEXITY: 950
 * YORKSHIRE CHAMPION MULTIPLIER: 15.0x
 */

#ifndef MEDUSA_THINGAMABOB_COMMUNICATION_PROTOCOL_HPP
#define MEDUSA_THINGAMABOB_COMMUNICATION_PROTOCOL_HPP

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <future>
#include <condition_variable>
#include <variant>
#include <cstdint>

namespace MedusaCommunication {

// Forward declarations
class ThingamabobMessage;
class MessageBus;
class ComponentRegistry;
class MessageRouter;
class SecurityValidator;
class PerformanceAnalyzer;

/**
 * Message Types for Yorkshire Champion Communication
 */
enum class MessageType {
    HEALTH_CHECK,
    PERFORMANCE_METRIC,
    SECURITY_ALERT,
    CONFIGURATION_UPDATE,
    COMMAND_EXECUTE,
    DATA_SYNC,
    STATUS_UPDATE,
    ERROR_REPORT,
    OPTIMIZATION_REQUEST,
    YORKSHIRE_CHAMPION_SCORE,
    SYSTEM_SHUTDOWN,
    COMPONENT_REGISTRATION,
    HEARTBEAT,
    QUANTUM_SECURE_MESSAGE
};

/**
 * Message Priority Levels
 */
enum class MessagePriority {
    CRITICAL = 0,      // System-critical messages
    HIGH = 1,          // Important operational messages
    NORMAL = 2,        // Standard communication
    LOW = 3,           // Background/maintenance messages
    YORKSHIRE_CHAMPION = -1  // Highest priority for Yorkshire Champion operations
};

/**
 * Component Types in the Thingamabob Ecosystem
 */
enum class ComponentType {
    SECURITY_COMPONENT,
    WEB_COMPONENT,
    INFRASTRUCTURE_COMPONENT,
    ACCELERATION_COMPONENT,
    UTILITY_COMPONENT,
    MONITOR_COMPONENT,
    API_GATEWAY,
    UNIFIED_MANAGER
};

/**
 * Message Payload - Flexible data container
 */
using MessagePayload = std::variant<
    std::string,
    int64_t,
    double,
    bool,
    std::vector<uint8_t>,
    std::unordered_map<std::string, std::string>
>;

/**
 * ThingamabobMessage - Core message structure
 */
class ThingamabobMessage {
public:
    ThingamabobMessage(
        const std::string& sender_id,
        const std::string& receiver_id,
        MessageType type,
        MessagePriority priority = MessagePriority::NORMAL
    );
    
    ~ThingamabobMessage() = default;

    // Message identification
    std::string get_message_id() const { return message_id_; }
    std::string get_sender_id() const { return sender_id_; }
    std::string get_receiver_id() const { return receiver_id_; }
    MessageType get_type() const { return type_; }
    MessagePriority get_priority() const { return priority_; }
    
    // Payload management
    void set_payload(const MessagePayload& payload);
    MessagePayload get_payload() const { return payload_; }
    
    template<typename T>
    void set_typed_payload(const T& data);
    
    template<typename T>
    T get_typed_payload() const;
    
    // Metadata
    void set_metadata(const std::string& key, const std::string& value);
    std::string get_metadata(const std::string& key) const;
    std::unordered_map<std::string, std::string> get_all_metadata() const;
    
    // Timing information
    std::chrono::system_clock::time_point get_timestamp() const { return timestamp_; }
    std::chrono::milliseconds get_age() const;
    
    // Yorkshire Champion scoring
    void set_yorkshire_champion_score(double score) { yorkshire_champion_score_ = score; }
    double get_yorkshire_champion_score() const { return yorkshire_champion_score_; }
    
    // Security
    void set_encryption_enabled(bool enabled) { encrypted_ = enabled; }
    bool is_encrypted() const { return encrypted_; }
    std::string get_security_token() const { return security_token_; }
    
    // Serialization
    std::vector<uint8_t> serialize() const;
    static std::unique_ptr<ThingamabobMessage> deserialize(const std::vector<uint8_t>& data);
    
private:
    std::string message_id_;
    std::string sender_id_;
    std::string receiver_id_;
    MessageType type_;
    MessagePriority priority_;
    MessagePayload payload_;
    std::unordered_map<std::string, std::string> metadata_;
    std::chrono::system_clock::time_point timestamp_;
    double yorkshire_champion_score_;
    bool encrypted_;
    std::string security_token_;
    
    std::string generate_message_id();
    void generate_security_token();
};

/**
 * Message Handler - Function type for processing messages
 */
using MessageHandler = std::function<void(std::shared_ptr<ThingamabobMessage>)>;

/**
 * Component Registration Information
 */
struct ComponentInfo {
    std::string component_id;
    std::string component_name;
    ComponentType type;
    std::string version;
    std::vector<MessageType> supported_messages;
    std::unordered_map<std::string, std::string> capabilities;
    std::chrono::system_clock::time_point registration_time;
    std::chrono::system_clock::time_point last_heartbeat;
    double yorkshire_champion_score;
    bool is_active;
};

/**
 * ComponentRegistry - Manages all registered thingamabob components
 */
class ComponentRegistry {
public:
    ComponentRegistry();
    ~ComponentRegistry();
    
    // Component management
    bool register_component(const ComponentInfo& info);
    bool unregister_component(const std::string& component_id);
    bool update_component_heartbeat(const std::string& component_id);
    
    // Component queries
    std::vector<std::string> get_all_component_ids() const;
    std::vector<std::string> get_components_by_type(ComponentType type) const;
    std::optional<ComponentInfo> get_component_info(const std::string& component_id) const;
    
    // Health monitoring
    std::vector<std::string> get_active_components() const;
    std::vector<std::string> get_inactive_components() const;
    bool is_component_active(const std::string& component_id) const;
    
    // Yorkshire Champion tracking
    double get_ecosystem_average_score() const;
    std::vector<std::string> get_yorkshire_champion_components() const;
    
private:
    mutable std::shared_mutex registry_mutex_;
    std::unordered_map<std::string, ComponentInfo> components_;
    std::chrono::seconds heartbeat_timeout_;
    
    void cleanup_inactive_components();
    std::thread cleanup_thread_;
    std::atomic<bool> cleanup_running_;
};

/**
 * MessageRouter - Intelligent message routing with load balancing
 */
class MessageRouter {
public:
    MessageRouter(std::shared_ptr<ComponentRegistry> registry);
    ~MessageRouter();
    
    // Routing operations
    std::vector<std::string> route_message(std::shared_ptr<ThingamabobMessage> message);
    bool can_route_to_component(const std::string& component_id, MessageType type) const;
    
    // Load balancing
    std::string get_best_component_for_type(ComponentType type) const;
    std::string get_least_loaded_component(const std::vector<std::string>& candidates) const;
    
    // Performance optimization
    void update_component_load(const std::string& component_id, double load_factor);
    double get_component_load(const std::string& component_id) const;
    
private:
    std::shared_ptr<ComponentRegistry> registry_;
    mutable std::shared_mutex routing_mutex_;
    std::unordered_map<std::string, double> component_loads_;
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> last_route_time_;
    
    double calculate_routing_score(const std::string& component_id, MessageType type) const;
};

/**
 * SecurityValidator - Quantum-secure message validation
 */
class SecurityValidator {
public:
    SecurityValidator();
    ~SecurityValidator();
    
    // Message security validation
    bool validate_message_security(std::shared_ptr<ThingamabobMessage> message);
    bool validate_component_authorization(const std::string& sender_id, const std::string& receiver_id);
    
    // Encryption/Decryption
    bool encrypt_message(std::shared_ptr<ThingamabobMessage> message);
    bool decrypt_message(std::shared_ptr<ThingamabobMessage> message);
    
    // Security policies
    void set_security_policy(const std::string& policy_name, bool enabled);
    bool is_security_policy_enabled(const std::string& policy_name) const;
    
    // Threat detection
    bool detect_anomalous_message_pattern(const std::string& sender_id);
    void record_message_for_analysis(std::shared_ptr<ThingamabobMessage> message);
    
private:
    mutable std::mutex security_mutex_;
    std::unordered_map<std::string, bool> security_policies_;
    std::unordered_map<std::string, std::vector<std::chrono::steady_clock::time_point>> message_history_;
    std::unordered_map<std::string, std::string> component_tokens_;
    
    std::string generate_encryption_key();
    bool perform_quantum_signature_validation(const std::string& message_data, const std::string& signature);
};

/**
 * PerformanceAnalyzer - Message and communication performance analysis
 */
class PerformanceAnalyzer {
public:
    PerformanceAnalyzer();
    ~PerformanceAnalyzer();
    
    // Performance tracking
    void record_message_sent(const std::string& sender_id, std::chrono::milliseconds duration);
    void record_message_received(const std::string& receiver_id, std::chrono::milliseconds duration);
    void record_message_processing_time(const std::string& component_id, std::chrono::milliseconds duration);
    
    // Performance metrics
    double get_average_message_latency() const;
    double get_component_performance_score(const std::string& component_id) const;
    double get_overall_communication_efficiency() const;
    
    // Yorkshire Champion analysis
    double calculate_yorkshire_champion_communication_multiplier() const;
    std::vector<std::string> get_performance_bottlenecks() const;
    
    // Optimization recommendations
    std::vector<std::string> get_optimization_recommendations() const;
    bool should_scale_component(const std::string& component_id) const;
    
private:
    mutable std::shared_mutex performance_mutex_;
    std::unordered_map<std::string, std::vector<std::chrono::milliseconds>> component_latencies_;
    std::unordered_map<std::string, std::vector<std::chrono::milliseconds>> processing_times_;
    std::unordered_map<std::string, uint64_t> message_counts_;
    std::chrono::steady_clock::time_point start_time_;
    
    double calculate_moving_average(const std::vector<std::chrono::milliseconds>& values, size_t window_size = 100) const;
};

/**
 * MessageBus - Central communication hub for all thingamabob components
 */
class MessageBus {
public:
    MessageBus();
    ~MessageBus();
    
    // Initialization and cleanup
    bool initialize();
    void shutdown();
    bool is_running() const { return running_.load(); }
    
    // Component management
    bool register_component(const ComponentInfo& info);
    bool unregister_component(const std::string& component_id);
    bool register_message_handler(const std::string& component_id, MessageType type, MessageHandler handler);
    
    // Message operations
    std::future<bool> send_message(std::shared_ptr<ThingamabobMessage> message);
    std::future<std::shared_ptr<ThingamabobMessage>> send_request(std::shared_ptr<ThingamabobMessage> request, std::chrono::milliseconds timeout);
    bool broadcast_message(std::shared_ptr<ThingamabobMessage> message, ComponentType target_type = ComponentType::INFRASTRUCTURE_COMPONENT);
    
    // Synchronous operations for critical messages
    bool send_critical_message(std::shared_ptr<ThingamabobMessage> message);
    bool send_yorkshire_champion_message(std::shared_ptr<ThingamabobMessage> message);
    
    // Health and monitoring
    void send_heartbeat(const std::string& component_id);
    bool is_component_responding(const std::string& component_id) const;
    std::vector<ComponentInfo> get_all_component_status() const;
    
    // Performance and analytics
    double get_yorkshire_champion_communication_score() const;
    std::unordered_map<std::string, double> get_component_performance_scores() const;
    void optimize_communication_routes();
    
    // Configuration
    void set_queue_size_limit(size_t limit) { max_queue_size_ = limit; }
    void set_worker_thread_count(size_t count);
    void enable_quantum_security(bool enabled) { quantum_security_enabled_ = enabled; }
    
private:
    // Core components
    std::shared_ptr<ComponentRegistry> registry_;
    std::shared_ptr<MessageRouter> router_;
    std::shared_ptr<SecurityValidator> security_validator_;
    std::shared_ptr<PerformanceAnalyzer> performance_analyzer_;
    
    // Message queuing
    struct PriorityMessage {
        std::shared_ptr<ThingamabobMessage> message;
        std::chrono::steady_clock::time_point enqueue_time;
        
        bool operator<(const PriorityMessage& other) const {
            return message->get_priority() > other.message->get_priority(); // Higher priority = lower value
        }
    };
    
    std::priority_queue<PriorityMessage> message_queue_;
    mutable std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
    size_t max_queue_size_;
    
    // Message handlers
    std::unordered_map<std::string, std::unordered_map<MessageType, MessageHandler>> message_handlers_;
    mutable std::shared_mutex handlers_mutex_;
    
    // Worker threads
    std::vector<std::thread> worker_threads_;
    std::atomic<bool> running_;
    size_t worker_count_;
    
    // Configuration
    bool quantum_security_enabled_;
    std::chrono::milliseconds default_timeout_;
    
    // Internal methods
    void worker_thread_function();
    bool process_message(std::shared_ptr<ThingamabobMessage> message);
    void deliver_message_to_component(std::shared_ptr<ThingamabobMessage> message, const std::string& component_id);
    bool validate_message(std::shared_ptr<ThingamabobMessage> message);
    void update_performance_metrics(std::shared_ptr<ThingamabobMessage> message, std::chrono::milliseconds processing_time);
    void cleanup_expired_messages();
    
    // Statistics
    std::atomic<uint64_t> messages_sent_;
    std::atomic<uint64_t> messages_processed_;
    std::atomic<uint64_t> messages_dropped_;
    std::chrono::steady_clock::time_point startup_time_;
};

/**
 * Utility Functions for Yorkshire Champion Communication
 */

// Message creation helpers
std::shared_ptr<ThingamabobMessage> create_health_check_message(const std::string& sender_id, const std::string& receiver_id);
std::shared_ptr<ThingamabobMessage> create_performance_metric_message(const std::string& sender_id, const std::string& metric_name, double value);
std::shared_ptr<ThingamabobMessage> create_yorkshire_champion_score_message(const std::string& sender_id, double score);
std::shared_ptr<ThingamabobMessage> create_security_alert_message(const std::string& sender_id, const std::string& alert_type, const std::string& details);

// Component helpers
ComponentInfo create_component_info(const std::string& id, const std::string& name, ComponentType type, const std::string& version);
bool register_thingamabob_component(MessageBus& bus, const ComponentInfo& info, const std::unordered_map<MessageType, MessageHandler>& handlers);

// Communication patterns
class ThingamabobCommunicator {
public:
    ThingamabobCommunicator(const std::string& component_id, std::shared_ptr<MessageBus> bus);
    ~ThingamabobCommunicator();
    
    // High-level communication methods
    bool send_health_update(double health_score);
    bool send_performance_metric(const std::string& metric_name, double value);
    bool report_yorkshire_champion_achievement(double multiplier);
    bool request_configuration_update(const std::string& config_key);
    
    // Event-based communication
    void on_health_check_received(std::function<void(double)> callback);
    void on_configuration_update(std::function<void(const std::string&, const std::string&)> callback);
    void on_shutdown_signal(std::function<void()> callback);
    
private:
    std::string component_id_;
    std::shared_ptr<MessageBus> bus_;
    std::unordered_map<MessageType, std::function<void(std::shared_ptr<ThingamabobMessage>)>> event_handlers_;
    
    void setup_default_handlers();
};

// Global communication manager
class GlobalCommunicationManager {
public:
    static GlobalCommunicationManager& getInstance();
    
    bool initialize_yorkshire_champion_communication();
    std::shared_ptr<MessageBus> get_message_bus() { return message_bus_; }
    void shutdown_communication();
    
    // Statistics
    uint64_t get_total_messages_processed() const;
    double get_overall_system_performance_score() const;
    std::vector<std::string> get_communication_health_report() const;
    
private:
    GlobalCommunicationManager() = default;
    ~GlobalCommunicationManager() = default;
    
    std::shared_ptr<MessageBus> message_bus_;
    std::atomic<bool> initialized_;
    
    // Singleton pattern enforcement
    GlobalCommunicationManager(const GlobalCommunicationManager&) = delete;
    GlobalCommunicationManager& operator=(const GlobalCommunicationManager&) = delete;
};

} // namespace MedusaCommunication

// Convenience macros for Yorkshire Champion communication
#define MEDUSA_SEND_HEALTH_UPDATE(component_id, score) \
    MedusaCommunication::GlobalCommunicationManager::getInstance().get_message_bus()->send_message( \
        MedusaCommunication::create_performance_metric_message(component_id, "health_score", score))

#define MEDUSA_REPORT_YORKSHIRE_CHAMPION(component_id, multiplier) \
    MedusaCommunication::GlobalCommunicationManager::getInstance().get_message_bus()->send_message( \
        MedusaCommunication::create_yorkshire_champion_score_message(component_id, multiplier))

#define MEDUSA_SEND_SECURITY_ALERT(component_id, alert_type, details) \
    MedusaCommunication::GlobalCommunicationManager::getInstance().get_message_bus()->send_critical_message( \
        MedusaCommunication::create_security_alert_message(component_id, alert_type, details))

#endif // MEDUSA_THINGAMABOB_COMMUNICATION_PROTOCOL_HPP