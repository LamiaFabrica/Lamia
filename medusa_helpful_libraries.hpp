#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <regex>
#include <algorithm>

// Forward declarations
class QwenRevolutionaryBuilder;

namespace HelpfulLibraries {

    // =====================================================
    // CLAUDE CODE INTEGRATION LIBRARY
    // Assists Qwen, Claude, and User with code generation
    // =====================================================
    
    class ClaudeCodeIntegration {
    public:
        struct CodeSnippet {
            std::string language;
            std::string code;
            std::string description;
            std::vector<std::string> dependencies;
            std::string filePath;
            std::chrono::system_clock::time_point createdAt;
        };
        
        struct DocumentationEntry {
            std::string title;
            std::string content;
            std::string category;
            std::vector<std::string> tags;
            std::string author; // "Claude", "Qwen", or "User"
        };
        
    private:
        std::vector<CodeSnippet> m_codeDatabase;
        std::vector<DocumentationEntry> m_documentationDatabase;
        std::unordered_map<std::string, std::string> m_codeTemplates;
        
    public:
        // Code Generation Assistance
        void AddCodeSnippet(const CodeSnippet& snippet);
        std::vector<CodeSnippet> FindSimilarCode(const std::string& query);
        std::string GenerateCodeFromTemplate(const std::string& templateName, 
                                           const std::unordered_map<std::string, std::string>& params);
        
        // Documentation Management
        void AddDocumentation(const DocumentationEntry& doc);
        std::vector<DocumentationEntry> SearchDocumentation(const std::string& query);
        std::string GenerateAPIDocumentation(const std::string& codeFile);
        
        // Claude-Qwen-User Collaboration
        void SaveCollaborationSession(const std::string& sessionId, 
                                    const std::vector<std::string>& interactions);
        std::vector<std::string> LoadCollaborationSession(const std::string& sessionId);
        
        // Code Quality Assistance
        std::vector<std::string> AnalyzeCodeQuality(const std::string& code);
        std::string SuggestImprovements(const std::string& code);
        bool ValidateCodeSyntax(const std::string& code, const std::string& language);
    };
    
    // =====================================================
    // TASK AUTOMATION LIBRARY
    // Helps automate repetitive development tasks
    // =====================================================
    
    class TaskAutomation {
    public:
        struct AutomatedTask {
            std::string taskId;
            std::string name;
            std::function<bool()> executor;
            std::vector<std::string> prerequisites;
            int priority;
            bool isRecurring;
            std::chrono::milliseconds interval;
        };
        
        struct TaskResult {
            std::string taskId;
            bool success;
            std::string output;
            std::string errorMessage;
            std::chrono::milliseconds executionTime;
        };
        
    private:
        std::vector<AutomatedTask> m_tasks;
        std::unordered_map<std::string, TaskResult> m_taskHistory;
        
    public:
        // Task Management
        void RegisterTask(const AutomatedTask& task);
        TaskResult ExecuteTask(const std::string& taskId);
        std::vector<TaskResult> ExecuteTaskChain(const std::vector<std::string>& taskIds);
        
        // Common Automation Tasks
        bool AutoGenerateCRUDOperations(const std::string& entityName, 
                                       const std::vector<std::string>& fields);
        bool AutoCreateDatabaseSchema(const std::string& schemaFile);
        bool AutoGenerateAPIEndpoints(const std::string& controllerFile);
        bool AutoCreateTestFiles(const std::string& sourceFile);
        
        // File Operations
        bool BatchRenameFiles(const std::string& directory, 
                            const std::string& pattern, 
                            const std::string& replacement);
        bool BatchProcessFiles(const std::string& directory,
                             const std::string& extension,
                             std::function<bool(const std::string&)> processor);
        
        // Code Generation
        std::string GenerateBoilerplateClass(const std::string& className,
                                            const std::vector<std::string>& members);
        std::string GenerateUnitTest(const std::string& functionName,
                                    const std::vector<std::string>& testCases);
    };
    
    // =====================================================
    // KNOWLEDGE BASE LIBRARY
    // Stores and retrieves development knowledge
    // =====================================================
    
    class KnowledgeBase {
    public:
        struct KnowledgeEntry {
            std::string id;
            std::string title;
            std::string content;
            std::string category;
            std::vector<std::string> tags;
            std::string source; // "Claude", "Qwen", "User", "Documentation"
            int usefulness; // 1-10 rating
            std::chrono::system_clock::time_point lastUsed;
        };
        
        struct SearchResult {
            KnowledgeEntry entry;
            float relevanceScore;
        };
        
    private:
        std::vector<KnowledgeEntry> m_knowledge;
        std::unordered_map<std::string, std::vector<std::string>> m_categoryIndex;
        std::unordered_map<std::string, std::vector<std::string>> m_tagIndex;
        
    public:
        // Knowledge Management
        void AddKnowledge(const KnowledgeEntry& entry);
        void UpdateKnowledge(const std::string& id, const KnowledgeEntry& entry);
        bool RemoveKnowledge(const std::string& id);
        
        // Search and Retrieval
        std::vector<SearchResult> SearchKnowledge(const std::string& query);
        std::vector<KnowledgeEntry> GetByCategory(const std::string& category);
        std::vector<KnowledgeEntry> GetByTag(const std::string& tag);
        KnowledgeEntry GetMostUseful(const std::string& category);
        
        // Common Development Knowledge
        std::string GetBestPractice(const std::string& technology);
        std::string GetCommonPattern(const std::string& problemType);
        std::vector<std::string> GetTroubleshootingSteps(const std::string& issue);
        
        // Learning and Adaptation
        void MarkAsUseful(const std::string& id);
        void AddUserFeedback(const std::string& id, const std::string& feedback);
        std::vector<std::string> GetRecommendations(const std::string& currentTask);
    };
    
    // =====================================================
    // PROJECT STRUCTURE ANALYZER
    // Analyzes and manages project architecture
    // =====================================================
    
    class ProjectStructureAnalyzer {
    public:
        struct FileInfo {
            std::string path;
            std::string type;
            size_t lineCount;
            std::vector<std::string> dependencies;
            std::vector<std::string> exports;
            std::string language;
        };
        
        struct ProjectMetrics {
            int totalFiles;
            int totalLines;
            std::unordered_map<std::string, int> languageDistribution;
            std::unordered_map<std::string, int> fileTypeDistribution;
            float codeComplexity;
            int dependencyCount;
        };
        
    private:
        std::string m_projectRoot;
        std::vector<FileInfo> m_fileDatabase;
        ProjectMetrics m_metrics;
        
    public:
        // Project Analysis
        bool AnalyzeProject(const std::string& projectRoot);
        ProjectMetrics GetProjectMetrics();
        std::vector<FileInfo> GetFilesByType(const std::string& type);
        std::vector<std::string> GetDependencyGraph(const std::string& filePath);
        
        // Architecture Suggestions
        std::vector<std::string> SuggestRefactoring();
        std::vector<std::string> IdentifyArchitecturalIssues();
        std::string RecommendProjectStructure();
        
        // Code Organization
        std::vector<std::string> FindUnusedFiles();
        std::vector<std::string> FindCircularDependencies();
        std::vector<std::string> SuggestModularization();
        
        // Migration Assistance
        std::string GenerateMigrationPlan(const std::string& targetArchitecture);
        std::vector<std::string> IdentifyMigrationRisks();
        std::string EstimateMigrationEffort();
    };
    
    // =====================================================
    // PERFORMANCE PROFILING LIBRARY
    // Helps analyze and optimize code performance
    // =====================================================
    
    class PerformanceProfiler {
    public:
        struct ProfilePoint {
            std::string name;
            std::chrono::high_resolution_clock::time_point startTime;
            std::chrono::high_resolution_clock::time_point endTime;
            std::chrono::nanoseconds duration;
        };
        
        struct PerformanceReport {
            std::vector<ProfilePoint> measurements;
            std::unordered_map<std::string, std::chrono::nanoseconds> averageTimes;
            std::unordered_map<std::string, std::chrono::nanoseconds> maxTimes;
            std::unordered_map<std::string, std::chrono::nanoseconds> minTimes;
            std::string bottleneck;
            std::vector<std::string> optimizationSuggestions;
        };
        
    private:
        std::vector<ProfilePoint> m_measurements;
        std::unordered_map<std::string, std::vector<std::chrono::nanoseconds>> m_historyDatabase;
        
    public:
        // Profiling Operations
        void StartProfiling(const std::string& name);
        void EndProfiling(const std::string& name);
        void ProfileFunction(const std::string& name, std::function<void()> func);
        
        // Analysis
        PerformanceReport GenerateReport();
        std::vector<std::string> IdentifyBottlenecks();
        std::vector<std::string> SuggestOptimizations();
        
        // Benchmarking
        std::chrono::nanoseconds BenchmarkFunction(std::function<void()> func, int iterations = 1000);
        std::vector<std::chrono::nanoseconds> BenchmarkComparison(
            const std::vector<std::pair<std::string, std::function<void()>>>& functions);
        
        // Memory Profiling
        size_t GetCurrentMemoryUsage();
        size_t GetPeakMemoryUsage();
        std::vector<std::string> DetectMemoryLeaks();
    };
    
    // =====================================================
    // COLLABORATION ASSISTANT
    // Facilitates Claude-Qwen-User collaboration
    // =====================================================
    
    class CollaborationAssistant {
    public:
        struct CollaborationSession {
            std::string sessionId;
            std::vector<std::string> participants; // "Claude", "Qwen", "User"
            std::string currentTask;
            std::unordered_map<std::string, std::string> sharedNotes;
            std::vector<std::string> actionItems;
            std::chrono::system_clock::time_point startTime;
        };
        
        struct Contribution {
            std::string author;
            std::string type; // "code", "documentation", "suggestion", "question"
            std::string content;
            std::chrono::system_clock::time_point timestamp;
        };
        
    private:
        std::unordered_map<std::string, CollaborationSession> m_sessions;
        std::unordered_map<std::string, std::vector<Contribution>> m_contributions;
        
    public:
        // Session Management
        std::string CreateSession(const std::string& task, const std::vector<std::string>& participants);
        bool JoinSession(const std::string& sessionId, const std::string& participant);
        void EndSession(const std::string& sessionId);
        
        // Contribution Tracking
        void AddContribution(const std::string& sessionId, const Contribution& contribution);
        std::vector<Contribution> GetContributions(const std::string& sessionId);
        std::vector<Contribution> GetContributionsByAuthor(const std::string& sessionId, 
                                                          const std::string& author);
        
        // Knowledge Sharing
        void ShareKnowledge(const std::string& sessionId, const std::string& knowledge);
        void AddActionItem(const std::string& sessionId, const std::string& actionItem);
        void UpdateSharedNotes(const std::string& sessionId, const std::string& key, 
                              const std::string& value);
        
        // Communication
        std::string GenerateStatusReport(const std::string& sessionId);
        std::vector<std::string> GetRecommendationsForParticipant(const std::string& sessionId,
                                                                  const std::string& participant);
        
        // Conflict Resolution
        std::vector<std::string> DetectConflicts(const std::string& sessionId);
        std::string SuggestResolution(const std::string& conflict);
    };
    
    // =====================================================
    // DEBUGGING ASSISTANT
    // Advanced debugging and troubleshooting tools
    // =====================================================
    
    class DebuggingAssistant {
    public:
        struct DebugInfo {
            std::string errorType;
            std::string errorMessage;
            std::string stackTrace;
            std::string sourceFile;
            int lineNumber;
            std::vector<std::string> variableValues;
        };
        
        struct Solution {
            std::string description;
            std::vector<std::string> steps;
            std::string codeExample;
            float confidence; // 0.0 - 1.0
        };
        
    private:
        std::unordered_map<std::string, std::vector<Solution>> m_solutionDatabase;
        std::vector<DebugInfo> m_debugHistory;
        
    public:
        // Error Analysis
        std::vector<Solution> AnalyzeError(const DebugInfo& debugInfo);
        std::string ExplainError(const std::string& errorMessage);
        std::vector<std::string> GetCommonCauses(const std::string& errorType);
        
        // Interactive Debugging
        std::string SuggestNextDebuggingStep(const DebugInfo& debugInfo);
        std::vector<std::string> GenerateTestCases(const std::string& suspectedIssue);
        std::string GenerateMinimalReproduction(const DebugInfo& debugInfo);
        
        // Code Analysis
        std::vector<std::string> FindPotentialBugs(const std::string& codeFile);
        std::vector<std::string> ValidateLogicFlow(const std::string& function);
        std::string AnalyzePerformanceIssue(const std::string& slowFunction);
        
        // Solution Management
        void AddSolution(const std::string& errorType, const Solution& solution);
        void RateSolution(const std::string& errorType, int solutionIndex, float rating);
        Solution GetBestSolution(const std::string& errorType);
    };
    
    // =====================================================
    // UTILITY FUNCTIONS
    // Common helper functions for all libraries
    // =====================================================
    
    namespace Utils {
        // String utilities
        std::vector<std::string> SplitString(const std::string& str, char delimiter);
        std::string JoinStrings(const std::vector<std::string>& strings, const std::string& separator);
        std::string ToLowerCase(const std::string& str);
        std::string ToUpperCase(const std::string& str);
        std::string TrimWhitespace(const std::string& str);
        
        // File utilities
        bool FileExists(const std::string& filePath);
        std::string ReadFileContents(const std::string& filePath);
        bool WriteFileContents(const std::string& filePath, const std::string& content);
        std::vector<std::string> ListFilesInDirectory(const std::string& directory, 
                                                     const std::string& extension = "");
        
        // Time utilities
        std::string CurrentTimestamp();
        std::string FormatDuration(std::chrono::nanoseconds duration);
        
        // Hash utilities
        std::string CalculateFileHash(const std::string& filePath);
        std::string CalculateStringHash(const std::string& content);
        
        // JSON utilities
        std::string SerializeToJSON(const std::unordered_map<std::string, std::string>& data);
        std::unordered_map<std::string, std::string> DeserializeFromJSON(const std::string& json);
    }
    
    // =====================================================
    // MEDUSA THEME LIBRARY
    // Advanced theming system for Medusa Builder
    // Collaborative workspace for Claude and User
    // =====================================================
    
    class MedusaThemeLibrary {
    public:
        struct ThemeConfig {
            std::string name;
            std::string version;
            std::string author; // "Claude", "User", "Collaborative"
            
            // Color Palette
            struct Colors {
                uint32_t primary;
                uint32_t secondary;
                uint32_t success;
                uint32_t warning;
                uint32_t error;
                uint32_t background;
                uint32_t surface;
                uint32_t text;
                uint32_t accent;
                uint32_t highlight;
            } colors;
            
            // Typography
            struct Typography {
                std::string fontFamily;
                std::string monospaceFontFamily;
                float baseFontSize;
                float headingScale;
                float lineHeight;
            } typography;
            
            // Layout
            struct Layout {
                float borderRadius;
                float padding;
                float margin;
                float shadowBlur;
                float shadowOffset;
            } layout;
            
            // Animation
            struct Animation {
                float transitionDuration;
                std::string easingFunction;
                bool enableAnimations;
                float animationSpeed;
            } animation;
            
            // Revolutionary Features
            struct Revolutionary {
                bool holographicEffects;
                bool quantumGlow;
                bool aiPoweredAdaptation;
                bool dynamicColorShifting;
                bool neuralPatterns;
                bool cosmicBackground;
            } revolutionary;
        };
        
        struct ComponentStyle {
            std::string componentName;
            std::unordered_map<std::string, std::string> styleProperties;
            std::vector<std::string> variants;
            bool isCustomizable;
        };
        
        struct AnimationSequence {
            std::string name;
            std::vector<std::pair<float, std::unordered_map<std::string, float>>> keyframes;
            float duration;
            bool loop;
        };
        
    private:
        std::vector<ThemeConfig> m_themes;
        std::unordered_map<std::string, ComponentStyle> m_componentStyles;
        std::vector<AnimationSequence> m_animations;
        ThemeConfig m_currentTheme;
        std::string m_collaborativeSessionId;
        
    public:
        // Theme Management
        void CreateTheme(const std::string& name, const std::string& author);
        void LoadTheme(const std::string& name);
        void SaveTheme(const ThemeConfig& theme);
        std::vector<std::string> GetAvailableThemes();
        
        // Collaborative Theme Development
        std::string StartCollaborativeSession(const std::string& themeName);
        void JoinCollaborativeSession(const std::string& sessionId);
        void ProposeThemeChange(const std::string& property, const std::string& value, 
                               const std::string& author);
        void ApproveThemeChange(const std::string& changeId);
        void RejectThemeChange(const std::string& changeId, const std::string& reason);
        
        // Revolutionary Theme Features
        void EnableQuantumGlow(bool enable = true);
        void SetHolographicIntensity(float intensity);
        void ConfigureAIPoweredAdaptation(bool enable);
        void SetDynamicColorShifting(bool enable, float speed = 1.0f);
        void ApplyNeuralPatterns(const std::string& pattern);
        void SetCosmicBackground(const std::string& cosmicType);
        
        // Component Styling
        void DefineComponentStyle(const ComponentStyle& style);
        ComponentStyle GetComponentStyle(const std::string& componentName);
        void CustomizeComponent(const std::string& componentName, 
                               const std::unordered_map<std::string, std::string>& overrides);
        
        // Animation System
        void CreateAnimation(const AnimationSequence& animation);
        void PlayAnimation(const std::string& animationName);
        void StopAnimation(const std::string& animationName);
        void SetGlobalAnimationSpeed(float speed);
        
        // Theme Presets
        void LoadMedusaDarkTheme();
        void LoadMedusaLightTheme();
        void LoadMedusaRevolutionaryTheme();
        void LoadMedusaQuantumTheme();
        void LoadMedusaCosmicTheme();
        void LoadMedusaAITheme();
        
        // Export/Import
        std::string ExportThemeToJSON(const std::string& themeName);
        bool ImportThemeFromJSON(const std::string& jsonData);
        void ExportThemeToCSS(const std::string& themeName, const std::string& outputPath);
        
        // Real-time Preview
        void EnableLivePreview(bool enable = true);
        void PreviewThemeChange(const std::string& property, const std::string& value);
        void ApplyPreviewChanges();
        void RevertPreviewChanges();
        
        // AI-Powered Theme Suggestions
        std::vector<std::string> GetAIThemeSuggestions(const std::string& mood);
        ThemeConfig GenerateAITheme(const std::string& description);
        void OptimizeThemeForAccessibility();
        void AdaptThemeForColorBlindness(const std::string& type);
        
        // User Experience
        void SetThemeBasedOnTime(); // Auto day/night themes
        void AdaptToSystemTheme();
        void CreatePersonalizedTheme(const std::vector<std::string>& preferences);
        
        // Performance
        void OptimizeThemeForPerformance();
        void EnableHardwareAcceleration(bool enable = true);
        void SetRenderingQuality(const std::string& quality); // "low", "medium", "high", "ultra"
    };
    
    // =====================================================
    // INTEGRATION WITH MEDUSA BUILDER
    // Direct integration with MedusaRevolutionaryBuilder
    // =====================================================
    
    class MedusaIntegration {
    private:
        QwenRevolutionaryBuilder* m_medusaBuilder;
        std::unique_ptr<MedusaThemeLibrary> m_themeLibrary;
        
    public:
        MedusaIntegration(QwenRevolutionaryBuilder* builder);
        
        // Task Integration
        void RegisterHelpfulLibraryTasks();
        void CreateAutomationWorkflows();
        void SetupKnowledgeSharing();
        
        // UI Integration
        void AddLibraryPanelsToUI();
        void CreateCollaborationInterface();
        void SetupDebuggingInterface();
        void IntegrateThemeSystem();
        
        // Data Integration
        void SyncWithTaskDatabase();
        void ExportCollaborationData();
        void ImportKnowledgeBase();
        
        // Theme Integration
        void InitializeThemeSystem();
        void CreateThemeCustomizationPanel();
        void EnableCollaborativeTheming();
        MedusaThemeLibrary* GetThemeLibrary() { return m_themeLibrary.get(); }
    };
    
    // Global instances for easy access
    extern std::unique_ptr<ClaudeCodeIntegration> g_ClaudeIntegration;
    extern std::unique_ptr<TaskAutomation> g_TaskAutomation;
    extern std::unique_ptr<KnowledgeBase> g_KnowledgeBase;
    extern std::unique_ptr<ProjectStructureAnalyzer> g_ProjectAnalyzer;
    extern std::unique_ptr<PerformanceProfiler> g_PerformanceProfiler;
    extern std::unique_ptr<CollaborationAssistant> g_CollaborationAssistant;
    extern std::unique_ptr<DebuggingAssistant> g_DebuggingAssistant;
    
    // Initialization function
    void InitializeHelpfulLibraries(QwenRevolutionaryBuilder* qwenBuilder);
    void ShutdownHelpfulLibraries();
}

// Convenience macros for common operations
#define PROFILE_FUNCTION() \
    HelpfulLibraries::g_PerformanceProfiler->StartProfiling(__FUNCTION__); \
    auto __prof_guard = [](){ HelpfulLibraries::g_PerformanceProfiler->EndProfiling(__FUNCTION__); }; \
    std::shared_ptr<void> __prof_ptr(nullptr, [=](void*){ __prof_guard(); })

#define ADD_KNOWLEDGE(title, content, category) \
    HelpfulLibraries::g_KnowledgeBase->AddKnowledge({ \
        HelpfulLibraries::Utils::CalculateStringHash(title), \
        title, content, category, {}, "System", 5, std::chrono::system_clock::now() \
    })

#define LOG_CONTRIBUTION(sessionId, type, content, author) \
    HelpfulLibraries::g_CollaborationAssistant->AddContribution(sessionId, { \
        author, type, content, std::chrono::system_clock::now() \
    })

#define AUTO_TASK(name, func) \
    HelpfulLibraries::g_TaskAutomation->RegisterTask({ \
        name, name, func, {}, 5, false, std::chrono::milliseconds(0) \
    })