#pragma once

/**
 * MEDUSA DEBUG ORACLE - AI-Powered Interface Debugging v0.2.7a
 * ============================================================
 * 
 * Revolutionary AI debugging system that explains UI issues in natural language
 * Provides intelligent solutions and real-time visual debugging overlays
 * 
 * Ground-up AI implementation for developer and user experience enhancement
 * Yorkshire Champion Gold Standard - Intelligent Debug Revolution
 */

#include "medusa_theme_engine.hpp"
#include "neural_design_system.hpp"
#include "../services/icewall/foundation/Types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <chrono>
#include <functional>
#include <optional>

namespace MedusaGUI::Debug {

    /**
     * Debug Issue Classifications
     */
    enum class IssueType {
        VISUAL_RENDERING = 1,      // Visual appearance problems
        INTERACTION_FAILURE = 2,   // User interaction issues
        PERFORMANCE_DEGRADATION = 3, // Speed/responsiveness issues
        ACCESSIBILITY_VIOLATION = 4, // A11Y compliance problems
        SECURITY_CONCERN = 5,      // Security-related issues
        USER_EXPERIENCE = 6,       // UX/usability problems
        THEME_INCONSISTENCY = 7,   // Theme/branding violations
        RESPONSIVE_LAYOUT = 8,     // Layout/responsive design issues
        ANIMATION_GLITCH = 9,      // Animation/transition problems
        DATA_DISPLAY = 10,         // Data visualization issues
        UNKNOWN = 99               // Unclassified issue
    };

    /**
     * Debug Severity Levels
     */
    enum class DebugSeverity {
        INFO = 1,        // Informational, no action needed
        LOW = 2,         // Minor issue, low priority
        MEDIUM = 3,      // Moderate issue, should be addressed
        HIGH = 4,        // Important issue, needs attention
        CRITICAL = 5     // Critical issue, immediate action required
    };

    /**
     * Solution Option Structure
     */
    struct SolutionOption {
        std::string solution_id;
        std::string title;
        std::string description;
        std::string implementation_steps;
        
        // Solution characteristics
        DebugSeverity complexity;
        std::chrono::minutes estimated_implementation_time;
        double success_probability;
        
        // Impact assessment
        double user_experience_improvement;
        double performance_impact;
        bool requires_restart;
        bool affects_other_components;
        
        // Code changes required
        struct CodeChanges {
            std::vector<std::string> files_to_modify;
            std::vector<std::string> css_changes;
            std::vector<std::string> js_changes;
            std::vector<std::string> html_changes;
            std::vector<std::string> config_changes;
        } code_changes;
        
        // Risks and considerations
        std::vector<std::string> potential_risks;
        std::vector<std::string> considerations;
        std::string rollback_plan;
    };

    /**
     * File Modification Structure
     */
    struct FileModification {
        std::string file_path;
        std::string modification_type;    // "edit", "create", "delete"
        std::string old_content;
        std::string new_content;
        std::vector<int> line_numbers;    // Affected line numbers
        std::string change_reason;
    };

    /**
     * Configuration Change Structure
     */
    struct ConfigurationChange {
        std::string config_section;
        std::string config_key;
        std::string old_value;
        std::string new_value;
        std::string change_reason;
        bool requires_restart;
    };

    /**
     * Debug Command Structure - AI Commands
     */
    struct DebugCommand {
        std::string command_id;
        std::string user_id;
        std::string element_id;
        std::string debug_intent;         // "analyze", "fix", "optimize", "explain"
        std::string context_data;         // Current state information
        std::vector<std::string> symptoms; // User-observed issues
        
        // AI Analysis Request
        enum class AnalysisDepth {
            SURFACE,        // Quick visual analysis
            DEEP,          // Comprehensive code analysis  
            NEURAL,        // AI pattern recognition
            QUANTUM        // Complete system analysis
        } analysis_depth = AnalysisDepth::DEEP;
        
        // User preferences for response
        bool prefer_natural_language = true;
        bool include_code_examples = true;
        bool request_visual_aids = true;
        
        // Context
        std::string browser_info;
        std::string device_info;
        std::string screen_resolution;
        std::unordered_map<std::string, std::string> environment_variables;
    };

    /**
     * Issue Analysis Structure
     */
    struct IssueAnalysis {
        std::string analysis_id;
        IssueType issue_type;
        DebugSeverity severity;
        
        std::string root_cause;
        double confidence_score;
        std::vector<std::string> contributing_factors;
        
        // Explanations
        std::string technical_explanation;
        std::string user_friendly_explanation;
        std::string impact_description;
        
        // Data sources
        std::vector<std::string> analysis_data_sources;
        std::chrono::system_clock::time_point analyzed_at;
        
        // Related issues
        std::vector<std::string> related_issue_ids;
        std::vector<std::string> potential_cascade_effects;
    };

    /**
     * Debug Response Structure
     */
    struct DebugResponse {
        std::string response_id;
        std::string command_id;
        std::chrono::system_clock::time_point generated_at;
        
        // Issue Analysis
        IssueAnalysis analysis;
        
        // Solution Recommendations
        struct Solutions {
            std::vector<SolutionOption> quick_fixes;
            std::vector<SolutionOption> comprehensive_fixes;
            std::vector<SolutionOption> optimization_suggestions;
            SolutionOption recommended_solution;
            
            // Alternative approaches
            std::vector<SolutionOption> alternative_approaches;
            std::vector<std::string> workaround_suggestions;
        } solutions;
        
        // Code Modifications
        struct CodeChanges {
            std::vector<FileModification> file_changes;
            std::vector<ConfigurationChange> config_changes;
            std::vector<std::string> dependency_updates;
            bool requires_rebuild;
            std::string build_command;
        } code_changes;
        
        // Performance Impact Assessment
        struct PerformanceImpact {
            double performance_improvement_percent;
            double user_experience_score_change;
            std::chrono::milliseconds estimated_fix_time;
            std::string impact_summary;
            
            // Resource usage predictions
            double cpu_impact_percent;
            double memory_impact_percent;
            double network_impact_percent;
        } performance_impact;
        
        // Visual aids and examples
        struct VisualAids {
            std::vector<std::string> before_after_comparisons;
            std::vector<std::string> code_examples;
            std::vector<std::string> screenshot_suggestions;
            std::string visual_explanation_url;
        } visual_aids;
        
        // Follow-up recommendations
        std::vector<std::string> monitoring_suggestions;
        std::vector<std::string> preventive_measures;
        std::string documentation_links;
    };

    /**
     * Debug Session Tracking
     */
    struct DebugSession {
        std::string session_id;
        std::string user_id;
        std::chrono::system_clock::time_point started_at;
        std::chrono::system_clock::time_point last_activity;
        
        // Session data
        std::vector<DebugCommand> commands_executed;
        std::vector<DebugResponse> responses_generated;
        std::vector<std::string> issues_discovered;
        std::vector<std::string> solutions_attempted;
        
        // Session outcomes
        struct SessionOutcome {
            bool issues_resolved;
            double user_satisfaction_score;
            std::chrono::minutes total_debug_time;
            std::vector<std::string> successful_solutions;
            std::vector<std::string> failed_solutions;
            std::string final_status;
        } outcome;
        
        // Learning data
        std::vector<std::string> user_feedback;
        std::vector<std::string> patterns_discovered;
        std::vector<std::string> knowledge_gained;
    };

    /**
     * Debug Knowledge Base Entry
     */
    struct DebugPattern {
        std::string pattern_id;
        std::string pattern_name;
        std::string description;
        
        // Pattern matching
        std::vector<std::string> symptom_keywords;
        std::vector<std::string> context_patterns;
        std::function<bool(const DebugCommand&)> matcher_function;
        
        // Solution mapping
        std::vector<SolutionOption> known_solutions;
        std::unordered_map<std::string, double> solution_success_rates;
        
        // Pattern evolution
        uint64_t occurrence_count = 0;
        double pattern_reliability_score = 0.0;
        std::chrono::system_clock::time_point last_updated;
        
        // Learning metadata
        std::vector<std::string> contributing_debug_sessions;
        std::vector<std::string> evolution_history;
    };

    /**
     * Performance Metrics for Debug Operations
     */
    struct PerformanceMetrics {
        std::string element_id;
        std::chrono::system_clock::time_point measured_at;
        
        // Rendering metrics
        double render_time_ms;
        double paint_time_ms;
        double layout_time_ms;
        double composite_time_ms;
        
        // Interaction metrics
        double first_input_delay_ms;
        double interaction_to_next_paint_ms;
        double cumulative_layout_shift;
        
        // Resource metrics
        double memory_usage_mb;
        double cpu_usage_percent;
        double network_requests_count;
        double network_transfer_size_kb;
        
        // User experience metrics
        double perceived_performance_score; // 1-10 scale
        bool user_satisfied_with_speed;
        std::vector<std::string> performance_complaints;
        
        // Comparative metrics
        double performance_vs_baseline_percent;
        double performance_vs_similar_elements_percent;
    };

    /**
     * AI Insights for Debugging
     */
    struct AIInsights {
        std::string insights_id;
        std::string element_id;
        std::chrono::system_clock::time_point generated_at;
        
        // Behavioral insights
        struct BehaviorInsights {
            std::vector<std::string> user_interaction_patterns;
            std::vector<std::string> common_user_difficulties;
            std::vector<std::string> successful_interaction_flows;
            std::vector<std::string> error_prone_areas;
        } behavior_insights;
        
        // Performance insights
        struct PerformanceInsights {
            std::vector<std::string> bottleneck_identifications;
            std::vector<std::string> optimization_opportunities;
            double predicted_improvement_potential;
            std::vector<std::string> resource_usage_analysis;
        } performance_insights;
        
        // Visual insights
        struct VisualInsights {
            std::vector<std::string> design_inconsistencies;
            std::vector<std::string> visual_hierarchy_issues;
            std::vector<std::string> color_accessibility_concerns;
            std::vector<std::string> typography_problems;
        } visual_insights;
        
        // Code insights
        struct CodeInsights {
            std::vector<std::string> code_quality_issues;
            std::vector<std::string> architectural_concerns;
            std::vector<std::string> maintainability_problems;
            std::vector<std::string> security_vulnerabilities;
        } code_insights;
        
        double overall_confidence_score;
        std::string summary_assessment;
    };

    /**
     * Debug Outcome Tracking
     */
    struct DebugOutcome {
        std::string outcome_id;
        std::string session_id;
        
        // Resolution data
        bool issue_fully_resolved;
        bool user_satisfied;
        double resolution_time_minutes;
        
        // Solutions effectiveness
        struct SolutionEffectiveness {
            std::string solution_id;
            bool was_successful;
            double implementation_time_minutes;
            double user_satisfaction_score;
            std::vector<std::string> unexpected_side_effects;
        };
        
        std::vector<SolutionEffectiveness> solutions_tried;
        
        // Learning value
        double learning_value_score;   // How much this outcome taught the AI
        std::vector<std::string> new_patterns_discovered;
        std::vector<std::string> knowledge_base_updates;
        
        // User feedback
        std::string user_feedback_text;
        std::unordered_map<std::string, int> user_ratings; // aspect -> rating (1-5)
    };

    /**
     * MAIN MEDUSA DEBUG ORACLE
     */
    class MedusaDebugOracle {
    private:
        // AI Analysis Components
        std::unique_ptr<void> issue_classifier_;        // ML model for issue classification
        std::unique_ptr<void> solution_generator_;      // ML model for solution generation
        std::unique_ptr<void> code_analyzer_;          // Code analysis engine
        std::unique_ptr<void> pattern_matcher_;        // Pattern matching system
        
        // Integration with other systems
        std::shared_ptr<Theme::MedusaThemeEngine> theme_engine_;
        std::shared_ptr<Neural::UserBehaviorLearner> behavior_learner_;
        std::shared_ptr<MedusaServ::IceWall::Core::IceWallEngine> icewall_engine_;
        
        // Debug State Management
        std::unordered_map<std::string, DebugSession> active_sessions_;
        std::vector<DebugPattern> learned_patterns_;
        std::unordered_map<std::string, AIInsights> insights_cache_;
        std::mutex debug_mutex_;
        
        // Knowledge Base
        struct KnowledgeBase {
            std::vector<DebugPattern> patterns;
            std::unordered_map<IssueType, std::vector<SolutionOption>> solution_templates;
            std::unordered_map<std::string, std::vector<std::string>> common_causes;
            std::unordered_map<std::string, double> solution_success_rates;
        } knowledge_base_;
        
        // Performance tracking
        std::atomic<uint64_t> total_debug_sessions_{0};
        std::atomic<uint64_t> successful_resolutions_{0};
        std::atomic<double> average_resolution_time_{0.0};
        
    public:
        MedusaDebugOracle();
        ~MedusaDebugOracle();
        
        // Initialization
        bool initialize(std::shared_ptr<Theme::MedusaThemeEngine> theme_engine,
                       std::shared_ptr<Neural::UserBehaviorLearner> behavior_learner);
        void shutdown();
        
        // Main Debug Operations
        DebugResponse analyze_element_issue(const DebugCommand& command);
        
        DebugResponse generate_fix_suggestions(const std::string& element_id,
                                             const std::string& issue_description);
        
        DebugResponse optimize_element_performance(const std::string& element_id,
                                                 const PerformanceMetrics& current_metrics);
        
        // AI-Powered Explanations
        std::string explain_element_behavior(const std::string& element_id,
                                           const std::string& behavior_question);
        
        std::string suggest_improvements(const std::string& element_id,
                                       const std::string& user_feedback);
        
        std::string explain_optimization_decision(const std::string& element_id,
                                               const Theme::OptimizationResult& result);
        
        // Interactive Debug Sessions
        std::string start_debug_session(const std::string& user_id);
        
        DebugResponse execute_debug_command_in_session(const std::string& session_id,
                                                     const DebugCommand& command);
        
        void end_debug_session(const std::string& session_id,
                              const DebugOutcome& outcome);
        
        // Visual Debug Integration
        AIInsights generate_visual_insights(const std::string& element_id);
        
        std::string generate_debug_overlay_data(const std::string& element_id);
        
        // Learning and Adaptation
        void learn_from_debug_session(const DebugSession& session,
                                    const DebugOutcome& outcome);
        
        void update_knowledge_base(const std::vector<DebugPattern>& new_patterns);
        
        // Pattern Management
        std::vector<DebugPattern> identify_patterns_from_session(const DebugSession& session);
        
        bool match_known_pattern(const DebugCommand& command);
        
        std::vector<SolutionOption> get_solutions_for_pattern(const std::string& pattern_id);
        
        // Analytics and Reporting
        struct DebugAnalytics {
            uint64_t total_sessions;
            uint64_t successful_resolutions;
            double success_rate_percent;
            double average_resolution_time_minutes;
            
            std::unordered_map<IssueType, uint64_t> issue_type_distribution;
            std::unordered_map<std::string, uint64_t> most_common_problems;
            std::unordered_map<std::string, double> solution_effectiveness_scores;
            
            std::vector<std::string> trending_issues;
            std::vector<std::string> emerging_patterns;
        };
        
        DebugAnalytics get_debug_analytics() const;
        
        struct SystemHealth {
            bool oracle_operational;
            bool ai_models_healthy;
            bool knowledge_base_current;
            double system_confidence_score;
            std::chrono::system_clock::time_point last_model_update;
        };
        
        SystemHealth get_system_health() const;
        
    private:
        // AI Implementation Details
        IssueAnalysis perform_ai_analysis(const std::string& element_id,
                                        const std::vector<std::string>& symptoms);
        
        std::vector<SolutionOption> generate_ai_solutions(const IssueAnalysis& analysis);
        
        std::string generate_natural_language_explanation(const IssueAnalysis& analysis);
        
        AIInsights generate_comprehensive_insights(const std::string& element_id);
        
        // Pattern matching and learning
        std::vector<DebugPattern> extract_patterns_from_outcome(const DebugOutcome& outcome);
        
        void update_pattern_reliability(const std::string& pattern_id, bool was_successful);
        
        // Code analysis helpers
        std::vector<std::string> analyze_element_code(const std::string& element_id);
        
        std::vector<FileModification> generate_code_fixes(const IssueAnalysis& analysis);
        
        // Performance analysis
        PerformanceMetrics measure_element_performance(const std::string& element_id);
        
        std::vector<std::string> identify_performance_bottlenecks(const PerformanceMetrics& metrics);
        
        // Utility methods
        std::string generate_debug_session_id();
        void save_debug_session(const DebugSession& session);
        std::optional<DebugSession> load_debug_session(const std::string& session_id);
        void cleanup_expired_sessions();
    };

    /**
     * Debug Metadata for Elements
     */
    struct DebugMetadata {
        std::string element_id;
        std::chrono::system_clock::time_point last_analyzed;
        
        // Debug history
        std::vector<std::string> past_issues;
        std::vector<std::string> applied_fixes;
        std::vector<std::string> performance_optimizations;
        
        // Current debug status
        bool has_known_issues;
        std::vector<std::string> active_warnings;
        double debug_health_score;
        
        // AI insights summary
        std::string ai_assessment_summary;
        std::vector<std::string> improvement_suggestions;
    };

    // Global debug oracle instance
    extern std::unique_ptr<MedusaDebugOracle> g_debug_oracle;
    
    // Initialization functions
    bool initialize_debug_oracle();
    void shutdown_debug_oracle();
    MedusaDebugOracle* get_debug_oracle();

} // namespace MedusaGUI::Debug