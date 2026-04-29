# Lamia 0.5.0 — Build Instructions

**© 2025 D Hargreaves AKA Roylepython | All Rights Reserved**

## Requirements

- **Compiler:** GCC 14+ with C++26 and module TS support
- **CMake:** 3.16 or later
- **Platform:** Linux or WSL recommended; Windows (MSVC) does not yet support C++26 modules

## Configure and build

From the `v0.5.0/lamia` directory:

```bash
cmake -B build -S .
cmake --build build
```

**Windows (GCC + C++ modules):** Use Unix Makefiles so module build order and `gcm.cache` work correctly. Ninja can hit “inputs may not also have inputs” with module dyndep. Example (MSYS2 ucrt64):

```bash
cmake -B build -S . -G "Unix Makefiles" \
  -DCMAKE_CXX_COMPILER=C:/msys64-2/ucrt64/bin/g++.exe \
  -DCMAKE_MAKE_PROGRAM=C:/msys64-2/usr/bin/make.exe
make -C build -j1
```

Libraries are produced in `build/`: `liblamia_core.a`, `liblamia_compiler.a`, `liblamia_runtime.a` (or `.so` on Linux with `BUILD_SHARED_LIBS=ON`).

## Options

| Option | Default | Description |
|--------|---------|-------------|
| `LAMIA_BUILD_TESTS` | ON | Build verification and compiler/runtime tests |
| `LAMIA_INSTALL` | ON | Add install rules for libraries and module interfaces |
| `LAMIA_BUILD_CAPI` | OFF | Build C API shared library for NGINX/Apache (`liblamia_capi.so`). On GCC 15.2 MinGW the C API build may hit an internal compiler error; use Linux/WSL or a different compiler. |

Examples:

```bash
# Build without tests
cmake -B build -S . -DLAMIA_BUILD_TESTS=OFF

# Build with install rules, then install
cmake -B build -S . -DLAMIA_INSTALL=ON
cmake --build build
cmake --install build --prefix /usr/local
```

## Run tests

After building with tests enabled:

```bash
./build/bin/lamia_verify
./build/bin/lamia_compiler_test
./build/bin/lamia_runtime_test
```

## Install layout

With `-DLAMIA_INSTALL=ON` and `cmake --install build --prefix <prefix>`:

- `<prefix>/lib/` — liblamia_core, liblamia_compiler, liblamia_runtime
- `<prefix>/include/lamia/` — lamia.core.cppm, lamia.compiler.cppm, lamia.runtime.cppm
- `<prefix>/share/doc/lamia/` — BUILD.md, OPTIONS.md

## Public readiness

Lamia 0.5.0 is structured for public use: clean directory, versioned API, compiler/transpiler options, tests and install by default.
