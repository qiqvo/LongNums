#!/bin/bash

echo "Compiling ARM NEON Matrix Multiplication Test..."

# Compile with ARM NEON support for Apple Silicon
g++ -std=c++17 -O3 -march=native -Iinclude \
    test_arm_neon_matrix.cpp \
    src/core/matrix_core.cpp \
    src/core/algorithms/matrix_multiplication_algorithm.cpp \
    src/core/algorithms/naive_matrix_multiplication_algorithm.cpp \
    src/core/algorithms/simd_naive_matrix_multiplication_algorithm.cpp \
    src/core/algorithms/arm_neon_matrix_multiplication_algorithm.cpp \
    src/core/algorithms/block_matrix_multiplication_algorithm.cpp \
    src/core/algorithms/strassen_matrix_multiplication_algorithm.cpp \
    src/core/algorithms/winograd_matrix_multiplication_algorithm.cpp \
    src/core/algorithms/hybrid_matrix_multiplication_algorithm.cpp \
    src/core/algorithms/auto_matrix_multiplication_algorithm.cpp \
    src/core/algorithms/alphatensor_algorithm.cpp \
    src/core/matrix_multiply.cpp \
    src/core/matrix_utils.cpp \
    -o test_arm_neon_matrix

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Run with: ./test_arm_neon_matrix"
else
    echo "Compilation failed!"
    exit 1
fi
