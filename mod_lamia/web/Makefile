# MedusaServ Control Panel Engine - Makefile
# Yorkshire Champion Gold Standard Build System

CXX = g++
CXXFLAGS = -std=gnu++26 -fPIC -O2 -Wall -Wextra -Werror -DSECURITY_ENABLED
LDFLAGS = -shared
LIBS = -ljsoncpp -lssl -lcrypto -ldl -lpthread

# Directories
INC_DIR = panel/inc
BUILD_DIR = build
LIB_DIR = $(BUILD_DIR)/lib

# Source files for modules
MODULE_SOURCES = $(wildcard $(INC_DIR)/*_module.cpp)
MODULE_TARGETS = $(patsubst $(INC_DIR)/%.cpp,$(LIB_DIR)/%.so,$(MODULE_SOURCES))

# Main engine
ENGINE_SOURCE = MedusaServ-cp.cpp
ENGINE_TARGET = $(BUILD_DIR)/MedusaServ-cp

# Default target
all: directories $(MODULE_TARGETS) $(ENGINE_TARGET)

# Create directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(INC_DIR)

# Compile modules
$(LIB_DIR)/%.so: $(INC_DIR)/%.cpp
	@echo "🔧 Compiling module: $<"
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@ $(LIBS)
	@echo "✅ Module compiled: $@"

# Compile main engine
$(ENGINE_TARGET): $(ENGINE_SOURCE)
	@echo "🔮 Compiling MedusaServ Control Panel Engine..."
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)
	@echo "✨ Engine compiled: $@"

# Install modules to inc directory
install: all
	@echo "📦 Installing modules to inc directory..."
	@cp -f $(LIB_DIR)/*.so $(INC_DIR)/
	@echo "✅ Installation complete!"

# Test the engine
test: all
	@echo "🧪 Testing MedusaServ Control Panel Engine..."
	@$(ENGINE_TARGET)
	@echo "✅ Engine test complete!"

# Clean build files
clean:
	@echo "🧹 Cleaning build files..."
	@rm -rf $(BUILD_DIR)
	@rm -f $(INC_DIR)/*.so
	@echo "✅ Clean complete!"

# Security check
security-check:
	@echo "🔒 Running security checks..."
	@echo "Checking for dangerous functions..."
	@grep -n "system\|exec\|popen" $(ENGINE_SOURCE) $(MODULE_SOURCES) || echo "✅ No dangerous functions found"
	@echo "Checking for buffer overflows..."
	@grep -n "strcpy\|strcat\|sprintf" $(ENGINE_SOURCE) $(MODULE_SOURCES) || echo "✅ No unsafe string functions found"
	@echo "✅ Security check complete!"

# Code quality check
quality-check:
	@echo "📊 Running code quality checks..."
	@cppcheck --enable=all --std=gnu++26 $(ENGINE_SOURCE) $(MODULE_SOURCES) 2>/dev/null || echo "cppcheck not available"
	@echo "✅ Quality check complete!"

# Full build with checks
full-build: security-check quality-check all test install
	@echo "🏆 Full build complete with Yorkshire Champion standards!"

# Development build (faster, less checks)
dev: directories $(MODULE_TARGETS) $(ENGINE_TARGET)
	@echo "🚀 Development build complete!"

# Production build (optimized)
production: CXXFLAGS += -DNDEBUG -O3 -march=native
production: clean full-build
	@echo "🏭 Production build complete!"

# Help
help:
	@echo "MedusaServ Control Panel Engine - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  all           - Build all modules and engine"
	@echo "  dev           - Fast development build"
	@echo "  production    - Optimized production build"
	@echo "  install       - Install modules to inc directory"
	@echo "  test          - Test the engine"
	@echo "  clean         - Clean build files"
	@echo "  security-check - Run security analysis"
	@echo "  quality-check  - Run code quality checks"
	@echo "  full-build    - Complete build with all checks"
	@echo "  help          - Show this help"

.PHONY: all directories install test clean security-check quality-check full-build dev production help

# Module-specific targets for development
system-info: $(LIB_DIR)/system_info_module.so
process-manager: $(LIB_DIR)/process_manager_module.so
service-controller: $(LIB_DIR)/service_controller_module.so

.PHONY: system-info process-manager service-controller