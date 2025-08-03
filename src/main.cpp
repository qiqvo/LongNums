#include "matrix.h"
#include "algorithms.h"
#include "benchmark.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace alphatensor;

void print_usage() {
    std::cout << "AlphaTensor C++ Matrix Multiplication\n";
    std::cout << "=====================================\n\n";
    std::cout << "Usage: alphatensor_cpp [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --help, -h              Show this help message\n";
    std::cout << "  --demo, -d              Run demonstration\n";
    std::cout << "  --benchmark, -b         Run comprehensive benchmark\n";
    std::cout << "  --algorithm <name>      Specify algorithm (naive, block, strassen, winograd, alphatensor-gpu, alphatensor-tpu, hybrid)\n";
    std::cout << "  --size <n>              Matrix size for demo\n";
    std::cout << "  --trials <n>            Number of benchmark trials\n";
    std::cout << "  --sizes <list>          Comma-separated list of matrix sizes for benchmark\n";
    std::cout << "  --output <file>         Output file for results\n";
    std::cout << "  --verbose, -v           Verbose output\n";
    std::cout << "  --compare <algo1> <algo2>  Compare two algorithms\n";
    std::cout << "  --scaling               Analyze algorithm scaling\n";
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "  alphatensor_cpp --demo\n";
    std::cout << "  alphatensor_cpp --benchmark --algorithm strassen --sizes 64,128,256,512\n";
    std::cout << "  alphatensor_cpp --compare naive strassen --size 256\n";
    std::cout << "  alphatensor_cpp --scaling --algorithm hybrid\n";
}

void run_demo(size_type size = 4) {
    std::cout << "Matrix Multiplication Demo\n";
    std::cout << "=========================\n\n";
    
    // Create test matrices
    Matrix A = create_random(size, size, -1.0, 1.0, 42);
    Matrix B = create_random(size, size, -1.0, 1.0, 43);
    
    std::cout << "Matrix A (" << size << "x" << size << "):\n";
    A.print(std::cout, 3);
    std::cout << "\nMatrix B (" << size << "x" << size << "):\n";
    B.print(std::cout, 3);
    
    // Test different algorithms
    std::vector<std::shared_ptr<MatrixMultiplicationAlgorithm<double>>> algorithms = {
        std::make_shared<NaiveAlgorithm<double>>(),
        std::make_shared<BlockAlgorithm<double>>(),
        std::make_shared<StrassenAlgorithm<double>>(),
        std::make_shared<WinogradAlgorithm<double>>(),
        std::make_shared<AlphaTensorAlgorithm<double>>("gpu"),
        std::make_shared<HybridAlgorithm<double>>()
    };
    
    std::cout << "\nResults:\n";
    std::cout << "========\n";
    
    for (const auto& algorithm : algorithms) {
        if (algorithm->can_handle(size, size, size, size)) {
            auto start = std::chrono::high_resolution_clock::now();
            Matrix result = algorithm->multiply(A, B);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            double time_ms = duration.count() / 1000.0;
            
            std::cout << "\nAlgorithm: " << algorithm->name() << "\n";
            std::cout << "Time: " << format_time(time_ms) << "\n";
            std::cout << "GFLOPs: " << std::fixed << std::setprecision(2) 
                      << (2.0 * size * size * size) / (time_ms * 1e6) << "\n";
            
            // Verify correctness
            Matrix expected = NaiveAlgorithm<double>().multiply(A, B);
            bool correct = result.is_equal(expected, 1e-10);
            std::cout << "Correct: " << (correct ? "Yes" : "No") << "\n";
            
            if (size <= 8) {
                std::cout << "Result:\n";
                result.print(std::cout, 3);
            }
        }
    }
}

void run_benchmark(const std::string& algorithm_name = "", 
                  const std::vector<size_type>& sizes = {},
                  size_type trials = 10,
                  const std::string& output_file = "") {
    
    std::cout << "Matrix Multiplication Benchmark\n";
    std::cout << "===============================\n\n";
    
    BenchmarkConfig config;
    config.num_trials = trials;
    config.verbose = true;
    config.save_results = !output_file.empty();
    config.output_file = output_file;
    
    if (!sizes.empty()) {
        config.matrix_sizes = sizes;
    }
    
    MatrixBenchmarker<double> benchmarker(config);
    
    if (!algorithm_name.empty()) {
        // Benchmark specific algorithm
        auto algorithm = AlgorithmFactory<double>::create(algorithm_name);
        std::cout << "Benchmarking " << algorithm->name() << " algorithm...\n\n";
        
        auto results = benchmarker.benchmark_algorithms({algorithm});
        std::cout << benchmarker.generate_report(results);
        
        if (!output_file.empty()) {
            benchmarker.save_results_to_csv(results, output_file);
            std::cout << "Results saved to: " << output_file << "\n";
        }
    } else {
        // Benchmark all algorithms
        std::cout << "Benchmarking all algorithms...\n\n";
        
        auto results = benchmarker.benchmark_all_algorithms();
        std::cout << benchmarker.generate_report(results);
        
        if (!output_file.empty()) {
            benchmarker.save_results_to_csv(results, output_file);
            std::cout << "Results saved to: " << output_file << "\n";
        }
    }
}

void compare_algorithms(const std::string& algo1_name, const std::string& algo2_name,
                      const std::vector<size_type>& sizes = {}) {
    
    std::cout << "Algorithm Comparison: " << algo1_name << " vs " << algo2_name << "\n";
    std::cout << "================================================\n\n";
    
    auto algo1 = AlgorithmFactory<double>::create(algo1_name);
    auto algo2 = AlgorithmFactory<double>::create(algo2_name);
    
    BenchmarkConfig config;
    config.num_trials = 5;
    config.verbose = false;
    
    if (!sizes.empty()) {
        config.matrix_sizes = sizes;
    }
    
    MatrixBenchmarker<double> benchmarker(config);
    auto comparison = benchmarker.compare_algorithms(algo1, algo2);
    
    std::cout << std::setw(10) << "Size" 
              << std::setw(20) << algo1->name() + " (ms)"
              << std::setw(20) << algo2->name() + " (ms)"
              << std::setw(15) << "Speedup"
              << std::setw(15) << "GFLOPs Diff" << "\n";
    std::cout << std::string(80, '-') << "\n";
    
    for (const auto& pair : comparison) {
        size_type size = pair.first;
        const auto& result1 = pair.second.first;
        const auto& result2 = pair.second.second;
        
        double speedup = result1.average_time_ms / result2.average_time_ms;
        double gflops_diff = result2.gflops - result1.gflops;
        
        std::cout << std::setw(10) << size
                  << std::setw(20) << std::fixed << std::setprecision(3) << result1.average_time_ms
                  << std::setw(20) << std::fixed << std::setprecision(3) << result2.average_time_ms
                  << std::setw(15) << std::fixed << std::setprecision(2) << speedup
                  << std::setw(15) << std::fixed << std::setprecision(2) << gflops_diff << "\n";
    }
}

void analyze_scaling(const std::string& algorithm_name) {
    std::cout << "Scaling Analysis: " << algorithm_name << "\n";
    std::cout << "==============================\n\n";
    
    auto algorithm = AlgorithmFactory<double>::create(algorithm_name);
    ScalabilityAnalyzer<double> analyzer;
    
    std::vector<size_type> sizes = {64, 128, 256, 512, 1024};
    auto scaling_result = analyzer.analyze_scaling(algorithm, sizes);
    
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
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);
    
    if (args.empty() || args[0] == "--help" || args[0] == "-h") {
        print_usage();
        return 0;
    }
    
    try {
        std::string algorithm_name;
        std::vector<size_type> sizes;
        size_type trials = 10;
        std::string output_file;
        bool verbose = false;
        
        for (size_t i = 0; i < args.size(); ++i) {
            if (args[i] == "--demo" || args[i] == "-d") {
                size_type demo_size = 4;
                if (i + 1 < args.size() && args[i + 1].substr(0, 2) != "--") {
                    demo_size = std::stoul(args[++i]);
                }
                run_demo(demo_size);
                return 0;
            }
            else if (args[i] == "--benchmark" || args[i] == "-b") {
                // Parse benchmark options
                while (i + 1 < args.size() && args[i + 1].substr(0, 2) != "--") {
                    ++i;
                    if (args[i] == "--algorithm") {
                        algorithm_name = args[++i];
                    }
                    else if (args[i] == "--sizes") {
                        std::string sizes_str = args[++i];
                        std::stringstream ss(sizes_str);
                        std::string size_str;
                        while (std::getline(ss, size_str, ',')) {
                            sizes.push_back(std::stoul(size_str));
                        }
                    }
                    else if (args[i] == "--trials") {
                        trials = std::stoul(args[++i]);
                    }
                    else if (args[i] == "--output") {
                        output_file = args[++i];
                    }
                    else if (args[i] == "--verbose" || args[i] == "-v") {
                        verbose = true;
                    }
                }
                run_benchmark(algorithm_name, sizes, trials, output_file);
                return 0;
            }
            else if (args[i] == "--compare") {
                if (i + 2 >= args.size()) {
                    std::cerr << "Error: --compare requires two algorithm names\n";
                    return 1;
                }
                std::string algo1 = args[++i];
                std::string algo2 = args[++i];
                
                // Parse sizes for comparison
                std::vector<size_type> compare_sizes;
                while (i + 1 < args.size() && args[i + 1].substr(0, 2) != "--") {
                    ++i;
                    if (args[i] == "--sizes") {
                        std::string sizes_str = args[++i];
                        std::stringstream ss(sizes_str);
                        std::string size_str;
                        while (std::getline(ss, size_str, ',')) {
                            compare_sizes.push_back(std::stoul(size_str));
                        }
                    }
                }
                compare_algorithms(algo1, algo2, compare_sizes);
                return 0;
            }
            else if (args[i] == "--scaling") {
                if (i + 1 >= args.size()) {
                    std::cerr << "Error: --scaling requires an algorithm name\n";
                    return 1;
                }
                std::string algo_name = args[++i];
                analyze_scaling(algo_name);
                return 0;
            }
        }
        
        // Default: run demo
        run_demo();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
} 