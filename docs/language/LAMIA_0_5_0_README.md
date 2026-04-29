# Lamia 0.5.0 — Public-Ready Build

**© 2025 D Hargreaves AKA Roylepython | All Rights Reserved**

Lamia is a project of love and soul: a language core, compiler, and runtime in C++26.

---

## Quick start

### Build (core only)

```bash
cd v0.5.0/lamia
cmake -B build -S .
cmake --build build
```

**Requirements:** GCC 14+ with C++26 support (`-std=c++2c`, `-fmodules-ts`). WSL or Linux recommended; MSVC does not support C++26 modules yet.

### Run tests

Tests are built by default. Run from the build directory:

```bash
./build/bin/lamia_verify
./build/bin/lamia_compiler_test
./build/bin/lamia_runtime_test
```

### Install

```bash
cmake -B build -S . -DLAMIA_INSTALL=ON
cmake --build build
cmake --install build --prefix /usr/local
```

Libraries go to `lib/`, module interfaces to `include/lamia/`, docs to `share/doc/lamia/`.

---

## Options

| CMake option | Default | Description |
|--------------|---------|-------------|
| `LAMIA_BUILD_TESTS` | ON | Build verification and compiler/runtime tests |
| `LAMIA_INSTALL` | ON | Generate install rules for libraries and headers |
| `LAMIA_BUILD_CAPI` | OFF | Build C API wrapper (`liblamia_capi.so`) for NGINX/Apache; enable with `-DLAMIA_BUILD_CAPI=ON` (build on Linux/WSL if MinGW ICEs) |

---

## Compiler and transpiler options (API)

The compiler and transpiler support options for public use:

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `target` | string | `"html"` | Output target: `html`, `js`, `gcode` |
| `pretty_print` | bool | true | Emit indented, readable output |
| `emit_comments` | bool | false | Emit source-location comments in output |
| `strict` | bool | false | Strict parsing (reject unknown constructs) |

See `docs/OPTIONS.md` for usage and `docs/BUILD.md` for detailed build instructions.

---

## Directory structure (0.5.0)

```
v0.5.0/lamia/
├── CMakeLists.txt      # Public-ready build
├── README.md           # This file
├── lamia.core.cppm     # Core types, Result, Error, MemoryPool, version
├── lamia.compiler.cppm # Lexer, Parser, Transpiler, Compiler + options
├── lamia.runtime.cppm  # ExecutionEngine, VM
├── docs/
│   ├── BUILD.md
│   └── OPTIONS.md
└── tests/
    ├── test_lamia_verify.cpp
    ├── test_lamia_compiler.cpp
    └── test_lamia_runtime.cpp
```

---

## Version

**0.5.0** — Clean directory structure, compiler/transpiler options, install support, public readiness.
