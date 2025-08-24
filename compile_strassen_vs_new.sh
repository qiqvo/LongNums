#!/bin/bash

echo "Compiling Strassen vs New Strassen Comparison Test..."

# Compile with all necessary source files
g++ -std=c++17 -O2 -march=native -Wall -Wextra \
    -I./include \
    test_strassen_vs_new_strassen.cpp \
    src/core/long.cpp \
    src/core/math.cpp \
    src/core/matrix_core.cpp \
    src/core/matrix_utils.cpp \
    src/core/real.cpp \
    src/core/matrix_multiplication_algorithms/matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/naive_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/naive_transposed_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/simd_naive_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/arm_neon_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/block_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/strassen_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/new_strassen_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/winograd_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/auto_matrix_multiplication_algorithm.cpp \
    src/core/matrix_multiplication_algorithms/alpha_tensor_matrix_multiplication_algorithm.cpp \
    src/core/strassen_mul.cpp \
    src/core/toomcook_mul.cpp \
    src/utils/utils.cpp \
    src/random/evenly_gen.cpp \
    src/random/generator_general.cpp \
    src/random/quad_con_gen.cpp \
    -o test_strassen_vs_new \
    -pthread

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Run with: ./test_strassen_vs_new"
    echo ""
    echo "This will compare the original Strassen algorithm with the new optimized version"
    echo "for matrix sizes 64x64, 128x128, 256x256, and 512x512."
else
    echo "Compilation failed!"
    exit 1
fi
