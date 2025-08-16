# SIMD Vectorization for Matrix Multiplication

## Overview

This document explains the SIMD (Single Instruction, Multiple Data) vectorization implementation for the naive matrix multiplication algorithm. SIMD allows us to process multiple data elements simultaneously using specialized CPU instructions, providing significant performance improvements.

## Key Concepts

### What is SIMD?
SIMD (Single Instruction, Multiple Data) is a parallel processing technique where a single instruction operates on multiple data elements simultaneously. Modern CPUs support SIMD instructions through extensions like:
- **SSE** (Streaming SIMD Extensions) - 128-bit registers
- **AVX** (Advanced Vector Extensions) - 256-bit registers  
- **AVX2** - Enhanced 256-bit instructions
- **AVX-512** - 512-bit registers

### Why SIMD for Matrix Multiplication?
Matrix multiplication involves many independent arithmetic operations that can be parallelized:
- **4x speedup potential** for double-precision with AVX2 (256-bit / 64-bit = 4 elements)
- **8x speedup potential** for single-precision with AVX2 (256-bit / 32-bit = 8 elements)
- **Better cache utilization** through optimized memory access patterns

## Implementation Details

### 1. Basic SIMD Operations

```cpp
// Load 4 double-precision values
__m256d a_vec = _mm256_loadu_pd(&A(i, k));

// Load 4 double-precision values  
__m256d b_vec = _mm256_loadu_pd(&B_transposed(j, k));

// Multiply 4 pairs of values simultaneously
__m256d product = _mm256_mul_pd(a_vec, b_vec);

// Add 4 pairs of values simultaneously
__m256d sum = _mm256_add_pd(sum, product);
```

### 2. Key Optimizations

#### A. Matrix Transposition
```cpp
Matrix<T> B_transposed = B.transpose();
```
**Why?** Transposing the second matrix improves cache locality and SIMD efficiency by making consecutive memory accesses.

#### B. Loop Reordering
```cpp
// Original: i -> j -> k
for (size_type i = 0; i < rows; ++i) {
    for (size_type j = 0; j < cols; ++j) {
        for (size_type k = 0; k < inner_dim; ++k) {
            result(i, j) += A(i, k) * B(k, j);
        }
    }
}

// Optimized: i -> k -> j (better cache performance)
for (size_type i = 0; i < rows; ++i) {
    for (size_type k = 0; k < inner_dim; ++k) {
        T a_ik = A(i, k);
        for (size_type j = 0; j < cols; ++j) {
            result(i, j) += a_ik * B_transposed(j, k);
        }
    }
}
```

#### C. Aligned vs Unaligned Data
```cpp
if (inner_dim % SIMD_WIDTH == 0) {
    // Use aligned SIMD operations for better performance
    simd_multiply_aligned(A, B, C);
} else {
    // Handle remaining elements with scalar operations
    simd_multiply_unaligned(A, B, C);
}
```

### 3. Horizontal Summation
After processing 4 elements in parallel, we need to sum them:
```cpp
// Extract 4 double values from SIMD register
double result_array[4];
_mm256_storeu_pd(result_array, sum);

// Sum the 4 values
C(i, j) = result_array[0] + result_array[1] + 
          result_array[2] + result_array[3];
```

## Performance Characteristics

### Expected Speedups
- **2-4x speedup** for typical matrix sizes (64x64 to 1024x1024)
- **Better cache performance** due to optimized memory access patterns
- **Reduced instruction count** through vectorization

### Factors Affecting Performance
1. **Matrix size** - Larger matrices benefit more from SIMD
2. **Memory alignment** - Aligned data performs better
3. **Cache size** - Matrices that fit in cache see maximum benefit
4. **CPU architecture** - Newer CPUs with wider SIMD units perform better

## Usage

### Compilation
```bash
# Compile with AVX2 support
g++ -std=c++17 -O3 -mavx2 -mfma -Iinclude source_files.cpp -o program
```

### Runtime Selection
```cpp
// Use SIMD-optimized algorithm
Matrix<double> result = A.multiply(B, 
    Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::SIMD_NAIVE);

// Check SIMD availability
bool available = Matrix<double>::SimdNaiveMatrixMultiplicationAlgorithm::is_simd_available();
```

## Testing and Benchmarking

### Running the Test
```bash
./compile_simd_test.sh
./test_simd_matrix
```

### Expected Output
```
=== SIMD Matrix Multiplication Benchmark ===

--- Testing 64x64 matrices ---
Naive algorithm took 2.45 ms
SIMD algorithm took 0.89 ms
✓ Results match
Speedup: 2.75x

--- Testing 256x256 matrices ---
Naive algorithm took 45.23 ms
SIMD algorithm took 12.67 ms
✓ Results match
Speedup: 3.57x
```

## Advanced Optimizations

### 1. Fused Multiply-Add (FMA)
```cpp
// Use FMA for better performance and precision
__m256d result = _mm256_fmadd_pd(a_vec, b_vec, c_vec);
```

### 2. Prefetching
```cpp
// Prefetch next cache line
_mm_prefetch((char*)&A(i, k + 16), _MM_HINT_T0);
```

### 3. Loop Unrolling
```cpp
// Process multiple SIMD operations per iteration
for (size_type k = 0; k < inner_dim; k += 16) {
    // Process 4 SIMD operations (16 elements)
}
```

### 4. Memory Pooling
```cpp
// Use aligned memory allocation for better performance
void* aligned_memory = aligned_alloc(32, size);  // 32-byte alignment for AVX2
```

## Limitations and Considerations

### 1. Data Type Support
- Currently optimized for `double` precision
- Single precision (`float`) would provide 8x speedup potential
- Integer types need different SIMD instructions

### 2. Memory Requirements
- Transposing matrix B requires additional memory
- For very large matrices, this may not be feasible

### 3. Compiler Dependencies
- Requires compiler support for AVX2 intrinsics
- May not work on older CPUs without AVX2 support

### 4. Numerical Precision
- SIMD operations may have slightly different rounding behavior
- Important for applications requiring exact numerical reproducibility

## Future Enhancements

1. **AVX-512 support** for 8x speedup with double precision
2. **Mixed precision** algorithms for even better performance
3. **GPU acceleration** using CUDA or OpenCL
4. **Automatic vectorization** hints for the compiler
5. **Sparse matrix** SIMD optimizations

## Conclusion

SIMD vectorization provides significant performance improvements for matrix multiplication, especially for larger matrices. The implementation includes:

- **Automatic algorithm selection** based on matrix size
- **Fallback mechanisms** for unsupported hardware
- **Memory optimization** through matrix transposition
- **Cache-friendly** access patterns

This optimization can provide 2-4x speedup over the naive implementation while maintaining numerical accuracy.
