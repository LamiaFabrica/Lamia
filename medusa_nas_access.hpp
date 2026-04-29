/*
 * MEDUSA NAS ACCESS SYSTEM
 * Backup and file management for 192.168.1.168
 * Secure file transfer with Yorkshire Champion authentication
 */

#pragma once

#include "medusa_credentials_vault.hpp"
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <functional>
#include <map>

namespace MedusaServer {

struct FileInfo {
    std::string filename;
    std::string full_path;
    uint64_t size;
    std::chrono::system_clock::time_point modified_time;
    bool is_directory;
    std::string permissions;
    std::string owner;
};

struct BackupJob {
    std::string job_id;
    std::string source_path;
    std::string destination_path;
    std::string schedule; // cron-style schedule
    bool enabled;
    std::chrono::system_clock::time_point last_run;
    std::chrono::system_clock::time_point next_run;
    std::string status; // "pending", "running", "completed", "failed"
    std::vector<std::string> exclude_patterns;
    bool compress;
    bool encrypt;
};

struct TransferProgress {
    std::string transfer_id;
    uint64_t total_bytes;
    uint64_t transferred_bytes;
    double percentage;
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point estimated_completion;
    std::string status; // "transferring", "completed", "failed", "paused"
    std::string error_message;
};

class NASAccessSystem {
private:
    std::shared_ptr<CredentialsVault> credentials_vault_;
    NASCredentials nas_credentials_;
    std::string mount_point_;
    bool connected_;
    std::map<std::string, BackupJob> backup_jobs_;
    std::map<std::string, TransferProgress> active_transfers_;
    
public:
    NASAccessSystem(std::shared_ptr<CredentialsVault> vault);
    ~NASAccessSystem();
    
    // Connection management
    bool initialize();
    bool connect();
    bool disconnect();
    bool isConnected() const { return connected_; }
    
    // File operations
    std::vector<FileInfo> listFiles(const std::string& remote_path = "/");
    bool uploadFile(const std::string& local_path, const std::string& remote_path);
    bool downloadFile(const std::string& remote_path, const std::string& local_path);
    bool deleteFile(const std::string& remote_path);
    bool createDirectory(const std::string& remote_path);
    bool deleteDirectory(const std::string& remote_path);
    
    // Backup operations
    std::string createBackupJob(const BackupJob& job);
    bool scheduleBackup(const std::string& job_id);
    bool runBackup(const std::string& job_id);
    bool cancelBackup(const std::string& job_id);
    std::vector<BackupJob> getBackupJobs();
    BackupJob getBackupJob(const std::string& job_id);
    bool updateBackupJob(const std::string& job_id, const BackupJob& job);
    bool deleteBackupJob(const std::string& job_id);
    
    // Server-specific backup methods
    bool backupBackendServer(const std::string& destination_path = "/medusa_backups/backend");
    bool backupFrontendServer(const std::string& destination_path = "/medusa_backups/frontend");
    bool backupDatabaseServer(const std::string& destination_path = "/medusa_backups/database");
    bool backupMedlabServer(const std::string& destination_path = "/medusa_backups/medlab");
    bool backupLocalBuild(const std::string& destination_path = "/medusa_backups/local_build");
    
    // Transfer monitoring
    std::vector<TransferProgress> getActiveTransfers();
    TransferProgress getTransferProgress(const std::string& transfer_id);
    bool pauseTransfer(const std::string& transfer_id);
    bool resumeTransfer(const std::string& transfer_id);
    bool cancelTransfer(const std::string& transfer_id);
    
    // System monitoring
    std::string getSystemStatus();
    std::map<std::string, std::string> getNASHealth();
    uint64_t getAvailableSpace();
    uint64_t getUsedSpace();
    
    // Utility methods
    bool testConnection();
    std::string getLastError() const { return last_error_; }
    void setProgressCallback(std::function<void(const TransferProgress&)> callback);
    
private:
    std::string last_error_;
    std::function<void(const TransferProgress&)> progress_callback_;
    
    // Internal operations
    bool mountNAS();
    bool unmountNAS();
    std::string generateTransferId();
    std::string generateJobId();
    bool validatePath(const std::string& path);
    std::string sanitizePath(const std::string& path);
    
    // SMB/CIFS operations
    bool smbConnect();
    bool smbDisconnect();
    bool smbUpload(const std::string& local_path, const std::string& remote_path, const std::string& transfer_id);
    bool smbDownload(const std::string& remote_path, const std::string& local_path, const std::string& transfer_id);
    std::vector<FileInfo> smbListFiles(const std::string& remote_path);
    
    // Backup implementation
    bool performBackup(const BackupJob& job);
    bool compressBackup(const std::string& source_path, const std::string& archive_path);
    bool encryptBackup(const std::string& source_path, const std::string& encrypted_path);
    
    // Progress tracking
    void updateTransferProgress(const std::string& transfer_id, uint64_t transferred, uint64_t total);
    void completeTransfer(const std::string& transfer_id, bool success, const std::string& error = "");
    
    // Error handling
    void setError(const std::string& error);
    void logError(const std::string& operation, const std::string& error);
    void logInfo(const std::string& operation, const std::string& info);
};

// Production backup scheduler
class BackupScheduler {
private:
    std::shared_ptr<NASAccessSystem> nas_system_;
    std::map<std::string, std::chrono::system_clock::time_point> next_runs_;
    bool running_;
    std::thread scheduler_thread_;
    
public:
    BackupScheduler(std::shared_ptr<NASAccessSystem> nas);
    ~BackupScheduler();
    
    bool start();
    void stop();
    bool isRunning() const { return running_; }
    
    // Schedule management
    bool addScheduledBackup(const std::string& job_id, const std::string& cron_schedule);
    bool removeScheduledBackup(const std::string& job_id);
    std::map<std::string, std::chrono::system_clock::time_point> getSchedule();
    
private:
    void schedulerLoop();
    std::chrono::system_clock::time_point parseCronSchedule(const std::string& cron);
    bool shouldRunBackup(const std::string& job_id);
};

// Production backup configuration
inline void setupProductionBackups(NASAccessSystem& nas) {
    // Daily backend server backup
    BackupJob backend_backup;
    backend_backup.job_id = "backend_daily";
    backend_backup.source_path = "/home/medusa";
    backend_backup.destination_path = "/medusa_backups/backend/daily";
    backend_backup.schedule = "0 2 * * *"; // 2 AM daily
    backend_backup.enabled = true;
    backend_backup.compress = true;
    backend_backup.encrypt = true;
    backend_backup.exclude_patterns = {"*.tmp", "*.log", "cache/*", "temp/*"};
    nas.createBackupJob(backend_backup);
    
    // Weekly database backup
    BackupJob db_backup;
    db_backup.job_id = "database_weekly";
    db_backup.source_path = "/var/lib/postgresql";
    db_backup.destination_path = "/medusa_backups/database/weekly";
    db_backup.schedule = "0 1 * * 0"; // 1 AM Sunday
    db_backup.enabled = true;
    db_backup.compress = true;
    db_backup.encrypt = true;
    nas.createBackupJob(db_backup);
    
    // Monthly full system backup
    BackupJob full_backup;
    full_backup.job_id = "full_system_monthly";
    full_backup.source_path = "/";
    full_backup.destination_path = "/medusa_backups/full_system/monthly";
    full_backup.schedule = "0 0 1 * *"; // 1st of month
    full_backup.enabled = true;
    full_backup.compress = true;
    full_backup.encrypt = true;
    full_backup.exclude_patterns = {"/proc/*", "/sys/*", "/dev/*", "/tmp/*", "/var/tmp/*"};
    nas.createBackupJob(full_backup);
    
    // Local build backup (daily)
    BackupJob build_backup;
    build_backup.job_id = "local_build_daily";
    build_backup.source_path = "/mnt/c/Build";
    build_backup.destination_path = "/medusa_backups/local_build/daily";
    build_backup.schedule = "0 3 * * *"; // 3 AM daily
    build_backup.enabled = true;
    build_backup.compress = true;
    build_backup.encrypt = false; // Local network, no encryption needed
    build_backup.exclude_patterns = {"*.obj", "*.exe", "build/*", "Debug/*", "Release/*"};
    nas.createBackupJob(build_backup);
}

} // namespace MedusaServer