#!/bin/bash

echo "Compiling Matrix Multiplication Performance Test..."

# Compile with optimizations for best performance
g++ -std=c++17 -O3 -march=native -Iinclude \
    performance_test.cpp \
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
