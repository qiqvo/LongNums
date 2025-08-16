#!/bin/bash

# SIMD Build Verification Script
# This script verifies that SIMD optimizations are properly enabled

set -e

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_status "Verifying SIMD build configuration..."

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    print_error "Please run this script from the LongNums project root directory"
    exit 1
fi

# Check compiler support
print_status "Checking compiler SIMD support..."

# Check GCC/Clang
if command -v g++ &> /dev/null; then
    print_status "Found GCC compiler"
    gcc_version=$(g++ --version | head -n1)
    echo "  Version: $gcc_version"
    
    # Check if AVX2 is supported
    if g++ -mavx2 -E - < /dev/null &> /dev/null; then
        print_status "✓ AVX2 support available in GCC"
    else
        print_warning "✗ AVX2 support not available in GCC"
    fi
fi

if command -v clang++ &> /dev/null; then
    print_status "Found Clang compiler"
    clang_version=$(clang++ --version | head -n1)
    echo "  Version: $clang_version"
    
    # Check if AVX2 is supported
    if clang++ -mavx2 -E - < /dev/null &> /dev/null; then
        print_status "✓ AVX2 support available in Clang"
    else
        print_warning "✗ AVX2 support not available in Clang"
    fi
fi

# Check CPU support
print_status "Checking CPU SIMD support..."

if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    cpu_brand=$(sysctl -n machdep.cpu.brand_string 2>/dev/null || echo "Unknown")
    print_status "CPU: $cpu_brand"
    
    if [[ "$cpu_brand" == *"Apple M"* ]] || [[ "$cpu_brand" == *"Apple Silicon"* ]]; then
        # Apple Silicon - check for NEON
        if g++ -march=native -E -dM - < /dev/null 2>/dev/null | grep -q __ARM_NEON; then
            print_status "✓ CPU supports ARM NEON"
        else
            print_warning "✗ CPU does not support ARM NEON"
        fi
    else
        # Intel/AMD - check for AVX2
        if sysctl -n machdep.cpu.features 2>/dev/null | grep -q AVX2; then
            print_status "✓ CPU supports AVX2"
        else
            print_warning "✗ CPU does not support AVX2"
        fi
    fi
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    if grep -q avx2 /proc/cpuinfo 2>/dev/null; then
        print_status "✓ CPU supports AVX2"
    elif grep -q neon /proc/cpuinfo 2>/dev/null; then
        print_status "✓ CPU supports ARM NEON"
    else
        print_warning "✗ CPU does not support AVX2 or NEON"
    fi
else
    print_warning "Cannot check CPU support on this platform"
fi

# Check build configuration
print_status "Checking build configuration..."

# Check CMakeLists.txt
if grep -q "mavx2\|march=native" CMakeLists.txt; then
    print_status "✓ CMakeLists.txt includes SIMD flags"
else
    print_error "✗ CMakeLists.txt missing SIMD flags"
fi

# Check Makefile
if grep -q "mavx2\|march=native" Makefile; then
    print_status "✓ Makefile includes SIMD flags"
else
    print_error "✗ Makefile missing SIMD flags"
fi

# Check if SIMD source files exist
if [ -f "src/core/matrix_multiplication_algorithms/simd_naive_matrix_multiplication_algorithm.cpp" ]; then
    print_status "✓ x86 SIMD algorithm source file exists"
else
    print_error "✗ x86 SIMD algorithm source file missing"
fi

if [ -f "src/core/matrix_multiplication_algorithms/arm_neon_matrix_multiplication_algorithm.cpp" ]; then
    print_status "✓ ARM NEON algorithm source file exists"
else
    print_error "✗ ARM NEON algorithm source file missing"
fi

# Check if SIMD test exists
if [ -f "test_simd_matrix.cpp" ]; then
    print_status "✓ SIMD test file exists"
else
    print_error "✗ SIMD test file missing"
fi

if [ -f "compile_simd_test.sh" ]; then
    print_status "✓ SIMD test compilation script exists"
else
    print_error "✗ SIMD test compilation script missing"
fi

# Try to build a simple test
print_status "Testing SIMD compilation..."

# Test for ARM NEON
cat > test_neon_compile.cpp << 'EOF'
#include <arm_neon.h>
#include <iostream>

int main() {
    float64x2_t test = vdupq_n_f64(0.0);
    double result[2];
    vst1q_f64(result, test);
    std::cout << "ARM NEON compilation test successful!" << std::endl;
    return 0;
}
EOF

if g++ -std=c++17 -march=native test_neon_compile.cpp -o test_neon_compile 2>/dev/null; then
    print_status "✓ ARM NEON compilation test passed"
    rm -f test_neon_compile test_neon_compile.cpp
else
    print_error "✗ ARM NEON compilation test failed"
    rm -f test_neon_compile.cpp
    
    # Fallback test for x86 AVX2
    cat > test_avx2_compile.cpp << 'EOF'
#include <immintrin.h>
#include <iostream>

int main() {
    __m256d test = _mm256_setzero_pd();
    double result[4];
    _mm256_storeu_pd(result, test);
    std::cout << "x86 AVX2 compilation test successful!" << std::endl;
    return 0;
}
EOF

    if g++ -std=c++17 -mavx2 -mfma test_avx2_compile.cpp -o test_avx2_compile 2>/dev/null; then
        print_status "✓ x86 AVX2 compilation test passed"
        rm -f test_avx2_compile test_avx2_compile.cpp
    else
        print_error "✗ x86 AVX2 compilation test failed"
        rm -f test_avx2_compile.cpp
    fi
fi

print_status "SIMD build verification completed!"
print_status "To test SIMD performance, run: ./build.sh simd-test"
