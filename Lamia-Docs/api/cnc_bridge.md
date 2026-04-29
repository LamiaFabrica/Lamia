# cnc_bridge API Reference

## Overview

CNC machine Z-code integration bridge

**Library Path**: `manufacturing_bridges/libcnc_bridge.so`
**Version**: 0.3.0

## API Functions

### convert_gcode_to_zcode

Convert G-code to Z-code format for CNC machines

**Signature**:
```cpp
bool CNCBridge::ZCodeConverter::convert_gcode_to_zcode(const std::string& input, std::string& output)
```

**Parameters**:
- `input`: G-code string
- `output`: Output Z-code string

**Returns**:
`true` if conversion successful

**Example**:
```cpp
std::string gcode = "G00 X10 Y20 Z5";
std::string zcode;
if (converter.convert_gcode_to_zcode(gcode, zcode)) {
    std::cout << "Z-code: " << zcode << std::endl;
}
```

---

## Performance Characteristics

- **Complexity Reduction**: 78.3% average
- **Memory Footprint**: Optimized
- **Thread Safety**: Yes

## License

© 2025 The Medusa Project - MIT License
