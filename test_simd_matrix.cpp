#include "core/matrix.h"
#include <iostream>
#include <chrono>
#include <iomanip>

// Function to measure execution time
template<typename Func>
double measure_time(Func func, const std::string& name) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double time_ms = duration.count() / 1000.0;
    
    std::cout << name << " took " << std::fixed << std::setprecision(2) 
              << time_ms << " ms" << std::endl;
    
    return time_ms;
}

int main() {
    std::cout << "=== SIMD Matrix Multiplication Benchmark ===" << std::endl;
    
    // Test different matrix sizes
    std::vector<size_t> sizes = {64, 128, 256, 512, 1024};
    
    for (size_t size : sizes) {
        std::cout << "\n--- Testing " << size << "x" << size << " matrices ---" << std::endl;
        
        // Create test matrices
        Matrix<double> A = create_random<double>(size, size, -1.0, 1.0, 42);
        Matrix<double> B = create_random<double>(size, size, -1.0, 1.0, 42);
        
        Matrix<double> result_naive, result_simd;
        
        // Test naive algorithm
        double naive_time = measure_time([&]() {
            result_naive = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE);
        }, "Naive algorithm");
        
        // Test SIMD algorithm
        double simd_time = measure_time([&]() {
            result_simd = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::SIMD_NAIVE);
        }, "SIMD algorithm");
        
        // Verify results are the same
        if (result_naive.is_equal(result_simd, 1e-10)) {
            std::cout << "✓ Results match" << std::endl;
        } else {
            std::cout << "✗ Results differ!" << std::endl;
        }
        
        // Calculate speedup
        if (naive_time > 0) {
            double speedup = naive_time / simd_time;
            std::cout << "Speedup: " << std::fixed << std::setprecision(2) 
                      << speedup << "x" << std::endl;
        }
    }
    
    // Test SIMD availability
    std::cout << "\n--- SIMD Support Check ---" << std::endl;
    bool simd_available = Matrix<double>::SimdNaiveMatrixMultiplicationAlgorithm::is_simd_available();
    std::cout << "SIMD (AVX2) available: " << (simd_available ? "Yes" : "No") << std::endl;
    
    // Test with non-aligned sizes
    std::cout << "\n--- Testing non-aligned sizes ---" << std::endl;
    Matrix<double> A_unaligned = create_random<double>(100, 100, -1.0, 1.0, 42);
    Matrix<double> B_unaligned = create_random<double>(100, 100, -1.0, 1.0, 42);
    
    Matrix<double> result_naive_unaligned, result_simd_unaligned;
    
    double naive_time_unaligned = measure_time([&]() {
        result_naive_unaligned = A_unaligned.multiply(B_unaligned, 
            Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE);
    }, "Naive algorithm (unaligned)");
    
    double simd_time_unaligned = measure_time([&]() {
        result_simd_unaligned = A_unaligned.multiply(B_unaligned, 
            Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::SIMD_NAIVE);
    }, "SIMD algorithm (unaligned)");
    
    if (result_naive_unaligned.is_equal(result_simd_unaligned, 1e-10)) {
        std::cout << "✓ Results match (unaligned)" << std::endl;
    } else {
        std::cout << "✗ Results differ (unaligned)!" << std::endl;
    }
    
    if (naive_time_unaligned > 0) {
        double speedup_unaligned = naive_time_unaligned / simd_time_unaligned;
        std::cout << "Speedup (unaligned): " << std::fixed << std::setprecision(2) 
                  << speedup_unaligned << "x" << std::endl;
    }
    
    return 0;
}
