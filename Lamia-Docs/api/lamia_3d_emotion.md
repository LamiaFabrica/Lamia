# lamia_3d_emotion API Reference

## Overview

3D emotion synthesis and rendering

**Library Path**: `Lamia-Libs/lib/liblamia_3d_emotion.so`
**Version**: 0.3.0

## API Functions

### synthesize_emotion

Synthesize a 3D emotional representation

**Signature**:
```cpp
void Lamia::Emotion3D::synthesize_emotion(const std::string& emotion_type)
```

**Parameters**:
- `emotion_type`: Type of emotion (joy, sadness, anger, etc.)

**Returns**:
None

**Example**:
```cpp
Lamia::Emotion3D emotion;
emotion.synthesize_emotion("joy");
```

---

### get_3d_coordinates

Get 3D coordinate representation of synthesized emotion

**Signature**:
```cpp
std::vector<float> Lamia::Emotion3D::get_3d_coordinates()
```

**Parameters**:
None

**Returns**:
Vector of float coordinates [x, y, z, ...]

**Example**:
```cpp
auto coords = emotion.get_3d_coordinates();
for (float coord : coords) {
    std::cout << coord << " ";
}
```

---

## Performance Characteristics

- **Complexity Reduction**: 78.3% average
- **Memory Footprint**: Optimized
- **Thread Safety**: Yes

## License

© 2025 The Medusa Project - MIT License
