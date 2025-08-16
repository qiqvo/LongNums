#!/bin/bash

echo "Compiling Strassen Correctness Test..."

# Compile with all necessary source files
g++ -std=c++17 -O2 -march=native -Wall -Wextra \
    -I./include \
    test_strassen_correctness.cpp \
    src/core/long.cpp \
    src/core/math.cpp \
    src/core/matrix_core.cpp \
    src/core/matrix_multiply.cpp \
    src/core/matrix_utils.cpp \
    src/core/real.cpp \
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
    src/core/strassen_mul.cpp \
    src/core/toomcook_mul.cpp \
    src/utils/utils.cpp \
    src/random/evenly_gen.cpp \
    src/random/generator_general.cpp \
    src/random/quad_con_gen.cpp \
    -o test_strassen_correctness \
    -pthread

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Run with: ./test_strassen_correctness"
    echo ""
    echo "This will test Strassen algorithm correctness and performance"
    echo "for matrix sizes 4x4, 8x8, 16x16, 32x32, 64x64, and 128x128."
else
    echo "Compilation failed!"
    exit 1
fi
