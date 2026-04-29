# Getting Started with Lamia

## Installation

### Prerequisites
- Ubuntu 20.04+ or compatible Linux distribution
- GCC 9+ with C++26 support
- Make build system
- 2GB RAM minimum

### Building from Source

```bash
# Clone the repository
git clone https://github.com/The-Medusa-Initiative-Project/Lamia.git
cd Lamia

# Build all libraries
make all

# Install system-wide
sudo make install

# Verify installation
lamia --version
```

## Your First Manifest

Create a file `hello.lamia`:

```lamia
manifest hello() -> crystal @ludicrous {
    create RADIANT_MESSAGE {
        text: "Hello, World!",
        emotion: JOY_3D
    }
}
```

Compile and run:

```bash
lamia compile hello.lamia
./hello
```

## Core Concepts

### Manifests
Manifests are the primary building blocks in Lamia. They define executable units with revolutionary features.

### Crystal Return Types
Crystal types encapsulate both data and emotional/social context.

### Performance Modes
- `@standard` - Normal execution
- `@ludicrous` - Maximum performance
- `@quantum` - Quantum-ready execution

## Using Libraries

### C++ Integration

```cpp
#include <lamia_core.hpp>
#include <lamia_3d_emotion.hpp>

int main() {
    Lamia::Core::initialize();
    
    Lamia::Emotion3D emotion;
    emotion.synthesize_emotion("joy");
    
    auto coords = emotion.get_3d_coordinates();
    
    Lamia::Core::shutdown();
    return 0;
}
```

### Linking

```bash
g++ -std=gnu++26 myapp.cpp -llamia_core -llamia_3d_emotion -o myapp
```

## Manufacturing Integration

### G-code Generation

```lamia
manifest machine_part() -> crystal @ludicrous {
    create CNC_OPERATION {
        type: MILLING,
        material: ALUMINUM,
        dimensions: [100, 50, 25],
        gcode: NATIVE_GENERATION
    }
}
```

### Arduino Transpilation

```lamia
manifest blink_led() -> crystal @embedded {
    create ARDUINO_SKETCH {
        board: UNO,
        pin: 13,
        pattern: BLINK(500)
    }
}
```

## Advanced Features

### 3D Emotion Synthesis

```lamia
manifest emotional_response() -> crystal @ludicrous {
    create EMOTION_3D {
        primary: JOY,
        secondary: EXCITEMENT,
        intensity: 0.8,
        duration: 5000
    }
}
```

### Social Protocols

```lamia
manifest social_interaction() -> crystal @standard {
    create SOCIAL_PROTOCOL {
        type: GREETING,
        culture: UNIVERSAL,
        formality: CASUAL
    }
}
```

## Troubleshooting

### Common Issues

1. **Library not found**: Ensure LD_LIBRARY_PATH includes Lamia lib directory
2. **Compilation errors**: Check GCC version (9+ required)
3. **Performance issues**: Try `@ludicrous` mode

## Next Steps

- [Manifest Language Specification](../specs/manifest-language.md)
- [API Reference](../api/)
- [Examples](../examples/)
- [Architecture Overview](../architecture/overview.md)

---

© 2025 The Medusa Project
