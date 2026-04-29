@echo off
REM Comprehensive compilation script for licensing server
REM © 2025 D Hargreaves AKA Roylepython | All Rights Reserved

echo.
echo ========================================
echo   LICENSING SERVER - COMPREHENSIVE COMPILATION
echo ========================================
echo.

cd /d "%~dp0"

if not exist build mkdir build

echo [*] Compiling all source files...
echo.

REM Compile each source file individually to catch errors
echo [*] Compiling psiforce_license_server_main.cpp...
g++.exe -std=c++2b -O2 -Wall -Wextra -I. -I./include -c psiforce_license_server_main.cpp -o build/main.o 2>&1
if errorlevel 1 (
    echo [✗] Failed to compile main.cpp
    exit /b 1
)

echo [*] Compiling psiforce_licensing_server.cpp...
g++.exe -std=c++2b -O2 -Wall -Wextra -I. -I./include -c psiforce_licensing_server.cpp -o build/licensing_server.o 2>&1
if errorlevel 1 (
    echo [✗] Failed to compile licensing_server.cpp
    exit /b 1
)

echo [*] Compiling medusa_discord_integration.cpp...
g++.exe -std=c++2b -O2 -Wall -Wextra -I. -I./include -c medusa_discord_integration.cpp -o build/discord_integration.o 2>&1
if errorlevel 1 (
    echo [✗] Failed to compile discord_integration.cpp
    exit /b 1
)

echo [*] Compiling medusa_discord_native_websocket.cpp...
g++.exe -std=c++2b -O2 -Wall -Wextra -I. -I./include -c medusa_discord_native_websocket.cpp -o build/discord_websocket.o 2>&1
if errorlevel 1 (
    echo [✗] Failed to compile discord_websocket.cpp
    exit /b 1
)

echo [*] Compiling medusa_native_http_client.cpp...
g++.exe -std=c++2b -O2 -Wall -Wextra -I. -I./include -c medusa_native_http_client.cpp -o build/http_client.o 2>&1
if errorlevel 1 (
    echo [✗] Failed to compile http_client.cpp
    exit /b 1
)

echo [*] Compiling medusa_patreon_integration.cpp...
g++.exe -std=c++2b -O2 -Wall -Wextra -I. -I./include -c medusa_patreon_integration.cpp -o build/patreon_integration.o 2>&1
if errorlevel 1 (
    echo [✗] Failed to compile patreon_integration.cpp
    exit /b 1
)

echo [*] Compiling medusa_stall_preventer_agent.cpp...
g++.exe -std=c++2b -O2 -Wall -Wextra -I. -I./include -c medusa_stall_preventer_agent.cpp -o build/stall_preventer.o 2>&1
if errorlevel 1 (
    echo [✗] Failed to compile stall_preventer.cpp
    exit /b 1
)

echo [*] Compiling medusa_logger_core.cpp...
g++.exe -std=c++2b -O2 -Wall -Wextra -I. -I./include -c medusa_logger_core.cpp -o build/logger.o 2>&1
if errorlevel 1 (
    echo [✗] Failed to compile logger.cpp
    exit /b 1
)

echo [*] Compiling medusa_api_gateway.cpp...
g++.exe -std=c++2b -O2 -Wall -Wextra -I. -I./include -c medusa_api_gateway.cpp -o build/api_gateway.o 2>&1
if errorlevel 1 (
    echo [✗] Failed to compile api_gateway.cpp
    exit /b 1
)

echo [*] Linking all object files...
g++.exe -std=c++2b -O2 -Wall -Wextra ^
  -o build\psiforcedb_license_server.exe ^
  build\main.o ^
  build\licensing_server.o ^
  build\discord_integration.o ^
  build\discord_websocket.o ^
  build\http_client.o ^
  build\patreon_integration.o ^
  build\stall_preventer.o ^
  build\logger.o ^
  build\api_gateway.o ^
  -lws2_32 -liphlpapi -lssl -lcrypto -static 2>&1

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [✓] Comprehensive compilation successful!
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
    echo [✗] Comprehensive compilation failed with error code: %ERRORLEVEL%
    echo.
    exit /b 1
)

