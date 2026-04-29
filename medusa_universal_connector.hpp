#pragma once
#include <string>
#include <memory>

namespace MedusaTheme::Database {

class UniversalConnector {
public:
    UniversalConnector();
    ~UniversalConnector();
    
    // Database Connections
    bool connectPostgreSQL(const std::string& host, const std::string& user, const std::string& password, const std::string& database);
    bool connectSQLite(const std::string& database_path);
    bool connectMongoDB(const std::string& uri);
    bool connectNeo4j(const std::string& uri, const std::string& user, const std::string& password);
    
    // Triforce Integration
    bool initializeTriforce();
    
    // OpenSSL Integration
    bool initializeSSL();
    
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}
