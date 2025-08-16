#!/bin/bash

echo "Compiling Simple ARM NEON Matrix Multiplication Test..."

# Compile with ARM NEON support for Apple Silicon
g++ -std=c++17 -O3 -march=native test_arm_neon_simple.cpp -o test_arm_neon_simple

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Run with: ./test_arm_neon_simple"
else
    echo "Compilation failed!"
    exit 1
fi
