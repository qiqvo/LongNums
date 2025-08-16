#!/bin/bash

# LongNums Build Script
# Usage: ./build.sh [debug|release|clean|run]

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ] || [ ! -f "Makefile" ]; then
    print_error "Please run this script from the LongNums project root directory"
    exit 1
fi

# Function to build with CMake
build_cmake() {
    local build_type=$1
    print_status "Building with CMake ($build_type)..."
    
    mkdir -p build
    cd build
    
    if [ "$build_type" = "debug" ]; then
        cmake -DCMAKE_BUILD_TYPE=Debug ..
    else
        cmake -DCMAKE_BUILD_TYPE=Release ..
    fi
    
    make -j$(sysctl -n hw.ncpu 2>/dev/null || echo 4)
    cd ..
    
    print_status "Build completed successfully!"
}

# Function to build with Makefile
build_make() {
    local build_type=$1
    print_status "Building with Makefile ($build_type)..."
    
    if [ "$build_type" = "debug" ]; then
        make debug
    else
        make release
    fi
    
    print_status "Build completed successfully!"
}

# Function to clean build artifacts
clean_build() {
    print_status "Cleaning build artifacts..."
    
    # Clean CMake build
    if [ -d "build" ]; then
        rm -rf build
    fi
    
    # Clean Makefile build
    if [ -d "bin" ]; then
        rm -rf bin
    fi
    
    if [ -d "build" ]; then
        rm -rf build
    fi
    
    print_status "Clean completed!"
}

# Function to run the program
run_program() {
    local executable=""
    
    # Check for executable in different locations
    if [ -f "bin/LongNums" ]; then
        executable="bin/LongNums"
    elif [ -f "build/bin/LongNums" ]; then
        executable="build/bin/LongNums"
    elif [ -f "LongNums" ]; then
        executable="./LongNums"
    else
        print_error "No executable found. Please build the project first."
        exit 1
    fi
    
    print_status "Running $executable..."
    $executable
}

# Main script logic
case "${1:-release}" in
    "debug")
        print_status "Building in debug mode..."
        build_cmake debug
        ;;
    "release")
        print_status "Building in release mode..."
        build_cmake release
        ;;
    "make-debug")
        print_status "Building with Makefile in debug mode..."
        build_make debug
        ;;
    "make-release")
        print_status "Building with Makefile in release mode..."
        build_make release
        ;;
    "clean")
        clean_build
        ;;
    "run")
        run_program
        ;;
    "performance-test")
        print_status "Building and running performance test..."
        if [ -f "performance_test" ] || [ -f "build/bin/performance_test" ]; then
            if [ -f "performance_test" ]; then
                ./performance_test
            else
                ./build/bin/performance_test
            fi
        else
            print_error "Performance test not found. Please build the project first."
            exit 1
        fi
        ;;
    "simd-test")
        print_status "Building and running SIMD optimization test..."
        if [ -f "compile_simd_test.sh" ]; then
            ./compile_simd_test.sh
            if [ $? -eq 0 ]; then
                print_status "Running SIMD test..."
                ./test_simd_matrix
            else
                print_error "SIMD test compilation failed!"
                exit 1
            fi
        else
            print_error "SIMD test script not found!"
            exit 1
        fi
        ;;
    "help"|"-h"|"--help")
        echo "LongNums Build Script"
        echo "===================="
        echo "Usage: $0 [OPTION]"
        echo ""
        echo "Options:"
        echo "  debug         - Build with CMake in debug mode"
        echo "  release       - Build with CMake in release mode (default)"
        echo "  make-debug    - Build with Makefile in debug mode"
        echo "  make-release  - Build with Makefile in release mode"
        echo "  clean         - Clean all build artifacts"
        echo "  run           - Run the program"
        echo "  performance-test - Run the performance test"
        echo "  simd-test     - Build and run SIMD optimization test"
        echo "  help          - Show this help message"
        echo ""
        echo "Examples:"
        echo "  $0              # Build release with CMake"
        echo "  $0 debug        # Build debug with CMake"
        echo "  $0 make-release # Build release with Makefile"
        echo "  $0 run          # Run the program"
        echo "  $0 performance-test # Run performance test"
        ;;
    *)
        print_error "Unknown option: $1"
        echo "Use '$0 help' for usage information"
        exit 1
        ;;
esac 