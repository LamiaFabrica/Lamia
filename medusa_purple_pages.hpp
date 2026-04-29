/*
 * MEDUSA PURPLE-PAGES SYSTEM
 * Audit logging and FAQ knowledge management
 * Yorkshire Champion integrated documentation and monitoring
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>

namespace MedusaServer {

// Audit Log System

enum class AuditEventType {
    USER_LOGIN,
    USER_LOGOUT,
    USER_FAILED_LOGIN,
    SYSTEM_START,
    SYSTEM_SHUTDOWN,
    CONFIGURATION_CHANGE,
    DATABASE_QUERY,
    API_REQUEST,
    FILE_ACCESS,
    SECURITY_VIOLATION,
    ERROR_OCCURRED,
    BACKUP_STARTED,
    BACKUP_COMPLETED,
    THEME_CHANGED,
    PLUGIN_LOADED,
    PERMISSION_CHANGED,
    DATA_EXPORT,
    DATA_IMPORT,
    CUSTOM_EVENT
};

enum class AuditSeverity {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    CRITICAL = 4,
    SECURITY = 5
};

struct AuditEvent {
    std::string event_id;
    AuditEventType event_type;
    AuditSeverity severity;
    std::chrono::system_clock::time_point timestamp;
    std::string user_id;
    std::string session_id;
    std::string ip_address;
    std::string user_agent;
    std::string description;
    std::map<std::string, std::string> metadata;
    std::string stack_trace;
    bool archived = false;
    
    std::string getEventTypeString() const;
    std::string getSeverityString() const;
    std::string toJson() const;
    std::string toLogLine() const;
};

struct AuditQuery {
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point end_time;
    std::vector<AuditEventType> event_types;
    std::vector<AuditSeverity> severities;
    std::string user_id;
    std::string session_id;
    std::string ip_address;
    std::string search_text;
    int limit = 100;
    int offset = 0;
    bool include_archived = false;
};

class AuditLogger {
private:
    std::string database_path_;
    std::queue<AuditEvent> event_queue_;
    std::mutex queue_mutex_;
    std::thread writer_thread_;
    std::atomic<bool> running_;
    std::function<void(const AuditEvent&)> event_callback_;
    
public:
    AuditLogger(const std::string& db_path = "/home/medusa/audit/audit.db");
    ~AuditLogger();
    
    bool initialize();
    void shutdown();
    
    // Event logging
    void logEvent(const AuditEvent& event);
    void logUserLogin(const std::string& user_id, const std::string& ip_address, bool success, const std::string& details = "");
    void logUserLogout(const std::string& user_id, const std::string& session_id);
    void logSystemEvent(AuditEventType type, AuditSeverity severity, const std::string& description, const std::map<std::string, std::string>& metadata = {});
    void logDatabaseQuery(const std::string& user_id, const std::string& query, const std::string& table, bool success);
    void logAPIRequest(const std::string& user_id, const std::string& endpoint, const std::string& method, const std::string& ip_address, int response_code);
    void logSecurityViolation(const std::string& user_id, const std::string& violation_type, const std::string& details, const std::string& ip_address);
    void logError(const std::string& component, const std::string& error_message, const std::string& stack_trace = "");
    
    // Query interface
    std::vector<AuditEvent> queryEvents(const AuditQuery& query);
    std::vector<AuditEvent> getRecentEvents(int limit = 50);
    std::vector<AuditEvent> getEventsByUser(const std::string& user_id, int limit = 100);
    std::vector<AuditEvent> getEventsByType(AuditEventType type, int limit = 100);
    std::vector<AuditEvent> getSecurityEvents(int limit = 100);
    
    // Statistics
    std::map<std::string, int> getEventTypeStatistics(const std::chrono::system_clock::time_point& since);
    std::map<std::string, int> getUserActivityStatistics(const std::chrono::system_clock::time_point& since);
    std::map<std::string, int> getSecurityStatistics(const std::chrono::system_clock::time_point& since);
    
    // Maintenance
    bool archiveOldEvents(const std::chrono::system_clock::time_point& before);
    bool cleanupEvents(const std::chrono::system_clock::time_point& before);
    bool exportEvents(const std::string& export_path, const AuditQuery& query);
    
    // Configuration
    void setEventCallback(std::function<void(const AuditEvent&)> callback);
    
private:
    void writerLoop();
    bool createDatabase();
    bool writeEventToDatabase(const AuditEvent& event);
    std::string generateEventId();
};

// FAQ Knowledge System

struct FAQEntry {
    std::string id;
    std::string category;
    std::string question;
    std::string answer;
    std::vector<std::string> tags;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
    std::string created_by;
    std::string updated_by;
    int view_count = 0;
    int helpfulness_score = 0;
    bool is_published = true;
    std::string markdown_content;
    std::vector<std::string> related_questions;
};

struct KnowledgeCategory {
    std::string id;
    std::string name;
    std::string description;
    std::string icon;
    std::vector<std::string> faq_ids;
    int sort_order = 0;
    std::string parent_category_id;
};

struct SearchQuery {
    std::string search_text;
    std::vector<std::string> categories;
    std::vector<std::string> tags;
    bool published_only = true;
    int limit = 20;
    int offset = 0;
};

class KnowledgeBase {
private:
    std::string database_path_;
    std::map<std::string, FAQEntry> faq_entries_;
    std::map<std::string, KnowledgeCategory> categories_;
    std::mutex data_mutex_;
    
public:
    KnowledgeBase(const std::string& db_path = "/home/medusa/knowledge/knowledge.db");
    ~KnowledgeBase() = default;
    
    bool initialize();
    bool loadFromDatabase();
    bool saveToDatabase();
    
    // FAQ Management
    std::string addFAQ(const FAQEntry& faq);
    bool updateFAQ(const std::string& id, const FAQEntry& faq);
    bool deleteFAQ(const std::string& id);
    FAQEntry getFAQ(const std::string& id);
    std::vector<FAQEntry> getAllFAQs();
    
    // Category Management
    std::string addCategory(const KnowledgeCategory& category);
    bool updateCategory(const std::string& id, const KnowledgeCategory& category);
    bool deleteCategory(const std::string& id);
    KnowledgeCategory getCategory(const std::string& id);
    std::vector<KnowledgeCategory> getAllCategories();
    std::vector<KnowledgeCategory> getCategoryHierarchy();
    
    // Search and Query
    std::vector<FAQEntry> searchFAQs(const SearchQuery& query);
    std::vector<FAQEntry> getFAQsByCategory(const std::string& category_id);
    std::vector<FAQEntry> getFAQsByTag(const std::string& tag);
    std::vector<FAQEntry> getPopularFAQs(int limit = 10);
    std::vector<FAQEntry> getRecentFAQs(int limit = 10);
    std::vector<std::string> getSuggestions(const std::string& query);
    
    // Analytics
    void recordView(const std::string& faq_id);
    void recordHelpfulness(const std::string& faq_id, bool helpful);
    std::map<std::string, int> getViewStatistics();
    std::map<std::string, int> getCategoryStatistics();
    std::vector<std::string> getPopularSearchTerms();
    
    // Content Management
    bool importFAQs(const std::string& import_file);
    bool exportFAQs(const std::string& export_file, const std::vector<std::string>& faq_ids = {});
    bool validateFAQContent(const FAQEntry& faq);
    
private:
    bool createDatabase();
    std::string generateFAQId();
    std::string generateCategoryId();
    std::vector<std::string> extractTags(const std::string& content);
    double calculateRelevanceScore(const FAQEntry& faq, const std::string& query);
};

// Purple Pages Integration System

class PurplePagesSystem {
private:
    std::shared_ptr<AuditLogger> audit_logger_;
    std::shared_ptr<KnowledgeBase> knowledge_base_;
    std::string base_path_;
    
public:
    PurplePagesSystem(const std::string& base_path = "/home/medusa/purple_pages");
    ~PurplePagesSystem() = default;
    
    bool initialize();
    void shutdown();
    
    // Component access
    AuditLogger* getAuditLogger() { return audit_logger_.get(); }
    KnowledgeBase* getKnowledgeBase() { return knowledge_base_.get(); }
    
    // Integrated operations
    void logKnowledgeAccess(const std::string& user_id, const std::string& faq_id, const std::string& ip_address);
    void logKnowledgeModification(const std::string& user_id, const std::string& faq_id, const std::string& action, const std::string& details);
    std::vector<AuditEvent> getKnowledgeAuditLog(const std::string& faq_id);
    
    // Web interface generation
    std::string generateAuditDashboard();
    std::string generateKnowledgeDashboard();
    std::string generateFAQPage(const std::string& faq_id);
    std::string generateCategoryPage(const std::string& category_id);
    std::string generateSearchResults(const std::string& query);
    
    // API endpoints
    std::string handleAuditQuery(const std::string& query_json);
    std::string handleFAQSearch(const std::string& search_json);
    std::string handleFAQCreate(const std::string& faq_json, const std::string& user_id);
    std::string handleFAQUpdate(const std::string& faq_id, const std::string& faq_json, const std::string& user_id);
    
    // System maintenance
    void performMaintenance();
    bool backupPurplePages(const std::string& backup_path);
    bool restorePurplePages(const std::string& backup_path);
    std::map<std::string, std::string> getSystemHealth();
    
private:
    bool createDirectoryStructure();
    std::string generateHTML(const std::string& title, const std::string& content);
    std::string generateTableHTML(const std::vector<std::vector<std::string>>& data, const std::vector<std::string>& headers);
    std::string formatTimestamp(const std::chrono::system_clock::time_point& time);
};

// Production Purple Pages setup
inline void setupProductionPurplePages(PurplePagesSystem& system) {
    auto knowledge_base = system.getKnowledgeBase();
    
    // Create default categories
    KnowledgeCategory system_admin;
    system_admin.name = "System Administration";
    system_admin.description = "Server management, configuration, and maintenance";
    system_admin.icon = "🔧";
    system_admin.sort_order = 1;
    std::string admin_cat_id = knowledge_base->addCategory(system_admin);
    
    KnowledgeCategory security;
    security.name = "Security";
    security.description = "Authentication, authorization, and security procedures";
    security.icon = "🔐";
    security.sort_order = 2;
    std::string security_cat_id = knowledge_base->addCategory(security);
    
    KnowledgeCategory troubleshooting;
    troubleshooting.name = "Troubleshooting";
    troubleshooting.description = "Common issues and their solutions";
    troubleshooting.icon = "🔍";
    troubleshooting.sort_order = 3;
    std::string trouble_cat_id = knowledge_base->addCategory(troubleshooting);
    
    // Add sample FAQ entries
    FAQEntry server_setup;
    server_setup.category = admin_cat_id;
    server_setup.question = "How do I set up a new Medusa server instance?";
    server_setup.answer = "Follow these steps to set up a new Medusa server...";
    server_setup.tags = {"server", "setup", "installation", "configuration"};
    server_setup.created_by = "system";
    server_setup.is_published = true;
    knowledge_base->addFAQ(server_setup);
    
    FAQEntry auth_config;
    auth_config.category = security_cat_id;
    auth_config.question = "How do I configure Yorkshire Champion authentication?";
    auth_config.answer = "Yorkshire Champion authentication requires the following configuration...";
    auth_config.tags = {"authentication", "yorkshire", "security", "config"};
    auth_config.created_by = "system";
    auth_config.is_published = true;
    knowledge_base->addFAQ(auth_config);
    
    FAQEntry db_connection;
    db_connection.category = trouble_cat_id;
    db_connection.question = "Database connection failed - how to troubleshoot?";
    db_connection.answer = "When database connections fail, check the following...";
    db_connection.tags = {"database", "connection", "postgresql", "troubleshooting"};
    db_connection.created_by = "system";
    db_connection.is_published = true;
    knowledge_base->addFAQ(db_connection);
}

} // namespace MedusaServer