cmake_minimum_required(VERSION 3.16)
project(LongNums VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
endif()

# Enable multi-threading
find_package(Threads REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/long.cpp
    src/real.cpp
    src/random.cpp
    src/sum.cpp
    src/mult.cpp
    src/operators.cpp
    src/compare.cpp
    src/karatsuba_mul.cpp
    src/toomcook_mul.cpp
    src/strassen_mul.cpp
    src/division_inverse.cpp
    src/math.cpp
    src/prime_test.cpp
    src/utils.cpp
)

# Header files
set(HEADERS
    include/Long.h
    include/Real.h
    include/Random.h
)

# Create executable
add_executable(LongNums ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(LongNums PRIVATE Threads::Threads)

# Include directories
target_include_directories(LongNums PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set output directory
set_target_properties(LongNums PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install target
install(TARGETS LongNums
    RUNTIME DESTINATION bin
)

# Print configuration info
message(STATUS "LongNums Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Output Directory: ${CMAKE_BINARY_DIR}/bin")
message(STATUS "  Source Directory: ${CMAKE_CURRENT_SOURCE_DIR}/src")
message(STATUS "  Include Directory: ${CMAKE_CURRENT_SOURCE_DIR}/include") 