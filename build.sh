#!/bin/bash

# Build script for AlphaTensor C++ Matrix Multiplication
# This script sets up environment variables to avoid hardcoded personal paths

set -e

# Set environment variables for relative paths
export PROJECT_ROOT="$(pwd)"
export BUILD_DIR="$(pwd)/build"

echo "Building AlphaTensor C++ Matrix Multiplication..."
echo "Project root: $PROJECT_ROOT"
echo "Build directory: $BUILD_DIR"

# Create build directory if it doesn't exist
mkdir -p build

# Configure and build
cd build
cmake .. -DCMAKE_USE_RELATIVE_PATHS=ON
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "Build completed successfully!"
echo "Executable location: $BUILD_DIR/alphatensor_cpp" 