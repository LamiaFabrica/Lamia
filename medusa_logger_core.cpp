#include "medusa_logger_core.hpp"
#include <cstdarg>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#include <dbghelp.h>
#else
#include <signal.h>
#include <unistd.h>
#endif

std::unique_ptr<Logger> Logger::s_instance;
std::mutex Logger::s_mutex;

Logger::Logger() = default;
Logger::~Logger() {
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

Logger& Logger::Instance() {
    std::lock_guard<std::mutex> lock(s_mutex);
    if (!s_instance) {
        s_instance.reset(new Logger());
    }
    return *s_instance;
}

void Logger::Initialize(const std::string& logFile) {
    Instance().m_logFilePath = logFile;
    Instance().m_logFile.open(logFile, std::ios::out | std::ios::app);
}

void Logger::Shutdown() {
    std::lock_guard<std::mutex> lock(s_mutex);
    s_instance.reset();
}

void Logger::Log(LogLevel level, const std::string& message) {
    if (level < m_minLevel) return;

    std::lock_guard<std::mutex> lock(m_logMutex);
    std::string timestamp = GetTimestamp();
    std::string levelStr = LevelToString(level);
    std::string finalMessage = timestamp + " [" + levelStr + "] " + message;

    if (m_consoleOutput) {
        WriteToConsole(level, finalMessage);
    }
    if (m_fileOutput && m_logFile.is_open()) {
        WriteToFile(finalMessage);
    }
    
    m_logBuffer.push_back(finalMessage);
    if (m_logBuffer.size() > 1000) {
        m_logBuffer.erase(m_logBuffer.begin());
    }
}

void Logger::Log(LogLevel level, const char* format, ...) {
    if (level < m_minLevel) return;

    char buffer[4096];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Log(level, std::string(buffer));
}

std::string Logger::GetTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Logger::LevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG_LEVEL: return "DEBUG";
        case LogLevel::INFO_LEVEL: return "INFO";
        case LogLevel::WARN_LEVEL: return "WARN";
        case LogLevel::ERROR_LEVEL: return "ERROR";
        case LogLevel::FATAL_LEVEL: return "FATAL";
        default: return "UNKNOWN";
    }
}

void Logger::WriteToConsole(LogLevel level, const std::string& message) {
    // Simple color output for windows console if needed, or just cout
    if (level >= LogLevel::ERROR_LEVEL) {
        std::cerr << message << std::endl;
    } else {
        std::cout << message << std::endl;
    }
}

void Logger::WriteToFile(const std::string& message) {
    m_logFile << message << std::endl;
}

// Full implementations for system info and debug utilities
void Logger::LogSystemInfo() {
#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    OSVERSIONINFO osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osInfo);
    
    std::ostringstream info;
    info << "System Info - OS: Windows " << osInfo.dwMajorVersion << "." << osInfo.dwMinorVersion
         << " | Processors: " << sysInfo.dwNumberOfProcessors
         << " | Page Size: " << sysInfo.dwPageSize << " bytes";
    Log(LogLevel::INFO_LEVEL, info.str());
#else
    std::ifstream os_release("/etc/os-release");
    std::string line;
    std::ostringstream info;
    info << "System Info - ";
    if (os_release.is_open()) {
        while (std::getline(os_release, line)) {
            if (line.find("PRETTY_NAME") == 0) {
                info << line.substr(line.find('=') + 1);
                break;
            }
        }
    }
    Log(LogLevel::INFO_LEVEL, info.str());
#endif
}

void Logger::LogOpenGLInfo() {
    Log(LogLevel::INFO_LEVEL, "OpenGL Info: Not available in server context");
}

void Logger::LogMemoryInfo() {
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    
    std::ostringstream info;
    info << "Memory Info - Total: " << (memInfo.ullTotalPhys / (1024 * 1024)) << " MB"
         << " | Available: " << (memInfo.ullAvailPhys / (1024 * 1024)) << " MB"
         << " | Used: " << ((memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024 * 1024)) << " MB";
    Log(LogLevel::INFO_LEVEL, info.str());
#else
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    std::ostringstream info;
    info << "Memory Info - ";
    if (meminfo.is_open()) {
        std::getline(meminfo, line);
        info << line;
    }
    Log(LogLevel::INFO_LEVEL, info.str());
#endif
}

void Logger::LogCPUInfo() {
#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    std::ostringstream info;
    info << "CPU Info - Architecture: " << sysInfo.wProcessorArchitecture
         << " | Processors: " << sysInfo.dwNumberOfProcessors;
    Log(LogLevel::INFO_LEVEL, info.str());
#else
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    std::ostringstream info;
    info << "CPU Info - ";
    if (cpuinfo.is_open()) {
        while (std::getline(cpuinfo, line)) {
            if (line.find("model name") == 0) {
                info << line.substr(line.find(':') + 1);
                break;
            }
        }
    }
    Log(LogLevel::INFO_LEVEL, info.str());
#endif
}

std::string Logger::GetRecentLogs(int lineCount) {
    std::lock_guard<std::mutex> lock(m_logMutex);
    std::ostringstream result;
    
    size_t start = (m_logBuffer.size() > static_cast<size_t>(lineCount)) 
                   ? m_logBuffer.size() - lineCount : 0;
    
    for (size_t i = start; i < m_logBuffer.size(); i++) {
        result << m_logBuffer[i] << "\n";
    }
    
    return result.str();
}

void Logger::SaveDebugReport(const std::string& additionalInfo) {
    std::lock_guard<std::mutex> lock(m_logMutex);
    std::ostringstream report;
    
    report << "=== DEBUG REPORT ===\n";
    report << "Timestamp: " << GetTimestamp() << "\n";
    report << "Log File: " << m_logFilePath << "\n";
    report << "Recent Logs:\n" << GetRecentLogs(100) << "\n";
    
    if (!additionalInfo.empty()) {
        report << "Additional Info:\n" << additionalInfo << "\n";
    }
    
    report << "=== END REPORT ===\n";
    
    std::string report_file = m_logFilePath + ".debug";
    std::ofstream debug_file(report_file, std::ios::app);
    if (debug_file.is_open()) {
        debug_file << report.str();
        debug_file.close();
        Log(LogLevel::INFO_LEVEL, "Debug report saved to: " + report_file);
    }
}

void Logger::CopyLogsToClipboard() {
#ifdef _WIN32
    std::string logs = GetRecentLogs(1000);
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, logs.length() + 1);
        if (hMem) {
            char* pMem = static_cast<char*>(GlobalLock(hMem));
            strcpy(pMem, logs.c_str());
            GlobalUnlock(hMem);
            SetClipboardData(CF_TEXT, hMem);
        }
        CloseClipboard();
        Log(LogLevel::INFO_LEVEL, "Logs copied to clipboard");
    }
#else
    Log(LogLevel::WARN_LEVEL, "Clipboard functionality not available on this platform");
#endif
}

void Logger::SetupCrashHandler() {
#ifdef _WIN32
    SetUnhandledExceptionFilter(WindowsCrashHandler);
#else
    signal(SIGSEGV, CrashHandler);
    signal(SIGABRT, CrashHandler);
    signal(SIGFPE, CrashHandler);
    signal(SIGILL, CrashHandler);
#endif
    Log(LogLevel::INFO_LEVEL, "Crash handler installed");
}

void Logger::CrashHandler(int signal) {
    Logger::Instance().Fatal("Crash detected - Signal: " + std::to_string(signal));
    Logger::Instance().SaveDebugReport("Crash signal: " + std::to_string(signal));
    exit(1);
}

#ifdef _WIN32
LONG WINAPI Logger::WindowsCrashHandler(EXCEPTION_POINTERS* pExceptionInfo) {
    Logger::Instance().Fatal("Windows crash detected");
    Logger::Instance().GenerateStackTrace(pExceptionInfo);
    Logger::Instance().SaveDebugReport("Windows exception occurred");
    return EXCEPTION_EXECUTE_HANDLER;
}

void Logger::GenerateStackTrace(EXCEPTION_POINTERS* pExceptionInfo) {
    if (!pExceptionInfo) return;
    
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();
    
    SymInitialize(process, NULL, TRUE);
    
    STACKFRAME64 stackFrame;
    memset(&stackFrame, 0, sizeof(STACKFRAME64));
    
#ifdef _M_AMD64
    stackFrame.AddrPC.Offset = pExceptionInfo->ContextRecord->Rip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = pExceptionInfo->ContextRecord->Rbp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = pExceptionInfo->ContextRecord->Rsp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#else
    stackFrame.AddrPC.Offset = pExceptionInfo->ContextRecord->Eip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = pExceptionInfo->ContextRecord->Ebp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = pExceptionInfo->ContextRecord->Esp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#endif
    
    std::ostringstream stackTrace;
    stackTrace << "Stack Trace:\n";
    
    for (int i = 0; i < 64; i++) {
        if (!StackWalk64(
#ifdef _M_AMD64
            IMAGE_FILE_MACHINE_AMD64,
#else
            IMAGE_FILE_MACHINE_I386,
#endif
            process, thread, &stackFrame, pExceptionInfo->ContextRecord,
            NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL)) {
            break;
        }
        
        char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char)];
        PSYMBOL_INFO symbol = (PSYMBOL_INFO)symbolBuffer;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYM_NAME;
        
        DWORD64 displacement = 0;
        if (SymFromAddr(process, stackFrame.AddrPC.Offset, &displacement, symbol)) {
            stackTrace << "  " << i << ": " << symbol->Name << " (+0x" 
                      << std::hex << displacement << std::dec << ")\n";
        }
    }
    
    Log(LogLevel::FATAL_LEVEL, stackTrace.str());
    SymCleanup(process);
}
#endif

std::string Logger::GetSystemInfoString() {
    std::ostringstream info;
#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    OSVERSIONINFO osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osInfo);
    
    info << "Windows " << osInfo.dwMajorVersion << "." << osInfo.dwMinorVersion
         << " | " << sysInfo.dwNumberOfProcessors << " processors";
#else
    std::ifstream os_release("/etc/os-release");
    if (os_release.is_open()) {
        std::string line;
        while (std::getline(os_release, line)) {
            if (line.find("PRETTY_NAME") == 0) {
                info << line.substr(line.find('=') + 1);
                break;
            }
        }
    }
#endif
    return info.str();
}
void Logger::FlushBuffer() { 
    if (m_logFile.is_open()) m_logFile.flush(); 
}


