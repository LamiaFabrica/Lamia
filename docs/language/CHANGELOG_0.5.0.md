# Changelog — Lamia 0.5.0

**© 2025 D Hargreaves AKA Roylepython | All Rights Reserved**

## 0.5.0 — Public-ready core build

- **Version:** All modules report `0.5.0` (`get_version()` / `get_version_cstr()`).
- **Directory:** Single clean tree under `v0.5.0/lamia/` (core, compiler, runtime, tests, docs).
- **Compiler / Transpiler options:**
  - `CompilerOptions`: `target`, `pretty_print`, `emit_comments`, `strict`.
  - `TranspilerOptions`: `target`, `pretty_print`, `emit_comments`.
  - Backward compatible: `set_target()` still supported.
- **Build:** CMake 3.16+, GCC 14+ with C++26 and module TS. Options: `LAMIA_BUILD_TESTS`, `LAMIA_INSTALL`.
- **Install:** Libraries to `lib/`, `.cppm` to `include/lamia/`, docs to `share/doc/lamia/`.
- **Tests:** `lamia_verify`, `lamia_compiler_test`, `lamia_runtime_test` in `build/bin/`.

This release focuses on core build public readiness and the new options the compiler and transpiler allow; component libraries (analytics, quantum, etc.) remain in v0.4.1 and can be integrated in a later 0.5.x release.
