/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * MEDUSA PATREON INTEGRATION v0.3.0c
 * ==================================
 * 
 * Revolutionary Patreon API integration with native C++
 * Ground-up implementation - Yorkshire Champion Standards
 * NO SHORTCUTS - Full Patreon API v2 support with webhook sync
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <functional>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <condition_variable>

namespace MedusaPatreon {

    /**
     * @brief Patreon authentication types
     */
    enum class AuthType {
        CREATOR_ACCESS_TOKEN,
        OAUTH2_CODE,
        CLIENT_CREDENTIALS
    };

    /**
     * @brief Patreon webhook event types
     */
    enum class WebhookEventType {
        MEMBERS_PLEDGE_CREATE,
        MEMBERS_PLEDGE_UPDATE,
        MEMBERS_PLEDGE_DELETE,
        POSTS_PUBLISHED,
        POSTS_UPDATE,
        POSTS_DELETE,
        UNKNOWN
    };

    /**
     * @brief Patreon user/patron information
     */
    struct PatreonUser {
        std::string id;
        std::string email;
        std::string first_name;
        std::string last_name;
        std::string full_name;
        std::string vanity;
        std::string about;
        std::string image_url;
        std::string thumb_url;
        std::string url;
        bool is_email_verified;
        std::string created;
        
        // Social connections
        std::string discord_id;
        std::string facebook_id;
        std::string google_id;
        std::string instagram_id;
        std::string reddit_id;
        std::string spotify_id;
        std::string twitch_id;
        std::string twitter_id;
        std::string youtube_id;

        // Licensing additions
        int currently_entitled_amount_cents = 0;
        bool is_active = false;
        std::string patron_status;
    };

    /**
     * @brief Patreon campaign information
     */
    struct PatreonCampaign {
        std::string id;
        std::string summary;
        std::string creation_name;
        std::string display_patron_goals;
        std::string main_video_embed;
        std::string main_video_url;
        std::string image_small_url;
        std::string image_url;
        std::string thanks_embed;
        std::string thanks_msg;
        std::string thanks_video_url;
        bool is_charged_immediately;
        bool is_monthly;
        bool is_nsfw;
        std::string created_at;
        std::string published_at;
        int patron_count;
        int pledge_sum; // in cents
        std::string pledge_url;
        
        // Goals and rewards
        std::vector<std::string> goals;
        std::vector<std::string> rewards;
        
        // Medusa-specific metadata
        bool medusa_enabled = false;
        std::string medusa_discord_role_mapping;
        std::string medusa_access_levels;
    };

    /**
     * @brief Patreon pledge/membership information
     */
    struct PatreonPledge {
        std::string id;
        std::string patron_id;
        std::string campaign_id;
        std::string reward_id;
        int amount_cents;
        std::string created_at;
        std::string declined_since;
        bool is_paused;
        bool has_shipping_address;
        int total_historical_amount_cents;
        std::string pledge_cap_cents;
        std::string patron_pays_fees;
        
        // Patron information
        PatreonUser patron;
        
        // Status tracking
        std::string status; // active, declined, paused, etc.
        std::string last_charge_status;
        std::string last_charge_date;
        
        // Medusa AI analysis
        double retention_score = 0.0;
        std::string engagement_level;
        std::vector<std::string> interaction_history;
    };

    /**
     * @brief Patreon post information
     */
    struct PatreonPost {
        std::string id;
        std::string title;
        std::string content;
        std::string embed_data;
        std::string embed_url;
        bool is_automated_monthly_charge;
        bool is_paid;
        std::string published_at;
        std::string url;
        std::string patreon_url;
        
        // Media attachments
        std::vector<std::string> image_urls;
        std::vector<std::string> video_urls;
        std::vector<std::string> audio_urls;
        
        // Access control
        std::string min_cents_pledged_to_view;
        std::vector<std::string> tiers_required;
        
        // Engagement metrics
        int like_count;
        int comment_count;
        std::vector<std::string> patron_interactions;
    };

    /**
     * @brief Patreon webhook event
     */
    struct PatreonWebhookEvent {
        WebhookEventType event_type;
        std::string event_name;
        std::string delivery_id;
        std::string signature;
        nlohmann::json payload;
        std::chrono::system_clock::time_point timestamp;
        
        // Event-specific data
        std::string campaign_id;
        std::string user_id;
        std::string pledge_id;
        std::string post_id;
        
        // Processed by Medusa
        bool medusa_processed = false;
        std::string medusa_response;
        nlohmann::json medusa_actions;
    };

    /**
     * @brief Medusa Patreon API Client
     */
    class MedusaPatreonClient {
    private:
        // Authentication
        std::string access_token_;
        std::string client_id_;
        std::string client_secret_;
        AuthType auth_type_;
        
        // HTTP client (native implementation)
        MedusaHttp::NativeHttpClient http_client_;
        std::string api_base_url_ = "https://www.patreon.com/api/oauth2/v2";
        
        // Rate limiting
        std::atomic<int> rate_limit_remaining_{10000};
        std::atomic<int> rate_limit_reset_{0};
        std::mutex rate_limit_mutex_;
        
        // Caching
        std::unordered_map<std::string, PatreonCampaign> campaign_cache_;
        std::unordered_map<std::string, PatreonUser> user_cache_;
        std::unordered_map<std::string, PatreonPledge> pledge_cache_;
        std::mutex cache_mutex_;
        
        // Webhook processing
        std::queue<PatreonWebhookEvent> webhook_queue_;
        std::mutex webhook_mutex_;
        std::atomic<bool> processing_{false};
        std::thread webhook_processor_;
        
        // Event handlers
        std::function<void(const PatreonWebhookEvent&)> webhook_handler_;
        std::function<void(const PatreonPledge&)> pledge_handler_;
        std::function<void(const PatreonPost&)> post_handler_;
        std::function<void(const PatreonUser&)> patron_handler_;
        
        // Medusa-specific features
        bool medusa_ai_enabled_ = true;
        std::string medusa_webhook_secret_;
        std::unordered_map<std::string, std::function<void(const PatreonWebhookEvent&)>> custom_handlers_;
        
        // Private methods
        std::string make_request(const std::string& endpoint, const std::string& method = "GET",
                                const std::string& data = "", const std::vector<std::string>& headers = {});
        bool verify_webhook_signature(const std::string& payload, const std::string& signature);
        void process_webhook_queue();
        void handle_rate_limit(const std::string& response_headers);
        
        // Parsing methods
        PatreonUser parse_user(const nlohmann::json& user_json);
        PatreonCampaign parse_campaign(const nlohmann::json& campaign_json);
        PatreonPledge parse_pledge(const nlohmann::json& pledge_json);
        PatreonPost parse_post(const nlohmann::json& post_json);
        
    public:
        MedusaPatreonClient(const std::string& access_token, AuthType type = AuthType::CREATOR_ACCESS_TOKEN);
        ~MedusaPatreonClient();
        
        // Authentication
        bool authenticate();
        bool is_authenticated() const;
        void set_oauth_credentials(const std::string& client_id, const std::string& client_secret);
        std::string get_oauth_url(const std::string& redirect_uri, const std::vector<std::string>& scopes);
        std::string exchange_code_for_token(const std::string& code, const std::string& redirect_uri);
        
        // User operations
        PatreonUser get_current_user();
        PatreonUser get_user(const std::string& user_id);
        
        // Campaign operations
        std::vector<PatreonCampaign> get_campaigns();
        PatreonCampaign get_campaign(const std::string& campaign_id);
        
        // Pledge/Member operations
        std::vector<PatreonPledge> get_campaign_pledges(const std::string& campaign_id);
        PatreonPledge get_pledge(const std::string& pledge_id);
        std::vector<PatreonUser> get_campaign_members(const std::string& campaign_id);
        
        // Post operations
        std::vector<PatreonPost> get_campaign_posts(const std::string& campaign_id);
        PatreonPost get_post(const std::string& post_id);
        PatreonPost create_post(const std::string& campaign_id, const std::string& title, 
                               const std::string& content, const std::vector<std::string>& tiers = {});
        bool update_post(const std::string& post_id, const nlohmann::json& updates);
        bool delete_post(const std::string& post_id);
        
        // Webhook operations
        std::string process_webhook(const std::string& payload, const std::string& signature = "",
                                   const std::string& event_type = "");
        void set_webhook_secret(const std::string& secret) { medusa_webhook_secret_ = secret; }
        void start_webhook_processing();
        void stop_webhook_processing();
        
        // Event handlers
        void set_webhook_handler(std::function<void(const PatreonWebhookEvent&)> handler) {
            webhook_handler_ = handler;
        }
        
        void set_pledge_handler(std::function<void(const PatreonPledge&)> handler) {
            pledge_handler_ = handler;
        }
        
        void set_post_handler(std::function<void(const PatreonPost&)> handler) {
            post_handler_ = handler;
        }
        
        void set_patron_handler(std::function<void(const PatreonUser&)> handler) {
            patron_handler_ = handler;
        }
        
        void add_custom_handler(const std::string& event_type,
                               std::function<void(const PatreonWebhookEvent&)> handler) {
            custom_handlers_[event_type] = handler;
        }
        
        // Medusa-specific features
        void enable_medusa_ai(bool enabled = true) { medusa_ai_enabled_ = enabled; }
        
        // AI-powered analysis
        std::string analyze_patron_engagement(const PatreonPledge& pledge);
        std::string classify_patron_risk(const PatreonPledge& pledge);
        std::vector<std::string> suggest_content_for_tier(const std::string& campaign_id, int tier_amount);
        double calculate_retention_probability(const PatreonPledge& pledge);
        
        // Discord integration helpers
        std::string map_pledge_to_discord_role(const PatreonPledge& pledge);
        bool should_grant_discord_access(const PatreonPledge& pledge, const std::string& channel_id);
        std::string format_patron_welcome_message(const PatreonPledge& pledge);
        
        // Rate limiting
        int get_rate_limit_remaining() const { return rate_limit_remaining_; }
        std::chrono::system_clock::time_point get_rate_limit_reset() const {
            return std::chrono::system_clock::from_time_t(rate_limit_reset_);
        }
        
        // Utility methods
        bool is_patron_active(const PatreonPledge& pledge);
        int get_patron_tier_amount(const PatreonPledge& pledge);
        std::string get_patron_display_name(const PatreonUser& patron);
        std::vector<std::string> get_patron_entitled_rewards(const PatreonPledge& pledge);

        // Compatibility methods
        bool initialize() { return authenticate(); }
        PatreonUser get_member(const std::string& member_id);
    };

    /**
     * @brief Medusa Patreon Webhook Server
     */
    class MedusaPatreonWebhookServer {
    private:
        int port_;
        std::atomic<bool> running_{false};
        std::thread server_thread_;
        std::shared_ptr<MedusaPatreonClient> patreon_client_;
        
        void server_loop();
        
    public:
        MedusaPatreonWebhookServer(int port, std::shared_ptr<MedusaPatreonClient> client);
        ~MedusaPatreonWebhookServer();
        
        bool start();
        void stop();
        bool is_running() const { return running_; }
    };

    /**
     * @brief Patreon-Discord Synchronization Engine
     */
    class MedusaPatreonDiscordSync {
    private:
        std::shared_ptr<MedusaPatreonClient> patreon_client_;
        std::string discord_guild_id_;
        std::string discord_bot_token_;
        
        // Role mapping configuration
        std::unordered_map<int, std::string> tier_to_role_mapping_;
        std::string default_patron_role_;
        std::string former_patron_role_;
        
        // Sync state
        std::atomic<bool> sync_enabled_{true};
        std::atomic<int> patrons_synced_{0};
        std::atomic<int> roles_updated_{0};
        
        void setup_patreon_handlers();
        void sync_patron_to_discord(const PatreonPledge& pledge);
        void update_discord_role(const std::string& discord_user_id, const std::string& role_id, bool add);
        std::string find_discord_user_by_patreon_id(const std::string& patreon_id);
        
    public:
        MedusaPatreonDiscordSync(std::shared_ptr<MedusaPatreonClient> patreon_client,
                               const std::string& discord_guild_id,
                               const std::string& discord_bot_token);
        
        void start();
        void stop();
        bool is_running() const;
        
        void set_tier_role_mapping(int tier_cents, const std::string& discord_role_id);
        void set_default_roles(const std::string& patron_role, const std::string& former_patron_role);
        
        struct SyncStats {
            int patrons_synced;
            int roles_updated;
        };
        
        SyncStats get_stats() const;
    };

} // namespace MedusaPatreon