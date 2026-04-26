include(CheckCXXCompilerFlag)

set(LAMIA_MINIMUM_GNU_VERSION "15.2.0" CACHE STRING "Minimum supported GCC version for Lamia C++26 builds")
option(LAMIA_REQUIRE_GCC_TOOLCHAIN "Require GCC for the canonical Lamia C++26 build" ON)
option(LAMIA_TREAT_WARNINGS_AS_ERRORS "Promote Lamia compiler warnings to errors" OFF)

function(lamia_detect_cxx26_flag out_variable)
    set(candidate_flags
        "-std=gnu++26"
        "-std=c++26"
        "-std=gnu++2c"
        "-std=c++2c"
    )

    foreach(candidate_flag IN LISTS candidate_flags)
        string(MAKE_C_IDENTIFIER "${candidate_flag}" flag_identifier)
        set(check_name "LAMIA_COMPILER_ACCEPTS_${flag_identifier}")
        check_cxx_compiler_flag("${candidate_flag}" "${check_name}")
        if(${check_name})
            set(${out_variable} "${candidate_flag}" PARENT_SCOPE)
            return()
        endif()
    endforeach()

    message(FATAL_ERROR
        "The active C++ compiler does not accept a C++26 dialect flag. "
        "Tried: ${candidate_flags}. "
        "Install or select GCC ${LAMIA_MINIMUM_GNU_VERSION}+ for the canonical Lamia build."
    )
endfunction()

function(lamia_configure_cxx26_toolchain)
    if(NOT CMAKE_CXX_COMPILER_LOADED)
        message(FATAL_ERROR "Lamia requires CXX to be enabled before configuring the C++26 toolchain")
    endif()

    if(LAMIA_REQUIRE_GCC_TOOLCHAIN AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(FATAL_ERROR
            "Lamia's canonical build requires GCC ${LAMIA_MINIMUM_GNU_VERSION}+ under WSL. "
            "Active compiler is ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}. "
            "Set LAMIA_REQUIRE_GCC_TOOLCHAIN=OFF only for an explicit compatibility experiment."
        )
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
       AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS LAMIA_MINIMUM_GNU_VERSION)
        message(FATAL_ERROR
            "Lamia requires GCC ${LAMIA_MINIMUM_GNU_VERSION}+ for C++26. "
            "Active compiler is GCC ${CMAKE_CXX_COMPILER_VERSION}."
        )
    endif()

    lamia_detect_cxx26_flag(detected_flag)
    set(LAMIA_CXX26_DIALECT_FLAG "${detected_flag}" CACHE STRING "Detected C++26 dialect flag for Lamia targets" FORCE)

    set(CMAKE_CXX_STANDARD 26 PARENT_SCOPE)
    set(CMAKE_CXX_STANDARD_REQUIRED ON PARENT_SCOPE)
    set(CMAKE_CXX_EXTENSIONS ON PARENT_SCOPE)

    message(STATUS "Lamia C++26 compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
    message(STATUS "Lamia C++26 dialect flag: ${LAMIA_CXX26_DIALECT_FLAG}")
    message(STATUS "Lamia C++26 minimum GCC: ${LAMIA_MINIMUM_GNU_VERSION}")
endfunction()

function(lamia_create_cxx26_contract target_name)
    if(TARGET "${target_name}")
        message(FATAL_ERROR "C++26 contract target '${target_name}' already exists")
    endif()

    if(NOT LAMIA_CXX26_DIALECT_FLAG)
        message(FATAL_ERROR "lamia_configure_cxx26_toolchain must run before creating a C++26 contract")
    endif()

    add_library("${target_name}" INTERFACE)
    target_compile_features("${target_name}" INTERFACE cxx_std_26)
    target_compile_options("${target_name}" INTERFACE
        "$<$<COMPILE_LANGUAGE:CXX>:${LAMIA_CXX26_DIALECT_FLAG}>"
    )

    set_target_properties("${target_name}" PROPERTIES
        INTERFACE_LAMIA_CXX26_DIALECT_FLAG "${LAMIA_CXX26_DIALECT_FLAG}"
    )
endfunction()

function(lamia_apply_strict_cxx26 target_name)
    if(NOT TARGET "${target_name}")
        message(FATAL_ERROR "Cannot apply Lamia C++26 rules to missing target '${target_name}'")
    endif()

    set_target_properties("${target_name}" PROPERTIES
        CXX_STANDARD 26
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS ON
    )

    if(MSVC)
        target_compile_options("${target_name}" PRIVATE /W4 /permissive-)
        if(LAMIA_TREAT_WARNINGS_AS_ERRORS)
            target_compile_options("${target_name}" PRIVATE /WX)
        endif()
    else()
        target_compile_options("${target_name}" PRIVATE -Wall -Wextra -Wpedantic)
        if(LAMIA_TREAT_WARNINGS_AS_ERRORS)
            target_compile_options("${target_name}" PRIVATE -Werror)
        endif()
    endif()
endfunction()
