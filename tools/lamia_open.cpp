/*
 * Lamia Open — compile .lamia to HTML and open in browser (C API only).
 * © 2025 D Hargreaves AKA Roylepython. MIT licensed in this repository.
 *
 * Usage: lamia_open [--windows] [--chrome|--firefox] <input.lamia> [output.html]
 *   --windows  Write HTML only; do not launch browser (for Windows batch caller).
 *   --chrome   Open with Google Chrome (Linux).
 *   --firefox  Open with Firefox (Linux).
 * If output is omitted (and not --windows), writes to temp and runs xdg-open or chosen browser.
 */

#include "lamia/capi/lamia_capi.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#if defined(_WIN32) || defined(__CYGWIN__)
#define LAMIA_OPEN_IS_WINDOWS 1
#else
#define LAMIA_OPEN_IS_WINDOWS 0
#endif

#if !LAMIA_OPEN_IS_WINDOWS
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

namespace {

constexpr unsigned kMaxFileSize = 1024 * 1024; // 1 MiB

std::string read_file(char const* path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) {
        std::cerr << "lamia_open: cannot open input: " << path << "\n";
        return {};
    }
    f.seekg(0, std::ios::end);
    auto size = f.tellg();
    if (size < 0 || static_cast<unsigned long>(size) > kMaxFileSize) {
        std::cerr << "lamia_open: file too large or invalid: " << path << "\n";
        return {};
    }
    f.seekg(0);
    std::string out(static_cast<size_t>(size), '\0');
    if (!f.read(&out[0], static_cast<std::streamsize>(size))) {
        std::cerr << "lamia_open: read failed: " << path << "\n";
        return {};
    }
    return out;
}

int write_file(char const* path, char const* html) {
    std::ofstream f(path, std::ios::binary);
    if (!f) {
        std::cerr << "lamia_open: cannot write output: " << path << "\n";
        return -1;
    }
    f << html;
    if (!f) return -1;
    return 0;
}

#if !LAMIA_OPEN_IS_WINDOWS
int run_browser(char const* html_path, bool use_chrome, bool use_firefox) {
    std::string cmd;
    if (use_chrome)
        cmd = "google-chrome \"file://" + std::string(html_path) + "\" 2>/dev/null || chromium \"file://" + std::string(html_path) + "\" 2>/dev/null || chromium-browser \"file://" + std::string(html_path) + "\" 2>/dev/null";
    else if (use_firefox)
        cmd = "firefox \"file://" + std::string(html_path) + "\" 2>/dev/null";
    else
        cmd = "xdg-open \"file://" + std::string(html_path) + "\" 2>/dev/null";
    int r = std::system(cmd.c_str());
    (void)r;
    return 0;
}
#endif

} // namespace

int main(int argc, char* argv[]) {
    bool windows_mode = false;
    bool use_chrome = false;
    bool use_firefox = false;
    int idx = 1;
    for (; idx < argc && argv[idx][0] == '-'; ++idx) {
        if (std::strcmp(argv[idx], "--windows") == 0)
            windows_mode = true;
        else if (std::strcmp(argv[idx], "--chrome") == 0)
            use_chrome = true;
        else if (std::strcmp(argv[idx], "--firefox") == 0)
            use_firefox = true;
        else {
            std::cerr << "lamia_open: unknown option: " << argv[idx] << "\n";
            return 1;
        }
    }
    if (idx >= argc) {
        std::cerr << "lamia_open: usage: lamia_open [--windows] [--chrome|--firefox] <input.lamia> [output.html]\n";
        return 1;
    }
char const* input_path = argv[idx++];
    char const* output_path = idx < argc ? argv[idx] : nullptr;

    std::string source = read_file(input_path);
    if (source.empty()) return 1;

    char const* html = lamia_compile_to_html(source.c_str());
    if (!html) {
        std::cerr << "lamia_open: compile failed: " << input_path << "\n";
        return 1;
    }
    std::unique_ptr<char const, decltype(&lamia_free_string)> html_guard(html, lamia_free_string);

    std::string out_path;
    if (output_path) {
        out_path = output_path;
    } else if (windows_mode) {
        std::cerr << "lamia_open: --windows requires output path\n";
        return 1;
    } else {
#if !LAMIA_OPEN_IS_WINDOWS
        char tmp[] = "/tmp/lamia_open_XXXXXX.html";
        int fd = mkstemps(tmp, 5);
        if (fd < 0) {
            std::cerr << "lamia_open: cannot create temp file\n";
            return 1;
        }
        close(fd);
        out_path = tmp;
#else
        std::cerr << "lamia_open: output path required on Windows\n";
        return 1;
#endif
    }

    if (write_file(out_path.c_str(), html_guard.get()) != 0) {
        return 1;
    }

    if (windows_mode) {
        std::cout << out_path << "\n";
        return 0;
    }

#if !LAMIA_OPEN_IS_WINDOWS
    return run_browser(out_path.c_str(), use_chrome, use_firefox);
#else
    (void)use_chrome;
    (void)use_firefox;
    return 0;
#endif
}
