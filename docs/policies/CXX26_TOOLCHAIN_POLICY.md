# C++26 Toolchain Policy

Lamia is a C++26-first project. The canonical build is WSL with GCC 15.2.0 or newer, driven through CMake and Ninja.

## Principle

The project should not freeze itself to one exact compiler build. It should require a proven minimum, probe the active compiler, and move forward when GCC/CMake advance.

## Current Contract

- Minimum compiler: GCC 15.2.0.
- Dialect: C++26 with GNU extensions enabled for the canonical LamiaFabrica build.
- Build system: CMake with central C++26 configuration in `cmake/LamiaCxx26.cmake`.
- Preset: `wsl-gcc15-cxx26`.
- Strict preset: `wsl-gcc15-cxx26-strict`.

## Expansion Rule

Every new Lamia C++ target must link the `lamia_cxx26_contract` interface target and run through `lamia_apply_strict_cxx26(target)`. That keeps compiler flags, warnings, version gates, and future dialect changes in one place.

## GCC/CMake Movement

When GCC changes the spelling or behaviour of C++26 flags, update `cmake/LamiaCxx26.cmake` once. The detector currently probes these dialect flags in order:

- `-std=gnu++26`
- `-std=c++26`
- `-std=gnu++2c`
- `-std=c++2c`

When CMake changes native feature support, keep `CXX_STANDARD 26` for metadata and target modelling, then adjust only the contract module if the explicit flag is no longer needed.

## Verification Commands

Run from WSL inside the repo:

```bash
cmake --preset wsl-gcc15-cxx26
cmake --build --preset wsl-gcc15-cxx26
ctest --preset wsl-gcc15-cxx26
```

For stricter release checks:

```bash
cmake --preset wsl-gcc15-cxx26-strict
cmake --build --preset wsl-gcc15-cxx26-strict
ctest --preset wsl-gcc15-cxx26-strict
```
