#include "lamia/runtime/shared_library.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

namespace {

int fail(const std::string& message)
{
    std::cerr << "FAILED: " << message << '\n';
    const int failure_code = 1;
    return failure_code;
}

} // namespace

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: lamia_shared_library_tests <sample-plugin-path>\n";
        const int usage_error = 2;
        return usage_error;
    }

    try {
        const std::filesystem::path plugin_path = std::filesystem::canonical(argv[1]);
        const std::filesystem::path plugin_root = plugin_path.parent_path();

        lamia::runtime::shared_library_locator locator;
        locator.add_root("test", plugin_root);
        locator.map_library("sample", "test", plugin_path.filename());

        auto descriptors = locator.descriptors();
        if (descriptors.size() != 1U || descriptors.front().logical_name != "sample") {
            return fail("locator did not preserve the logical sample mapping");
        }

        auto resolved = locator.resolve("sample");
        if (resolved != plugin_path) {
            return fail("locator resolved a different plugin path than the canonical sample plugin");
        }

        auto library = locator.load("sample");
        if (!library.is_loaded()) {
            return fail("sample plugin did not report as loaded");
        }

        using add_function = int (*)(int, int);
        auto add = library.symbol<add_function>("lamia_sample_add");
        const int answer = add(19, 23);
        if (answer != 42) {
            return fail("sample add symbol returned an unexpected result");
        }

        using identity_function = const char* (*)();
        auto identity = library.symbol<identity_function>("lamia_sample_identity");
        const std::string identity_value(identity());
        if (identity_value != "lamia-runtime-shared-library-test-plugin") {
            return fail("sample identity symbol returned an unexpected string");
        }

        bool missing_symbol_threw = false;
        try {
            auto missing = library.raw_symbol("lamia_symbol_that_is_not_exported");
            if (missing != nullptr) {
                return fail("missing symbol unexpectedly produced a valid address");
            }
        } catch (const lamia::runtime::shared_library_error&) {
            missing_symbol_threw = true;
        }
        if (!missing_symbol_threw) {
            return fail("missing symbol lookup did not throw");
        }

        auto moved = std::move(library);
        if (library.is_loaded()) {
            return fail("moved-from library retained ownership");
        }
        if (!moved.is_loaded()) {
            return fail("moved library lost ownership");
        }

        moved.close();
        if (moved.is_loaded()) {
            return fail("closed library still reports as loaded");
        }

        bool escape_blocked = false;
        try {
            auto escaped = locator.open_path("../outside-library");
            if (escaped.is_loaded()) {
                return fail("escaped relative path loaded unexpectedly");
            }
        } catch (const lamia::runtime::shared_library_error&) {
            escape_blocked = true;
        }
        if (!escape_blocked) {
            return fail("relative path escape was not rejected");
        }

        bool absolute_blocked = false;
        try {
            auto absolute = locator.open_path(plugin_path);
            if (absolute.is_loaded()) {
                return fail("absolute load succeeded while absolute loading was disabled");
            }
        } catch (const lamia::runtime::shared_library_error&) {
            absolute_blocked = true;
        }
        if (!absolute_blocked) {
            return fail("absolute path load was not rejected");
        }

        lamia::runtime::shared_library_options absolute_options;
        absolute_options.allow_absolute_paths = true;
        auto absolute_library = locator.open_path(plugin_path, absolute_options);
        if (!absolute_library.is_loaded()) {
            return fail("absolute load under a registered root did not succeed");
        }
        absolute_library.close();

    } catch (const std::exception& exception) {
        return fail(exception.what());
    }

    std::cout << "lamia shared library runtime tests passed\n";
    const int success = EXIT_SUCCESS;
    return success;
}
