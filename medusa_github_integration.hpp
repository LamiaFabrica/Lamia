/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * MEDUSA GITHUB INTEGRATION v0.3.0c
 * =================================
 * 
 * Revolutionary GitHub API integration with native C++
 * Ground-up implementation - Yorkshire Champion Standards
 * NO SHORTCUTS - Full GitHub REST API v4 and GraphQL support
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
#include <iomanip>

namespace MedusaGitHub {

    /**
     * @brief GitHub authentication types
     */
    enum class AuthType {
        PERSONAL_ACCESS_TOKEN,
        GITHUB_APP,
        OAUTH_TOKEN,
        INSTALLATION_TOKEN
    };

    /**
     * @brief GitHub event types
     */
    enum class EventType {
        PUSH,
        PULL_REQUEST,
        ISSUES,
        ISSUE_COMMENT,
        PULL_REQUEST_REVIEW,
        RELEASE,
        STAR,
        FORK,
        WATCH,
        CREATE,
        DELETE,
        WORKFLOW_RUN,
        CHECK_RUN,
        DEPLOYMENT,
        COMMIT_COMMENT,
        GOLLUM, // Wiki changes
        MEMBER,
        PING
    };

    /**
     * @brief GitHub repository information
     */
    struct GitHubRepository {
        std::string id;
        std::string name;
        std::string full_name;
        std::string description;
        std::string html_url;
        std::string clone_url;
        std::string ssh_url;
        std::string default_branch;
        std::string language;
        bool private_repo;
        bool fork;
        bool archived;
        bool disabled;
        int stars_count;
        int forks_count;
        int open_issues_count;
        int watchers_count;
        std::string created_at;
        std::string updated_at;
        std::string pushed_at;
        
        // Medusa-specific metadata
        bool medusa_enabled = false;
        std::string medusa_config;
        std::vector<std::string> medusa_hooks;
    };

    /**
     * @brief GitHub user/organization information
     */
    struct GitHubUser {
        std::string id;
        std::string login;
        std::string name;
        std::string email;
        std::string avatar_url;
        std::string html_url;
        std::string type; // User or Organization
        bool site_admin;
        std::string company;
        std::string blog;
        std::string location;
        std::string bio;
        int public_repos;
        int public_gists;
        int followers;
        int following;
        std::string created_at;
        std::string updated_at;
    };

    /**
     * @brief GitHub commit information
     */
    struct GitHubCommit {
        std::string sha;
        std::string message;
        GitHubUser author;
        GitHubUser committer;
        std::string author_date;
        std::string committer_date;
        std::string html_url;
        std::vector<std::string> added_files;
        std::vector<std::string> modified_files;
        std::vector<std::string> removed_files;
        int additions = 0;
        int deletions = 0;
        int total_changes = 0;
    };

    /**
     * @brief GitHub pull request information
     */
    struct GitHubPullRequest {
        std::string id;
        int number;
        std::string title;
        std::string body;
        std::string state; // open, closed, merged
        GitHubUser author;
        std::string base_branch;
        std::string head_branch;
        std::string base_sha;
        std::string head_sha;
        std::string html_url;
        std::string diff_url;
        std::string patch_url;
        bool mergeable;
        std::string mergeable_state;
        bool merged;
        std::string merged_at;
        std::string created_at;
        std::string updated_at;
        std::string closed_at;
        int additions = 0;
        int deletions = 0;
        int changed_files = 0;
        int commits_count = 0;
        int comments_count = 0;
        int review_comments_count = 0;
        
        // Medusa AI analysis
        double code_quality_score = 0.0;
        std::string ai_summary;
        std::vector<std::string> ai_suggestions;
        bool auto_mergeable = false;
    };

    /**
     * @brief GitHub issue information
     */
    struct GitHubIssue {
        std::string id;
        int number;
        std::string title;
        std::string body;
        std::string state; // open, closed
        GitHubUser author;
        std::vector<std::string> labels;
        std::string milestone;
        std::vector<GitHubUser> assignees;
        std::string html_url;
        std::string created_at;
        std::string updated_at;
        std::string closed_at;
        int comments_count = 0;
        
        // Medusa AI analysis
        std::string priority_level;
        std::string category;
        double urgency_score = 0.0;
        std::string ai_classification;
        std::vector<std::string> related_issues;
    };

    /**
     * @brief GitHub webhook event
     */
    struct GitHubWebhookEvent {
        EventType event_type;
        std::string event_name;
        std::string delivery_id;
        std::string signature;
        nlohmann::json payload;
        std::string repository_name;
        std::string sender_login;
        std::chrono::system_clock::time_point timestamp;
        
        // Processed by Medusa
        bool medusa_processed = false;
        std::string medusa_response;
        nlohmann::json medusa_actions;
    };

    /**
     * @brief GitHub workflow run information
     */
    struct GitHubWorkflowRun {
        std::string id;
        std::string name;
        std::string head_branch;
        std::string head_sha;
        std::string status; // queued, in_progress, completed
        std::string conclusion; // success, failure, neutral, cancelled, skipped, timed_out
        std::string workflow_id;
        std::string html_url;
        std::string created_at;
        std::string updated_at;
        int run_number;
        
        // Job details
        struct Job {
            std::string id;
            std::string name;
            std::string status;
            std::string conclusion;
            std::string started_at;
            std::string completed_at;
        };
        std::vector<Job> jobs;
    };

    /**
     * @brief Medusa GitHub API Client
     */
    class MedusaGitHubClient {
    private:
        // Authentication
        std::string auth_token_;
        std::string app_id_;
        std::string private_key_;
        AuthType auth_type_;
        
        // HTTP client
        CURL* curl_handle_;
        std::string api_base_url_ = "https://api.github.com";
        std::string graphql_url_ = "https://api.github.com/graphql";
        
        // Rate limiting
        std::atomic<int> rate_limit_remaining_{5000};
        std::atomic<int> rate_limit_reset_{0};
        std::mutex rate_limit_mutex_;
        
        // Caching
        std::unordered_map<std::string, GitHubRepository> repo_cache_;
        std::unordered_map<std::string, GitHubUser> user_cache_;
        std::mutex cache_mutex_;
        
        // Event processing
        std::queue<GitHubWebhookEvent> webhook_queue_;
        std::mutex webhook_mutex_;
        std::atomic<bool> processing_{false};
        std::thread webhook_processor_;
        
        // Callbacks
        std::function<void(const GitHubWebhookEvent&)> webhook_handler_;
        std::function<void(const GitHubPullRequest&)> pr_handler_;
        std::function<void(const GitHubIssue&)> issue_handler_;
        std::function<void(const GitHubWorkflowRun&)> workflow_handler_;
        
        // Medusa-specific features
        bool medusa_ai_enabled_ = true;
        std::string medusa_webhook_secret_;
        std::unordered_map<std::string, std::function<void(const GitHubWebhookEvent&)>> custom_handlers_;
        
        // Private methods
        std::string make_request(const std::string& endpoint, const std::string& method = "GET",
                                const std::string& data = "", const std::vector<std::string>& headers = {});
        std::string make_graphql_request(const std::string& query, const nlohmann::json& variables = {});
        bool verify_webhook_signature(const std::string& payload, const std::string& signature);
        void process_webhook_queue();
        void handle_rate_limit(const std::string& response_headers);
        static size_t curl_write_callback(void* contents, size_t size, size_t nmemb, std::string* userp);
        static size_t curl_header_callback(void* contents, size_t size, size_t nmemb, std::string* userp);
        
        // Parsing methods
        GitHubRepository parse_repository(const nlohmann::json& repo_json);
        GitHubUser parse_user(const nlohmann::json& user_json);
        GitHubCommit parse_commit(const nlohmann::json& commit_json);
        GitHubPullRequest parse_pull_request(const nlohmann::json& pr_json);
        GitHubIssue parse_issue(const nlohmann::json& issue_json);
        GitHubWorkflowRun parse_workflow_run(const nlohmann::json& run_json);
        
    public:
        MedusaGitHubClient(const std::string& token, AuthType type = AuthType::PERSONAL_ACCESS_TOKEN);
        ~MedusaGitHubClient();
        
        // Authentication
        bool authenticate();
        bool is_authenticated() const;
        void set_app_credentials(const std::string& app_id, const std::string& private_key);
        
        // Repository operations
        std::vector<GitHubRepository> get_repositories(const std::string& owner = "");
        GitHubRepository get_repository(const std::string& owner, const std::string& repo);
        GitHubRepository create_repository(const std::string& name, const std::string& description = "",
                                          bool private_repo = false);
        bool update_repository(const std::string& owner, const std::string& repo, const nlohmann::json& updates);
        bool delete_repository(const std::string& owner, const std::string& repo);
        
        // User operations
        GitHubUser get_authenticated_user();
        GitHubUser get_user(const std::string& username);
        std::vector<GitHubUser> get_collaborators(const std::string& owner, const std::string& repo);
        
        // Commit operations
        std::vector<GitHubCommit> get_commits(const std::string& owner, const std::string& repo,
                                             const std::string& branch = "");
        GitHubCommit get_commit(const std::string& owner, const std::string& repo, const std::string& sha);
        
        // Pull request operations
        std::vector<GitHubPullRequest> get_pull_requests(const std::string& owner, const std::string& repo,
                                                        const std::string& state = "open");
        GitHubPullRequest get_pull_request(const std::string& owner, const std::string& repo, int number);
        GitHubPullRequest create_pull_request(const std::string& owner, const std::string& repo,
                                             const std::string& title, const std::string& body,
                                             const std::string& head, const std::string& base);
        bool update_pull_request(const std::string& owner, const std::string& repo, int number,
                                const nlohmann::json& updates);
        bool merge_pull_request(const std::string& owner, const std::string& repo, int number,
                               const std::string& commit_title = "", const std::string& commit_message = "");
        
        // Issue operations
        std::vector<GitHubIssue> get_issues(const std::string& owner, const std::string& repo,
                                           const std::string& state = "open");
        GitHubIssue get_issue(const std::string& owner, const std::string& repo, int number);
        GitHubIssue create_issue(const std::string& owner, const std::string& repo,
                                const std::string& title, const std::string& body = "");
        bool update_issue(const std::string& owner, const std::string& repo, int number,
                         const nlohmann::json& updates);
        bool close_issue(const std::string& owner, const std::string& repo, int number);
        
        // Workflow operations
        std::vector<GitHubWorkflowRun> get_workflow_runs(const std::string& owner, const std::string& repo);
        GitHubWorkflowRun get_workflow_run(const std::string& owner, const std::string& repo, const std::string& run_id);
        bool trigger_workflow(const std::string& owner, const std::string& repo, const std::string& workflow_id,
                             const std::string& ref = "main", const nlohmann::json& inputs = {});
        
        // Webhook operations
        std::string process_webhook(const std::string& payload, const std::string& signature = "",
                                   const std::string& event_type = "");
        void set_webhook_secret(const std::string& secret) { medusa_webhook_secret_ = secret; }
        void start_webhook_processing();
        void stop_webhook_processing();
        
        // Event handlers
        void set_webhook_handler(std::function<void(const GitHubWebhookEvent&)> handler) {
            webhook_handler_ = handler;
        }
        
        void set_pull_request_handler(std::function<void(const GitHubPullRequest&)> handler) {
            pr_handler_ = handler;
        }
        
        void set_issue_handler(std::function<void(const GitHubIssue&)> handler) {
            issue_handler_ = handler;
        }
        
        void set_workflow_handler(std::function<void(const GitHubWorkflowRun&)> handler) {
            workflow_handler_ = handler;
        }
        
        void add_custom_handler(const std::string& event_type,
                               std::function<void(const GitHubWebhookEvent&)> handler) {
            custom_handlers_[event_type] = handler;
        }
        
        // Medusa-specific features
        void enable_medusa_ai(bool enabled = true) { medusa_ai_enabled_ = enabled; }
        
        // AI-powered analysis
        std::string analyze_pull_request_with_ai(const GitHubPullRequest& pr);
        std::string classify_issue_with_ai(const GitHubIssue& issue);
        std::vector<std::string> suggest_reviewers_with_ai(const GitHubPullRequest& pr);
        bool should_auto_merge_with_ai(const GitHubPullRequest& pr);
        
        // Rate limiting
        int get_rate_limit_remaining() const { return rate_limit_remaining_; }
        std::chrono::system_clock::time_point get_rate_limit_reset() const {
            return std::chrono::system_clock::from_time_t(rate_limit_reset_);
        }
        
        // GraphQL operations
        nlohmann::json query_graphql(const std::string& query, const nlohmann::json& variables = {});
        
        // Utility methods
        std::string get_repo_readme(const std::string& owner, const std::string& repo);
        std::vector<std::string> get_repo_languages(const std::string& owner, const std::string& repo);
        bool is_repo_medusa_enabled(const std::string& owner, const std::string& repo);
        void enable_medusa_for_repo(const std::string& owner, const std::string& repo);
    };

    /**
     * @brief Medusa GitHub Webhook Server
     */
    class MedusaGitHubWebhookServer {
    private:
        int port_;
        std::atomic<bool> running_{false};
        std::thread server_thread_;
        std::shared_ptr<MedusaGitHubClient> github_client_;
        
        void server_loop();
        
    public:
        MedusaGitHubWebhookServer(int port, std::shared_ptr<MedusaGitHubClient> client);
        ~MedusaGitHubWebhookServer();
        
        bool start();
        void stop();
        bool is_running() const { return running_; }
    };

} // namespace MedusaGitHub