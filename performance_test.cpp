#include "core/matrix.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <iomanip>

// Function to measure execution time
template<typename Func>
double measure_time(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    return duration.count() / 1e6; // Convert to milliseconds
}

// Statistics calculation
struct PerformanceStats {
    double min_time;
    double max_time;
    double mean_time;
    double median_time;
    double std_dev;
    double total_time;
    
    void calculate(const std::vector<double>& times) {
        if (times.empty()) return;
        
        total_time = std::accumulate(times.begin(), times.end(), 0.0);
        min_time = *std::min_element(times.begin(), times.end());
        max_time = *std::max_element(times.begin(), times.end());
        mean_time = total_time / times.size();
        
        // Calculate median
        std::vector<double> sorted_times = times;
        std::sort(sorted_times.begin(), sorted_times.end());
        median_time = sorted_times[sorted_times.size() / 2];
        
        // Calculate standard deviation
        double variance = 0.0;
        for (double time : times) {
            variance += (time - mean_time) * (time - mean_time);
        }
        variance /= times.size();
        std_dev = std::sqrt(variance);
    }
};

// Test a specific algorithm
template<typename T>
PerformanceStats test_algorithm(
    const std::string& algorithm_name,
    typename Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType algo_type,
    int num_iterations = 100,
    size_t matrix_size = 512) {
    
    std::cout << "Testing " << algorithm_name << "..." << std::endl;
    
    std::vector<double> times;
    times.reserve(num_iterations);
    
    // Use a fixed seed for reproducible results
    std::mt19937 gen(42);
    std::uniform_real_distribution<T> dist(-1.0, 1.0);
    
    for (int i = 0; i < num_iterations; ++i) {
        // Generate random matrices
        Matrix<T> A(matrix_size, matrix_size);
        Matrix<T> B(matrix_size, matrix_size);
        
        for (size_t row = 0; row < matrix_size; ++row) {
            for (size_t col = 0; col < matrix_size; ++col) {
                A(row, col) = dist(gen);
                B(row, col) = dist(gen);
            }
        }
        
        // Measure multiplication time
        double time = measure_time([&]() {
            Matrix<T> result = A.multiply(B, algo_type);
            // Prevent compiler from optimizing away the result
            volatile T dummy = result(0, 0);
            (void)dummy;
        });
        
        times.push_back(time);
        
        // Progress indicator
        if ((i + 1) % 10 == 0) {
            std::cout << "  Completed " << (i + 1) << "/" << num_iterations << " iterations" << std::endl;
        }
    }
    
    PerformanceStats stats;
    stats.calculate(times);
    
    return stats;
}

// Print performance statistics
void print_stats(const std::string& algorithm_name, const PerformanceStats& stats) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << algorithm_name << " Performance Statistics" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Total time (100 iterations): " << std::setw(10) << stats.total_time << " ms" << std::endl;
    std::cout << "Average time per iteration:  " << std::setw(10) << stats.mean_time << " ms" << std::endl;
    std::cout << "Median time per iteration:   " << std::setw(10) << stats.median_time << " ms" << std::endl;
    std::cout << "Minimum time per iteration:  " << std::setw(10) << stats.min_time << " ms" << std::endl;
    std::cout << "Maximum time per iteration:  " << std::setw(10) << stats.max_time << " ms" << std::endl;
    std::cout << "Standard deviation:          " << std::setw(10) << stats.std_dev << " ms" << std::endl;
    std::cout << "Coefficient of variation:    " << std::setw(10) << (stats.std_dev / stats.mean_time * 100) << "%" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

// Compare algorithms
void compare_algorithms(const std::vector<std::pair<std::string, PerformanceStats>>& results) {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "ALGORITHM COMPARISON" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    // Find the Naive algorithm as baseline
    auto naive_it = std::find_if(results.begin(), results.end(),
        [](const auto& a) {
            return a.first == "Naive";
        });
    
    if (naive_it == results.end()) {
        std::cout << "Error: Naive algorithm not found for comparison baseline" << std::endl;
        return;
    }
    
    double naive_time = naive_it->second.mean_time;
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << std::setw(20) << "Algorithm" 
              << std::setw(12) << "Mean (ms)" 
              << std::setw(12) << "Median (ms)"
              << std::setw(12) << "Speedup"
              << std::setw(12) << "Efficiency" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    for (const auto& [name, stats] : results) {
        double speedup = naive_time / stats.mean_time;
        double efficiency = speedup * 100.0; // Efficiency as percentage of naive
        
        std::cout << std::setw(20) << name
                  << std::setw(12) << stats.mean_time
                  << std::setw(12) << stats.median_time
                  << std::setw(12) << speedup
                  << std::setw(12) << efficiency << "%" << std::endl;
    }
    
    // Find the fastest algorithm for summary
    auto fastest = std::min_element(results.begin(), results.end(),
        [](const auto& a, const auto& b) {
            return a.second.mean_time < b.second.mean_time;
        });
    
    std::cout << "\nFastest algorithm: " << fastest->first << std::endl;
    std::cout << "Best mean time: " << fastest->second.mean_time << " ms" << std::endl;
    std::cout << "Baseline (Naive): " << naive_time << " ms" << std::endl;
}

int main(int argc, char* argv[]) {
    // Default matrix size
    size_t N = 10;
    int num_iterations = 100;
    
    // Parse command line arguments
    if (argc > 1) {
        N = std::stoul(argv[1]);
    }
    if (argc > 2) {
        num_iterations = std::stoi(argv[2]);
    }
    
    std::cout << "=== Matrix Multiplication Performance Test ===" << std::endl;
    std::cout << "Matrix size: " << N << "x" << N << std::endl;
    std::cout << "Iterations per algorithm: " << num_iterations << std::endl;
    std::cout << "Total operations: " << num_iterations << " * " << N << "^3 = " << (static_cast<unsigned long long>(num_iterations) * N * N * N) << " multiplications" << std::endl;
    
    // Check SIMD availability
    std::cout << "\n--- Hardware Support ---" << std::endl;
#ifdef __ARM_NEON
    std::cout << "ARM NEON: ✓ Available" << std::endl;
#else
    std::cout << "ARM NEON: ✗ Not available" << std::endl;
#endif

#ifdef __AVX2__
    std::cout << "AVX2: ✓ Available" << std::endl;
#else
    std::cout << "AVX2: ✗ Not available" << std::endl;
#endif

    std::cout << "\n--- Starting Performance Tests ---" << std::endl;
    
    std::vector<std::pair<std::string, PerformanceStats>> results;
    
    // Test all available algorithms
    try {
        // Naive algorithm
        auto naive_stats = test_algorithm<double>(
            "Naive Algorithm",
            Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE,
            num_iterations,
            N
        );
        print_stats("Naive Algorithm", naive_stats);
        results.emplace_back("Naive", naive_stats);
        
        // SIMD Naive (x86)
#ifdef __AVX2__
        auto simd_stats = test_algorithm<double>(
            "SIMD Naive Algorithm (AVX2)",
            Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::SIMD_NAIVE,
            num_iterations,
            N
        );
        print_stats("SIMD Naive Algorithm (AVX2)", simd_stats);
        results.emplace_back("SIMD Naive (AVX2)", simd_stats);
#endif

        // ARM NEON
#ifdef __ARM_NEON
        auto neon_stats = test_algorithm<double>(
            "ARM NEON Algorithm",
            Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::ARM_NEON,
            num_iterations,
            N
        );
        print_stats("ARM NEON Algorithm", neon_stats);
        results.emplace_back("ARM NEON", neon_stats);
#endif

        // Block algorithm
        auto block_stats = test_algorithm<double>(
            "Block Algorithm",
            Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::BLOCK,
            num_iterations,
            N
        );
        print_stats("Block Algorithm", block_stats);
        results.emplace_back("Block", block_stats);
        
        // Strassen algorithm
        auto strassen_stats = test_algorithm<double>(
            "Strassen Algorithm",
            Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN,
            num_iterations,
            N
        );
        print_stats("Strassen Algorithm", strassen_stats);
        
        results.emplace_back("Strassen", strassen_stats);
        
        // Winograd algorithm
        auto winograd_stats = test_algorithm<double>(
            "Winograd Algorithm",
            Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::WINOGRAD,
            num_iterations,
            N
        );
        print_stats("Winograd Algorithm", winograd_stats);
        results.emplace_back("Winograd", winograd_stats);
        
        // Hybrid algorithm
        auto hybrid_stats = test_algorithm<double>(
            "Hybrid Algorithm",
            Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::HYBRID,
            num_iterations,
            N
        );
        print_stats("Hybrid Algorithm", hybrid_stats);
        results.emplace_back("Hybrid", hybrid_stats);
        
        // Auto algorithm
        auto auto_stats = test_algorithm<double>(
            "Auto Algorithm",
            Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::AUTO,
            num_iterations,
            N
        );
        print_stats("Auto Algorithm", auto_stats);
        results.emplace_back("Auto", auto_stats);
        
        // AlphaTensor algorithm
        auto alphatensor_stats = test_algorithm<double>(
            "AlphaTensor Algorithm",
            Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::ALPHATENSOR,
            num_iterations,
            N
        );
        print_stats("AlphaTensor Algorithm", alphatensor_stats);
        results.emplace_back("AlphaTensor", alphatensor_stats);
        
    } catch (const std::exception& e) {
        std::cerr << "Error during testing: " << e.what() << std::endl;
        return 1;
    }
    
    // Compare all algorithms
    compare_algorithms(results);
    
    // Performance summary
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "PERFORMANCE SUMMARY" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    auto fastest = std::min_element(results.begin(), results.end(),
        [](const auto& a, const auto& b) {
            return a.second.mean_time < b.second.mean_time;
        });
    
    auto slowest = std::max_element(results.begin(), results.end(),
        [](const auto& a, const auto& b) {
            return a.second.mean_time < b.second.mean_time;
        });
    
    std::cout << "Fastest algorithm: " << fastest->first << " (" << fastest->second.mean_time << " ms)" << std::endl;
    std::cout << "Slowest algorithm: " << slowest->first << " (" << slowest->second.mean_time << " ms)" << std::endl;
    std::cout << "Speedup range: " << (slowest->second.mean_time / fastest->second.mean_time) << "x" << std::endl;
    
    std::cout << "\nTest completed successfully!" << std::endl;
    
    return 0;
}
