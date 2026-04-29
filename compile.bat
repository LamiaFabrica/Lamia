@echo off
REM Simple compilation script for licensing server
REM © 2025 D Hargreaves AKA Roylepython | All Rights Reserved

echo.
echo ========================================
echo   LICENSING SERVER - COMPILATION
echo ========================================
echo.

cd /d "%~dp0"

if not exist build mkdir build

echo [*] Compiling...
echo.

g++.exe -std=c++2b -O2 -Wall -Wextra ^
  -o build\psiforcedb_license_server.exe ^
  psiforce_license_server_main.cpp ^
  psiforce_licensing_server.cpp ^
  medusa_discord_integration.cpp ^
  medusa_discord_native_websocket.cpp ^
  medusa_native_http_client.cpp ^
  medusa_patreon_integration.cpp ^
  medusa_stall_preventer_agent.cpp ^
  medusa_logger_core.cpp ^
  medusa_api_gateway.cpp ^
  -I. -I./include ^
  -lws2_32 -liphlpapi -lssl -lcrypto -static

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [✓] Compilation successful!
    echo [*] Executable: build\psiforcedb_license_server.exe
    echo.
    if exist build\psiforcedb_license_server.exe (
        dir build\psiforcedb_license_server.exe
        echo.
        echo [*] To test, run:
        echo     build\psiforcedb_license_server.exe
        echo.
    )
) else (
    echo.
    echo [✗] Compilation failed with error code: %ERRORLEVEL%
    echo.
    exit /b 1
)

