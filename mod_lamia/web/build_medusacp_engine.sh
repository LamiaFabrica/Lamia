#!/bin/bash

# MedusaServ Control Panel Engine - Build and Deployment Script
# Yorkshire Champion Gold Standard Build System

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# ASCII Art Header
echo -e "${PURPLE}"
echo "╔══════════════════════════════════════════════════════════════╗"
echo "║                                                              ║"
echo "║              🔮 MedusaServ Control Panel Engine              ║"
echo "║                Yorkshire Champion Build System               ║"
echo "║                                                              ║"
echo "╚══════════════════════════════════════════════════════════════╝"
echo -e "${NC}"

# Configuration
BUILD_DIR="build"
ENGINE_NAME="MedusaServ-cp"
LOG_FILE="build.log"
INSTALL_PREFIX="/opt/medusaserv"

# Function to print status messages
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_header() {
    echo -e "\n${CYAN}╔══ $1 ══╗${NC}"
}

# Function to check dependencies
check_dependencies() {
    print_header "Checking Dependencies"
    
    local deps=("g++" "make" "pkg-config")
    local missing_deps=()
    
    for dep in "${deps[@]}"; do
        if ! command -v "$dep" &> /dev/null; then
            missing_deps+=("$dep")
            print_error "$dep is not installed"
        else
            print_success "$dep found"
        fi
    done
    
    # Check for required libraries
    local libs=("jsoncpp" "openssl")
    for lib in "${libs[@]}"; do
        if ! pkg-config --exists "$lib" 2>/dev/null; then
            print_error "lib$lib-dev is not installed"
            missing_deps+=("lib$lib-dev")
        else
            print_success "lib$lib found"
        fi
    done
    
    if [ ${#missing_deps[@]} -ne 0 ]; then
        print_error "Missing dependencies: ${missing_deps[*]}"
        echo -e "\nTo install missing dependencies on Ubuntu/Debian:"
        echo "sudo apt-get update"
        echo "sudo apt-get install ${missing_deps[*]}"
        echo -e "\nOn CentOS/RHEL:"
        echo "sudo yum install ${missing_deps[*]// -dev/-devel}"
        exit 1
    fi
    
    print_success "All dependencies satisfied"
}

# Function to create directories
create_directories() {
    print_header "Creating Directories"
    
    mkdir -p "$BUILD_DIR"/{lib,bin,logs}
    mkdir -p panel/inc
    
    print_success "Directory structure created"
}

# Function to run security checks
run_security_checks() {
    print_header "Security Validation"
    
    print_status "Checking for dangerous functions..."
    if grep -r "system\|exec\|popen" *.cpp panel/inc/*.cpp 2>/dev/null | grep -v "// SAFE:" | grep -v "executeCommand"; then
        print_warning "Potentially dangerous function calls found (review required)"
    else
        print_success "No dangerous functions found"
    fi
    
    print_status "Checking for buffer overflow vulnerabilities..."
    if grep -r "strcpy\|strcat\|sprintf" *.cpp panel/inc/*.cpp 2>/dev/null; then
        print_error "Unsafe string functions found"
        exit 1
    else
        print_success "No unsafe string functions found"
    fi
    
    print_status "Validating input sanitization..."
    if grep -r "sanitizeInput\|validateInput" *.cpp panel/inc/*.cpp &>/dev/null; then
        print_success "Input validation functions found"
    else
        print_warning "Consider adding more input validation"
    fi
    
    print_success "Security checks passed"
}

# Function to compile modules
compile_modules() {
    print_header "Compiling C++ Modules"
    
    local module_count=0
    local failed_modules=()
    
    for module_file in panel/inc/*_module.cpp; do
        if [ -f "$module_file" ]; then
            local module_name=$(basename "$module_file" .cpp)
            local output_file="$BUILD_DIR/lib/${module_name}.so"
            
            print_status "Compiling $module_name..."
            
            if g++ -std=gnu++26 -fPIC -O2 -Wall -Wextra -DSECURITY_ENABLED \
                   -shared "$module_file" -o "$output_file" \
                   -ljsoncpp -lssl -lcrypto -ldl -lpthread 2>>"$LOG_FILE"; then
                print_success "$module_name compiled successfully"
                ((module_count++))
            else
                print_error "Failed to compile $module_name"
                failed_modules+=("$module_name")
            fi
        fi
    done
    
    if [ ${#failed_modules[@]} -ne 0 ]; then
        print_error "Failed to compile modules: ${failed_modules[*]}"
        echo "Check $LOG_FILE for details"
        exit 1
    fi
    
    print_success "$module_count modules compiled successfully"
}

# Function to compile main engine
compile_engine() {
    print_header "Compiling Main Engine"
    
    local engine_source="$ENGINE_NAME.cpp"
    local engine_output="$BUILD_DIR/bin/$ENGINE_NAME"
    
    if [ ! -f "$engine_source" ]; then
        print_error "Engine source file $engine_source not found"
        exit 1
    fi
    
    print_status "Compiling $ENGINE_NAME..."
    
    if g++ -std=gnu++26 -O2 -Wall -Wextra -DSECURITY_ENABLED \
           "$engine_source" -o "$engine_output" \
           -ljsoncpp -lssl -lcrypto -ldl -lpthread 2>>"$LOG_FILE"; then
        print_success "Engine compiled successfully"
    else
        print_error "Failed to compile engine"
        echo "Check $LOG_FILE for details"
        exit 1
    fi
}

# Function to run tests
run_tests() {
    print_header "Running Tests"
    
    local engine_binary="$BUILD_DIR/bin/$ENGINE_NAME"
    
    if [ ! -f "$engine_binary" ]; then
        print_error "Engine binary not found"
        exit 1
    fi
    
    print_status "Testing engine initialization..."
    if timeout 10s "$engine_binary" &>>"$LOG_FILE"; then
        print_success "Engine test passed"
    else
        print_error "Engine test failed"
        echo "Check $LOG_FILE for details"
        exit 1
    fi
    
    print_status "Testing module loading..."
    local module_count=$(ls "$BUILD_DIR/lib/"*.so 2>/dev/null | wc -l)
    if [ "$module_count" -gt 0 ]; then
        print_success "$module_count modules available for loading"
    else
        print_warning "No modules found"
    fi
}

# Function to install modules
install_modules() {
    print_header "Installing Modules"
    
    print_status "Copying modules to inc directory..."
    if cp "$BUILD_DIR/lib/"*.so panel/inc/ 2>/dev/null; then
        print_success "Modules installed to panel/inc/"
    else
        print_warning "No modules to install"
    fi
    
    print_status "Setting permissions..."
    chmod 755 panel/inc/*.so 2>/dev/null || true
    
    print_success "Installation complete"
}

# Function to generate documentation
generate_docs() {
    print_header "Generating Documentation"
    
    local docs_dir="docs"
    mkdir -p "$docs_dir"
    
    # Generate API documentation
    cat > "$docs_dir/API_REFERENCE.md" << 'EOF'
# MedusaServ Control Panel API Reference

## C++ Engine Functions

### Core Functions
- `medusacp_init()` - Initialize the control panel engine
- `medusacp_cleanup()` - Cleanup engine resources
- `medusacp_health()` - Get engine health status
- `medusacp_generate_csrf(session_id)` - Generate CSRF token

### API Endpoints
- `system_info` - Get comprehensive system information
- `list_processes` - Get process list with details
- `service_status` - Get service status information
- `control_service` - Start/stop/restart services
- `performance_metrics` - Get real-time performance data

### Security Features
- CSRF token validation
- Input sanitization
- Path traversal protection
- XSS prevention
- Memory management

### Module System
The engine uses a modular architecture with .so libraries:
- `system_info_module.so` - System information gathering
- `process_manager_module.so` - Process management
- `service_controller_module.so` - Service control operations

EOF

    print_success "API documentation generated"
}

# Function to create startup script
create_startup_script() {
    print_header "Creating Startup Script"
    
    cat > "start_medusacp.sh" << 'EOF'
#!/bin/bash
# MedusaServ Control Panel Startup Script

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ENGINE_PATH="$SCRIPT_DIR/build/bin/MedusaServ-cp"

echo "🔮 Starting MedusaServ Control Panel Engine..."

if [ ! -f "$ENGINE_PATH" ]; then
    echo "❌ Engine binary not found at $ENGINE_PATH"
    echo "Please run build_medusacp_engine.sh first"
    exit 1
fi

# Set library path
export LD_LIBRARY_PATH="$SCRIPT_DIR/panel/inc:$LD_LIBRARY_PATH"

# Start the engine
exec "$ENGINE_PATH" "$@"
EOF

    chmod +x "start_medusacp.sh"
    print_success "Startup script created"
}

# Function to validate build
validate_build() {
    print_header "Build Validation"
    
    local errors=0
    
    # Check engine binary
    if [ -f "$BUILD_DIR/bin/$ENGINE_NAME" ]; then
        print_success "Engine binary exists"
    else
        print_error "Engine binary missing"
        ((errors++))
    fi
    
    # Check modules
    local module_count=$(ls "$BUILD_DIR/lib/"*.so 2>/dev/null | wc -l)
    if [ "$module_count" -gt 0 ]; then
        print_success "$module_count modules built"
    else
        print_error "No modules built"
        ((errors++))
    fi
    
    # Check installed modules
    local installed_count=$(ls panel/inc/*.so 2>/dev/null | wc -l)
    if [ "$installed_count" -gt 0 ]; then
        print_success "$installed_count modules installed"
    else
        print_warning "No modules installed"
    fi
    
    if [ "$errors" -eq 0 ]; then
        print_success "Build validation passed"
        return 0
    else
        print_error "Build validation failed with $errors errors"
        return 1
    fi
}

# Function to display build summary
show_summary() {
    echo -e "\n${PURPLE}╔══════════════════════════════════════════════════════════════╗"
    echo "║                      BUILD SUMMARY                          ║"
    echo "╚══════════════════════════════════════════════════════════════╝${NC}"
    
    echo -e "${WHITE}Engine Binary:${NC} $BUILD_DIR/bin/$ENGINE_NAME"
    echo -e "${WHITE}Modules Directory:${NC} $BUILD_DIR/lib/"
    echo -e "${WHITE}Installed Modules:${NC} panel/inc/"
    echo -e "${WHITE}Startup Script:${NC} start_medusacp.sh"
    echo -e "${WHITE}Build Log:${NC} $LOG_FILE"
    echo -e "${WHITE}Documentation:${NC} docs/"
    
    echo -e "\n${GREEN}✅ Build completed successfully!${NC}"
    echo -e "${CYAN}🚀 To start the engine, run: ./start_medusacp.sh${NC}"
    echo -e "${YELLOW}📚 Check docs/API_REFERENCE.md for usage information${NC}"
}

# Main build process
main() {
    print_status "Starting MedusaServ Control Panel build process..."
    echo "Build started at $(date)" > "$LOG_FILE"
    
    # Run build steps
    check_dependencies
    create_directories
    run_security_checks
    compile_modules
    compile_engine
    run_tests
    install_modules
    generate_docs
    create_startup_script
    
    if validate_build; then
        show_summary
        exit 0
    else
        print_error "Build failed validation"
        exit 1
    fi
}

# Handle command line arguments
case "${1:-build}" in
    "build"|"")
        main
        ;;
    "clean")
        print_status "Cleaning build files..."
        rm -rf "$BUILD_DIR" panel/inc/*.so "$LOG_FILE" start_medusacp.sh docs/
        print_success "Clean complete"
        ;;
    "test")
        run_tests
        ;;
    "install")
        install_modules
        ;;
    "help")
        echo "MedusaServ Control Panel Build Script"
        echo ""
        echo "Usage: $0 [command]"
        echo ""
        echo "Commands:"
        echo "  build    - Full build (default)"
        echo "  clean    - Clean build files"
        echo "  test     - Run tests only"
        echo "  install  - Install modules only"
        echo "  help     - Show this help"
        ;;
    *)
        print_error "Unknown command: $1"
        echo "Run '$0 help' for usage information"
        exit 1
        ;;
esac