#!/bin/bash
# Lamia Documentation Builder
# © 2025 The Medusa Project

echo "🔨 Building Lamia Documentation..."

# Generate API docs from source
echo "Generating API documentation..."
./lamia_docs_generator

# Build GitHub Pages site
echo "Building GitHub Pages site..."
bundle exec jekyll build

# Generate PDF documentation
echo "Generating PDF documentation..."
pandoc -s README.md guides/*.md api/*.md -o lamia_docs.pdf

# Update version numbers
VERSION=$(grep "version_" src/lamia_core.cpp | cut -d'"' -f2)
echo "Updating version to $VERSION..."
find . -name "*.md" -exec sed -i "s/Version: .*/Version: $VERSION/g" {} \;

echo "✅ Documentation build complete!"
