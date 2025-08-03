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
    src/Source.cpp
    src/Long.cpp
    src/Real.cpp
    src/Random.cpp
    src/Sum.cpp
    src/Mult.cpp
    src/Operators.cpp
    src/Compare.cpp
    src/Karac_mul.cpp
    src/Toomcook_mul.cpp
    src/Strassen_mul.cpp
    src/Division_Inverse.cpp
    src/Math.cpp
    src/Prime_Test.cpp
    src/Utils.cpp
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