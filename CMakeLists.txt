cmake_minimum_required(VERSION 3.25)

project(Lamia
    VERSION 0.5.0
    DESCRIPTION "LamiaFabrica C++26 language runtime and bridge layer"
    LANGUAGES CXX
)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(LamiaCxx26)

lamia_configure_cxx26_toolchain()
lamia_create_cxx26_contract(lamia_cxx26_contract)

option(LAMIA_BUILD_TESTS "Build Lamia runtime tests" ON)
option(LAMIA_BUILD_CAPI "Build the public Lamia C ABI wrapper for server/module consumers" ON)
option(LAMIA_BUILD_TOOLS "Build Lamia command-line tools" ON)

set(LAMIA_LANGUAGE_MODULE_OPTIONS
    -fmodules-ts
    -Wno-global-module
)

set(LAMIA_LANGUAGE_MODULES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/language/lamia.core.cppm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/language/lamia.compiler.cppm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/language/lamia.runtime.cppm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/capi/lamia_capi.cppm
)

set_source_files_properties(${LAMIA_LANGUAGE_MODULES}
    PROPERTIES
        COMPILE_OPTIONS "-x;c++"
)

add_library(lamia_platform_runtime STATIC
    src/runtime/shared_library.cpp
)

target_include_directories(lamia_platform_runtime
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(NOT WIN32)
    target_link_libraries(lamia_platform_runtime PUBLIC dl)
endif()

target_link_libraries(lamia_platform_runtime PUBLIC lamia_cxx26_contract)
lamia_apply_strict_cxx26(lamia_platform_runtime)

add_library(lamia_core STATIC
    src/language/lamia.core.cppm
)
target_include_directories(lamia_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/language)
target_link_libraries(lamia_core PUBLIC lamia_cxx26_contract)
target_compile_options(lamia_core PRIVATE ${LAMIA_LANGUAGE_MODULE_OPTIONS})
lamia_apply_strict_cxx26(lamia_core)

add_library(lamia_compiler STATIC
    src/language/lamia.compiler.cppm
)
target_include_directories(lamia_compiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/language)
target_link_libraries(lamia_compiler PUBLIC lamia_core lamia_cxx26_contract)
target_compile_options(lamia_compiler PRIVATE ${LAMIA_LANGUAGE_MODULE_OPTIONS})
lamia_apply_strict_cxx26(lamia_compiler)

add_library(lamia_language_runtime STATIC
    src/language/lamia.runtime.cppm
)
target_include_directories(lamia_language_runtime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/language)
target_link_libraries(lamia_language_runtime PUBLIC lamia_core lamia_cxx26_contract)
target_compile_options(lamia_language_runtime PRIVATE ${LAMIA_LANGUAGE_MODULE_OPTIONS})
lamia_apply_strict_cxx26(lamia_language_runtime)

if(LAMIA_BUILD_CAPI)
    set_target_properties(lamia_core lamia_compiler PROPERTIES POSITION_INDEPENDENT_CODE ON)
    add_library(lamia_capi SHARED
        src/capi/lamia_capi.cppm
    )
    target_include_directories(lamia_capi
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/language
    )
    target_link_libraries(lamia_capi PRIVATE lamia_compiler lamia_core lamia_cxx26_contract)
    target_compile_options(lamia_capi PRIVATE ${LAMIA_LANGUAGE_MODULE_OPTIONS})
    set_target_properties(lamia_capi PROPERTIES
        OUTPUT_NAME lamia_capi
        VERSION ${PROJECT_VERSION}
        SOVERSION 0
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    lamia_apply_strict_cxx26(lamia_capi)
endif()

if(LAMIA_BUILD_TOOLS AND LAMIA_BUILD_CAPI)
    add_executable(lamia_open
        tools/lamia_open.cpp
    )
    target_include_directories(lamia_open PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(lamia_open PRIVATE lamia_capi lamia_cxx26_contract)
    set_target_properties(lamia_open PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
    if(UNIX AND NOT APPLE)
        target_link_libraries(lamia_open PRIVATE dl)
    endif()
    lamia_apply_strict_cxx26(lamia_open)
endif()

if(LAMIA_BUILD_TESTS)
    enable_testing()

    add_library(lamia_runtime_test_plugin SHARED
        tests/runtime/sample_plugin.cpp
    )
    target_link_libraries(lamia_runtime_test_plugin PRIVATE lamia_cxx26_contract)
    lamia_apply_strict_cxx26(lamia_runtime_test_plugin)

    add_executable(lamia_shared_library_tests
        tests/runtime/test_shared_library.cpp
    )

    target_link_libraries(lamia_shared_library_tests PRIVATE lamia_platform_runtime)
    lamia_apply_strict_cxx26(lamia_shared_library_tests)

    add_test(NAME lamia_shared_library_tests
        COMMAND lamia_shared_library_tests $<TARGET_FILE:lamia_runtime_test_plugin>
    )

    add_executable(lamia_core_tests
        tests/language/test_lamia_verify.cpp
    )
    target_link_libraries(lamia_core_tests PRIVATE lamia_core lamia_cxx26_contract)
    target_compile_options(lamia_core_tests PRIVATE ${LAMIA_LANGUAGE_MODULE_OPTIONS})
    lamia_apply_strict_cxx26(lamia_core_tests)
    add_test(NAME lamia_core_tests COMMAND lamia_core_tests)

    add_executable(lamia_compiler_tests
        tests/language/test_lamia_compiler.cpp
    )
    target_link_libraries(lamia_compiler_tests PRIVATE lamia_compiler lamia_core lamia_cxx26_contract)
    target_compile_options(lamia_compiler_tests PRIVATE ${LAMIA_LANGUAGE_MODULE_OPTIONS})
    lamia_apply_strict_cxx26(lamia_compiler_tests)
    add_test(NAME lamia_compiler_tests COMMAND lamia_compiler_tests)

    add_executable(lamia_language_runtime_tests
        tests/language/test_lamia_runtime.cpp
    )
    target_link_libraries(lamia_language_runtime_tests PRIVATE lamia_language_runtime lamia_core lamia_cxx26_contract)
    target_compile_options(lamia_language_runtime_tests PRIVATE ${LAMIA_LANGUAGE_MODULE_OPTIONS})
    lamia_apply_strict_cxx26(lamia_language_runtime_tests)
    add_test(NAME lamia_language_runtime_tests COMMAND lamia_language_runtime_tests)
endif()
