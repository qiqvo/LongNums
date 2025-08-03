# LongNums

A C++ library for long number arithmetic with multiple multiplication algorithms and prime testing capabilities.

## Features

### Multiplication Algorithms
- **Karatsuba Algorithm** - Fast multiplication for large numbers
- **Toom-Cook Algorithm** - Efficient multiplication using polynomial evaluation
- **Strassen Algorithm** - Multi-threaded multiplication implementation

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
- **Mathematical Functions** - GCD, factorial, modular exponentiation

## Project Structure

```
LongNums/
├── CMakeLists.txt          # CMake build configuration
├── Makefile               # Traditional Makefile
├── build.sh               # Convenient build script
├── README.md              # Project documentation
├── BUILD_GUIDE.md         # Build instructions
├── .gitignore             # Git ignore rules
├── include/               # Header files
│   ├── Long.h            # Long integer class
│   ├── Real.h            # Real number class
│   └── Random.h          # Random number generation
├── src/                   # Source files
│   ├── Source.cpp        # Main program entry point
│   ├── Long.cpp          # Long integer implementation
│   ├── Real.cpp          # Real number implementation
│   ├── Random.cpp        # Random number generation
│   ├── Karac_mul.cpp     # Karatsuba multiplication
│   ├── Toomcook_mul.cpp  # Toom-Cook multiplication
│   ├── Strassen_mul.cpp  # Strassen multiplication
│   ├── Sum.cpp           # Addition operations
│   ├── Mult.cpp          # Basic multiplication
│   ├── Operators.cpp     # Arithmetic operators
│   ├── Compare.cpp       # Comparison operations
│   ├── Division_Inverse.cpp # Division operations
│   ├── Math.cpp          # Mathematical functions
│   └── Prime_Test.cpp    # Prime testing algorithms
└── tests/                # Test files
    └── test_basic.cpp    # Basic functionality tests
```

## Build Instructions

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.16+ (for CMake build)
- Make (for Makefile build)
- pthread library (usually included with compiler)

### Using CMake (Recommended)

```bash
# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run the program
./bin/LongNums
```

### Using Makefile

```bash
# Build release version
make

# Build debug version
make debug

# Build optimized release
make release

# Run the program
make run

# Clean build artifacts
make clean
```

### Using Build Script

```bash
# Build and run
./build.sh run

# Build release version
./build.sh release

# Build debug version
./build.sh debug

# Clean build artifacts
./build.sh clean
```

### Direct Compilation

```bash
# Simple compilation
g++ -std=c++17 -O2 -pthread -Iinclude src/*.cpp -o LongNums

# With debug symbols
g++ -std=c++17 -g -DDEBUG -pthread -Iinclude src/*.cpp -o LongNums

# With maximum optimization
g++ -std=c++17 -O3 -DNDEBUG -pthread -Iinclude src/*.cpp -o LongNums
```

## Usage Examples

### Basic Arithmetic
```cpp
#include "include/Long.h"

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

### Prime Testing
```cpp
Long prime_candidate = 12247;
double solovay_result = prtest_SolovStras(prime_candidate, 26);
double lehmann_result = prtest_Lehmann(prime_candidate, 26);
double rabin_result = prtest_RabinMiller(prime_candidate, 26);
```

### Loading Small Primes
```cpp
#include "Long.h"

// Load primes from external file
auto small_primes = load_small_primes("data/small_primes.txt");
cout << "Loaded " << small_primes.size() << " primes up to " << small_primes.back() << endl;
```

### Real Numbers
```cpp
#include "include/Real.h"

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

## Performance Considerations

- **Karatsuba** is efficient for medium-sized numbers
- **Toom-Cook** performs well for larger numbers
- **Strassen** uses multi-threading and is best for very large numbers
- Prime tests use probabilistic algorithms with configurable accuracy

## Threading

The Strassen multiplication algorithm uses multi-threading for parallel computation. The number of threads can be controlled by the system's thread pool.

## License

This project appears to be an educational implementation for long number arithmetic algorithms.

## Contributing

This appears to be a lab/educational project. For educational purposes, feel free to study and modify the code to understand the algorithms better.
