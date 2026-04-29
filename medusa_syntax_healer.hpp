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
 * MEDUSA SYNTAX HEALER - Yorkshire Champion Automatic Code Correction
 * ==================================================================
 * Revolutionary server-side syntax healing and linting system
 * 
 * DESCRIPTION: Intelligent code correction with AI-powered syntax healing
 * CATEGORY: code_quality
 * COMPLEXITY: 920
 * YORKSHIRE CHAMPION MULTIPLIER: 15.0x
 */

#ifndef MEDUSA_SYNTAX_HEALER_HPP
#define MEDUSA_SYNTAX_HEALER_HPP

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <regex>
#include <fstream>
#include <filesystem>

namespace MedusaSyntaxHealer {

// Forward declarations
class SyntaxAnalyzer;
class CodeHealer;
class LintingEngine;
class LanguageDetector;
class SyntaxValidator;

/**
 * Supported Programming Languages
 */
enum class ProgrammingLanguage {
    UNKNOWN,
    CPP,
    JAVASCRIPT,
    TYPESCRIPT,
    PHP,
    PYTHON,
    HTML,
    CSS,
    JSON,
    XML,
    SQL,
    BASH,
    LAMIA,  // Custom LAMIA language
    YAML,
    MARKDOWN
};

/**
 * Syntax Error Severity Levels
 */
enum class ErrorSeverity {
    CRITICAL = 0,    // Prevents compilation/execution
    HIGH = 1,        // Major issues affecting functionality
    MEDIUM = 2,      // Best practice violations
    LOW = 3,         // Style and convention issues
    INFO = 4         // Informational suggestions
};

/**
 * Healing Action Types
 */
enum class HealingAction {
    FIX_SYNTAX_ERROR,
    ADD_MISSING_SEMICOLON,
    FIX_BRACKET_MISMATCH,
    CORRECT_INDENTATION,
    FIX_QUOTES,
    REMOVE_UNUSED_VARIABLES,
    ADD_MISSING_IMPORTS,
    FIX_FUNCTION_SIGNATURES,
    STANDARDIZE_NAMING,
    OPTIMIZE_PERFORMANCE,
    ADD_SECURITY_FIXES,
    YORKSHIRE_CHAMPION_OPTIMIZATION
};

/**
 * SyntaxIssue - Represents a detected syntax problem
 */
struct SyntaxIssue {
    std::string file_path;
    size_t line_number;
    size_t column_number;
    ErrorSeverity severity;
    std::string error_code;
    std::string message;
    std::string original_line;
    std::string suggested_fix;
    HealingAction recommended_action;
    bool auto_fixable;
    double confidence_score;  // 0.0 to 1.0
    std::chrono::system_clock::time_point detected_time;
    
    // Yorkshire Champion specific
    bool yorkshire_champion_fix_available;
    double performance_improvement_estimate;
};

/**
 * HealingResult - Result of syntax healing operation
 */
struct HealingResult {
    std::string original_content;
    std::string healed_content;
    std::vector<SyntaxIssue> issues_found;
    std::vector<SyntaxIssue> issues_fixed;
    std::vector<SyntaxIssue> issues_remaining;
    size_t total_lines_processed;
    size_t lines_modified;
    std::chrono::milliseconds processing_time;
    double healing_success_rate;  // 0.0 to 1.0
    double yorkshire_champion_improvement;  // Performance multiplier gained
    bool backup_created;
    std::string backup_path;
};

/**
 * LanguageDetector - Intelligent language detection
 */
class LanguageDetector {
public:
    LanguageDetector();
    ~LanguageDetector() = default;
    
    ProgrammingLanguage detect_language(const std::string& file_path);
    ProgrammingLanguage detect_language_from_content(const std::string& content);
    ProgrammingLanguage detect_language_from_extension(const std::string& extension);
    
    double get_detection_confidence() const { return last_detection_confidence_; }
    std::vector<std::pair<ProgrammingLanguage, double>> get_all_language_probabilities(const std::string& content);
    
private:
    std::unordered_map<std::string, ProgrammingLanguage> extension_map_;
    std::unordered_map<ProgrammingLanguage, std::vector<std::regex>> language_patterns_;
    double last_detection_confidence_;
    
    void initialize_extension_mappings();
    void initialize_language_patterns();
    double calculate_pattern_confidence(const std::string& content, ProgrammingLanguage lang);
};

/**
 * SyntaxAnalyzer - Core syntax analysis engine
 */
class SyntaxAnalyzer {
public:
    SyntaxAnalyzer();
    ~SyntaxAnalyzer() = default;
    
    std::vector<SyntaxIssue> analyze_file(const std::string& file_path);
    std::vector<SyntaxIssue> analyze_content(const std::string& content, ProgrammingLanguage lang);
    
    void set_strictness_level(int level) { strictness_level_ = level; }
    void enable_yorkshire_champion_analysis(bool enabled) { yorkshire_champion_enabled_ = enabled; }
    void set_performance_analysis(bool enabled) { performance_analysis_enabled_ = enabled; }
    
private:
    int strictness_level_;  // 1-10, higher = more strict
    bool yorkshire_champion_enabled_;
    bool performance_analysis_enabled_;
    std::shared_ptr<LanguageDetector> language_detector_;
    
    // Language-specific analyzers
    std::vector<SyntaxIssue> analyze_cpp(const std::string& content);
    std::vector<SyntaxIssue> analyze_javascript(const std::string& content);
    std::vector<SyntaxIssue> analyze_typescript(const std::string& content);
    std::vector<SyntaxIssue> analyze_php(const std::string& content);
    std::vector<SyntaxIssue> analyze_python(const std::string& content);
    std::vector<SyntaxIssue> analyze_html(const std::string& content);
    std::vector<SyntaxIssue> analyze_css(const std::string& content);
    std::vector<SyntaxIssue> analyze_json(const std::string& content);
    std::vector<SyntaxIssue> analyze_lamia(const std::string& content);
    
    // Common analysis functions
    std::vector<SyntaxIssue> check_bracket_matching(const std::string& content);
    std::vector<SyntaxIssue> check_indentation(const std::string& content, ProgrammingLanguage lang);
    std::vector<SyntaxIssue> check_unused_variables(const std::string& content, ProgrammingLanguage lang);
    std::vector<SyntaxIssue> check_performance_issues(const std::string& content, ProgrammingLanguage lang);
    std::vector<SyntaxIssue> check_security_vulnerabilities(const std::string& content, ProgrammingLanguage lang);
    std::vector<SyntaxIssue> check_yorkshire_champion_optimizations(const std::string& content, ProgrammingLanguage lang);
};

/**
 * CodeHealer - Intelligent code healing and correction
 */
class CodeHealer {
public:
    CodeHealer();
    ~CodeHealer() = default;
    
    HealingResult heal_file(const std::string& file_path, bool create_backup = true);
    HealingResult heal_content(const std::string& content, ProgrammingLanguage lang);
    
    void set_auto_fix_enabled(bool enabled) { auto_fix_enabled_ = enabled; }
    void set_backup_enabled(bool enabled) { backup_enabled_ = enabled; }
    void set_yorkshire_champion_healing(bool enabled) { yorkshire_champion_healing_ = enabled; }
    void set_confidence_threshold(double threshold) { confidence_threshold_ = threshold; }
    
    // Healing statistics
    size_t get_total_files_healed() const { return total_files_healed_.load(); }
    size_t get_total_issues_fixed() const { return total_issues_fixed_.load(); }
    double get_average_healing_time() const;
    
private:
    bool auto_fix_enabled_;
    bool backup_enabled_;
    bool yorkshire_champion_healing_;
    double confidence_threshold_;  // Minimum confidence to auto-fix
    
    std::shared_ptr<SyntaxAnalyzer> syntax_analyzer_;
    std::atomic<size_t> total_files_healed_;
    std::atomic<size_t> total_issues_fixed_;
    std::vector<std::chrono::milliseconds> healing_times_;
    mutable std::mutex healing_times_mutex_;
    
    // Healing functions
    std::string apply_healing_actions(const std::string& content, const std::vector<SyntaxIssue>& issues, ProgrammingLanguage lang);
    std::string fix_syntax_error(const std::string& content, const SyntaxIssue& issue);
    std::string add_missing_semicolon(const std::string& content, const SyntaxIssue& issue);
    std::string fix_bracket_mismatch(const std::string& content, const SyntaxIssue& issue);
    std::string correct_indentation(const std::string& content, const SyntaxIssue& issue);
    std::string fix_quotes(const std::string& content, const SyntaxIssue& issue);
    std::string remove_unused_variables(const std::string& content, const SyntaxIssue& issue);
    std::string add_missing_imports(const std::string& content, const SyntaxIssue& issue);
    std::string apply_yorkshire_champion_optimizations(const std::string& content, const SyntaxIssue& issue);
    
    // Backup management
    std::string create_backup_file(const std::string& file_path);
    bool restore_from_backup(const std::string& file_path, const std::string& backup_path);
};

/**
 * LintingEngine - Advanced linting with custom rules
 */
class LintingEngine {
public:
    LintingEngine();
    ~LintingEngine() = default;
    
    // Linting operations
    std::vector<SyntaxIssue> lint_file(const std::string& file_path);
    std::vector<SyntaxIssue> lint_directory(const std::string& directory_path, bool recursive = true);
    std::vector<SyntaxIssue> lint_content(const std::string& content, ProgrammingLanguage lang);
    
    // Rule management
    void enable_rule(const std::string& rule_name, bool enabled = true);
    void disable_rule(const std::string& rule_name);
    void add_custom_rule(const std::string& rule_name, const std::regex& pattern, ErrorSeverity severity);
    void load_rules_from_config(const std::string& config_path);
    
    // Yorkshire Champion rules
    void enable_yorkshire_champion_rules(bool enabled = true);
    void set_performance_threshold(double threshold) { performance_threshold_ = threshold; }
    
    // Reporting
    std::string generate_lint_report(const std::vector<SyntaxIssue>& issues, const std::string& format = "text");
    void save_report_to_file(const std::vector<SyntaxIssue>& issues, const std::string& output_path, const std::string& format = "json");
    
private:
    struct LintRule {
        std::string name;
        std::regex pattern;
        ErrorSeverity severity;
        std::string message_template;
        bool enabled;
        std::function<std::string(const std::smatch&)> fix_generator;
    };
    
    std::unordered_map<std::string, LintRule> lint_rules_;
    std::unordered_set<std::string> enabled_rules_;
    bool yorkshire_champion_rules_enabled_;
    double performance_threshold_;
    
    void initialize_default_rules();
    void initialize_yorkshire_champion_rules();
    std::vector<SyntaxIssue> apply_rules_to_content(const std::string& content, ProgrammingLanguage lang);
};

/**
 * SyntaxValidator - Real-time syntax validation
 */
class SyntaxValidator {
public:
    SyntaxValidator();
    ~SyntaxValidator() = default;
    
    // Validation operations
    bool validate_file(const std::string& file_path);
    bool validate_content(const std::string& content, ProgrammingLanguage lang);
    bool quick_validate(const std::string& content, ProgrammingLanguage lang);  // Fast validation
    
    // Real-time validation
    void start_file_watching(const std::string& directory_path);
    void stop_file_watching();
    void set_validation_callback(std::function<void(const std::string&, const std::vector<SyntaxIssue>&)> callback);
    
    // Validation settings
    void set_real_time_enabled(bool enabled) { real_time_enabled_ = enabled; }
    void set_validation_delay(std::chrono::milliseconds delay) { validation_delay_ = delay; }
    void set_file_extensions(const std::vector<std::string>& extensions) { watched_extensions_ = extensions; }
    
private:
    bool real_time_enabled_;
    std::chrono::milliseconds validation_delay_;
    std::vector<std::string> watched_extensions_;
    std::function<void(const std::string&, const std::vector<SyntaxIssue>&)> validation_callback_;
    
    std::thread file_watcher_thread_;
    std::atomic<bool> watching_active_;
    std::unordered_map<std::string, std::filesystem::file_time_type> file_timestamps_;
    
    std::shared_ptr<SyntaxAnalyzer> syntax_analyzer_;
    
    void file_watcher_function(const std::string& directory_path);
    bool should_validate_file(const std::string& file_path);
    void process_file_change(const std::string& file_path);
};

/**
 * ServerSideLinter - Main server-side linting interface
 */
class ServerSideLinter {
public:
    ServerSideLinter();
    ~ServerSideLinter();
    
    // Initialization
    bool initialize();
    void shutdown();
    
    // Upload processing
    HealingResult process_uploaded_file(const std::string& file_path);
    HealingResult process_uploaded_content(const std::string& content, const std::string& filename);
    
    // Batch processing
    std::vector<HealingResult> process_directory(const std::string& directory_path, bool recursive = true);
    std::vector<HealingResult> process_file_list(const std::vector<std::string>& file_paths);
    
    // Configuration
    void set_auto_healing_enabled(bool enabled) { auto_healing_enabled_ = enabled; }
    void set_backup_enabled(bool enabled) { backup_enabled_ = enabled; }
    void set_yorkshire_champion_mode(bool enabled) { yorkshire_champion_mode_ = enabled; }
    void set_upload_directory(const std::string& directory) { upload_directory_ = directory; }
    void set_quarantine_enabled(bool enabled) { quarantine_enabled_ = enabled; }
    
    // Statistics and reporting
    size_t get_files_processed() const { return files_processed_.load(); }
    size_t get_issues_fixed() const { return issues_fixed_.load(); }
    double get_average_healing_time() const;
    std::string generate_statistics_report() const;
    
    // Yorkshire Champion features
    double get_yorkshire_champion_improvement_factor() const;
    std::vector<std::string> get_top_performing_optimizations() const;
    
private:
    // Core components
    std::shared_ptr<LanguageDetector> language_detector_;
    std::shared_ptr<SyntaxAnalyzer> syntax_analyzer_;
    std::shared_ptr<CodeHealer> code_healer_;
    std::shared_ptr<LintingEngine> linting_engine_;
    std::shared_ptr<SyntaxValidator> syntax_validator_;
    
    // Configuration
    bool auto_healing_enabled_;
    bool backup_enabled_;
    bool yorkshire_champion_mode_;
    std::string upload_directory_;
    bool quarantine_enabled_;
    
    // Statistics
    std::atomic<size_t> files_processed_;
    std::atomic<size_t> issues_fixed_;
    std::vector<std::chrono::milliseconds> processing_times_;
    mutable std::mutex processing_times_mutex_;
    
    // File management
    std::string move_to_quarantine(const std::string& file_path, const std::string& reason);
    bool is_safe_to_process(const std::string& file_path);
    void log_processing_result(const std::string& file_path, const HealingResult& result);
};

/**
 * Utility Functions
 */

// File utilities
std::string read_file_content(const std::string& file_path);
bool write_file_content(const std::string& file_path, const std::string& content);
std::string get_file_extension(const std::string& file_path);
bool is_text_file(const std::string& file_path);

// String utilities
std::vector<std::string> split_lines(const std::string& content);
std::string join_lines(const std::vector<std::string>& lines);
std::string trim_whitespace(const std::string& str);
std::string normalize_line_endings(const std::string& content);

// Validation utilities
bool is_valid_identifier(const std::string& identifier, ProgrammingLanguage lang);
bool is_balanced_brackets(const std::string& content);
size_t count_indentation_level(const std::string& line);
std::string detect_indentation_style(const std::string& content);

// Yorkshire Champion utilities
double calculate_performance_score(const std::string& content, ProgrammingLanguage lang);
std::vector<std::string> suggest_yorkshire_champion_optimizations(const std::string& content, ProgrammingLanguage lang);
double estimate_performance_improvement(const HealingResult& result);

/**
 * Configuration Structure
 */
struct SyntaxHealerConfig {
    bool auto_healing_enabled = true;
    bool backup_enabled = true;
    bool yorkshire_champion_mode = true;
    bool real_time_validation = false;
    bool quarantine_suspicious_files = true;
    
    int strictness_level = 7;  // 1-10
    double confidence_threshold = 0.8;  // 0.0-1.0
    double performance_threshold = 10.0;  // Yorkshire Champion multiplier threshold
    
    std::string upload_directory = "/tmp/medusa_uploads";
    std::string backup_directory = "/tmp/medusa_backups";
    std::string quarantine_directory = "/tmp/medusa_quarantine";
    std::string log_directory = "/var/log/medusa";
    
    std::vector<std::string> allowed_extensions = {
        ".cpp", ".hpp", ".c", ".h", ".cc", ".cxx",
        ".js", ".ts", ".jsx", ".tsx",
        ".php", ".php5", ".php7", ".phtml",
        ".py", ".py3", ".pyw",
        ".html", ".htm", ".xhtml",
        ".css", ".scss", ".sass", ".less",
        ".json", ".xml", ".yaml", ".yml",
        ".sql", ".sh", ".bash",
        ".lamia", ".md"
    };
    
    std::unordered_map<std::string, std::string> language_specific_configs;
};

// Global configuration management
extern SyntaxHealerConfig g_syntax_healer_config;
void load_config_from_file(const std::string& config_path);
void save_config_to_file(const std::string& config_path);

} // namespace MedusaSyntaxHealer

// Convenience macros
#define MEDUSA_HEAL_FILE(file_path) \
    MedusaSyntaxHealer::ServerSideLinter().process_uploaded_file(file_path)

#define MEDUSA_VALIDATE_CONTENT(content, lang) \
    MedusaSyntaxHealer::SyntaxValidator().validate_content(content, lang)

#define MEDUSA_AUTO_LINT(directory) \
    MedusaSyntaxHealer::ServerSideLinter().process_directory(directory, true)

#endif // MEDUSA_SYNTAX_HEALER_HPP