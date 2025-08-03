cmake_minimum_required(VERSION 3.16)
project(AlphaTensorCpp VERSION 1.0.0 LANGUAGES CXX)

# Use relative paths when possible
set(CMAKE_USE_RELATIVE_PATHS ON)

# Set source and binary directories to be relative
set(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

# Find required packages
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/matrix.cpp
    src/algorithms.cpp
    src/benchmark.cpp
    src/main.cpp
)

# Create executable
add_executable(alphatensor_cpp ${SOURCES})

# Create unified matrix example executable (disabled - missing unified_matrix.h)
# add_executable(unified_example example_unified.cpp)

# Link libraries
if(OpenMP_CXX_FOUND)
    target_link_libraries(alphatensor_cpp OpenMP::OpenMP_CXX)
endif()

# Install target
install(TARGETS alphatensor_cpp DESTINATION bin)

# Testing
enable_testing()
add_subdirectory(tests) 