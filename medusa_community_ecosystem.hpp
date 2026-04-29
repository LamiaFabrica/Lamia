/*
 * MEDUSA COMMUNITY ECOSYSTEM - SOCIAL PLATFORM
 * ============================================
 * 
 * "Social community ecosystem for community.poweredbymedusa.com!" 🌐⚔️👥
 * Complete social platform with authentication integration
 * 
 * DOMAIN: community.poweredbymedusa.com
 * PURPOSE: Social ecosystem with user profiles, projects, collaboration
 * INTEGRATION: JWT authentication from portal.poweredbymedusa.com
 * 
 * SOCIAL FEATURES:
 * - User Profiles: Comprehensive social profiles with customization
 * - Project Showcase: Portfolio and project sharing platform
 * - Community Forums: Discussion boards and knowledge sharing
 * - Real-time Messaging: Direct messages and group conversations
 * - Activity Feeds: Social timeline and activity streams
 * - Follow/Friend System: Social connections and networking
 * - Groups and Teams: Collaborative workspaces and communities
 * - Events and Meetups: Community event management
 * - Achievements and Badges: Gamification and recognition system
 * - Content Sharing: Media, documents, and resource sharing
 * 
 * AUTHENTICATION INTEGRATION:
 * - SSO with portal.poweredbymedusa.com
 * - Cross-domain session management
 * - Permission inheritance from main portal
 * - Social-specific permissions and roles
 * 
 * "Social networking worthy of Yorkshire Champions!" 🏆💬
 */

#pragma once

#include "medusa_jwt_session_manager.hpp"
#include "medusa_portal_authentication_system.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <chrono>
#include <optional>
#include <functional>

namespace medusa {
namespace community {

using json = nlohmann::json;
using namespace portal::jwt;

// ========================================
// SOCIAL PLATFORM CONFIGURATION
// ========================================

struct CommunityConfig {
    // Domain and integration
    std::string domain = "community.poweredbymedusa.com";
    std::string portal_domain = "portal.poweredbymedusa.com";
    std::string sso_endpoint = "https://portal.poweredbymedusa.com/api/auth/sso";
    
    // Feature toggles
    bool enable_user_profiles = true;
    bool enable_project_showcase = true;
    bool enable_forums = true;
    bool enable_messaging = true;
    bool enable_activity_feeds = true;
    bool enable_social_connections = true;
    bool enable_groups = true;
    bool enable_events = true;
    bool enable_achievements = true;
    bool enable_content_sharing = true;
    bool enable_real_time_notifications = true;
    
    // Privacy and moderation
    bool enable_content_moderation = true;
    bool enable_user_blocking = true;
    bool enable_private_profiles = true;
    bool enable_content_reporting = true;
    bool require_profile_approval = false;
    
    // Social features
    int max_friends_per_user = 1000;
    int max_groups_per_user = 50;
    int max_projects_per_user = 100;
    int max_file_upload_size_mb = 100;
    std::vector<std::string> allowed_file_types = {"jpg", "jpeg", "png", "gif", "pdf", "doc", "docx", "zip"};
    
    // Performance
    int activity_feed_page_size = 20;
    int search_results_page_size = 25;
    std::chrono::seconds real_time_update_interval{30};
    bool enable_content_caching = true;
    std::chrono::seconds cache_ttl{600};  // 10 minutes
};

// ========================================
// SOCIAL ENTITY DEFINITIONS
// ========================================

struct CommunityUser {
    std::string user_id;
    std::string username;
    std::string display_name;
    std::string email;
    std::string bio;
    std::string location;
    std::string website;
    std::string avatar_url;
    std::string banner_url;
    std::vector<std::string> interests;
    std::vector<std::string> skills;
    
    // Social stats
    int followers_count = 0;
    int following_count = 0;
    int friends_count = 0;
    int projects_count = 0;
    int posts_count = 0;
    int reputation_score = 0;
    
    // Privacy settings
    bool public_profile = true;
    bool show_email = false;
    bool show_real_name = false;
    bool allow_messages = true;
    bool allow_friend_requests = true;
    
    // Activity
    std::chrono::system_clock::time_point last_active;
    std::chrono::system_clock::time_point joined_at;
    bool is_online = false;
    
    // Achievements
    std::vector<std::string> badges;
    std::vector<std::string> achievements;
    int community_level = 1;
};

struct Project {
    std::string project_id;
    std::string title;
    std::string description;
    std::string owner_id;
    std::vector<std::string> collaborator_ids;
    std::vector<std::string> tags;
    std::string repository_url;
    std::string demo_url;
    std::vector<std::string> screenshot_urls;
    std::string project_type;  // "open_source", "commercial", "personal", "educational"
    std::string status;        // "planning", "development", "completed", "archived"
    
    // Social engagement
    int likes_count = 0;
    int views_count = 0;
    int forks_count = 0;
    int comments_count = 0;
    
    // Metadata
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
    json custom_fields;
};

struct ForumTopic {
    std::string topic_id;
    std::string title;
    std::string content;
    std::string author_id;
    std::string category_id;
    std::vector<std::string> tags;
    bool is_pinned = false;
    bool is_locked = false;
    bool is_solved = false;
    
    // Engagement
    int views_count = 0;
    int replies_count = 0;
    int likes_count = 0;
    
    // Timestamps
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point last_reply_at;
    std::string last_reply_author_id;
};

struct Message {
    std::string message_id;
    std::string sender_id;
    std::string recipient_id;
    std::string content;
    std::string message_type;  // "text", "image", "file", "system"
    std::vector<std::string> attachment_urls;
    bool is_read = false;
    std::chrono::system_clock::time_point sent_at;
    std::chrono::system_clock::time_point read_at;
};

struct ActivityItem {
    std::string activity_id;
    std::string user_id;
    std::string activity_type;  // "project_created", "post_liked", "user_followed", etc.
    std::string target_type;    // "project", "user", "post", "comment"
    std::string target_id;
    json activity_data;
    std::chrono::system_clock::time_point timestamp;
    bool is_public = true;
};

struct Group {
    std::string group_id;
    std::string name;
    std::string description;
    std::string owner_id;
    std::vector<std::string> admin_ids;
    std::vector<std::string> member_ids;
    std::string group_type;    // "public", "private", "invite_only"
    std::string avatar_url;
    std::string banner_url;
    std::vector<std::string> tags;
    
    // Stats
    int member_count = 0;
    int post_count = 0;
    
    // Settings
    bool allow_member_invites = true;
    bool require_approval = false;
    
    std::chrono::system_clock::time_point created_at;
};

// ========================================
// MEDUSA COMMUNITY ECOSYSTEM MANAGER
// ========================================

class MedusaCommunityEcosystem {
private:
    CommunityConfig config_;
    std::shared_ptr<JWTSessionManager> jwt_manager_;
    std::shared_ptr<portal::TriforceAuthenticationManager> auth_manager_;
    
    // Component managers
    std::unique_ptr<class UserProfileManager> profile_manager_;
    std::unique_ptr<class ProjectShowcaseManager> project_manager_;
    std::unique_ptr<class ForumManager> forum_manager_;
    std::unique_ptr<class MessagingManager> messaging_manager_;
    std::unique_ptr<class ActivityFeedManager> activity_manager_;
    std::unique_ptr<class SocialConnectionManager> social_manager_;
    std::unique_ptr<class GroupManager> group_manager_;
    std::unique_ptr<class EventManager> event_manager_;
    std::unique_ptr<class AchievementManager> achievement_manager_;
    std::unique_ptr<class ContentModerationManager> moderation_manager_;
    std::unique_ptr<class NotificationManager> notification_manager_;
    
    // Real-time features
    std::unique_ptr<class WebSocketManager> websocket_manager_;
    std::unique_ptr<class RealTimeUpdater> realtime_updater_;
    
public:
    MedusaCommunityEcosystem(const CommunityConfig& config,
                            std::shared_ptr<JWTSessionManager> jwt_manager,
                            std::shared_ptr<portal::TriforceAuthenticationManager> auth_manager);
    ~MedusaCommunityEcosystem();
    
    // System lifecycle
    bool initialize();
    void shutdown();
    
    // ========================================
    // USER PROFILE MANAGEMENT
    // ========================================
    
    // Profile operations
    json get_user_profile(const std::string& username, const std::optional<std::string>& viewer_session);
    json update_user_profile(const std::string& session_id, const json& profile_data);
    json upload_profile_avatar(const std::string& session_id, const std::string& image_data);
    json upload_profile_banner(const std::string& session_id, const std::string& image_data);
    
    // Profile customization
    json get_profile_themes(const std::string& session_id);
    json set_profile_theme(const std::string& session_id, const std::string& theme_id);
    json add_profile_widget(const std::string& session_id, const json& widget_config);
    json reorder_profile_widgets(const std::string& session_id, const json& widget_order);
    
    // Privacy settings
    json get_privacy_settings(const std::string& session_id);
    json update_privacy_settings(const std::string& session_id, const json& privacy_settings);
    json get_blocked_users(const std::string& session_id);
    json block_user(const std::string& session_id, const std::string& target_user_id);
    json unblock_user(const std::string& session_id, const std::string& target_user_id);
    
    // ========================================
    // PROJECT SHOWCASE
    // ========================================
    
    // Project management
    json get_user_projects(const std::string& username, const std::optional<std::string>& viewer_session);
    json create_project(const std::string& session_id, const json& project_data);
    json update_project(const std::string& session_id, const std::string& project_id, const json& project_data);
    json delete_project(const std::string& session_id, const std::string& project_id);
    json get_project_details(const std::string& project_id, const std::optional<std::string>& viewer_session);
    
    // Project collaboration
    json invite_collaborator(const std::string& session_id, const std::string& project_id, const std::string& user_id);
    json accept_collaboration(const std::string& session_id, const std::string& project_id);
    json remove_collaborator(const std::string& session_id, const std::string& project_id, const std::string& user_id);
    json fork_project(const std::string& session_id, const std::string& project_id);
    
    // Project engagement
    json like_project(const std::string& session_id, const std::string& project_id);
    json comment_on_project(const std::string& session_id, const std::string& project_id, const json& comment_data);
    json share_project(const std::string& session_id, const std::string& project_id, const json& share_data);
    json get_project_analytics(const std::string& session_id, const std::string& project_id);
    
    // ========================================
    // COMMUNITY FORUMS
    // ========================================
    
    // Forum browsing
    json get_forum_categories(const std::optional<std::string>& session_id);
    json browse_forum_topics(const std::optional<std::string>& session_id, const json& filters);
    json get_topic_details(const std::optional<std::string>& session_id, const std::string& topic_id);
    json get_topic_replies(const std::optional<std::string>& session_id, const std::string& topic_id, const json& pagination);
    
    // Forum participation (requires login)
    json create_forum_topic(const std::string& session_id, const json& topic_data);
    json reply_to_topic(const std::string& session_id, const std::string& topic_id, const json& reply_data);
    json edit_forum_post(const std::string& session_id, const std::string& post_id, const json& post_data);
    json delete_forum_post(const std::string& session_id, const std::string& post_id);
    
    // Forum engagement
    json vote_on_post(const std::string& session_id, const std::string& post_id, const std::string& vote_type);
    json mark_topic_as_solved(const std::string& session_id, const std::string& topic_id, const std::string& solution_post_id);
    json subscribe_to_topic(const std::string& session_id, const std::string& topic_id);
    json report_forum_content(const std::string& session_id, const std::string& content_id, const json& report_data);
    
    // ========================================
    // MESSAGING SYSTEM
    // ========================================
    
    // Direct messaging
    json get_conversations(const std::string& session_id, const json& filters);
    json get_conversation_messages(const std::string& session_id, const std::string& conversation_id, const json& pagination);
    json send_message(const std::string& session_id, const std::string& recipient_id, const json& message_data);
    json mark_messages_as_read(const std::string& session_id, const std::vector<std::string>& message_ids);
    json delete_conversation(const std::string& session_id, const std::string& conversation_id);
    
    // Group messaging
    json create_group_conversation(const std::string& session_id, const json& group_data);
    json add_to_group_conversation(const std::string& session_id, const std::string& group_id, const std::string& user_id);
    json leave_group_conversation(const std::string& session_id, const std::string& group_id);
    json send_group_message(const std::string& session_id, const std::string& group_id, const json& message_data);
    
    // ========================================
    // SOCIAL CONNECTIONS
    // ========================================
    
    // Following system
    json follow_user(const std::string& session_id, const std::string& target_user_id);
    json unfollow_user(const std::string& session_id, const std::string& target_user_id);
    json get_followers(const std::string& username, const std::optional<std::string>& viewer_session);
    json get_following(const std::string& username, const std::optional<std::string>& viewer_session);
    
    // Friend system
    json send_friend_request(const std::string& session_id, const std::string& target_user_id);
    json accept_friend_request(const std::string& session_id, const std::string& request_id);
    json decline_friend_request(const std::string& session_id, const std::string& request_id);
    json remove_friend(const std::string& session_id, const std::string& friend_user_id);
    json get_friends(const std::string& username, const std::optional<std::string>& viewer_session);
    json get_friend_requests(const std::string& session_id);
    
    // Social discovery
    json get_suggested_users(const std::string& session_id, const json& preferences);
    json get_mutual_connections(const std::string& session_id, const std::string& target_user_id);
    json search_users(const std::optional<std::string>& session_id, const json& search_params);
    
    // ========================================
    // ACTIVITY FEEDS
    // ========================================
    
    // Personal activity feed
    json get_user_activity_feed(const std::string& session_id, const json& filters);
    json get_user_activity_history(const std::string& username, const std::optional<std::string>& viewer_session);
    json get_following_activity_feed(const std::string& session_id, const json& filters);
    
    // Public activity streams
    json get_public_activity_feed(const std::optional<std::string>& session_id, const json& filters);
    json get_trending_activity(const std::optional<std::string>& session_id, const json& time_range);
    json get_category_activity(const std::optional<std::string>& session_id, const std::string& category, const json& filters);
    
    // Activity interactions
    json like_activity(const std::string& session_id, const std::string& activity_id);
    json comment_on_activity(const std::string& session_id, const std::string& activity_id, const json& comment_data);
    json share_activity(const std::string& session_id, const std::string& activity_id, const json& share_data);
    
    // ========================================
    // GROUPS AND COMMUNITIES
    // ========================================
    
    // Group management
    json get_user_groups(const std::string& username, const std::optional<std::string>& viewer_session);
    json create_group(const std::string& session_id, const json& group_data);
    json update_group(const std::string& session_id, const std::string& group_id, const json& group_data);
    json delete_group(const std::string& session_id, const std::string& group_id);
    json get_group_details(const std::string& group_id, const std::optional<std::string>& viewer_session);
    
    // Group membership
    json join_group(const std::string& session_id, const std::string& group_id);
    json leave_group(const std::string& session_id, const std::string& group_id);
    json invite_to_group(const std::string& session_id, const std::string& group_id, const std::string& user_id);
    json approve_group_membership(const std::string& session_id, const std::string& group_id, const std::string& user_id);
    
    // Group content
    json get_group_feed(const std::optional<std::string>& session_id, const std::string& group_id, const json& filters);
    json post_to_group(const std::string& session_id, const std::string& group_id, const json& post_data);
    json get_group_projects(const std::optional<std::string>& session_id, const std::string& group_id);
    json get_group_events(const std::optional<std::string>& session_id, const std::string& group_id);
    
    // ========================================
    // NOTIFICATIONS AND REAL-TIME UPDATES
    // ========================================
    
    // Notification management
    json get_notifications(const std::string& session_id, const json& filters);
    json mark_notifications_as_read(const std::string& session_id, const std::vector<std::string>& notification_ids);
    json get_notification_settings(const std::string& session_id);
    json update_notification_settings(const std::string& session_id, const json& settings);
    
    // Real-time updates
    bool subscribe_to_real_time_updates(const std::string& session_id, const std::string& websocket_id);
    void broadcast_real_time_update(const std::string& update_type, const json& update_data, const std::vector<std::string>& target_users = {});
    
    // ========================================
    // SEARCH AND DISCOVERY
    // ========================================
    
    // Global search
    json global_search(const std::optional<std::string>& session_id, const json& search_params);
    json search_users(const std::optional<std::string>& session_id, const json& search_params);
    json search_projects(const std::optional<std::string>& session_id, const json& search_params);
    json search_groups(const std::optional<std::string>& session_id, const json& search_params);
    json search_forum_content(const std::optional<std::string>& session_id, const json& search_params);
    
    // Trending and recommendations
    json get_trending_projects(const std::optional<std::string>& session_id, const json& time_range);
    json get_trending_topics(const std::optional<std::string>& session_id, const json& time_range);
    json get_recommended_users(const std::string& session_id, const json& preferences);
    json get_recommended_projects(const std::string& session_id, const json& preferences);
    json get_recommended_groups(const std::string& session_id, const json& preferences);
    
    // ========================================
    // CONTENT MODERATION
    // ========================================
    
    // Reporting and moderation
    json report_content(const std::string& session_id, const json& report_data);
    json get_moderation_queue(const std::string& session_id, const json& filters);  // Moderator only
    json moderate_content(const std::string& session_id, const json& moderation_action);  // Moderator only
    json get_user_reports(const std::string& session_id, const std::string& target_user_id);  // Moderator only
    
    // ========================================
    // ANALYTICS AND INSIGHTS
    // ========================================
    
    // User analytics
    json get_user_analytics(const std::string& session_id);
    json get_project_analytics(const std::string& session_id, const std::string& project_id);
    json get_group_analytics(const std::string& session_id, const std::string& group_id);
    
    // Community analytics (for admins)
    json get_community_analytics(const std::string& session_id, const json& time_range);
    json get_engagement_metrics(const std::string& session_id, const json& filters);
    json get_growth_metrics(const std::string& session_id, const json& time_range);
    
private:
    // Internal helper methods
    bool initialize_components();
    void setup_cross_domain_authentication();
    void configure_social_features();
    
    // Authentication helpers
    bool validate_sso_token(const std::string& sso_token, std::string& session_id);
    CommunityUser convert_portal_user_to_community_user(const portal::MedusaUser& portal_user);
    
    // Permission helpers
    bool check_social_permission(const std::string& session_id, const std::string& permission);
    bool can_view_profile(const std::string& viewer_session, const std::string& target_user_id);
    bool can_message_user(const std::string& sender_session, const std::string& recipient_id);
    
    // Content filtering helpers
    json filter_content_for_user(const json& content, const std::string& viewer_session);
    json apply_privacy_filters(const json& user_data, const std::string& viewer_session);
    
    // Database operations
    json load_user_data_from_database(const std::string& user_id);
    json load_projects_from_database(const std::string& user_id);
    json load_social_connections_from_database(const std::string& user_id);
    json load_activity_feed_from_database(const std::string& user_id, const json& filters);
};

} // namespace community
} // namespace medusa

// C API for community integration
extern "C" {
    // Community ecosystem management
    void* medusa_community_create(const char* config_json, void* jwt_manager, void* auth_manager);
    void medusa_community_destroy(void* community);
    bool medusa_community_initialize(void* community);
    
    // User profile operations
    const char* medusa_community_get_profile(void* community, const char* username, const char* viewer_session);
    bool medusa_community_update_profile(void* community, const char* session_id, const char* profile_json);
    
    // Social operations
    bool medusa_community_follow_user(void* community, const char* session_id, const char* target_user_id);
    const char* medusa_community_get_activity_feed(void* community, const char* session_id, const char* filters_json);
    
    // Project operations
    const char* medusa_community_create_project(void* community, const char* session_id, const char* project_json);
    const char* medusa_community_get_projects(void* community, const char* username, const char* viewer_session);
}