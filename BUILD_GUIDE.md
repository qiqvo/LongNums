# LongNums Build Guide

This guide explains how to build and run the LongNums project using the various build systems provided.

## Quick Start

### Using the Build Script (Recommended)
```bash
# Build and run
./build.sh run

# Build release version
./build.sh release

# Build debug version
./build.sh debug
```

## Build Systems

### 1. CMake Build System (Recommended)

**Prerequisites:**
- CMake 3.16+
- C++17 compatible compiler
- pthread library

**Build Commands:**
```bash
# Create build directory and configure
mkdir build && cd build
cmake ..

# Build
make

# Run
./bin/LongNums
```

**Using the build script:**
```bash
./build.sh release    # Build release with CMake
./build.sh debug      # Build debug with CMake
```

### 2. Makefile Build System

**Prerequisites:**
- Make
- C++17 compatible compiler
- pthread library

**Build Commands:**
```bash
# Build release version
make

# Build debug version
make debug

# Build optimized release
make release

# Clean build artifacts
make clean

# Run the program
make run
```

**Using the build script:**
```bash
./build.sh make-release  # Build release with Makefile
./build.sh make-debug    # Build debug with Makefile
```

### 3. Direct Compilation

**Simple compilation:**
```bash
g++ -std=c++17 -O2 -pthread *.cpp -o LongNums
```

**With debug symbols:**
```bash
g++ -std=c++17 -g -DDEBUG -pthread *.cpp -o LongNums
```

**With maximum optimization:**
```bash
g++ -std=c++17 -O3 -DNDEBUG -pthread *.cpp -o LongNums
```

## Build Script Options

The `build.sh` script provides a convenient interface for all build operations:

```bash
./build.sh [OPTION]

Options:
  debug         - Build with CMake in debug mode
  release       - Build with CMake in release mode (default)
  make-debug    - Build with Makefile in debug mode
  make-release  - Build with Makefile in release mode
  clean         - Clean all build artifacts
  run           - Run the program
  help          - Show help message
```

## Project Structure

```
LongNums/
├── CMakeLists.txt          # CMake configuration
├── Makefile               # Traditional Makefile
├── build.sh               # Convenient build script
├── README.md              # Project documentation
├── BUILD_GUIDE.md         # This file
├── .gitignore             # Git ignore rules
├── Source.cpp             # Main program
├── Long.h/.cpp            # Long integer class
├── Real.h/.cpp            # Real number class
├── Random.h/.cpp          # Random number generation
├── Karac_mul.cpp          # Karatsuba multiplication
├── Toomcook_mul.cpp       # Toom-Cook multiplication
├── Strassen_mul.cpp       # Strassen multiplication
├── Sum.cpp                # Addition operations
├── Mult.cpp               # Basic multiplication
├── Operators.cpp          # Arithmetic operators
├── Compare.cpp            # Comparison operations
├── Division_Inverse.cpp   # Division operations
├── Math.cpp               # Mathematical functions
└── Prime_Test.cpp         # Prime testing algorithms
```

## Build Outputs

### CMake Build
- **Output directory:** `build/bin/`
- **Executable:** `build/bin/LongNums`

### Makefile Build
- **Output directory:** `bin/`
- **Executable:** `bin/LongNums`

### Direct Compilation
- **Executable:** `LongNums` (in current directory)

## Troubleshooting

### Common Issues

1. **"nproc: command not found"**
   - Fixed in build script for macOS compatibility
   - Uses `sysctl -n hw.ncpu` on macOS

2. **Threading compilation errors**
   - Temporarily disabled in Prime_Test.cpp
   - Will be fixed in future versions

3. **Compiler warnings**
   - Most warnings are harmless
   - Can be suppressed by adding `-Wno-sign-compare` to compiler flags

### Platform-Specific Notes

**macOS:**
- Uses AppleClang compiler
- pthread is included by default
- Use `sysctl -n hw.ncpu` for CPU count

**Linux:**
- Uses GCC or Clang
- pthread library required
- Use `nproc` for CPU count

**Windows:**
- Requires MSVC or MinGW
- pthread library may need to be installed separately
- Use `echo 4` for CPU count (fallback)

## Performance Considerations

- **Debug builds** include symbols and disable optimizations
- **Release builds** enable optimizations (`-O2` or `-O3`)
- **Multi-threading** is used in Strassen multiplication
- **Prime testing** uses probabilistic algorithms

## Development Workflow

1. **Initial setup:**
   ```bash
   git clone <repository>
   cd LongNums
   ./build.sh release
   ```

2. **Development cycle:**
   ```bash
   # Make changes to source files
   ./build.sh debug    # Build with debug symbols
   ./build.sh run      # Test the program
   ```

3. **Clean build:**
   ```bash
   ./build.sh clean    # Remove all build artifacts
   ./build.sh release  # Fresh build
   ```

## Contributing

When contributing to the project:

1. Use the build script for consistency
2. Test both CMake and Makefile builds
3. Ensure the program compiles without errors
4. Address any new compiler warnings

## License

This build system is provided as part of the LongNums educational project. 