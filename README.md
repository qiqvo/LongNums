# LongNums

A C++ library for long number arithmetic with multiple multiplication algorithms and prime testing capabilities.

## Quick Start

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.16+ (for CMake build)
- Make (for Makefile build)
- pthread library (usually included with compiler)
- **SIMD Support**: AVX2-capable CPU for optimal performance (Intel Haswell+ or AMD Excavator+)

### Build and Run

**Option 1: Using the build script (Recommended)**
```bash
# Build and run in one command
./build.sh run

# Build release version
./build.sh release

# Build debug version
./build.sh debug
```

**Option 2: Using CMake**
```bash
# Create build directory and configure
mkdir build && cd build
cmake ..

# Build
make

# Run
./bin/LongNums
```

**Option 3: Using Makefile**
```bash
# Build release version
make

# Build debug version
make debug

# Run the program
make run
```

**Option 4: Direct compilation**
```bash
# Simple compilation
g++ -std=c++17 -O2 -pthread -Iinclude src/*.cpp -o LongNums

# Compilation with SIMD optimizations
g++ -std=c++17 -O3 -mavx2 -mfma -pthread -Iinclude src/*.cpp -o LongNums

# Run
./LongNums
```

## Features

### Multiplication Algorithms
- **Karatsuba Algorithm** - Fast multiplication for large numbers
- **Toom-Cook Algorithm** - Efficient multiplication using polynomial evaluation
- **Strassen Algorithm** - Multi-threaded multiplication implementation
- **SIMD-Optimized Algorithms** - Vectorized matrix operations using AVX2 instructions

### Number Representations
- **Long Class** - Arbitrary-precision integer arithmetic
- **Real Class** - Real number representation with configurable precision

### Prime Testing
- **Solovay-Strassen Test** - Probabilistic primality test
- **Lehmann Test** - Another probabilistic primality test  
- **Rabin-Miller Test** - Strong probabilistic primality test

### Additional Features
- **Random Number Generation** - Custom random number generators
- **Multi-threading Support** - Parallel computation for large operations
- **SIMD Vectorization** - AVX2-optimized matrix operations for 2-4x speedup
- **Mathematical Functions** - GCD, factorial, modular exponentiation

## Project Structure

```
LongNums/
├── CMakeLists.txt          # CMake build configuration
├── Makefile               # Traditional Makefile
├── build.sh               # Convenient build script
├── README.md              # Project documentation
├── BUILD_GUIDE.md         # Detailed build instructions
├── .gitignore             # Git ignore rules
├── include/               # Header files
│   ├── core/             # Core number classes
│   │   ├── long.h        # Long integer class
│   │   └── real.h        # Real number class
│   ├── prime/            # Prime testing
│   ├── random/           # Random number generation
│   │   └── random.h      # Random number generation
│   ├── math/             # Mathematical functions
│   └── utils/            # Utility functions
├── src/                   # Source files
│   ├── main.cpp          # Main program entry point
│   ├── core/             # Core number implementations
│   │   ├── long.cpp      # Long integer implementation
│   │   └── real.cpp      # Real number implementation
│   ├── prime/            # Prime testing algorithms
│   │   └── prime_test.cpp # Prime testing algorithms
│   ├── random/           # Random number generation
│   │   └── random.cpp    # Random number generation
│   ├── math/             # Mathematical functions and algorithms
│   │   ├── math.cpp      # Mathematical functions
│   │   ├── toomcook_mul.cpp # Toom-Cook multiplication
│   │   └── strassen_mul.cpp # Strassen multiplication
│   └── utils/            # Utility functions
│       └── utils.cpp     # Utility functions
└── tests/                # Test files
    └── test_basic.cpp    # Basic functionality tests
```

## Usage Examples

### Basic Arithmetic
```cpp
#include "core/long.h"

Long a = "12345678901234567890";
Long b = "98765432109876543210";
Long c = a + b;
Long d = a * b;  // Uses naive multiplication
```

### Advanced Multiplication
```cpp
Long a = "12345678901234567890";
Long b = "98765432109876543210";

Long karatsuba_result = a.karac_mul(b);
Long toomcook_result = a.toomcook_mul(b);
Long strassen_result = a.strassen_mul(b);  // Multi-threaded
```

### Matrix Operations with SIMD
```cpp
#include "core/matrix.h"

Matrix<double> A = create_random<double>(256, 256, -1.0, 1.0);
Matrix<double> B = create_random<double>(256, 256, -1.0, 1.0);

// Use SIMD-optimized multiplication
Matrix<double> result = A.multiply(B, 
    Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::SIMD_NAIVE);

// Check SIMD availability
bool simd_available = Matrix<double>::SimdNaiveMatrixMultiplicationAlgorithm::is_simd_available();
```

### Prime Testing
```cpp
Long prime_candidate = 12247;
double solovay_result = prtest_SolovStras(prime_candidate, 26);
double lehmann_result = prtest_Lehmann(prime_candidate, 26);
double rabin_result = prtest_RabinMiller(prime_candidate, 26);
```

### Loading Small Primes
```cpp
#include "core/long.h"

// Load primes from external file
auto small_primes = load_small_primes("data/small_primes.txt");
cout << "Loaded " << small_primes.size() << " primes up to " << small_primes.back() << endl;
```

### Real Numbers
```cpp
#include "core/real.h"

Real a(1000, 1);  // Represents 0.1
Real b(2000, 1);  // Represents 0.2
Real c = a + b;    // Result: 0.3
```

## Testing

The project includes a basic test framework in the `tests/` directory:

```bash
# Compile and run tests
g++ -std=c++17 -Iinclude tests/test_basic.cpp src/*.cpp -o test_basic
./test_basic
```

### SIMD Optimization Testing

Test the SIMD-optimized matrix multiplication algorithms:

```bash
# Using the build script
./build.sh simd-test

# Or manually
./compile_simd_test.sh
./test_simd_matrix
```

This will benchmark the SIMD-optimized algorithm against the naive implementation and show speedup ratios.

### Verify SIMD Build Configuration

Check that SIMD optimizations are properly configured:

```bash
./verify_simd_build.sh
```

This script verifies compiler support, CPU capabilities, and build configuration for SIMD optimizations.

## Performance Considerations

- **Karatsuba** is efficient for medium-sized numbers
- **Toom-Cook** performs well for larger numbers
- **Strassen** uses multi-threading and is best for very large numbers
- **SIMD algorithms** provide 2-4x speedup for matrix operations on AVX2-capable CPUs
- Prime tests use probabilistic algorithms with configurable accuracy

## Threading

The Strassen multiplication algorithm uses multi-threading for parallel computation. The number of threads can be controlled by the system's thread pool.

## Troubleshooting

### Common Issues

1. **"nproc: command not found"** (macOS)
   - The build script automatically uses `sysctl -n hw.ncpu` on macOS

2. **Threading compilation errors**
   - Most threading issues are resolved in the current version

3. **Include path issues**
   - Make sure to use `-Iinclude` flag when compiling directly
   - Build systems handle this automatically

### Platform-Specific Notes

**macOS:**
- Uses AppleClang compiler
- pthread is included by default

**Linux:**
- Uses GCC or Clang
- pthread library required

**Windows:**
- Requires MSVC or MinGW
- pthread library may need to be installed separately

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
  simd-test     - Build and run SIMD optimization test
  help          - Show help message
```

## License

This project appears to be an educational implementation for long number arithmetic algorithms.

## Contributing

This appears to be a lab/educational project. For educational purposes, feel free to study and modify the code to understand the algorithms better.

For detailed build instructions, see [BUILD_GUIDE.md](BUILD_GUIDE.md).
