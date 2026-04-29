/*
 * Lamia C API — public ABI for NGINX/Apache/MedusaServ-style module consumers.
 * © 2025 D Hargreaves AKA Roylepython. MIT licensed in this repository.
 */

module;
#include <cstring>
#include <cstdlib>
#include <string>
#include <string_view>

export module lamia.capi;

import lamia.core;
import lamia.compiler;

namespace {
bool g_initialized = false;
}

extern "C" {

const char* lamia_compile_to_html(const char* lamia_source) {
    if (!lamia_source) return nullptr;
    if (!g_initialized && !lamia::initialize()) return nullptr;
    g_initialized = true;
    auto r = lamia::compiler::compile_source(std::string_view(lamia_source));
    if (!r.is_ok()) return nullptr;
    std::string out = r.value();
    char* copy = static_cast<char*>(std::malloc(out.size() + 1));
    if (!copy) return nullptr;
    std::memcpy(copy, out.data(), out.size());
    copy[out.size()] = '\0';
    return copy;
}

void lamia_free_string(const char* s) {
    if (s) std::free(const_cast<char*>(s));
}

int lamia_compile_to_html_buf(const char* lamia_source, char* out_buf, unsigned out_size) {
    if (!lamia_source || !out_buf || out_size == 0) return 0;
    if (!g_initialized && !lamia::initialize()) return 0;
    g_initialized = true;
    auto r = lamia::compiler::compile_source(std::string_view(lamia_source));
    if (!r.is_ok()) return 0;
    std::string const& out = r.value();
    if (out.size() >= out_size) return 0;
    std::memcpy(out_buf, out.data(), out.size());
    out_buf[out.size()] = '\0';
    return 1;
}

} // extern "C"
