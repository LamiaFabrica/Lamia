// Test lamia.compiler 0.5.0: compile snippet, options, check HTML.
// © 2025 D Hargreaves AKA Roylepython | MIT licensed in this repository.

module;
#include <cstdio>
#include <cstdlib>
#include <new>
#include <string>
#include <string_view>
import lamia.core;
import lamia.compiler;

int main() {
    using namespace lamia;
    using namespace lamia::compiler;

    if (!initialize()) {
        std::fprintf(stderr, "FAIL: initialize()\n");
        return EXIT_FAILURE;
    }

    Compiler c;
    auto r = c.compile("manifest app {}");
    if (!r.is_ok()) {
        std::fprintf(stderr, "FAIL: compile() error %d %s\n", r.error().code, r.error().message);
        shutdown();
        return EXIT_FAILURE;
    }

    std::string const& html = r.value();
    if (html.find("<!DOCTYPE html") == std::string::npos || html.find("lamia-app") == std::string::npos) {
        std::fprintf(stderr, "FAIL: expected <!DOCTYPE html> and lamia-app in output\n");
        shutdown();
        return EXIT_FAILURE;
    }

    // Options: pretty_print off → no leading spaces (optional check)
    CompilerOptions opts;
    opts.pretty_print = false;
    c.set_options(opts);
    r = c.compile("manifest app {}");
    if (r.is_ok() && r.value().find("    ") != std::string::npos) {
        std::fprintf(stderr, "FAIL: pretty_print=false should omit indentation\n");
        shutdown();
        return EXIT_FAILURE;
    }

    shutdown();
    std::fprintf(stdout, "PASS: lamia.compiler test (0.5.0)\n");
    return EXIT_SUCCESS;
}
