#!/bin/bash

echo "Compiling SIMD Matrix Multiplication Test..."

# Compile with AVX2 support
g++ -std=c++17 -O3 -mavx2 -mfma -Iinclude \
    test_simd_matrix.cpp \
    src/core/matrix_core.cpp \
    src/core/matrix_multiplication_algorithms/matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/naive_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/naive_transposed_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/simd_naive_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/block_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/strassen_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/winograd_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/auto_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/alpha_tensor_matrix_multiplication_algorithm.cpp \
    src/core/matrix_utils.cpp \
    -o test_simd_matrix

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Run with: ./test_simd_matrix"
else
    echo "Compilation failed!"
    exit 1
fi
