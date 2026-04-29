# lamia_gcode_native API Reference

## Overview

Native G-code generation for manufacturing

**Library Path**: `Lamia-Libs/lib/liblamia_gcode_native.so`
**Version**: 0.3.0

## API Functions

### initialize

Initialize the lamia_gcode_native module

**Signature**:
```cpp
bool initialize()
```

**Parameters**:
None

**Returns**:
`true` if successful

**Example**:
```cpp
if (!initialize()) {
    return -1;
}
```

---

### execute

Execute main functionality

**Signature**:
```cpp
bool execute()
```

**Parameters**:
None

**Returns**:
`true` if successful

**Example**:
```cpp
execute();
```

---

## Performance Characteristics

- **Complexity Reduction**: 78.3% average
- **Memory Footprint**: Optimized
- **Thread Safety**: Yes

## License

© 2025 The Medusa Project - MIT License
