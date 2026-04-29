# PowerShell script to compile, validate, test, and prepare for deployment
# © 2025 D Hargreaves AKA Roylepython | All Rights Reserved

$ErrorActionPreference = "Stop"

Write-Host "╔═══════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║         LICENSING SERVER - COMPILE & TEST SCRIPT              ║" -ForegroundColor Cyan
Write-Host "╚═══════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
Write-Host ""

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptDir

# Configuration
$buildDir = Join-Path $scriptDir "build"
$exeName = "psiforcedb_license_server.exe"
$exePath = Join-Path $buildDir $exeName

# Create build directory
if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
    Write-Host "[✓] Created build directory: $buildDir" -ForegroundColor Green
}

# Check for compiler
Write-Host "[*] Checking for g++ compiler..." -ForegroundColor Yellow
$gpp = Get-Command g++ -ErrorAction SilentlyContinue
if (-not $gpp) {
    Write-Host "[✗] Error: g++ not found in PATH" -ForegroundColor Red
    Write-Host "    Please install MinGW-w64 and add it to your PATH" -ForegroundColor Yellow
    exit 1
}
Write-Host "[✓] Found compiler: $($gpp.Source)" -ForegroundColor Green

# Compile
Write-Host ""
Write-Host "[*] Compiling licensing server..." -ForegroundColor Yellow
Write-Host ""

$sourceFiles = @(
    "psiforce_license_server_main.cpp",
    "psiforce_licensing_server.cpp",
    "medusa_discord_integration.cpp",
    "medusa_discord_native_websocket.cpp",
    "medusa_native_http_client.cpp",
    "medusa_patreon_integration.cpp",
    "medusa_stall_preventer_agent.cpp",
    "medusa_logger_core.cpp"
)

$cxxFlags = "-std=c++2b -O2 -Wall -Wextra"
$includes = "-I. -I./include"
$libs = "-lws2_32 -liphlpapi -lssl -lcrypto -static"

$compileCmd = "g++ $cxxFlags -o `"$exePath`" $($sourceFiles -join ' ') $includes $libs"

Write-Host "Command: $compileCmd" -ForegroundColor Gray
Write-Host ""

try {
    Invoke-Expression $compileCmd
    if ($LASTEXITCODE -ne 0) {
        Write-Host "[✗] Compilation failed with exit code: $LASTEXITCODE" -ForegroundColor Red
        exit 1
    }
    Write-Host ""
    Write-Host "[✓] Compilation successful!" -ForegroundColor Green
} catch {
    Write-Host "[✗] Compilation error: $_" -ForegroundColor Red
    exit 1
}

# Validate executable exists
Write-Host ""
Write-Host "[*] Validating executable..." -ForegroundColor Yellow
if (-not (Test-Path $exePath)) {
    Write-Host "[✗] Error: Executable not found: $exePath" -ForegroundColor Red
    exit 1
}

$fileInfo = Get-Item $exePath
Write-Host "[✓] Executable found: $exePath" -ForegroundColor Green
Write-Host "    Size: $([math]::Round($fileInfo.Length / 1MB, 2)) MB" -ForegroundColor Gray
Write-Host "    Created: $($fileInfo.CreationTime)" -ForegroundColor Gray

# Test executable (dry run - check if it starts and shows help/version)
Write-Host ""
Write-Host "[*] Testing executable..." -ForegroundColor Yellow

# Create test config if it doesn't exist
$testConfig = Join-Path $scriptDir "license_server_config.json"
if (-not (Test-Path $testConfig)) {
    Write-Host "[*] Creating test configuration file..." -ForegroundColor Yellow
    $configContent = @{
        database_path = "licenses.db.json"
        discord_bot_token = "TEST_TOKEN_PLACEHOLDER"
        patreon_access_token = "TEST_TOKEN_PLACEHOLDER"
        discord_server_id = "TEST_SERVER_ID"
        patreon_campaign_id = "TEST_CAMPAIGN_ID"
        minimum_patreon_cents = 100
        api_port = 8443
        use_ssl = $false
        ssl_cert_path = ""
        ssl_key_path = ""
    } | ConvertTo-Json -Depth 10
    
    $configContent | Out-File -FilePath $testConfig -Encoding UTF8
    Write-Host "[✓] Test configuration created" -ForegroundColor Green
}

# Try to run executable with timeout (should fail gracefully with config error)
Write-Host "[*] Running executable validation test..." -ForegroundColor Yellow
$process = Start-Process -FilePath $exePath -ArgumentList $testConfig -NoNewWindow -PassThru -RedirectStandardOutput "$buildDir\test_output.txt" -RedirectStandardError "$buildDir\test_error.txt"
Start-Sleep -Seconds 2
if (-not $process.HasExited) {
    Stop-Process -Id $process.Id -Force -ErrorAction SilentlyContinue
}

if (Test-Path "$buildDir\test_output.txt") {
    $output = Get-Content "$buildDir\test_output.txt" -Raw
    if ($output -match "LICENSING SERVER|PSIFORCEDB|Discord|Patreon") {
        Write-Host "[✓] Executable runs and shows expected output" -ForegroundColor Green
    } else {
        Write-Host "[!] Executable runs but output is unexpected" -ForegroundColor Yellow
    }
}

# Deployment preparation
Write-Host ""
Write-Host "[*] Preparing for deployment..." -ForegroundColor Yellow

$deployDir = Join-Path $scriptDir "deploy"
if (-not (Test-Path $deployDir)) {
    New-Item -ItemType Directory -Path $deployDir | Out-Null
}

# Copy executable
Copy-Item $exePath (Join-Path $deployDir $exeName) -Force
Write-Host "[✓] Copied executable to deploy directory" -ForegroundColor Green

# Create deployment package info
$deployInfo = @{
    executable = $exeName
    version = "1.0.0"
    build_date = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    compiler = "g++"
    cpp_standard = "C++2b"
    libraries = @("OpenSSL", "nlohmann/json", "Windows Sockets")
} | ConvertTo-Json -Depth 10

$deployInfo | Out-File -FilePath (Join-Path $deployDir "deployment_info.json") -Encoding UTF8
Write-Host "[✓] Created deployment info" -ForegroundColor Green

# Summary
Write-Host ""
Write-Host "╔═══════════════════════════════════════════════════════════════╗" -ForegroundColor Green
Write-Host "║                    BUILD SUCCESSFUL!                         ║" -ForegroundColor Green
Write-Host "╚═══════════════════════════════════════════════════════════════╝" -ForegroundColor Green
Write-Host ""
Write-Host "Executable: $exePath" -ForegroundColor Cyan
Write-Host "Deploy Directory: $deployDir" -ForegroundColor Cyan
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. Configure license_server_config.json with real API tokens" -ForegroundColor White
Write-Host "  2. Run: .\build\psiforcedb_license_server.exe" -ForegroundColor White
Write-Host "  3. Deploy files from 'deploy' directory to production server" -ForegroundColor White
Write-Host ""

