/**
 * MEDUSA JSON - Standalone Implementation
 * ======================================
 * Ground-up JSON implementation with no external dependencies
 * Extracted from libanti_gumph_filter for IPA AI Command System
 */

#ifndef MEDUSA_JSON_STANDALONE_HPP
#define MEDUSA_JSON_STANDALONE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <chrono>
#include <atomic>
#include <mutex>

class JSONParseException : public std::runtime_error {
public:
    JSONParseException(const std::string& message) : std::runtime_error(message) {}
};

// JSON Metrics System - Yorkshire Champion Performance Tracking
struct MedusaJSONMetrics {
    // Creation metrics
    std::atomic<uint64_t> objects_created{0};
    std::atomic<uint64_t> arrays_created{0};
    std::atomic<uint64_t> strings_created{0};
    std::atomic<uint64_t> numbers_created{0};
    std::atomic<uint64_t> booleans_created{0};
    std::atomic<uint64_t> nulls_created{0};
    
    // Operation metrics
    std::atomic<uint64_t> serialize_operations{0};
    std::atomic<uint64_t> get_operations{0};
    std::atomic<uint64_t> set_operations{0};
    std::atomic<uint64_t> push_operations{0};
    std::atomic<uint64_t> access_operations{0};
    
    // Performance metrics
    std::atomic<uint64_t> total_serialize_time_ns{0};
    std::atomic<uint64_t> total_access_time_ns{0};
    std::atomic<uint64_t> memory_usage_bytes{0};
    std::atomic<uint64_t> max_object_depth{0};
    std::atomic<uint64_t> max_array_length{0};
    
    // Error metrics
    std::atomic<uint64_t> serialization_errors{0};
    std::atomic<uint64_t> access_errors{0};
    std::atomic<uint64_t> type_conversion_errors{0};
    
    // Yorkshire Champion features
    std::chrono::steady_clock::time_point start_time;
    std::atomic<bool> metrics_enabled{true};
    mutable std::mutex metrics_mutex;
    
    MedusaJSONMetrics() : start_time(std::chrono::steady_clock::now()) {}
    
    // Performance calculations
    double get_average_serialize_time_ns() const {
        auto ops = serialize_operations.load();
        return ops > 0 ? static_cast<double>(total_serialize_time_ns.load()) / ops : 0.0;
    }
    
    double get_average_access_time_ns() const {
        auto ops = access_operations.load();
        return ops > 0 ? static_cast<double>(total_access_time_ns.load()) / ops : 0.0;
    }
    
    uint64_t get_total_objects_created() const {
        return objects_created + arrays_created + strings_created + 
               numbers_created + booleans_created + nulls_created;
    }
    
    uint64_t get_total_operations() const {
        return serialize_operations + get_operations + set_operations + 
               push_operations + access_operations;
    }
    
    double get_operations_per_second() const {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start_time);
        auto seconds = duration.count();
        return seconds > 0 ? static_cast<double>(get_total_operations()) / seconds : 0.0;
    }
    
    double get_error_rate() const {
        auto total_ops = get_total_operations();
        auto total_errors = serialization_errors.load() + access_errors.load() + type_conversion_errors.load();
        return total_ops > 0 ? static_cast<double>(total_errors) / total_ops : 0.0;
    }
    
    // Reset all metrics
    void reset() {
        std::lock_guard<std::mutex> lock(metrics_mutex);
        objects_created = 0;
        arrays_created = 0;
        strings_created = 0;
        numbers_created = 0;
        booleans_created = 0;
        nulls_created = 0;
        serialize_operations = 0;
        get_operations = 0;
        set_operations = 0;
        push_operations = 0;
        access_operations = 0;
        total_serialize_time_ns = 0;
        total_access_time_ns = 0;
        memory_usage_bytes = 0;
        max_object_depth = 0;
        max_array_length = 0;
        serialization_errors = 0;
        access_errors = 0;
        type_conversion_errors = 0;
        start_time = std::chrono::steady_clock::now();
    }
    
    // Generate JSON metrics report
    std::string to_json_string() const {
        std::string result = "{";
        result += "\"objects_created\":" + std::to_string(objects_created.load()) + ",";
        result += "\"arrays_created\":" + std::to_string(arrays_created.load()) + ",";
        result += "\"strings_created\":" + std::to_string(strings_created.load()) + ",";
        result += "\"numbers_created\":" + std::to_string(numbers_created.load()) + ",";
        result += "\"booleans_created\":" + std::to_string(booleans_created.load()) + ",";
        result += "\"nulls_created\":" + std::to_string(nulls_created.load()) + ",";
        result += "\"total_objects\":" + std::to_string(get_total_objects_created()) + ",";
        result += "\"serialize_operations\":" + std::to_string(serialize_operations.load()) + ",";
        result += "\"get_operations\":" + std::to_string(get_operations.load()) + ",";
        result += "\"set_operations\":" + std::to_string(set_operations.load()) + ",";
        result += "\"push_operations\":" + std::to_string(push_operations.load()) + ",";
        result += "\"access_operations\":" + std::to_string(access_operations.load()) + ",";
        result += "\"total_operations\":" + std::to_string(get_total_operations()) + ",";
        result += "\"average_serialize_time_ns\":" + std::to_string(get_average_serialize_time_ns()) + ",";
        result += "\"average_access_time_ns\":" + std::to_string(get_average_access_time_ns()) + ",";
        result += "\"operations_per_second\":" + std::to_string(get_operations_per_second()) + ",";
        result += "\"memory_usage_bytes\":" + std::to_string(memory_usage_bytes.load()) + ",";
        result += "\"max_object_depth\":" + std::to_string(max_object_depth.load()) + ",";
        result += "\"max_array_length\":" + std::to_string(max_array_length.load()) + ",";
        result += "\"serialization_errors\":" + std::to_string(serialization_errors.load()) + ",";
        result += "\"access_errors\":" + std::to_string(access_errors.load()) + ",";
        result += "\"type_conversion_errors\":" + std::to_string(type_conversion_errors.load()) + ",";
        result += "\"error_rate\":" + std::to_string(get_error_rate()) + ",";
        result += "\"metrics_enabled\":"; 
        result += (metrics_enabled.load() ? "true" : "false");
        result += ",";
        result += "\"yorkshire_status\":\"JSON Metrics - Yorkshire Champion Performance!\"";
        result += "}";
        return result;
    }
};

// Global metrics instance
extern MedusaJSONMetrics g_medusa_json_metrics;

enum class JsonType {
    OBJECT,
    ARRAY,
    STRING,
    NUMBER,
    BOOLEAN,
    NULL_VALUE
};

class MedusaJSON {
public:
    JsonType type;
    std::string string_value;
    double number_value = 0.0;
    bool boolean_value = false;
    std::unordered_map<std::string, std::shared_ptr<MedusaJSON>> object_value;
    std::vector<std::shared_ptr<MedusaJSON>> array_value;

    MedusaJSON(JsonType t = JsonType::NULL_VALUE) : type(t) {}
    
    // Factory methods with metrics tracking
    static std::shared_ptr<MedusaJSON> createObject() {
        if (g_medusa_json_metrics.metrics_enabled.load()) {
            g_medusa_json_metrics.objects_created.fetch_add(1);
        }
        return std::make_shared<MedusaJSON>(JsonType::OBJECT);
    }
    
    static std::shared_ptr<MedusaJSON> createArray() {
        if (g_medusa_json_metrics.metrics_enabled.load()) {
            g_medusa_json_metrics.arrays_created.fetch_add(1);
        }
        return std::make_shared<MedusaJSON>(JsonType::ARRAY);
    }
    
    static std::shared_ptr<MedusaJSON> createString(const std::string& value) {
        if (g_medusa_json_metrics.metrics_enabled.load()) {
            g_medusa_json_metrics.strings_created.fetch_add(1);
            g_medusa_json_metrics.memory_usage_bytes.fetch_add(value.size());
        }
        auto json = std::make_shared<MedusaJSON>(JsonType::STRING);
        json->string_value = value;
        return json;
    }
    
    static std::shared_ptr<MedusaJSON> createNumber(double value) {
        if (g_medusa_json_metrics.metrics_enabled.load()) {
            g_medusa_json_metrics.numbers_created.fetch_add(1);
        }
        auto json = std::make_shared<MedusaJSON>(JsonType::NUMBER);
        json->number_value = value;
        return json;
    }
    
    static std::shared_ptr<MedusaJSON> createBoolean(bool value) {
        if (g_medusa_json_metrics.metrics_enabled.load()) {
            g_medusa_json_metrics.booleans_created.fetch_add(1);
        }
        auto json = std::make_shared<MedusaJSON>(JsonType::BOOLEAN);
        json->boolean_value = value;
        return json;
    }
    
    static std::shared_ptr<MedusaJSON> createNull() {
        if (g_medusa_json_metrics.metrics_enabled.load()) {
            g_medusa_json_metrics.nulls_created.fetch_add(1);
        }
        return std::make_shared<MedusaJSON>(JsonType::NULL_VALUE);
    }
    
    // Basic serialization for IPA responses with metrics tracking
    std::string serialize() const {
        auto start_time = std::chrono::steady_clock::now();
        
        if (g_medusa_json_metrics.metrics_enabled.load()) {
            g_medusa_json_metrics.serialize_operations.fetch_add(1);
        }
        
        std::string result;
        
        try {
            switch (type) {
                case JsonType::OBJECT: {
                    result = "{";
                bool first = true;
                for (const auto& pair : object_value) {
                    if (!first) result += ",";
                    result += "\"" + pair.first + "\":" + pair.second->serialize();
                    first = false;
                }
                    result += "}";
                    break;
                }
                case JsonType::ARRAY: {
                    result = "[";
                bool first = true;
                for (const auto& value : array_value) {
                    if (!first) result += ",";
                    result += value->serialize();
                    first = false;
                }
                    result += "]";
                    break;
                }
                case JsonType::STRING:
                    result = "\"" + string_value + "\"";
                    break;
                case JsonType::NUMBER:
                    result = std::to_string(number_value);
                    break;
                case JsonType::BOOLEAN:
                    result = boolean_value ? "true" : "false";
                    break;
                case JsonType::NULL_VALUE:
                    result = "null";
                    break;
            }
            
            // Record timing metrics
            if (g_medusa_json_metrics.metrics_enabled.load()) {
                auto end_time = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
                g_medusa_json_metrics.total_serialize_time_ns.fetch_add(duration.count());
            }
            
            return result;
        } catch (const std::exception& e) {
            if (g_medusa_json_metrics.metrics_enabled.load()) {
                g_medusa_json_metrics.serialization_errors.fetch_add(1);
            }
            return "null";
        }
    }
    
    // Access methods with metrics tracking
    std::shared_ptr<MedusaJSON> get(const std::string& key) const {
        auto start_time = std::chrono::steady_clock::now();
        
        if (g_medusa_json_metrics.metrics_enabled.load()) {
            g_medusa_json_metrics.get_operations.fetch_add(1);
            g_medusa_json_metrics.access_operations.fetch_add(1);
        }
        
        try {
            if (type != JsonType::OBJECT) {
                if (g_medusa_json_metrics.metrics_enabled.load()) {
                    g_medusa_json_metrics.access_errors.fetch_add(1);
                }
                return nullptr;
            }
            
            auto it = object_value.find(key);
            auto result = (it != object_value.end()) ? it->second : nullptr;
            
            // Record timing
            if (g_medusa_json_metrics.metrics_enabled.load()) {
                auto end_time = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
                g_medusa_json_metrics.total_access_time_ns.fetch_add(duration.count());
            }
            
            return result;
        } catch (const std::exception& e) {
            if (g_medusa_json_metrics.metrics_enabled.load()) {
                g_medusa_json_metrics.access_errors.fetch_add(1);
            }
            return nullptr;
        }
    }
    
    void set(const std::string& key, std::shared_ptr<MedusaJSON> value) {
        if (g_medusa_json_metrics.metrics_enabled.load()) {
            g_medusa_json_metrics.set_operations.fetch_add(1);
        }
        
        try {
            if (type != JsonType::OBJECT) {
                if (g_medusa_json_metrics.metrics_enabled.load()) {
                    g_medusa_json_metrics.access_errors.fetch_add(1);
                }
                return;
            }
            object_value[key] = value;
        } catch (const std::exception& e) {
            if (g_medusa_json_metrics.metrics_enabled.load()) {
                g_medusa_json_metrics.access_errors.fetch_add(1);
            }
        }
    }
    
    void push(std::shared_ptr<MedusaJSON> value) {
        if (g_medusa_json_metrics.metrics_enabled.load()) {
            g_medusa_json_metrics.push_operations.fetch_add(1);
        }
        
        try {
            if (type != JsonType::ARRAY) {
                if (g_medusa_json_metrics.metrics_enabled.load()) {
                    g_medusa_json_metrics.access_errors.fetch_add(1);
                }
                return;
            }
            array_value.push_back(value);
            
            // Update max array length tracking
            if (g_medusa_json_metrics.metrics_enabled.load()) {
                uint64_t current_length = array_value.size();
                uint64_t current_max = g_medusa_json_metrics.max_array_length.load();
                while (current_length > current_max) {
                    if (g_medusa_json_metrics.max_array_length.compare_exchange_weak(current_max, current_length)) {
                        break;
                    }
                }
            }
        } catch (const std::exception& e) {
            if (g_medusa_json_metrics.metrics_enabled.load()) {
                g_medusa_json_metrics.access_errors.fetch_add(1);
            }
        }
    }
    
    // Convenience methods for IPA
    std::string value(const std::string& key, const std::string& default_val) const {
        auto val = get(key);
        return (val && val->isString()) ? val->string_value : default_val;
    }
    
    bool value(const std::string& key, bool default_val) const {
        auto val = get(key);
        return (val && val->isBoolean()) ? val->boolean_value : default_val;
    }
    
    double value(const std::string& key, double default_val) const {
        auto val = get(key);
        return (val && val->isNumber()) ? val->number_value : default_val;
    }
    
    // Type checking
    bool isObject() const { return type == JsonType::OBJECT; }
    bool isArray() const { return type == JsonType::ARRAY; }
    bool isString() const { return type == JsonType::STRING; }
    bool isNumber() const { return type == JsonType::NUMBER; }
    bool isBoolean() const { return type == JsonType::BOOLEAN; }
    bool isNull() const { return type == JsonType::NULL_VALUE; }
    
    // Operator overloads for convenience (IPA-style JSON building)
    std::shared_ptr<MedusaJSON> operator[](const std::string& key) {
        if (type != JsonType::OBJECT) return nullptr;
        if (object_value.find(key) == object_value.end()) {
            object_value[key] = createNull();
        }
        return object_value[key];
    }
    
    // Static metrics access methods
    static std::string getMetrics() {
        return g_medusa_json_metrics.to_json_string();
    }
    
    static void resetMetrics() {
        g_medusa_json_metrics.reset();
    }
    
    static void enableMetrics(bool enable = true) {
        g_medusa_json_metrics.metrics_enabled.store(enable);
    }
    
    static bool isMetricsEnabled() {
        return g_medusa_json_metrics.metrics_enabled.load();
    }
    
    static uint64_t getTotalOperations() {
        return g_medusa_json_metrics.get_total_operations();
    }
    
    static double getOperationsPerSecond() {
        return g_medusa_json_metrics.get_operations_per_second();
    }
    
    static double getErrorRate() {
        return g_medusa_json_metrics.get_error_rate();
    }
};

// Global convenience functions for metrics
inline std::string getMedusaJSONMetrics() {
    return MedusaJSON::getMetrics();
}

inline void resetMedusaJSONMetrics() {
    MedusaJSON::resetMetrics();
}

inline void enableMedusaJSONMetrics(bool enable = true) {
    MedusaJSON::enableMetrics(enable);
}

// Typedef for compatibility with existing IPA code
using json = std::shared_ptr<MedusaJSON>;

// Helper function to create JSON objects (nlohmann-style)
inline json createJson() {
    return MedusaJSON::createObject();
}

#endif // MEDUSA_JSON_STANDALONE_HPP