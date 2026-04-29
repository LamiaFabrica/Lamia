# Contributing to Lamia

Thank you for your interest in contributing to the Lamia project!

## Code of Conduct

- Be respectful and inclusive
- Welcome newcomers and help them get started
- Focus on constructive criticism
- Respect differing viewpoints and experiences

## How to Contribute

### Reporting Bugs

1. Check if the bug has already been reported
2. Create a detailed bug report including:
   - Lamia version
   - Operating system
   - Steps to reproduce
   - Expected vs actual behavior
   - Error messages and logs

### Suggesting Features

1. Check if the feature has been suggested
2. Create a feature request with:
   - Use case description
   - Proposed syntax/API
   - Example code
   - Performance implications

### Submitting Code

1. Fork the repository
2. Create a feature branch
3. Write clean, documented code
4. Add tests for new functionality
5. Ensure all tests pass
6. Submit a pull request

## Development Setup

```bash
# Clone your fork
git clone https://github.com/YOUR_USERNAME/Lamia.git
cd Lamia

# Create a branch
git checkout -b feature/your-feature

# Build the project
make all

# Run tests
make test

# Run benchmarks
make benchmark
```

## Coding Standards

### C++ Guidelines

- Use C++26 features
- Follow Google C++ Style Guide
- Document all public APIs
- Write unit tests for new code
- Maintain 78%+ complexity reduction

### Lamia Code Guidelines

- Use descriptive manifest names
- Include emotion context where appropriate
- Document performance modes
- Provide examples for new features

## Testing

### Unit Tests

```bash
make test-unit
```

### Integration Tests

```bash
make test-integration
```

### Performance Tests

```bash
make test-performance
```

## Documentation

- Update API documentation for new features
- Add examples for new functionality
- Update benchmarks if performance changes
- Keep README files current

## Pull Request Process

1. Update documentation
2. Add tests for new functionality
3. Ensure all tests pass
4. Update CHANGELOG.md
5. Request review from maintainers

## Release Process

1. Version bumps follow semantic versioning
2. All tests must pass
3. Documentation must be complete
4. Performance benchmarks updated
5. CHANGELOG updated

## Ground-Up Implementation Principles

- NO SHORTCUTS
- All code must be functional, not mock
- Performance metrics from actual execution
- No destructive operations
- Favor native C++ conversion

## Recognition

Contributors will be recognized in:
- CONTRIBUTORS.md file
- Release notes
- Project documentation

## Questions?

- GitHub Issues: https://github.com/The-Medusa-Initiative-Project/Lamia/issues
- Email: lamia@medusa-project.org

---

© 2025 The Medusa Project - Contributing Guidelines
