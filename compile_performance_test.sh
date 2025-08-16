#!/bin/bash

echo "Compiling Matrix Multiplication Performance Test..."

# Compile with optimizations for best performance
g++ -std=c++17 -O3 -march=native -Iinclude \
    performance_test.cpp \
    src/core/matrix_core.cpp \
    src/core/matrix_multiplication_algorithms/matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/naive_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/simd_naive_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/arm_neon_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/block_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/strassen_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/winograd_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/auto_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/alpha_tensor_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiply.cpp \
    src/core/matrix_utils.cpp \
    -o performance_test

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo ""
    echo "Usage:"
    echo "  ./performance_test [N] [iterations]"
    echo ""
    echo "Parameters:"
    echo "  N          - Matrix size (default: 512)"
    echo "  iterations - Number of iterations per algorithm (default: 100)"
    echo ""
    echo "Examples:"
    echo "  ./performance_test              # 512x512 matrices, 100 iterations"
    echo "  ./performance_test 256          # 256x256 matrices, 100 iterations"
    echo "  ./performance_test 1024 50      # 1024x1024 matrices, 50 iterations"
    echo ""
    echo "Expected runtime: 5-15 minutes for 512x512, scales with N^3"
else
    echo "Compilation failed!"
    exit 1
fi
