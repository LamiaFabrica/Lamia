#ifndef MEDUSA_MEDUSA_EDITOR_H
#define MEDUSA_MEDUSA_EDITOR_H

/**
 * @file medusa_editor.h
 * @brief Revolutionary WYSIWYG Editor Library - TipTap Killer
 * @author MedusaServ Development Team
 * @date 2025
 * @copyright Copyright (c) 2025 The Medusa Initiative
 * 
 * MedusaEditor: Ground-up C++ WYSIWYG editor with Virtual Host management,
 * real-time collaboration, and zero third-party dependencies.
 * Built to completely rival and surpass TipTap.
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <atomic>
#include <mutex>
#include <thread>

// Export for shared library
#ifdef _WIN32
    #ifdef MEDUSA_EDITOR_EXPORTS
        #define MEDUSA_EDITOR_API __declspec(dllexport)
    #else
        #define MEDUSA_EDITOR_API __declspec(dllimport)
    #endif
#else
    #define MEDUSA_EDITOR_API __attribute__((visibility("default")))
#endif

namespace MedusaEditor {

/**
 * @brief Document node types for rich content
 */
enum class NodeType {
    Document,
    Paragraph,
    Heading,
    Text,
    Bold,
    Italic,
    Underline,
    Strike,
    Code,
    CodeBlock,
    Blockquote,
    List,
    ListItem,
    Link,
    Image,
    Table,
    TableRow,
    TableCell,
    HorizontalRule,
    LineBreak,
    CustomHTML,
    VirtualHost,      // Special node for Virtual Host config
    ServerBlock,      // Server configuration block
    LocationBlock,    // Location configuration block
    SSLConfig,        // SSL configuration section
    CustomDirective   // Custom server directive
};

/**
 * @brief Text formatting attributes
 */
struct TextAttributes {
    bool bold = false;
    bool italic = false;
    bool underline = false;
    bool strikethrough = false;
    bool code = false;
    std::string color = "";
    std::string backgroundColor = "";
    std::string fontSize = "";
    std::string fontFamily = "";
    std::unordered_map<std::string, std::string> customAttributes;
};

/**
 * @brief Document node structure
 */
struct DocumentNode {
    NodeType type;
    std::string content;
    TextAttributes attributes;
    std::unordered_map<std::string, std::string> props;
    std::vector<std::shared_ptr<DocumentNode>> children;
    std::weak_ptr<DocumentNode> parent;
    
    // Virtual Host specific properties
    std::string virtualHostName;
    std::string domainName;
    std::string documentRoot;
    bool sslEnabled = false;
    std::string sslCertPath;
    std::string sslKeyPath;
    
    // Position tracking for real-time collaboration
    size_t position = 0;
    size_t length = 0;
    std::string nodeId;
    std::chrono::steady_clock::time_point lastModified;
    std::string lastModifiedBy;
};

/**
 * @brief Virtual Host configuration
 */
struct VirtualHost {
    std::string id;
    std::string name;
    std::string serverName;
    std::vector<std::string> serverAliases;
    std::string documentRoot;
    std::string accessLog;
    std::string errorLog;
    bool sslEnabled = false;
    std::string sslCertificate;
    std::string sslPrivateKey;
    std::string sslCertificateChain;
    std::vector<std::string> customDirectives;
    std::unordered_map<std::string, std::string> locationBlocks;
    bool active = true;
    std::chrono::steady_clock::time_point created;
    std::chrono::steady_clock::time_point lastModified;
    std::string configContent; // Raw configuration as editable content
};

/**
 * @brief Editor state for real-time collaboration
 */
struct EditorState {
    std::shared_ptr<DocumentNode> document;
    size_t cursorPosition = 0;
    size_t selectionStart = 0;
    size_t selectionEnd = 0;
    std::string activeVirtualHost;
    std::unordered_map<std::string, std::string> metadata;
    std::vector<std::string> undoStack;
    std::vector<std::string> redoStack;
    bool isDirty = false;
    std::chrono::steady_clock::time_point lastChange;
};

/**
 * @brief Change operation for real-time sync
 */
struct EditorOperation {
    enum Type {
        Insert,
        Delete,
        Replace,
        FormatText,
        InsertNode,
        DeleteNode,
        MoveNode,
        UpdateVirtualHost,
        CreateVirtualHost,
        DeleteVirtualHost
    };
    
    Type type;
    size_t position;
    size_t length;
    std::string content;
    std::string nodeId;
    TextAttributes attributes;
    std::unordered_map<std::string, std::string> props;
    std::string userId;
    std::chrono::steady_clock::time_point timestamp;
    std::string operationId;
};

/**
 * @brief CRUD operation result
 */
struct CRUDResult {
    bool success = false;
    std::string message;
    std::string id;
    std::unordered_map<std::string, std::string> data;
};

/**
 * @brief Editor event callbacks
 */
struct EditorCallbacks {
    std::function<void(const std::string&)> onContentChange;
    std::function<void(const EditorOperation&)> onOperation;
    std::function<void(const std::string&, const VirtualHost&)> onVirtualHostChange;
    std::function<void(const std::string&)> onSave;
    std::function<void(const std::string&)> onError;
    std::function<void(const std::string&, const std::string&)> onCollaboratorJoin;
    std::function<void(const std::string&)> onCollaboratorLeave;
};

/**
 * @brief Revolutionary MedusaEditor - The TipTap Killer
 * 
 * Features:
 * - Zero third-party dependencies (pure C++)
 * - Real-time collaborative editing
 * - Virtual Host configuration with live preview
 * - Advanced rich text editing with custom nodes
 * - Real-time CRUD operations
 * - WebSocket-based synchronization
 * - Plugin system for extensibility
 * - Performance optimized for 1000+ concurrent users
 * - SSL Grade A+ security built-in
 */
class MEDUSA_EDITOR_API MedusaEditor {
public:
    MedusaEditor();
    virtual ~MedusaEditor();
    
    // Core editor operations
    bool Initialize(const std::string& config_path = "");
    void Shutdown();
    
    // Document management
    std::string CreateDocument(const std::string& title = "Untitled");
    bool LoadDocument(const std::string& document_id);
    bool SaveDocument(const std::string& document_id = "");
    bool DeleteDocument(const std::string& document_id);
    std::vector<std::string> ListDocuments();
    
    // Content manipulation
    bool InsertText(const std::string& text, size_t position = SIZE_MAX);
    bool DeleteText(size_t position, size_t length);
    bool ReplaceText(size_t position, size_t length, const std::string& new_text);
    bool FormatText(size_t start, size_t end, const TextAttributes& attributes);
    
    // Node operations
    std::string InsertNode(NodeType type, size_t position, const std::unordered_map<std::string, std::string>& props = {});
    bool DeleteNode(const std::string& node_id);
    bool UpdateNode(const std::string& node_id, const std::unordered_map<std::string, std::string>& props);
    bool MoveNode(const std::string& node_id, size_t new_position);
    
    // Virtual Host management (The game-changer!)
    CRUDResult CreateVirtualHost(const VirtualHost& vhost);
    CRUDResult ReadVirtualHost(const std::string& vhost_id);
    CRUDResult UpdateVirtualHost(const std::string& vhost_id, const VirtualHost& vhost);
    CRUDResult DeleteVirtualHost(const std::string& vhost_id);
    std::vector<VirtualHost> ListVirtualHosts();
    bool SetActiveVirtualHost(const std::string& vhost_id);
    std::string GetActiveVirtualHost() const;
    
    // Virtual Host configuration editing
    bool EditVirtualHostConfig(const std::string& vhost_id);
    bool SaveVirtualHostConfig(const std::string& vhost_id, const std::string& config);
    std::string GetVirtualHostConfig(const std::string& vhost_id);
    bool ValidateVirtualHostConfig(const std::string& config, std::vector<std::string>& errors);
    bool ApplyVirtualHostConfig(const std::string& vhost_id);
    
    // Real-time collaboration
    bool StartCollaboration(const std::string& session_id);
    bool JoinCollaborationSession(const std::string& session_id, const std::string& user_id);
    bool LeaveCollaborationSession();
    void BroadcastOperation(const EditorOperation& operation);
    void ApplyRemoteOperation(const EditorOperation& operation);
    std::vector<std::string> GetActiveCollaborators();
    
    // State management
    EditorState GetState() const;
    bool SetState(const EditorState& state);
    bool Undo();
    bool Redo();
    void SaveState();
    bool HasUnsavedChanges() const;
    
    // Export/Import
    std::string ExportToHTML(bool include_styles = true);
    std::string ExportToMarkdown();
    std::string ExportToJSON();
    bool ImportFromHTML(const std::string& html);
    bool ImportFromMarkdown(const std::string& markdown);
    bool ImportFromJSON(const std::string& json);
    
    // Search and replace
    std::vector<size_t> Find(const std::string& query, bool case_sensitive = false, bool regex = false);
    bool Replace(const std::string& search, const std::string& replace, bool replace_all = false);
    
    // Event handling
    void SetCallbacks(const EditorCallbacks& callbacks);
    void RemoveCallbacks();
    
    // Configuration
    bool SetConfiguration(const std::string& key, const std::string& value);
    std::string GetConfiguration(const std::string& key) const;
    
    // Performance and statistics
    struct Statistics {
        size_t total_operations = 0;
        size_t active_collaborators = 0;
        size_t document_size_bytes = 0;
        double avg_operation_time_ms = 0.0;
        std::chrono::steady_clock::time_point session_start;
        size_t virtual_hosts_managed = 0;
        size_t configs_applied = 0;
    };
    
    Statistics GetStatistics() const;
    void ResetStatistics();
    
    // Plugin system
    bool LoadPlugin(const std::string& plugin_path);
    bool UnloadPlugin(const std::string& plugin_name);
    std::vector<std::string> ListLoadedPlugins();
    
    // Server integration
    bool StartWebSocketServer(int port = 8765);
    bool StopWebSocketServer();
    bool IsWebSocketServerRunning() const;
    
    // SSL/Security
    bool EnableSSL(const std::string& cert_path, const std::string& key_path);
    bool IsSSLEnabled() const;
    std::string GetSSLFingerprint() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

/**
 * @brief Editor command interface for plugins
 */
class MEDUSA_EDITOR_API EditorCommand {
public:
    virtual ~EditorCommand() = default;
    virtual bool Execute(MedusaEditor* editor, const std::unordered_map<std::string, std::string>& params) = 0;
    virtual std::string GetName() const = 0;
    virtual std::string GetDescription() const = 0;
    virtual std::vector<std::string> GetRequiredParams() const = 0;
};

/**
 * @brief Plugin interface
 */
class MEDUSA_EDITOR_API EditorPlugin {
public:
    virtual ~EditorPlugin() = default;
    virtual bool Initialize(MedusaEditor* editor) = 0;
    virtual void Shutdown() = 0;
    virtual std::string GetName() const = 0;
    virtual std::string GetVersion() const = 0;
    virtual std::vector<std::shared_ptr<EditorCommand>> GetCommands() = 0;
};

// C API for language bindings
extern "C" {
    // Core functions
    MEDUSA_EDITOR_API void* medusa_editor_create();
    MEDUSA_EDITOR_API void medusa_editor_destroy(void* editor);
    MEDUSA_EDITOR_API int medusa_editor_initialize(void* editor, const char* config_path);
    
    // Virtual Host management
    MEDUSA_EDITOR_API int medusa_editor_create_vhost(void* editor, const char* vhost_json);
    MEDUSA_EDITOR_API char* medusa_editor_read_vhost(void* editor, const char* vhost_id);
    MEDUSA_EDITOR_API int medusa_editor_update_vhost(void* editor, const char* vhost_id, const char* vhost_json);
    MEDUSA_EDITOR_API int medusa_editor_delete_vhost(void* editor, const char* vhost_id);
    MEDUSA_EDITOR_API char* medusa_editor_list_vhosts(void* editor);
    
    // Content operations
    MEDUSA_EDITOR_API int medusa_editor_insert_text(void* editor, const char* text, size_t position);
    MEDUSA_EDITOR_API int medusa_editor_delete_text(void* editor, size_t position, size_t length);
    MEDUSA_EDITOR_API char* medusa_editor_get_content(void* editor);
    
    // Collaboration
    MEDUSA_EDITOR_API int medusa_editor_start_collaboration(void* editor, const char* session_id);
    MEDUSA_EDITOR_API int medusa_editor_join_collaboration(void* editor, const char* session_id, const char* user_id);
    
    // Export/Import
    MEDUSA_EDITOR_API char* medusa_editor_export_html(void* editor, int include_styles);
    MEDUSA_EDITOR_API char* medusa_editor_export_json(void* editor);
    MEDUSA_EDITOR_API int medusa_editor_import_html(void* editor, const char* html);
    
    // Memory management for returned strings
    MEDUSA_EDITOR_API void medusa_editor_free_string(char* str);
}

// Helper macros
#define MEDUSA_EDITOR_VERSION_MAJOR 1
#define MEDUSA_EDITOR_VERSION_MINOR 0
#define MEDUSA_EDITOR_VERSION_PATCH 0
#define MEDUSA_EDITOR_VERSION_STRING "1.0.0-alpha"

// Success/Error codes
#define MEDUSA_EDITOR_SUCCESS 0
#define MEDUSA_EDITOR_ERROR_INVALID_PARAM -1
#define MEDUSA_EDITOR_ERROR_NOT_INITIALIZED -2
#define MEDUSA_EDITOR_ERROR_OPERATION_FAILED -3
#define MEDUSA_EDITOR_ERROR_PERMISSION_DENIED -4
#define MEDUSA_EDITOR_ERROR_NETWORK_ERROR -5
#define MEDUSA_EDITOR_ERROR_INVALID_VHOST -6
#define MEDUSA_EDITOR_ERROR_VHOST_NOT_FOUND -7
#define MEDUSA_EDITOR_ERROR_CONFIG_INVALID -8

} // namespace MedusaEditor
#endif // MEDUSA_MEDUSA_EDITOR_H
