cmake_minimum_required(VERSION 3.25)

project(Lamia
    VERSION 0.4.0
    DESCRIPTION "LamiaFabrica C++26 language runtime and bridge layer"
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(LamiaCxx26)

lamia_configure_cxx26_toolchain()
lamia_create_cxx26_contract(lamia_cxx26_contract)

option(LAMIA_BUILD_TESTS "Build Lamia runtime tests" ON)

add_library(lamia_runtime STATIC
    src/runtime/shared_library.cpp
)

target_include_directories(lamia_runtime
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(NOT WIN32)
    target_link_libraries(lamia_runtime PUBLIC dl)
endif()

target_link_libraries(lamia_runtime PUBLIC lamia_cxx26_contract)
lamia_apply_strict_cxx26(lamia_runtime)

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

    target_link_libraries(lamia_shared_library_tests PRIVATE lamia_runtime)
    lamia_apply_strict_cxx26(lamia_shared_library_tests)

    add_test(NAME lamia_shared_library_tests
        COMMAND lamia_shared_library_tests $<TARGET_FILE:lamia_runtime_test_plugin>
    )
endif()
