#pragma once

/**
 * MEDUSA INTEGRATED DASHBOARD - Complete End-to-End CAD Generation Platform
 * =========================================================================
 * 
 * cPanel-styled user dashboard with PHPFox-styled profile management
 * Steam Community Portal-styled searchable directory with CAD model designs
 * Wizard-style text-to-photorealistic texture generation with physics-based textures
 * Integrated connectors (dowels, hinges) and dynamic plugin system
 * Deep integration with Triforce database using Neo4j scoring
 * 
 * Yorkshire Champion Gold Standard - Complete Platform Implementation
 */

#include "cad_jobs_core.hpp"
#include "medusa_json_standalone.hpp"
#include "medusa_universal_connector.hpp"
#include "medusa_triforce_connector.hpp"
#include "../lib/3d_generation/text_to_cad/libtext_to_cad_core.hpp"
#include "../lib/3d_generation/janeway/libjaneway_cad_core.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <unordered_map>
#include <queue>
#include <set>
#include <nlohmann/json.hpp>

namespace MedusaServ {
namespace IntegratedDashboard {

// User Profile Types (PHPFox-style)
enum class ProfileVisibility {
    PUBLIC,
    FRIENDS_ONLY,
    PRIVATE
};

enum class UserRole {
    VISITOR,
    USER,
    CREATOR,
    PREMIUM,
    MODERATOR,
    ADMIN
};

// Admin verification levels for content filter management
enum class AdminLevel {
    USER = 0,
    JUNIOR_ADMIN = 1,
    SENIOR_ADMIN = 2,
    GODMODE = 9
};

// Content filter access levels based on verification status
enum class FilterAccess {
    STRICT_REQUIRED = 0,    // Always enforce filtering (minors, unverified)
    MODERATE_ALLOWED = 1,   // Can disable some filters (age verified)
    ADULT_VERIFIED = 2,     // Can disable NSFW filters (admin verified adults)
    ADMIN_OVERRIDE = 9      // Can disable all filters (admin/godmode only)
};

// CAD Model Categories (Steam Community-style)
enum class ModelCategory {
    MECHANICAL_PARTS,
    ARCHITECTURAL,
    ARTISTIC_DESIGNS,
    EDUCATIONAL,
    PROTOTYPES,
    MINIATURES,
    JEWELRY,
    TOOLS,
    TOYS_GAMES,
    HOUSEHOLD,
    AUTOMOTIVE,
    ELECTRONICS,
    CUSTOM
};

// Texture Types and Physics Properties
enum class TextureType {
    PHOTOREALISTIC,
    PHYSICS_BASED,
    PROCEDURAL,
    MATERIAL_ACCURATE,
    ARTISTIC,
    TECHNICAL
};

enum class ConnectorType {
    DOWEL_JOINT,
    HINGE,
    SCREW_THREAD,
    SNAP_FIT,
    DOVETAIL,
    MORTISE_TENON,
    BALL_SOCKET,
    CUSTOM
};

// User Profile Structure (PHPFox-styled)
struct UserProfile {
    std::string user_id;
    std::string username;
    std::string display_name;
    std::string email;
    std::string avatar_url;
    std::string cover_image_url;
    ProfileVisibility visibility;
    UserRole role;
    
    // Admin verification and access control
    AdminLevel admin_level;
    FilterAccess filter_access_level;
    bool age_verified_by_admin;
    std::string verifying_admin_id;
    std::chrono::system_clock::time_point admin_verification_timestamp;
    std::string verification_notes;
    bool godmode_access;
    
    // Content filtering preferences (only available to verified users)
    bool nsfw_filter_enabled;
    bool profanity_filter_enabled;
    bool hate_speech_filter_enabled;  // Always enabled regardless of verification
    bool adult_content_allowed;
    
    // Profile information
    std::string bio;
    std::string location;
    std::string website;
    std::string skills;
    std::map<std::string, std::string> social_links;
    
    // Statistics
    int models_created;
    int models_downloaded;
    int followers;
    int following;
    double reputation_score;
    int community_points;
    
    // Preferences
    nlohmann::json notification_settings;
    nlohmann::json privacy_settings;
    nlohmann::json dashboard_preferences;
    
    // Timestamps
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point last_login;
    std::chrono::system_clock::time_point profile_updated_at;
    
    UserProfile(const std::string& id, const std::string& name);
    nlohmann::json toJson() const;
    void updateReputationScore(double delta);
    bool canViewProfile(const std::string& viewer_id) const;
    
    // Admin verification methods
    bool canDisableContentFilters() const;
    bool hasAdminAccess() const;
    bool hasGodmodeAccess() const;
    FilterAccess getFilterAccessLevel() const;
};

// CAD Model Structure (Community Portal-styled)
struct CADModel {
    std::string model_id;
    std::string creator_id;
    std::string title;
    std::string description;
    std::vector<std::string> tags;
    ModelCategory category;
    
    // File information
    std::string preview_image_url;
    std::vector<std::string> file_urls;
    std::map<std::string, std::string> file_formats;
    int64_t total_file_size_bytes;
    
    // Community metrics
    int downloads;
    int likes;
    int views;
    int comments;
    double rating;
    int rating_count;
    
    // Technical details
    nlohmann::json dimensions;
    nlohmann::json materials;
    nlohmann::json print_settings;
    nlohmann::json texture_properties;
    nlohmann::json physics_properties;
    std::vector<ConnectorType> connectors;
    
    // Neo4j scoring
    double neo4j_score;
    nlohmann::json quality_metrics;
    nlohmann::json compatibility_score;
    
    // Visibility and permissions
    ProfileVisibility visibility;
    bool is_featured;
    bool is_downloadable;
    bool requires_attribution;
    
    // Timestamps
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
    
    CADModel(const std::string& id, const std::string& creator, const std::string& name);
    nlohmann::json toJson() const;
    void updateCommunityMetrics(int download_delta = 0, int like_delta = 0, int view_delta = 0);
    void calculateNeo4jScore();
};

// Texture Generation Request
struct TextureGenerationRequest {
    std::string request_id;
    std::string user_id;
    std::string model_id;
    
    // Input parameters
    std::string text_prompt;
    TextureType texture_type;
    nlohmann::json generation_parameters;
    nlohmann::json physics_parameters;
    
    // Generation settings
    int resolution_width;
    int resolution_height;
    bool generate_normal_map;
    bool generate_roughness_map;
    bool generate_metallic_map;
    bool generate_displacement_map;
    
    // Status
    std::string status;
    int progress_percent;
    std::string current_step;
    
    // Results
    std::map<std::string, std::string> generated_textures;
    nlohmann::json material_properties;
    nlohmann::json physics_simulation_data;
    
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point completed_at;
    
    TextureGenerationRequest(const std::string& id, const std::string& user, const std::string& prompt);
    nlohmann::json toJson() const;
    void updateProgress(int percent, const std::string& step);
};

// Wizard Step Structure
struct WizardStep {
    std::string step_id;
    std::string title;
    std::string description;
    int step_number;
    bool is_completed;
    bool is_current;
    nlohmann::json step_data;
    nlohmann::json validation_rules;
    std::vector<std::string> dependencies;
    
    WizardStep(const std::string& id, const std::string& name, int number);
    bool validate() const;
    nlohmann::json toJson() const;
};

// Main Integrated Dashboard Class
class MedusaIntegratedDashboard {
private:
    // Core systems
    std::unique_ptr<MedusaServ::CADJobs::CADJobsCore> cad_jobs_core_;
    std::unique_ptr<medusa::triforce::UniversalDBConnector> universal_connector_;
    std::unique_ptr<medusa::text_to_cad::TextToCADCore> text_to_cad_core_;
    
    // User management
    std::unordered_map<std::string, std::shared_ptr<UserProfile>> users_;
    std::unordered_map<std::string, std::shared_ptr<CADModel>> models_;
    std::unordered_map<std::string, std::shared_ptr<TextureGenerationRequest>> texture_requests_;
    
    // Community directory (Steam-style)
    std::multimap<ModelCategory, std::string> category_index_;
    std::map<std::string, std::set<std::string>> tag_index_;
    std::priority_queue<std::pair<double, std::string>> featured_models_;
    
    // Search and filtering
    std::map<std::string, std::vector<std::string>> search_cache_;
    std::chrono::system_clock::time_point last_cache_update_;
    
    // Performance metrics
    std::atomic<uint64_t> total_users_;
    std::atomic<uint64_t> total_models_;
    std::atomic<uint64_t> total_downloads_;
    std::atomic<uint64_t> total_generations_;
    std::atomic<uint64_t> wizard_completions_;
    
    // Thread safety
    mutable std::shared_mutex users_mutex_;
    mutable std::shared_mutex models_mutex_;
    mutable std::shared_mutex search_mutex_;
    std::atomic<bool> is_initialized_;
    
    // Configuration
    bool enable_public_directory_;
    bool enable_texture_generation_;
    bool enable_physics_simulation_;
    bool enable_neo4j_scoring_;
    int max_models_per_user_;
    int max_texture_requests_per_hour_;
    
public:
    explicit MedusaIntegratedDashboard();
    ~MedusaIntegratedDashboard();
    
    // Initialization
    bool initialize();
    void shutdown();
    bool isInitialized() const;
    
    // User management (cPanel-styled)
    std::shared_ptr<UserProfile> createUser(const std::string& username, 
                                           const std::string& email,
                                           const std::string& password);
    std::shared_ptr<UserProfile> getUser(const std::string& user_id);
    bool updateUser(const std::string& user_id, const nlohmann::json& updates);
    bool deleteUser(const std::string& user_id);
    
    // Profile management (PHPFox-styled)
    bool updateProfileVisibility(const std::string& user_id, ProfileVisibility visibility);
    bool updateProfileInfo(const std::string& user_id, const nlohmann::json& profile_data);
    nlohmann::json getUserDashboard(const std::string& user_id);
    nlohmann::json getPublicProfile(const std::string& user_id, const std::string& viewer_id = "");
    
    // Admin verification and content filter management
    bool verifyUserForAdultContent(const std::string& user_id, 
                                  const std::string& admin_id, 
                                  const std::string& verification_notes = "");
    bool revokeAdultVerification(const std::string& user_id, const std::string& admin_id);
    bool grantAdminAccess(const std::string& user_id, AdminLevel level, const std::string& granting_admin_id);
    bool revokeAdminAccess(const std::string& user_id, const std::string& revoking_admin_id);
    bool grantGodmodeAccess(const std::string& user_id, const std::string& granting_admin_id);
    bool updateContentFilterSettings(const std::string& user_id, const nlohmann::json& filter_settings);
    nlohmann::json getContentFilterStatus(const std::string& user_id);
    nlohmann::json getAdminVerificationLog(const std::string& user_id = "");
    
    // Model management (Community Portal-styled)
    std::shared_ptr<CADModel> createModel(const std::string& creator_id,
                                         const std::string& title,
                                         const std::string& description,
                                         ModelCategory category);
    std::shared_ptr<CADModel> getModel(const std::string& model_id);
    bool updateModel(const std::string& model_id, const nlohmann::json& updates);
    bool deleteModel(const std::string& model_id);
    
    // Community directory (Steam Community Portal-styled)
    nlohmann::json searchModels(const std::string& query,
                               ModelCategory category = ModelCategory::CUSTOM,
                               int page = 1,
                               int per_page = 20);
    nlohmann::json getFeaturedModels(int limit = 10);
    nlohmann::json getPopularModels(int limit = 20);
    nlohmann::json getRecentModels(int limit = 20);
    nlohmann::json getModelsByCategory(ModelCategory category, int limit = 50);
    nlohmann::json getModelsByTags(const std::vector<std::string>& tags, int limit = 30);
    
    // Wizard-style CAD generation system
    struct CADGenerationWizard {
        std::string wizard_id;
        std::string user_id;
        std::vector<std::shared_ptr<WizardStep>> steps;
        int current_step;
        nlohmann::json generation_data;
        std::string status;
        
        CADGenerationWizard(const std::string& id, const std::string& user);
        nlohmann::json toJson() const;
    };
    
    std::shared_ptr<CADGenerationWizard> createCADWizard(const std::string& user_id);
    bool advanceWizardStep(const std::string& wizard_id, const nlohmann::json& step_data);
    nlohmann::json getWizardStatus(const std::string& wizard_id);
    bool executeWizardGeneration(const std::string& wizard_id);
    
    // Text-to-photorealistic texture generation
    std::shared_ptr<TextureGenerationRequest> generateTexture(const std::string& user_id,
                                                              const std::string& model_id,
                                                              const std::string& prompt,
                                                              TextureType type = TextureType::PHOTOREALISTIC);
    nlohmann::json getTextureGenerationStatus(const std::string& request_id);
    nlohmann::json getGeneratedTextures(const std::string& request_id);
    
    // Physics-based connectors and joints
    nlohmann::json generateConnector(ConnectorType type,
                                    const nlohmann::json& parameters);
    nlohmann::json simulatePhysics(const std::string& model_id,
                                  const nlohmann::json& physics_parameters);
    nlohmann::json validateConnectorFit(const std::string& model1_id,
                                       const std::string& model2_id,
                                       ConnectorType connector_type);
    
    // Plugin system integration
    bool registerPlugin(const std::string& plugin_id, const nlohmann::json& plugin_config);
    bool enablePlugin(const std::string& plugin_id);
    bool disablePlugin(const std::string& plugin_id);
    nlohmann::json getAvailablePlugins();
    nlohmann::json executePlugin(const std::string& plugin_id, const nlohmann::json& parameters);
    
    // Triforce database integration with Neo4j scoring
    bool storeModelInTriforce(const std::string& model_id);
    nlohmann::json queryTriforceData(const std::string& query);
    double calculateNeo4jScore(const std::string& model_id);
    nlohmann::json getQualityMetrics(const std::string& model_id);
    nlohmann::json getCompatibilityMatrix(const std::vector<std::string>& model_ids);
    
    // Dashboard analytics
    struct DashboardMetrics {
        uint64_t total_users;
        uint64_t active_users_24h;
        uint64_t total_models;
        uint64_t models_created_24h;
        uint64_t total_downloads;
        uint64_t downloads_24h;
        uint64_t texture_generations_24h;
        uint64_t wizard_completions_24h;
        double avg_model_rating;
        double avg_generation_time_seconds;
        nlohmann::json category_distribution;
        nlohmann::json top_creators;
    };
    
    DashboardMetrics getMetrics() const;
    nlohmann::json getDetailedAnalytics(const std::string& time_range = "24h");
    
    // Settings and configuration
    void enablePublicDirectory(bool enable);
    void enableTextureGeneration(bool enable);
    void enablePhysicsSimulation(bool enable);
    void enableNeo4jScoring(bool enable);
    void setMaxModelsPerUser(int max_models);
    void setMaxTextureRequestsPerHour(int max_requests);
    
private:
    // Initialization helpers
    bool initialize_core_systems();
    bool initialize_database_connections();
    bool initialize_cad_systems();
    bool initialize_texture_generation();
    
    // Search helpers
    void rebuild_search_index();
    std::vector<std::string> search_models_internal(const std::string& query);
    void update_category_index();
    void update_tag_index();
    
    // Scoring helpers
    double calculate_model_score(const std::shared_ptr<CADModel>& model);
    void update_featured_models();
    void update_neo4j_scores();
    
    // Validation helpers
    bool validate_user_permissions(const std::string& user_id, const std::string& action);
    bool validate_model_data(const nlohmann::json& model_data);
    bool validate_texture_request(const std::shared_ptr<TextureGenerationRequest>& request);
    
    // Processing helpers
    void process_texture_generation(std::shared_ptr<TextureGenerationRequest> request);
    void process_cad_generation(std::shared_ptr<CADGenerationWizard> wizard);
    void process_physics_simulation(const std::string& model_id, const nlohmann::json& params);
    
    // Database helpers
    bool store_user_data(const std::shared_ptr<UserProfile>& user);
    bool store_model_data(const std::shared_ptr<CADModel>& model);
    bool store_texture_data(const std::shared_ptr<TextureGenerationRequest>& request);
    
    // Performance helpers
    void update_performance_metrics();
    void cleanup_expired_data();
    void optimize_search_cache();
};

// Utility functions for dashboard integration
namespace DashboardUtils {
    std::string generateSecureUserId();
    std::string generateSecureModelId();
    std::string generateSecureRequestId();
    std::string sanitizeUsername(const std::string& username);
    std::string sanitizeModelTitle(const std::string& title);
    bool isValidEmail(const std::string& email);
    std::string hashPassword(const std::string& password);
    bool validatePassword(const std::string& password, const std::string& hash);
    nlohmann::json createSuccessResponse(const nlohmann::json& data);
    nlohmann::json createErrorResponse(const std::string& error_message, int error_code = 400);
}

} // namespace IntegratedDashboard
} // namespace MedusaServ