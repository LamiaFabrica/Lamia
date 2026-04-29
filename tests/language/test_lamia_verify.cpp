// Lamia 0.5.0 core verification — imports lamia.core only.
// © 2025 D Hargreaves AKA Roylepython | MIT licensed in this repository.

module;
#include <cstdio>
#include <cstdlib>
#include <new>
import lamia.core;

int main() {
    using namespace lamia;

    if (!initialize()) {
        std::fprintf(stderr, "FAIL: initialize() returned false\n");
        return EXIT_FAILURE;
    }

    char const* ver = get_version_cstr();
    if (!ver || ver[0] == '\0') {
        std::fprintf(stderr, "FAIL: get_version_cstr() empty\n");
        return EXIT_FAILURE;
    }
    std::fprintf(stdout, "lamia.core version: %s\n", ver);

    auto r = Result<int, Error>::ok(42);
    if (!r.is_ok() || r.value() != 42) {
        std::fprintf(stderr, "FAIL: Result::ok(42)\n");
        return EXIT_FAILURE;
    }

    auto err = Result<int, Error>::err(Error(1, "test"));
    if (!err.is_err() || err.error().code != 1) {
        std::fprintf(stderr, "FAIL: Result::err\n");
        return EXIT_FAILURE;
    }

    MemoryPool pool(64, 8);
    if (pool.block_size() != 64 || pool.block_count() != 8 || pool.used() != 0) {
        std::fprintf(stderr, "FAIL: MemoryPool ctor\n");
        return EXIT_FAILURE;
    }
    void* p = pool.allocate();
    if (!p || pool.used() != 1) {
        std::fprintf(stderr, "FAIL: MemoryPool::allocate\n");
        return EXIT_FAILURE;
    }
    pool.deallocate(p);
    if (pool.used() != 0) {
        std::fprintf(stderr, "FAIL: MemoryPool::deallocate\n");
        return EXIT_FAILURE;
    }

    shutdown();
    std::fprintf(stdout, "PASS: lamia.core verification (0.5.0)\n");
    return EXIT_SUCCESS;
}
