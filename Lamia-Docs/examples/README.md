# Lamia Examples

This directory contains example Lamia programs demonstrating various features.

## Examples

1. **01_basic.lamia** - Basic language features
2. **02_manufacturing.lamia** - CNC G-code generation
3. **03_arduino.lamia** - Arduino transpilation
4. **04_emotion.lamia** - 3D emotion synthesis

## Running Examples

```bash
# Compile and run an example
lamia compile examples/01_basic.lamia
./01_basic

# Transpile to C++
lamia transpile examples/01_basic.lamia --output basic.cpp

# Generate G-code
lamia transpile examples/02_manufacturing.lamia --target gcode

# Generate Arduino sketch
lamia transpile examples/03_arduino.lamia --target arduino --board uno
```

## Contributing Examples

To contribute an example:
1. Create a new .lamia file
2. Add clear comments explaining the features
3. Test the example
4. Submit a pull request

---

© 2025 The Medusa Project
