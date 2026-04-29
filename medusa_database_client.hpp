#pragma once

/**
 * MEDUSA DATABASE MANAGEMENT CLIENT v0.2.7
 * ========================================
 * 
 * Yorkshire Champion Gold Standard Database Management Interface
 * Complete replacement for Prisma, pgAdmin, Neo4j Browser, and MongoDB Compass
 * 
 * NO EXTERNAL DEPENDENCIES - Pure C++ ground-up implementation
 * Manages PostgreSQL + Neo4j + MongoDB through unified interface
 * 
 * Author: Claude Code (Yorkshire Champion)
 * Version: 0.2.7 (Pre-Alpha - Foundation Stage)
 * Purpose: Unified database management without the Prisma headaches!
 */

#include "medusa_triforce_connector.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <optional>
#include <regex>

namespace medusa::database {

    /**
     * Schema Information Structures
     */
    struct ColumnInfo {
        std::string name;
        std::string data_type;
        bool is_nullable;
        bool is_primary_key;
        bool is_foreign_key;
        std::string default_value;
        std::string foreign_table;
        std::string foreign_column;
        size_t max_length;
        
        ColumnInfo() : is_nullable(true), is_primary_key(false), 
                      is_foreign_key(false), max_length(0) {}
    };

    struct TableInfo {
        std::string name;
        std::string schema_name;
        std::vector<ColumnInfo> columns;
        std::vector<std::string> indexes;
        std::vector<std::string> constraints;
        size_t row_count;
        size_t size_bytes;
        
        TableInfo() : row_count(0), size_bytes(0) {}
    };

    struct DatabaseInfo {
        std::string name;
        std::string version;
        std::vector<TableInfo> tables;
        std::vector<std::string> schemas;
        std::vector<std::string> users;
        size_t total_size_bytes;
        std::chrono::system_clock::time_point last_updated;
        
        DatabaseInfo() : total_size_bytes(0) {}
    };

    /**
     * Query Builder - NO PRISMA NEEDED!
     */
    class MedusaQueryBuilder {
    private:
        DatabaseType target_database_;
        std::string table_name_;
        std::vector<std::string> select_columns_;
        std::vector<std::string> where_conditions_;
        std::vector<std::string> join_clauses_;
        std::vector<std::string> order_by_clauses_;
        std::optional<size_t> limit_value_;
        std::optional<size_t> offset_value_;
        
    public:
        MedusaQueryBuilder(DatabaseType db_type) : target_database_(db_type) {}
        
        // PostgreSQL Query Building
        MedusaQueryBuilder& select(const std::vector<std::string>& columns);
        MedusaQueryBuilder& from(const std::string& table);
        MedusaQueryBuilder& where(const std::string& condition);
        MedusaQueryBuilder& join(const std::string& table, const std::string& condition);
        MedusaQueryBuilder& left_join(const std::string& table, const std::string& condition);
        MedusaQueryBuilder& order_by(const std::string& column, bool ascending = true);
        MedusaQueryBuilder& limit(size_t count);
        MedusaQueryBuilder& offset(size_t count);
        
        // Neo4j Cypher Building
        MedusaQueryBuilder& match(const std::string& pattern);
        MedusaQueryBuilder& create(const std::string& pattern);
        MedusaQueryBuilder& merge(const std::string& pattern);
        MedusaQueryBuilder& return_fields(const std::vector<std::string>& fields);
        MedusaQueryBuilder& with(const std::string& expression);
        
        // MongoDB Query Building
        MedusaQueryBuilder& collection(const std::string& collection_name);
        MedusaQueryBuilder& find(const std::string& filter_json);
        MedusaQueryBuilder& insert(const std::string& document_json);
        MedusaQueryBuilder& update(const std::string& filter_json, const std::string& update_json);
        MedusaQueryBuilder& aggregate(const std::vector<std::string>& pipeline_stages);
        
        // Build final query
        std::string build_query();
        QueryResult execute();
        
        // Reset builder for reuse
        void reset();
    };

    /**
     * Schema Inspector - Understand your databases without external tools!
     */
    class SchemaInspector {
    private:
        std::shared_ptr<TriforceConnector> connector_;
        std::unordered_map<DatabaseType, DatabaseInfo> cached_schemas_;
        std::chrono::system_clock::time_point last_cache_update_;
        std::chrono::minutes cache_expiry_time_;
        
    public:
        SchemaInspector(std::shared_ptr<TriforceConnector> connector)
            : connector_(connector), cache_expiry_time_(15) {}
        
        // PostgreSQL Schema Inspection
        std::vector<std::string> get_postgresql_schemas();
        std::vector<TableInfo> get_postgresql_tables(const std::string& schema = "public");
        ColumnInfo get_postgresql_column_info(const std::string& table, const std::string& column);
        std::vector<std::string> get_postgresql_indexes(const std::string& table);
        std::vector<std::string> get_postgresql_constraints(const std::string& table);
        
        // Neo4j Schema Inspection
        std::vector<std::string> get_neo4j_node_labels();
        std::vector<std::string> get_neo4j_relationship_types();
        std::vector<std::string> get_neo4j_property_keys();
        std::vector<std::string> get_neo4j_indexes();
        std::vector<std::string> get_neo4j_constraints();
        
        // MongoDB Schema Inspection
        std::vector<std::string> get_mongodb_databases();
        std::vector<std::string> get_mongodb_collections(const std::string& database = "");
        std::vector<std::string> get_mongodb_indexes(const std::string& collection);
        std::unordered_map<std::string, std::string> get_mongodb_collection_stats(const std::string& collection);
        
        // Unified operations
        DatabaseInfo get_database_info(DatabaseType db_type);
        bool refresh_schema_cache(DatabaseType db_type);
        void clear_cache();
        
        // Schema analysis
        std::vector<std::string> find_orphaned_records(DatabaseType db_type, const std::string& table);
        std::vector<std::string> suggest_indexes(DatabaseType db_type, const std::string& table);
        std::vector<std::string> analyze_performance_issues(DatabaseType db_type);
    };

    /**
     * Data Migration Manager - Move data between all three databases!
     */
    class DataMigrationManager {
    private:
        std::shared_ptr<TriforceConnector> connector_;
        std::shared_ptr<SchemaInspector> inspector_;
        
    public:
        DataMigrationManager(std::shared_ptr<TriforceConnector> connector,
                           std::shared_ptr<SchemaInspector> inspector)
            : connector_(connector), inspector_(inspector) {}
        
        // Cross-database migrations
        bool migrate_postgresql_to_mongodb(const std::string& source_table, 
                                          const std::string& target_collection,
                                          const std::function<std::string(const std::unordered_map<std::string, std::string>&)>& transformer = nullptr);
        
        bool migrate_mongodb_to_postgresql(const std::string& source_collection,
                                          const std::string& target_table,
                                          const std::function<std::unordered_map<std::string, std::string>(const std::string&)>& transformer = nullptr);
        
        bool migrate_postgresql_to_neo4j(const std::string& source_table,
                                        const std::string& node_label,
                                        const std::vector<std::string>& relationship_columns = {});
        
        bool migrate_neo4j_to_postgresql(const std::string& node_label,
                                        const std::string& target_table,
                                        bool include_relationships = false);
        
        // Schema migrations
        bool create_postgresql_table_from_mongodb(const std::string& collection, const std::string& table_name);
        bool create_mongodb_collection_from_postgresql(const std::string& table, const std::string& collection_name);
        
        // Data synchronization
        bool sync_postgresql_mongodb(const std::string& table, const std::string& collection, bool bidirectional = false);
        bool sync_postgresql_neo4j(const std::string& table, const std::string& node_label, bool bidirectional = false);
        
        // Backup and restore
        bool backup_database_to_json(DatabaseType db_type, const std::string& output_file);
        bool restore_database_from_json(DatabaseType db_type, const std::string& input_file);
    };

    /**
     * Query Performance Analyzer - Optimize without external tools!
     */
    class PerformanceAnalyzer {
    private:
        std::shared_ptr<TriforceConnector> connector_;
        std::vector<std::pair<std::string, std::chrono::milliseconds>> query_history_;
        std::unordered_map<std::string, size_t> query_frequency_;
        
    public:
        PerformanceAnalyzer(std::shared_ptr<TriforceConnector> connector)
            : connector_(connector) {}
        
        struct PerformanceReport {
            std::string query;
            DatabaseType database_type;
            std::chrono::milliseconds execution_time;
            std::chrono::milliseconds average_time;
            size_t execution_count;
            std::vector<std::string> optimization_suggestions;
        };
        
        // Performance monitoring
        void record_query_execution(const std::string& query, DatabaseType db_type, std::chrono::milliseconds execution_time);
        std::vector<PerformanceReport> get_slow_queries(std::chrono::milliseconds threshold = std::chrono::milliseconds(1000));
        std::vector<PerformanceReport> get_frequent_queries(size_t min_frequency = 10);
        
        // PostgreSQL specific analysis
        std::vector<std::string> analyze_postgresql_query_plan(const std::string& query);
        std::vector<std::string> suggest_postgresql_indexes(const std::string& query);
        
        // Neo4j specific analysis
        std::vector<std::string> analyze_neo4j_query_profile(const std::string& cypher);
        std::vector<std::string> suggest_neo4j_indexes(const std::string& cypher);
        
        // MongoDB specific analysis
        std::vector<std::string> analyze_mongodb_query_stats(const std::string& collection, const std::string& query_json);
        std::vector<std::string> suggest_mongodb_indexes(const std::string& collection, const std::string& query_json);
        
        // General optimization
        void clear_query_history();
        std::unordered_map<std::string, double> get_database_performance_metrics();
    };

    /**
     * Main Database Management Client - THE UNIFIED INTERFACE
     */
    class MedusaDatabaseClient {
    private:
        std::shared_ptr<TriforceConnector> connector_;
        std::shared_ptr<SchemaInspector> inspector_;
        std::shared_ptr<DataMigrationManager> migration_manager_;
        std::shared_ptr<PerformanceAnalyzer> performance_analyzer_;
        
        // Client state
        DatabaseType active_database_;
        std::string active_schema_;
        std::string active_collection_;
        std::unordered_map<std::string, std::string> saved_queries_;
        
    public:
        MedusaDatabaseClient();
        ~MedusaDatabaseClient();
        
        // Initialization
        bool initialize(const DatabaseConfig& config = DatabaseConfig{});
        void shutdown();
        bool is_connected() const;
        
        // Database selection
        void set_active_database(DatabaseType db_type);
        void set_active_schema(const std::string& schema);
        void set_active_collection(const std::string& collection);
        
        DatabaseType get_active_database() const { return active_database_; }
        std::string get_active_schema() const { return active_schema_; }
        std::string get_active_collection() const { return active_collection_; }
        
        // Query execution
        QueryResult execute_raw_query(const std::string& query, DatabaseType db_type = DatabaseType::POSTGRESQL);
        QueryResult execute_query_builder(MedusaQueryBuilder& builder);
        
        // Schema management
        SchemaInspector* get_schema_inspector() { return inspector_.get(); }
        std::vector<TableInfo> list_tables(DatabaseType db_type);
        std::vector<std::string> list_databases(DatabaseType db_type);
        
        bool create_table(const std::string& table_name, const std::vector<ColumnInfo>& columns);
        bool drop_table(const std::string& table_name);
        bool alter_table(const std::string& table_name, const std::vector<std::string>& alterations);
        
        // Data management
        QueryResult select_data(const std::string& table_or_collection, 
                               const std::string& filter = "",
                               size_t limit = 100);
        bool insert_data(const std::string& table_or_collection, 
                        const std::unordered_map<std::string, std::string>& data);
        bool update_data(const std::string& table_or_collection,
                        const std::string& filter,
                        const std::unordered_map<std::string, std::string>& data);
        bool delete_data(const std::string& table_or_collection, const std::string& filter);
        
        // Migration operations
        DataMigrationManager* get_migration_manager() { return migration_manager_.get(); }
        
        // Performance monitoring
        PerformanceAnalyzer* get_performance_analyzer() { return performance_analyzer_.get(); }
        
        // Query management
        void save_query(const std::string& name, const std::string& query);
        std::optional<std::string> load_query(const std::string& name);
        std::vector<std::string> list_saved_queries();
        bool delete_saved_query(const std::string& name);
        
        // Health and status
        TriforceConnector::HealthStatus get_health_status();
        std::unordered_map<std::string, std::string> get_connection_info();
        
        // Yorkshire Champion utilities
        std::string get_yorkshire_status() const;
        void log_operation(const std::string& operation, bool success, const std::string& details = "");
        
        // Export and import
        bool export_table_to_csv(const std::string& table_name, const std::string& output_file);
        bool import_csv_to_table(const std::string& table_name, const std::string& input_file);
        bool export_collection_to_json(const std::string& collection_name, const std::string& output_file);
        bool import_json_to_collection(const std::string& collection_name, const std::string& input_file);
        
    private:
        // Helper methods
        std::string format_query_result(const QueryResult& result, const std::string& format = "table");
        std::vector<std::string> parse_csv_line(const std::string& line);
        std::string escape_csv_field(const std::string& field);
        bool validate_column_info(const std::vector<ColumnInfo>& columns);
        std::string generate_create_table_sql(const std::string& table_name, const std::vector<ColumnInfo>& columns);
    };

    /**
     * CLI Interface for Database Client - Command line management
     */
    class DatabaseCLI {
    private:
        std::shared_ptr<MedusaDatabaseClient> client_;
        std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> commands_;
        bool running_;
        
    public:
        DatabaseCLI();
        ~DatabaseCLI();
        
        bool initialize();
        void run_interactive_mode();
        void execute_command(const std::string& command_line);
        void shutdown();
        
    private:
        void register_commands();
        void print_help();
        void print_welcome();
        std::vector<std::string> parse_command_line(const std::string& line);
        
        // Command implementations
        void cmd_connect(const std::vector<std::string>& args);
        void cmd_disconnect(const std::vector<std::string>& args);
        void cmd_use(const std::vector<std::string>& args);
        void cmd_show_tables(const std::vector<std::string>& args);
        void cmd_describe(const std::vector<std::string>& args);
        void cmd_select(const std::vector<std::string>& args);
        void cmd_insert(const std::vector<std::string>& args);
        void cmd_update(const std::vector<std::string>& args);
        void cmd_delete(const std::vector<std::string>& args);
        void cmd_migrate(const std::vector<std::string>& args);
        void cmd_backup(const std::vector<std::string>& args);
        void cmd_restore(const std::vector<std::string>& args);
        void cmd_health(const std::vector<std::string>& args);
        void cmd_performance(const std::vector<std::string>& args);
        void cmd_help(const std::vector<std::string>& args);
        void cmd_exit(const std::vector<std::string>& args);
    };

} // namespace medusa::database