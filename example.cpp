#include "matrix.h"
#include "algorithms.h"
#include "benchmark.h"
#include <iostream>
#include <iomanip>

using namespace alphatensor;

int main() {
    std::cout << "AlphaTensor C++ Matrix Multiplication Example\n";
    std::cout << "=============================================\n\n";
    
    // Create test matrices
    std::cout << "Creating test matrices...\n";
    Matrix A = create_random(4, 4, -1.0, 1.0, 42);
    Matrix B = create_random(4, 4, -1.0, 1.0, 43);
    
    std::cout << "Matrix A:\n";
    A.print(std::cout, 3);
    std::cout << "\nMatrix B:\n";
    B.print(std::cout, 3);
    
    // Test different algorithms
    std::cout << "\nTesting different algorithms...\n";
    std::cout << "================================\n";
    
    std::vector<std::shared_ptr<MatrixMultiplicationAlgorithm<double>>> algorithms = {
        std::make_shared<NaiveAlgorithm<double>>(),
        std::make_shared<BlockAlgorithm<double>>(),
        std::make_shared<StrassenAlgorithm<double>>(),
        std::make_shared<WinogradAlgorithm<double>>(),
        std::make_shared<AlphaTensorAlgorithm<double>>("gpu"),
        std::make_shared<HybridAlgorithm<double>>()
    };
    
    Matrix reference_result;
    bool first = true;
    
    for (const auto& algorithm : algorithms) {
        std::cout << "\nAlgorithm: " << algorithm->name() << "\n";
        std::cout << std::string(algorithm->name().length() + 10, '-') << "\n";
        
        // Measure execution time
        auto start = std::chrono::high_resolution_clock::now();
        Matrix result = algorithm->multiply(A, B);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double time_ms = duration.count() / 1000.0;
        
        std::cout << "Time: " << format_time(time_ms) << "\n";
        std::cout << "GFLOPs: " << std::fixed << std::setprecision(2) 
                  << (2.0 * 4 * 4 * 4) / (time_ms * 1e6) << "\n";
        
        // Verify correctness
        if (first) {
            reference_result = result;
            first = false;
            std::cout << "Correct: Yes (reference)\n";
        } else {
            bool correct = result.is_equal(reference_result, 1e-10);
            std::cout << "Correct: " << (correct ? "Yes" : "No") << "\n";
        }
        
        std::cout << "Result:\n";
        result.print(std::cout, 3);
    }
    
    // Benchmark comparison
    std::cout << "\n\nBenchmark Comparison\n";
    std::cout << "====================\n";
    
    BenchmarkConfig config;
    config.num_trials = 5;
    config.verbose = false;
    config.matrix_sizes = {64, 128, 256};
    
    MatrixBenchmarker<double> benchmarker(config);
    
    // Compare naive vs strassen
    auto naive = std::make_shared<NaiveAlgorithm<double>>();
    auto strassen = std::make_shared<StrassenAlgorithm<double>>();
    
    auto comparison = benchmarker.compare_algorithms(naive, strassen);
    
    std::cout << std::setw(10) << "Size" 
              << std::setw(20) << "Naive (ms)"
              << std::setw(20) << "Strassen (ms)"
              << std::setw(15) << "Speedup" << "\n";
    std::cout << std::string(65, '-') << "\n";
    
    for (const auto& pair : comparison) {
        size_type size = pair.first;
        const auto& result1 = pair.second.first;
        const auto& result2 = pair.second.second;
        
        double speedup = result1.average_time_ms / result2.average_time_ms;
        
        std::cout << std::setw(10) << size
                  << std::setw(20) << std::fixed << std::setprecision(3) << result1.average_time_ms
                  << std::setw(20) << std::fixed << std::setprecision(3) << result2.average_time_ms
                  << std::setw(15) << std::fixed << std::setprecision(2) << speedup << "x\n";
    }
    
    // Scaling analysis
    std::cout << "\n\nScaling Analysis\n";
    std::cout << "================\n";
    
    ScalabilityAnalyzer<double> analyzer;
    std::vector<size_type> sizes = {64, 128, 256, 512};
    auto scaling_result = analyzer.analyze_scaling(strassen, sizes);
    
    std::cout << "Algorithm: " << scaling_result.algorithm_name << "\n";
    std::cout << "Complexity Class: " << scaling_result.complexity_class << "\n";
    std::cout << "Scaling Factor: " << std::fixed << std::setprecision(3) 
              << scaling_result.scaling_factor << "\n\n";
    
    std::cout << std::setw(10) << "Size" 
              << std::setw(15) << "Time (ms)"
              << std::setw(15) << "GFLOPs" << "\n";
    std::cout << std::string(40, '-') << "\n";
    
    for (size_t i = 0; i < scaling_result.matrix_sizes.size(); ++i) {
        std::cout << std::setw(10) << scaling_result.matrix_sizes[i]
                  << std::setw(15) << std::fixed << std::setprecision(3) 
                  << scaling_result.execution_times[i]
                  << std::setw(15) << std::fixed << std::setprecision(2) 
                  << scaling_result.gflops[i] << "\n";
    }
    
    std::cout << "\nExample completed successfully!\n";
    return 0;
} 