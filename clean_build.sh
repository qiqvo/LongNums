#!/bin/bash

# Clean build script for AlphaTensor C++ Matrix Multiplication
# This script cleans old build artifacts and rebuilds with relative paths

set -e

echo "Cleaning previous build artifacts..."

# Remove build directory
rm -rf build

# Set environment variables for relative paths
export PROJECT_ROOT="$(pwd)"
export BUILD_DIR="$(pwd)/build"

echo "Building AlphaTensor C++ Matrix Multiplication with relative paths..."
echo "Project root: $PROJECT_ROOT"
echo "Build directory: $BUILD_DIR"

# Create build directory and build
mkdir -p build
cd build
cmake .. -DCMAKE_USE_RELATIVE_PATHS=ON
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "Clean build completed successfully!"
echo "Executable location: $BUILD_DIR/alphatensor_cpp" 