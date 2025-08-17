# LongNums Python Module

A high-performance Python module for long number arithmetic, matrix operations, and prime testing, built with pybind11.

## Features

- **Arbitrary-Precision Arithmetic**: Handle numbers of any size with the `Long` class
- **Multiple Multiplication Algorithms**: Karatsuba, Toom-Cook, and Strassen algorithms
- **Matrix Operations**: Full matrix algebra with SIMD optimizations
- **Prime Testing**: Multiple probabilistic primality tests
- **Mathematical Functions**: GCD, LCM, factorial, modular exponentiation, and more
- **Numpy Integration**: Seamless conversion between numpy arrays and LongNums matrices
- **SIMD Optimizations**: AVX2-optimized matrix operations for 2-4x speedup

## Installation

### Prerequisites

- Python 3.7 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.16+ (optional, for development)

### Quick Installation

```bash
# Clone the repository
git clone <your-repo-url>
cd LongNums

# Build and install the Python module
./build_python.sh
```

### Manual Installation

```bash
# Install Python dependencies
pip install -r requirements.txt

# Build the module
python pybind11_setup.py build_ext --inplace

# Install the module
pip install -e .
```

## Usage Examples

### Basic Long Number Arithmetic

```python
import longnums as ln

# Create large numbers
a = ln.Long("12345678901234567890")
b = ln.Long("98765432109876543210")

# Basic arithmetic
print(f"a + b = {a + b}")
print(f"a * b = {a * b}")

# Advanced multiplication algorithms
karatsuba_result = a.karatsuba_mul(b)
toomcook_result = a.toomcook_mul(b)
strassen_result = a.strassen_mul(b)
```

### Matrix Operations

```python
import longnums as ln
import numpy as np

# Create matrices
A = ln.Matrix(3, 3, 1.0)
B = ln.Matrix(3, 3, 2.0)

# Matrix multiplication with different algorithms
C_naive = A.multiply(B, ln.MatrixAlgorithm.NAIVE)
C_simd = A.multiply(B, ln.MatrixAlgorithm.SIMD_NAIVE)
C_strassen = A.multiply(B, ln.MatrixAlgorithm.STRASSEN)

# Numpy integration
np_array = np.array([[1.0, 2.0], [3.0, 4.0]])
ln_matrix = ln.matrix_from_numpy(np_array)
np_result = ln.matrix_to_numpy(ln_matrix)
```

### Prime Testing

```python
import longnums as ln

# Test numbers for primality
test_numbers = [17, 25, 97, 100]

for num in test_numbers:
    n = ln.Long(str(num))
    
    # Multiple probabilistic tests
    solovay = ln.solovay_strassen_test(n, 10)
    lehmann = ln.lehmann_test(n, 10)
    rabin = ln.rabin_miller_test(n, 10)
    
    print(f"Number {num}:")
    print(f"  Solovay-Strassen: {solovay:.6f}")
    print(f"  Lehmann: {lehmann:.6f}")
    print(f"  Rabin-Miller: {rabin:.6f}")
```

### Mathematical Functions

```python
import longnums as ln

# GCD and LCM
x = ln.Long("48")
y = ln.Long("18")
gcd_result = ln.gcd(x, y)
lcm_result = ln.lcm(x, y)

# Factorial
n = ln.Long("10")
factorial_result = ln.factorial(n)

# Modular exponentiation
base = ln.Long("2")
exponent = ln.Long("100")
modulus = ln.Long("1000000007")
mod_exp = ln.modular_exponentiation(base, exponent, modulus)
```

### SIMD Support

```python
import longnums as ln

# Check SIMD availability
if ln.is_simd_available():
    print("SIMD optimizations are available!")
else:
    print("SIMD optimizations are not available.")

# Create random matrices for performance testing
A = ln.create_random_matrix(256, 256, -1.0, 1.0)
B = ln.create_random_matrix(256, 256, -1.0, 1.0)

# Compare performance of different algorithms
import time

# Naive algorithm
start = time.time()
C_naive = A.multiply(B, ln.MatrixAlgorithm.NAIVE)
naive_time = time.time() - start

# SIMD algorithm
start = time.time()
C_simd = A.multiply(B, ln.MatrixAlgorithm.SIMD_NAIVE)
simd_time = time.time() - start

print(f"Naive: {naive_time:.4f}s")
print(f"SIMD: {simd_time:.4f}s")
print(f"Speedup: {naive_time/simd_time:.2f}x")
```

## API Reference

### Long Class

The `Long` class provides arbitrary-precision integer arithmetic.

#### Constructors

```python
Long()                    # Default constructor (zero)
Long(int_value)          # From integer
Long("string_value")     # From string
Long(other_long)         # Copy constructor
```

#### Arithmetic Operations

```python
a + b    # Addition
a - b    # Subtraction
a * b    # Multiplication
a / b    # Division
a % b    # Modulo
```

#### Advanced Methods

```python
a.karatsuba_mul(b)    # Karatsuba multiplication
a.toomcook_mul(b)     # Toom-Cook multiplication
a.strassen_mul(b)     # Strassen multiplication
a.divide(b)           # Division with remainder (returns tuple)
```

### Matrix Class

The `Matrix` class provides matrix operations with SIMD optimizations.

#### Constructors

```python
Matrix()                    # Default constructor
Matrix(rows, cols)         # Create matrix with given dimensions
Matrix(rows, cols, value)  # Create matrix filled with value
Matrix(other_matrix)       # Copy constructor
```

#### Access

```python
matrix[i, j] = value       # Set element
value = matrix[i, j]       # Get element
```

#### Operations

```python
A + B    # Matrix addition
A - B    # Matrix subtraction
A * B    # Matrix multiplication
A * s    # Scalar multiplication
```

#### Methods

```python
A.multiply(B, algorithm)   # Matrix multiplication with specific algorithm
A.transpose()              # Matrix transpose
A.determinant()            # Matrix determinant
A.inverse()                # Matrix inverse
A.trace()                  # Matrix trace
A.norm()                   # Matrix norm
```

### Matrix Algorithms

Available matrix multiplication algorithms:

```python
ln.MatrixAlgorithm.NAIVE              # Standard naive algorithm
ln.MatrixAlgorithm.NAIVE_TRANSPOSED   # Transposed naive algorithm
ln.MatrixAlgorithm.SIMD_NAIVE         # SIMD-optimized naive algorithm
ln.MatrixAlgorithm.ARM_NEON           # ARM NEON optimizations
ln.MatrixAlgorithm.STRASSEN           # Strassen algorithm
ln.MatrixAlgorithm.WINOGRAD           # Winograd algorithm
ln.MatrixAlgorithm.ALPHA_TENSOR       # Alpha Tensor algorithm
ln.MatrixAlgorithm.AUTO               # Automatic algorithm selection
```

### Prime Testing Functions

```python
ln.solovay_strassen_test(number, iterations)  # Solovay-Strassen test
ln.lehmann_test(number, iterations)           # Lehmann test
ln.rabin_miller_test(number, iterations)      # Rabin-Miller test
```

### Mathematical Functions

```python
ln.gcd(a, b)                    # Greatest common divisor
ln.lcm(a, b)                    # Least common multiple
ln.factorial(n)                 # Factorial
ln.modular_exponentiation(base, exp, mod)  # Modular exponentiation
ln.is_prime(n)                  # Primality test
ln.next_prime(n)                # Next prime number
ln.prev_prime(n)                # Previous prime number
```

### Utility Functions

```python
ln.is_simd_available()                    # Check SIMD support
ln.create_random_matrix(rows, cols, min, max)  # Create random matrix
ln.matrix_from_numpy(np_array)            # Convert numpy array to matrix
ln.matrix_to_numpy(matrix)                # Convert matrix to numpy array
ln.long_from_string(string)               # Convert string to Long
ln.long_to_string(long_number)            # Convert Long to string
```

## Performance Considerations

### Long Number Arithmetic

- **Karatsuba**: Best for medium-sized numbers (typically 100-1000 digits)
- **Toom-Cook**: Efficient for larger numbers (1000+ digits)
- **Strassen**: Multi-threaded, best for very large numbers

### Matrix Operations

- **SIMD algorithms**: Provide 2-4x speedup on AVX2-capable CPUs
- **Strassen**: Best for large matrices (typically 256x256 and larger)
- **Auto selection**: Automatically chooses the best algorithm based on matrix size

### Prime Testing

- All tests are probabilistic with configurable accuracy
- Higher iteration counts provide better accuracy but slower performance
- Recommended: 10-50 iterations for most applications

## Platform Support

### Supported Platforms

- **Linux**: GCC/Clang with AVX2 support
- **macOS**: AppleClang with AVX2 support
- **Windows**: MSVC with AVX2 support

### SIMD Requirements

- **x86-64**: AVX2 instruction set (Intel Haswell+ or AMD Excavator+)
- **ARM**: NEON instructions (most ARM64 processors)

## Troubleshooting

### Common Issues

1. **"pybind11 not found"**
   ```bash
   pip install pybind11
   ```

2. **"Compiler not found"**
   - Install a C++17 compatible compiler
   - On macOS: Install Xcode Command Line Tools
   - On Linux: Install gcc or clang
   - On Windows: Install Visual Studio Build Tools

3. **"SIMD not available"**
   - Check if your CPU supports AVX2
   - Ensure compiler flags include `-march=native` or `-mavx2`

4. **"Import error"**
   - Ensure the module is built and installed correctly
   - Check Python version compatibility (3.7+)

### Performance Issues

1. **Slow matrix multiplication**
   - Use SIMD algorithms when available
   - Consider using Strassen for large matrices
   - Ensure proper compiler optimizations

2. **Memory issues with large numbers**
   - Long numbers use significant memory
   - Consider using Real class for decimal arithmetic
   - Monitor memory usage for very large computations

## Development

### Building from Source

```bash
# Clone and setup
git clone <repo-url>
cd LongNums

# Install development dependencies
pip install -r requirements.txt

# Build in development mode
python pybind11_setup.py build_ext --inplace

# Run tests
python example_usage.py
```

### Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Built with [pybind11](https://pybind11.readthedocs.io/)
- SIMD optimizations using AVX2 instructions
- Matrix algorithms based on classical computer science research
