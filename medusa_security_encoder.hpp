#pragma once
#include <string>
#include <vector>
#include <cstdint>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Logger.hpp"

// Revolutionary Security Encoder for DLL Protection
class SecurityEncoder {
public:
    // XOR-based encoding with dynamic key generation
    static std::vector<uint8_t> EncodeBuffer(const std::vector<uint8_t>& data, const std::string& key);
    static std::vector<uint8_t> DecodeBuffer(const std::vector<uint8_t>& data, const std::string& key);
    
    // Generate unique key based on system characteristics
    static std::string GenerateSystemKey();
    
    // Validate DLL integrity
    static bool ValidateDLLIntegrity(const std::string& dllPath);
    
    // Encode DLL file
    static bool EncodeDLL(const std::string& inputPath, const std::string& outputPath, const std::string& key);
    
    // Decode and load DLL
#ifdef _WIN32
    static HMODULE LoadEncodedDLL(const std::string& encodedPath, const std::string& key);
#endif

private:
    static constexpr size_t KEY_EXPANSION_SIZE = 256;
    static std::vector<uint8_t> ExpandKey(const std::string& key, size_t targetSize);
#ifdef _WIN32
    static HMODULE LoadLibraryFromMemory(const void* data, size_t size);
#endif
};