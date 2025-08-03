# Building AlphaTensor C++ Matrix Multiplication

## Overview

This project uses CMake for building. To avoid exposing personal paths in generated build files, we provide build scripts that set up environment variables for relative paths.

## Quick Build

### Option 1: Use the build script (Recommended)
```bash
./build.sh
```

### Option 2: Use the clean build script
```bash
./clean_build.sh
```

### Option 3: Manual build with environment variables
```bash
export PROJECT_ROOT="$(pwd)"
export BUILD_DIR="$(pwd)/build"
mkdir -p build
cd build
cmake .. -DCMAKE_USE_RELATIVE_PATHS=ON
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
```

## Build Artifacts

The following files are generated during the build process:

### Included in Git (Build Recipes)
- `CMakeLists.txt` - Main build configuration
- `build.sh` - Build script
- `clean_build.sh` - Clean build script
- `BUILD.md` - This documentation

### Excluded from Git (Generated Files)
- `build/` directory and all contents
- `CMakeCache.txt`
- `CMakeFiles/`
- `cmake_install.cmake`
- `Makefile` files
- Compiled executables (`alphatensor_cpp`, `test_simple`)
- Object files (`*.o`)

## Why This Approach?

1. **Build Recipes in Git**: The build configuration files are committed to git so others can reproduce the build process.

2. **Generated Files Excluded**: The actual build artifacts (Makefiles, cache files, executables) are excluded because they contain system-specific paths.

3. **Environment Variables**: The build scripts use environment variables to set relative paths, avoiding hardcoded personal paths.

4. **Portable**: This approach works across different systems and users.

## Troubleshooting

### Personal Paths Still Appearing
If you see personal paths in generated files, make sure you're using the build scripts:

```bash
# Clean everything and rebuild
./clean_build.sh
```

### Build Fails
If the build fails, try:

```bash
# Remove build directory and start fresh
rm -rf build
./build.sh
```

### Git Status Shows Build Files
If git shows build files as untracked, they should be ignored by `.gitignore`. If not, check that the `.gitignore` file is properly configured.

## Customization

To customize the build process:

1. Edit `CMakeLists.txt` for build configuration changes
2. Edit `build.sh` or `clean_build.sh` for build script changes
3. Edit `.gitignore` to change what files are tracked vs ignored

## Dependencies

- CMake 3.16 or higher
- C++17 compatible compiler
- OpenMP (optional, for parallelization) 