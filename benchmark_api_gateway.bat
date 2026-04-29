@echo off
REM Comprehensive API Gateway benchmarking script
REM © 2025 D Hargreaves AKA Roylepython | All Rights Reserved

echo.
echo ========================================
echo   API GATEWAY - COMPREHENSIVE BENCHMARKING
echo ========================================
echo.

cd /d "%~dp0"

if not exist build\psiforcedb_license_server.exe (
    echo [✗] Executable not found. Please compile first.
    echo [*] Run: compile_comprehensive.bat
    exit /b 1
)

echo [*] Starting comprehensive API Gateway benchmark...
echo.
echo [*] This will test:
echo     - Request throughput
echo     - Response latency
echo     - Rate limiting
echo     - Load balancing
echo     - Error handling
echo     - Statistics tracking
echo.

REM Create test configuration
echo [*] Creating test configuration...
(
echo {
echo   "database_path": "test_licenses.db.json",
echo   "discord_bot_token": "TEST_TOKEN",
echo   "patreon_access_token": "TEST_TOKEN",
echo   "discord_server_id": "TEST_SERVER",
echo   "patreon_campaign_id": "TEST_CAMPAIGN",
echo   "minimum_patreon_cents": 100,
echo   "api_port": 8443,
echo   "use_ssl": false,
echo   "ssl_cert_path": "",
echo   "ssl_key_path": ""
echo }
) > test_config.json

echo [✓] Test configuration created
echo.

echo [*] To run comprehensive benchmarks:
echo     1. Start server: build\psiforcedb_license_server.exe test_config.json
echo     2. In another terminal, run load tests against http://localhost:8443
echo     3. Monitor API Gateway statistics
echo.

echo [*] Benchmark test endpoints:
echo     - POST http://localhost:8443/api/v1/validate
echo     - POST http://localhost:8443/api/v1/generate
echo     - GET  http://localhost:8443/api/v1/status
echo     - POST http://localhost:8443/api/v1/verify/discord
echo     - POST http://localhost:8443/api/v1/verify/patreon
echo.

echo [*] Comprehensive benchmarking ready.
echo.

