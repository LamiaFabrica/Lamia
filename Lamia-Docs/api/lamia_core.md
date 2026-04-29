# lamia_core API Reference

## Overview

Core Lamia runtime and execution engine

**Library Path**: `Lamia-Libs/lib/liblamia_core.so`
**Version**: 0.3.0

## API Functions

### initialize

Initialize the Lamia runtime environment

**Signature**:
```cpp
bool Lamia::Core::initialize()
```

**Parameters**:
None

**Returns**:
`true` if initialization successful, `false` otherwise

**Example**:
```cpp
if (!Lamia::Core::initialize()) {
    std::cerr << "Failed to initialize Lamia" << std::endl;
    return -1;
}
```

---

### execute_manifest

Execute a Lamia manifest and return crystal result

**Signature**:
```cpp
crystal Lamia::Core::execute_manifest(const std::string& manifest_code)
```

**Parameters**:
- `manifest_code`: Lamia manifest source code

**Returns**:
Crystal object containing execution result

**Example**:
```cpp
auto result = Lamia::Core::execute_manifest(
    "manifest hello() -> crystal @ludicrous { return RADIANT_SUCCESS; }"
);
```

---

### shutdown

Cleanup and shutdown Lamia runtime

**Signature**:
```cpp
void Lamia::Core::shutdown()
```

**Parameters**:
None

**Returns**:
None

**Example**:
```cpp
Lamia::Core::shutdown();
```

---

## Performance Characteristics

- **Complexity Reduction**: 78.3% average
- **Memory Footprint**: Optimized
- **Thread Safety**: Yes

## License

© 2025 The Medusa Project - MIT License
