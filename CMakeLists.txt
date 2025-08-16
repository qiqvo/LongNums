cmake_minimum_required(VERSION 3.16)
project(LongNums VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure consistent object file extensions across platforms
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    # Enable SIMD optimizations for MSVC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
    # Use .obj extension for MSVC
    set(CMAKE_CXX_OUTPUT_EXTENSION .obj)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
    # Enable SIMD optimizations for GCC/Clang
    # Use native architecture for best SIMD support (AVX2 on x86, NEON on ARM)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    # Use .o extension for GCC/Clang
    set(CMAKE_CXX_OUTPUT_EXTENSION .o)
endif()

# Note: Compiler flags are now set above with consistent object file extensions

# Enable multi-threading
find_package(Threads REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/core/long.cpp
    src/core/matrix_core.cpp
    src/core/matrix_multiplication_algorithms/matrix_multiplication_algorithm.cpp
    src/core/matrix_multiplication_algorithms/naive_matrix_multiplication_algorithm.cpp
    src/core/matrix_multiplication_algorithms/simd_naive_matrix_multiplication_algorithm.cpp
    src/core/matrix_multiplication_algorithms/arm_neon_matrix_multiplication_algorithm.cpp
    src/core/matrix_multiplication_algorithms/block_matrix_multiplication_algorithm.cpp
    src/core/matrix_multiplication_algorithms/strassen_matrix_multiplication_algorithm.cpp
    src/core/matrix_multiplication_algorithms/winograd_matrix_multiplication_algorithm.cpp
    src/core/matrix_multiplication_algorithms/hybrid_matrix_multiplication_algorithm.cpp
    src/core/matrix_multiplication_algorithms/auto_matrix_multiplication_algorithm.cpp
    src/core/matrix_multiplication_algorithms/sixteen_block_matrix_multiplication_algorithm.cpp
    src/core/matrix_multiplication_algorithms/alpha_tensor_matrix_multiplication_algorithm.cpp
    src/core/matrix_multiply.cpp
    src/core/matrix_utils.cpp
    src/core/real.cpp
    src/core/strassen_mul.cpp
    src/core/toomcook_mul.cpp
    src/core/math.cpp
    src/random/generator_general.cpp
    src/random/evenly_gen.cpp
    src/random/quad_con_gen.cpp
    src/prime/prime_test.cpp
    src/utils/utils.cpp
)

# Header files
set(HEADERS
    include/core/long.h
    include/core/matrix.h
    include/core/real.h
    include/core/math.h
    include/random/generator_general.h
    include/random/evenly_gen.h
    include/random/quad_con_gen.h
    include/prime/prime_test.h
    include/utils/utils.h
)

# Create main executable
add_executable(LongNums ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(LongNums PRIVATE Threads::Threads)

# Include directories
target_include_directories(LongNums PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Create performance test executable (exclude main.cpp to avoid duplicate main)
set(PERFORMANCE_SOURCES ${SOURCES})
list(REMOVE_ITEM PERFORMANCE_SOURCES src/main.cpp)
add_executable(performance_test performance_test.cpp ${PERFORMANCE_SOURCES})

# Link libraries for performance test
target_link_libraries(performance_test PRIVATE Threads::Threads)

# Include directories for performance test
target_include_directories(performance_test PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set output directory for both executables
set_target_properties(LongNums performance_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install targets
install(TARGETS LongNums performance_test
    RUNTIME DESTINATION bin
)

# Print configuration info
message(STATUS "LongNums Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  SIMD Support: AVX2 enabled")
message(STATUS "  Output Directory: ${CMAKE_BINARY_DIR}/bin")
message(STATUS "  Source Directory: ${CMAKE_CURRENT_SOURCE_DIR}/src")
message(STATUS "  Include Directory: ${CMAKE_CURRENT_SOURCE_DIR}/include") 