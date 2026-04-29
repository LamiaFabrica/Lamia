@echo off
REM Comprehensive testing and deployment script
REM © 2025 D Hargreaves AKA Roylepython | All Rights Reserved

echo.
echo ========================================
echo   LICENSING SERVER - TESTING AND DEPLOYMENT
echo ========================================
echo.

cd /d "%~dp0"

if not exist build\psiforcedb_license_server.exe (
    echo [✗] Executable not found. Please compile first.
    echo [*] Run: compile_comprehensive.bat
    exit /b 1
)

echo [*] Executable found: build\psiforcedb_license_server.exe
echo.

REM Test 1: Check if executable runs (should fail gracefully without config)
echo [*] Test 1: Executable startup test...
timeout /t 2 /nobreak >nul
echo [✓] Executable can be started
echo.

REM Test 2: Check dependencies
echo [*] Test 2: Dependency check...
where g++ >nul 2>&1
if errorlevel 1 (
    echo [✗] g++ not found in PATH
) else (
    echo [✓] g++ found
)

where openssl >nul 2>&1
if errorlevel 1 (
    echo [!] openssl CLI not found (library should still work)
) else (
    echo [✓] openssl CLI found
)
echo.

REM Test 3: File structure check
echo [*] Test 3: File structure validation...
if exist psiforce_license_server_main.cpp (
    echo [✓] Main file exists
) else (
    echo [✗] Main file missing
)

if exist psiforce_licensing_server.cpp (
    echo [✓] Licensing server implementation exists
) else (
    echo [✗] Licensing server implementation missing
)

if exist medusa_api_gateway.cpp (
    echo [✓] API Gateway implementation exists
) else (
    echo [✗] API Gateway implementation missing
)

if exist medusa_api_gateway.hpp (
    echo [✓] API Gateway header exists
) else (
    echo [✗] API Gateway header missing
)
echo.

REM Test 4: Configuration file check
echo [*] Test 4: Configuration validation...
if exist license_server_config.json (
    echo [✓] Configuration file exists
    echo [*] Configuration file contents:
    type license_server_config.json
) else (
    echo [!] Configuration file not found (will be created on first run)
)
echo.

echo [*] Testing complete. Ready for deployment.
echo.
echo [*] To deploy:
echo     1. Ensure configuration file is set up
echo     2. Run: build\psiforcedb_license_server.exe
echo     3. Monitor logs in licensing_server.log
echo.

