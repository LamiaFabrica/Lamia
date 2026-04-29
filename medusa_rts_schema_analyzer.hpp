#ifndef MEDUSA_RTS_SCHEMA_ANALYZER_HPP
#define MEDUSA_RTS_SCHEMA_ANALYZER_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <nlohmann/json.hpp>
#include "universal_database_connector.hpp"
#include "credentials_vault.hpp"

namespace MedusaServ {
namespace Database {

/**
 * @brief Medusa RTS Schema Analyzer
 * 
 * Yorkshire Champion Gold Standard database schema analysis system
 * Analyzes existing medusa_rts database schema for 104,219 files conversion
 * Uses Claude's Universal Database Connector and Credentials Vault
 * 
 * @author MedusaServ Team
 * @version 1.0.0
 * @date 2025-01-11
 */
class MedusaRtsSchemaAnalyzer {
private:
    std::unique_ptr<UniversalDatabaseConnector> universal_connector_;
    std::unique_ptr<CredentialsVault> credentials_vault_;
    std::map<std::string, nlohmann::json> schema_cache_;
    std::vector<std::string> analyzed_tables_;
    bool is_connected_;
    
    // Schema analysis results
    struct SchemaAnalysis {
        nlohmann::json table_definitions;
        nlohmann::json relationships;
        nlohmann::json constraints;
        nlohmann::json indexes;
        nlohmann::json triggers;
        nlohmann::json views;
        nlohmann::json stored_procedures;
        nlohmann::json functions;
    } analysis_results_;

public:
    /**
     * @brief Constructor
     * @param vault_config_path Path to credentials vault configuration
     */
    explicit MedusaRtsSchemaAnalyzer(const std::string& vault_config_path);
    
    /**
     * @brief Destructor
     */
    ~MedusaRtsSchemaAnalyzer();
    
    /**
     * @brief Initialize the schema analyzer with universal connector
     * @return true if initialization successful
     */
    bool initialize();
    
    /**
     * @brief Connect to medusa_rts database using universal connector
     * @return true if connection successful
     */
    bool connect_to_database();
    
    /**
     * @brief Analyze complete database schema
     * @return Schema analysis results as JSON
     */
    nlohmann::json analyze_complete_schema();
    
    /**
     * @brief Analyze specific table schema
     * @param table_name Table name to analyze
     * @return Table schema analysis as JSON
     */
    nlohmann::json analyze_table_schema(const std::string& table_name);
    
    /**
     * @brief Analyze table relationships
     * @param table_name Table name to analyze relationships for
     * @return Relationship analysis as JSON
     */
    nlohmann::json analyze_table_relationships(const std::string& table_name);
    
    /**
     * @brief Analyze database constraints
     * @return Constraints analysis as JSON
     */
    nlohmann::json analyze_constraints();
    
    /**
     * @brief Analyze database indexes
     * @return Indexes analysis as JSON
     */
    nlohmann::json analyze_indexes();
    
    /**
     * @brief Analyze database triggers
     * @return Triggers analysis as JSON
     */
    nlohmann::json analyze_triggers();
    
    /**
     * @brief Analyze database views
     * @return Views analysis as JSON
     */
    nlohmann::json analyze_views();
    
    /**
     * @brief Analyze stored procedures
     * @return Stored procedures analysis as JSON
     */
    nlohmann::json analyze_stored_procedures();
    
    /**
     * @brief Analyze database functions
     * @return Functions analysis as JSON
     */
    nlohmann::json analyze_functions();
    
    /**
     * @brief Generate schema documentation mapping
     * @param file_conversion_list List of files to be converted
     * @return Documentation mapping as JSON
     */
    nlohmann::json generate_schema_documentation_mapping(const std::vector<std::string>& file_conversion_list);
    
    /**
     * @brief Map Next.js components to database schema
     * @param component_list List of Next.js components
     * @return Component to schema mapping as JSON
     */
    nlohmann::json map_components_to_schema(const std::vector<std::string>& component_list);
    
    /**
     * @brief Map API endpoints to database operations
     * @param endpoint_list List of API endpoints
     * @return Endpoint to database mapping as JSON
     */
    nlohmann::json map_endpoints_to_database(const std::vector<std::string>& endpoint_list);
    
    /**
     * @brief Generate C++ database connector specifications
     * @return C++ connector specifications as JSON
     */
    nlohmann::json generate_cpp_connector_specifications();
    
    /**
     * @brief Validate schema compatibility with C++ conversion
     * @return Validation results as JSON
     */
    nlohmann::json validate_schema_compatibility();
    
    /**
     * @brief Generate migration scripts for schema updates
     * @return Migration scripts as JSON
     */
    nlohmann::json generate_migration_scripts();
    
    /**
     * @brief Export schema analysis to various formats
     * @param format Export format (sql, json, xml, yaml)
     * @param output_path Output file path
     * @return Export result as JSON
     */
    nlohmann::json export_schema_analysis(const std::string& format, const std::string& output_path);
    
    /**
     * @brief Get schema analysis statistics
     * @return Statistics as JSON
     */
    nlohmann::json get_schema_statistics();
    
    /**
     * @brief Get connection status
     * @return true if connected to database
     */
    bool is_connected() const { return is_connected_; }
    
    /**
     * @brief Get analyzed tables list
     * @return List of analyzed tables
     */
    std::vector<std::string> get_analyzed_tables() const { return analyzed_tables_; }

private:
    /**
     * @brief Initialize universal database connector
     * @return true if initialization successful
     */
    bool initialize_universal_connector();
    
    /**
     * @brief Load credentials from vault
     * @return true if credentials loaded successfully
     */
    bool load_credentials_from_vault();
    
    /**
     * @brief Execute database query using universal connector
     * @param query SQL query
     * @param params Query parameters
     * @return Query result as JSON
     */
    nlohmann::json execute_query(const std::string& query, const nlohmann::json& params = {});
    
    /**
     * @brief Get table column information
     * @param table_name Table name
     * @return Column information as JSON
     */
    nlohmann::json get_table_columns(const std::string& table_name);
    
    /**
     * @brief Get table primary key information
     * @param table_name Table name
     * @return Primary key information as JSON
     */
    nlohmann::json get_table_primary_keys(const std::string& table_name);
    
    /**
     * @brief Get table foreign key information
     * @param table_name Table name
     * @return Foreign key information as JSON
     */
    nlohmann::json get_table_foreign_keys(const std::string& table_name);
    
    /**
     * @brief Get table unique constraints
     * @param table_name Table name
     * @return Unique constraints as JSON
     */
    nlohmann::json get_table_unique_constraints(const std::string& table_name);
    
    /**
     * @brief Get table check constraints
     * @param table_name Table name
     * @return Check constraints as JSON
     */
    nlohmann::json get_table_check_constraints(const std::string& table_name);
    
    /**
     * @brief Get table indexes
     * @param table_name Table name
     * @return Indexes as JSON
     */
    nlohmann::json get_table_indexes(const std::string& table_name);
    
    /**
     * @brief Get table triggers
     * @param table_name Table name
     * @return Triggers as JSON
     */
    nlohmann::json get_table_triggers(const std::string& table_name);
    
    /**
     * @brief Parse SQL data types to C++ types
     * @param sql_type SQL data type
     * @return C++ type mapping
     */
    std::string map_sql_type_to_cpp(const std::string& sql_type);
    
    /**
     * @brief Generate C++ class definition for table
     * @param table_name Table name
     * @param table_schema Table schema
     * @return C++ class definition
     */
    std::string generate_cpp_class_definition(const std::string& table_name, const nlohmann::json& table_schema);
    
    /**
     * @brief Generate C++ method definitions for table operations
     * @param table_name Table name
     * @param table_schema Table schema
     * @return C++ method definitions
     */
    std::string generate_cpp_method_definitions(const std::string& table_name, const nlohmann::json& table_schema);
    
    /**
     * @brief Log schema analysis operation
     * @param operation Operation type
     * @param table_name Table name
     * @param details Operation details
     */
    void log_analysis_operation(const std::string& operation, const std::string& table_name, const std::string& details);
    
    /**
     * @brief Validate table name format
     * @param table_name Table name
     * @return true if valid format
     */
    bool validate_table_name_format(const std::string& table_name);
    
    /**
     * @brief Sanitize SQL identifier
     * @param identifier SQL identifier
     * @return Sanitized identifier
     */
    std::string sanitize_sql_identifier(const std::string& identifier);
};

} // namespace Database
} // namespace MedusaServ

#endif // MEDUSA_RTS_SCHEMA_ANALYZER_HPP 