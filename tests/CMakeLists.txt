# Test configuration
find_package(GTest QUIET)

if(GTest_FOUND)
    # Matrix tests
    add_executable(matrix_test matrix_test.cpp)
    target_link_libraries(matrix_test alphatensor_cpp GTest::GTest GTest::Main)
    
    # Algorithm tests
    add_executable(algorithm_test algorithm_test.cpp)
    target_link_libraries(algorithm_test alphatensor_cpp GTest::GTest GTest::Main)
    
    # Benchmark tests
    add_executable(benchmark_test benchmark_test.cpp)
    target_link_libraries(benchmark_test alphatensor_cpp GTest::GTest GTest::Main)
    
    # Add tests
    add_test(NAME MatrixTest COMMAND matrix_test)
    add_test(NAME AlgorithmTest COMMAND algorithm_test)
    add_test(NAME BenchmarkTest COMMAND benchmark_test)
    
else()
    message(WARNING "Google Test not found. Tests will not be built.")
endif() 