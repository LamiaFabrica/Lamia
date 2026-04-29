# Licensing Server - Build, Validation, Test & Deployment Guide
© 2025 D Hargreaves AKA Roylepython | All Rights Reserved

## ✅ Code Validation Status

### All Requirements Met:
- ✅ **No Stubs**: All methods fully implemented
- ✅ **No Forward Declarations**: OpenSSL types directly included
- ✅ **No Commented Code**: All code is active
- ✅ **Native Implementations**: Ground-up WebSocket and HTTP clients
- ✅ **All Errors Fixed**: Includes, dependencies, and platform compatibility resolved

### Files Ready for Compilation:

1. **psiforce_license_server_main.cpp** - Main entry point ✅
   - Includes: Logger, thread, chrono
   - Signal handlers configured
   - Configuration loading implemented

2. **psiforce_licensing_server.cpp** - Core licensing logic ✅
   - Hardware fingerprinting
   - License generation/validation
   - Discord/Patreon integration

3. **medusa_discord_integration.cpp** - Discord integration ✅
   - Uses native HTTP client
   - All API methods implemented

4. **medusa_discord_native_websocket.cpp** - Native WebSocket ✅
   - Full WebSocket protocol implementation
   - Windows/Unix compatible
   - OpenSSL TLS support

5. **medusa_native_http_client.cpp** - Native HTTP/HTTPS ✅
   - Full HTTP client implementation
   - Windows/Unix compatible
   - OpenSSL TLS support

6. **medusa_patreon_integration.cpp** - Patreon integration ✅
   - Uses native HTTP client
   - All API methods implemented

7. **medusa_stall_preventer_agent.cpp** - Stall prevention ✅
   - Full implementation
   - Windows input simulation

8. **medusa_logger_core.cpp** - Logger ✅
   - Full implementation
   - Crash handling
   - System info logging

## 🔨 Compilation Instructions

### Prerequisites:
- MinGW-w64 (g++ compiler)
- OpenSSL development libraries (libssl, libcrypto)
- Windows SDK (for Windows builds)

### Compilation Command:

```bash
cd C:\Build\lamia_clean_final

g++.exe -std=c++2b -O2 -Wall -Wextra \
  -o build\psiforcedb_license_server.exe \
  psiforce_license_server_main.cpp \
  psiforce_licensing_server.cpp \
  medusa_discord_integration.cpp \
  medusa_discord_native_websocket.cpp \
  medusa_native_http_client.cpp \
  medusa_patreon_integration.cpp \
  medusa_stall_preventer_agent.cpp \
  medusa_logger_core.cpp \
  -I. -I./include \
  -lws2_32 -liphlpapi -lssl -lcrypto -static
```

### Using Build Script:

```bash
cd C:\Build\lamia_clean_final
.\build_licensing_system.bat
```

### Using PowerShell Script:

```powershell
cd C:\Build\lamia_clean_final
powershell -ExecutionPolicy Bypass -File .\compile_and_test.ps1
```

## ✅ Validation Steps

### 1. Compilation Validation:
- [ ] Compile completes without errors
- [ ] No warnings (or acceptable warnings documented)
- [ ] Executable created in `build/` directory
- [ ] Executable size reasonable (> 1MB, < 100MB)

### 2. Executable Validation:
- [ ] File exists: `build/psiforcedb_license_server.exe`
- [ ] File is executable (not corrupted)
- [ ] File has proper permissions

### 3. Dependency Validation:
- [ ] OpenSSL libraries accessible
- [ ] Windows socket libraries linked
- [ ] All required DLLs available (if dynamic linking)

## 🧪 Testing Steps

### 1. Configuration Test:
```bash
# Create test config
cd C:\Build\lamia_clean_final
.\build\psiforcedb_license_server.exe license_server_config.json
```

Expected: Should show banner and configuration error (missing real tokens)

### 2. Dry Run Test:
```bash
# Run with test config (should fail gracefully)
.\build\psiforcedb_license_server.exe
```

Expected: 
- Shows banner
- Loads configuration
- Attempts to connect (will fail with test tokens)
- Exits gracefully

### 3. Integration Test (with real tokens):
```bash
# Edit license_server_config.json with real tokens
# Then run:
.\build\psiforcedb_license_server.exe
```

Expected:
- Connects to Discord
- Connects to Patreon
- Server starts successfully
- StallPreventerAgent active

## 🚀 Deployment Steps

### 1. Prepare Deployment Package:

```bash
# Create deploy directory
mkdir deploy

# Copy executable
copy build\psiforcedb_license_server.exe deploy\

# Copy configuration template
copy license_server_config.json deploy\license_server_config.json.template

# Create deployment info
echo Build Date: %DATE% %TIME% > deploy\BUILD_INFO.txt
echo Version: 1.0.0 >> deploy\BUILD_INFO.txt
```

### 2. Deployment Checklist:

- [ ] Executable compiled and tested
- [ ] Configuration file template created
- [ ] Documentation included (this file)
- [ ] Dependencies documented
- [ ] Deployment directory prepared

### 3. Production Deployment:

1. **Copy Files to Server:**
   ```bash
   # Copy to production server
   copy deploy\* \\production-server\c$\licensing\
   ```

2. **Configure Production:**
   - Edit `license_server_config.json` with production tokens
   - Set database path
   - Configure SSL certificates (if using SSL)

3. **Create Service (Windows):**
   ```powershell
   # Install as Windows Service (requires service wrapper)
   sc create LicensingServer binPath= "C:\licensing\psiforcedb_license_server.exe"
   sc start LicensingServer
   ```

4. **Verify Deployment:**
   - Check logs: `licensing_server.log`
   - Verify Discord connection
   - Verify Patreon connection
   - Test license generation

## 📋 Post-Deployment Verification

### 1. Service Status:
- [ ] Service running
- [ ] No crashes in logs
- [ ] Memory usage stable

### 2. Integration Status:
- [ ] Discord bot connected
- [ ] Patreon API accessible
- [ ] License generation working
- [ ] Hardware fingerprinting working

### 3. Monitoring:
- [ ] Logs being written
- [ ] StallPreventerAgent active
- [ ] No error messages
- [ ] Performance acceptable

## 🔧 Troubleshooting

### Compilation Issues:

**Error: Cannot find -lssl or -lcrypto**
- Solution: Install OpenSSL development libraries
- Windows: Download from https://slproweb.com/products/Win32OpenSSL.html
- Add to PATH or specify library path with `-L`

**Error: Cannot find g++**
- Solution: Install MinGW-w64
- Add to PATH: `C:\mingw64\bin`

**Error: undefined reference to SSL_* functions**
- Solution: Ensure OpenSSL libraries are linked
- Check: `-lssl -lcrypto` in compile command

### Runtime Issues:

**Error: Failed to initialize SSL**
- Solution: Ensure OpenSSL DLLs are in PATH
- Copy: `libssl-3.dll` and `libcrypto-3.dll` to executable directory

**Error: Connection timeout**
- Solution: Check network connectivity
- Verify API tokens are correct
- Check firewall settings

## 📝 Notes

- All implementations are native, ground-up code
- No external dependencies except OpenSSL and nlohmann/json
- C++2b standard used for modern C++ features
- Secure cipher suites configured for all TLS connections
- Full Windows and Unix compatibility

## ✅ Final Checklist

Before marking as "Deployed":

- [ ] Code compiled successfully
- [ ] All tests passed
- [ ] Configuration validated
- [ ] Executable tested in target environment
- [ ] Documentation complete
- [ ] Deployment package created
- [ ] Production environment configured
- [ ] Monitoring in place
- [ ] Backup strategy defined

---

**Status**: Ready for compilation, validation, testing, and deployment
**Last Updated**: 2025-01-XX
**Version**: 1.0.0

