#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <mutex>
#include <chrono>
#include <sstream>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <dbghelp.h>
#include <tchar.h>
#pragma comment(lib, "dbghelp.lib")
#endif

enum class LogLevel {
    DEBUG_LEVEL = 0,
    INFO_LEVEL = 1,
    WARN_LEVEL = 2,
    ERROR_LEVEL = 3,
    FATAL_LEVEL = 4
};

class Logger {
private:
    static std::unique_ptr<Logger> s_instance;
    static std::mutex s_mutex;
    
    std::ofstream m_logFile;
    std::vector<std::string> m_logBuffer;
    LogLevel m_minLevel = LogLevel::INFO_LEVEL;
    bool m_consoleOutput = true;
    bool m_fileOutput = true;
    bool m_verboseMode = false;
    std::string m_logFilePath;
    std::mutex m_logMutex;
    
    Logger();

public:
    ~Logger();
    
    static Logger& Instance();
    static void Initialize(const std::string& logFile = "qwen_builder.log");
    static void Shutdown();
    
    // Configuration
    void SetMinLevel(LogLevel level) { m_minLevel = level; }
    void SetConsoleOutput(bool enable) { m_consoleOutput = enable; }
    void SetFileOutput(bool enable) { m_fileOutput = enable; }
    void SetVerboseMode(bool enable) { m_verboseMode = enable; }
    
    // Logging functions
    void Log(LogLevel level, const std::string& message);
    void Log(LogLevel level, const char* format, ...);
    
    // Convenience functions
    void Debug(const std::string& message) { Log(LogLevel::DEBUG_LEVEL, message); }
    void Info(const std::string& message) { Log(LogLevel::INFO_LEVEL, message); }
    void Warn(const std::string& message) { Log(LogLevel::WARN_LEVEL, message); }
    void Error(const std::string& message) { Log(LogLevel::ERROR_LEVEL, message); }
    void Fatal(const std::string& message) { Log(LogLevel::FATAL_LEVEL, message); }
    
    // Template versions for convenience
    template<typename... Args>
    void Debug(const char* format, Args... args) { Log(LogLevel::DEBUG_LEVEL, format, args...); }
    
    template<typename... Args>
    void Info(const char* format, Args... args) { Log(LogLevel::INFO_LEVEL, format, args...); }
    
    template<typename... Args>
    void Warn(const char* format, Args... args) { Log(LogLevel::WARN_LEVEL, format, args...); }
    
    template<typename... Args>
    void Error(const char* format, Args... args) { Log(LogLevel::ERROR_LEVEL, format, args...); }
    
    template<typename... Args>
    void Fatal(const char* format, Args... args) { Log(LogLevel::FATAL_LEVEL, format, args...); }
    
    // System info logging
    void LogSystemInfo();
    void LogOpenGLInfo();
    void LogMemoryInfo();
    void LogCPUInfo();
    
    // Debug utilities
    std::string GetRecentLogs(int lineCount = 100);
    void SaveDebugReport(const std::string& additionalInfo = "");
    void CopyLogsToClipboard();
    
    // Crash handling
    void SetupCrashHandler();
    static void CrashHandler(int signal);
    
#ifdef _WIN32
    static LONG WINAPI WindowsCrashHandler(EXCEPTION_POINTERS* pExceptionInfo);
    void GenerateStackTrace(EXCEPTION_POINTERS* pExceptionInfo = nullptr);
#endif

private:
    std::string GetTimestamp();
    std::string LevelToString(LogLevel level);
    std::string GetSystemInfoString();
    void FlushBuffer();
    void WriteToConsole(LogLevel level, const std::string& message);
    void WriteToFile(const std::string& message);
};

// Convenience macros with file/line info
#define LOG_DEBUG(fmt, ...) Logger::Instance().Debug("[%s:%d] " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) Logger::Instance().Info("[%s:%d] " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)  
#define LOG_WARN(fmt, ...) Logger::Instance().Warn("[%s:%d] " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Logger::Instance().Error("[%s:%d] " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) Logger::Instance().Fatal("[%s:%d] " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

// Scope-based logging for function entry/exit
class ScopeLogger {
    std::string m_function;
    std::chrono::high_resolution_clock::time_point m_start;
public:
    ScopeLogger(const std::string& function) : m_function(function) {
        m_start = std::chrono::high_resolution_clock::now();
        Logger::Instance().Debug("ENTER %s", m_function.c_str());
    }
    
    ~ScopeLogger() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start);
        Logger::Instance().Debug("EXIT %s (took %lld μs)", m_function.c_str(), duration.count());
    }
};

#define LOG_FUNCTION() ScopeLogger _scope_logger(__FUNCTION__)
#define LOG_SCOPE(name) ScopeLogger _scope_logger(name)

// Performance timing macro
#define LOG_TIMING_START(name) auto _timing_##name##_start = std::chrono::high_resolution_clock::now()
#define LOG_TIMING_END(name) do { \
    auto _timing_##name##_end = std::chrono::high_resolution_clock::now(); \
    auto _timing_##name##_duration = std::chrono::duration_cast<std::chrono::microseconds>(_timing_##name##_end - _timing_##name##_start); \
    Logger::Instance().Debug("TIMING %s: %lld μs", #name, _timing_##name##_duration.count()); \
} while(0)

// Error checking macros
#define CHECK_GL_ERROR() do { \
    GLenum err = glGetError(); \
    if (err != GL_NO_ERROR) { \
        Logger::Instance().Error("OpenGL Error 0x%x at %s:%d", err, __FILE__, __LINE__); \
    } \
} while(0)

#ifdef _WIN32
#define CHECK_WIN32_ERROR() do { \
    DWORD err = GetLastError(); \
    if (err != ERROR_SUCCESS) { \
        Logger::Instance().Error("Win32 Error %lu at %s:%d", err, __FILE__, __LINE__); \
    } \
} while(0)
#endif