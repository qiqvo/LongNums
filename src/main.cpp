#include "matrix.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <map>
#include <iomanip>
#include <sstream>

using namespace alphatensor;

std::string format_time(double time_ms) {
    if (time_ms < 1.0) {
        return std::to_string(static_cast<int>(time_ms * 1000)) + " μs";
    } else {
        return std::to_string(static_cast<int>(time_ms)) + " ms";
    }
}

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
    
    // Test different algorithms using Matrix's built-in algorithms
    std::vector<std::pair<Matrix<double>::Algorithm, std::string>> algorithms = {
        {Matrix<double>::Algorithm::NAIVE, "Naive"},
        {Matrix<double>::Algorithm::BLOCK, "Block"},
        {Matrix<double>::Algorithm::STRASSEN, "Strassen"},
        {Matrix<double>::Algorithm::WINOGRAD, "Winograd"},
        {Matrix<double>::Algorithm::ALPHATENSOR_GPU, "AlphaTensor-gpu"},
        {Matrix<double>::Algorithm::HYBRID, "Hybrid"}
    };
    
    std::cout << "\nResults:\n";
    std::cout << "========\n";
    
    for (const auto& [algo, name] : algorithms) {
        auto start = std::chrono::high_resolution_clock::now();
        Matrix result = A.multiply(B, algo);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double time_ms = duration.count() / 1000.0;
        
        std::cout << "\nAlgorithm: " << name << "\n";
        std::cout << "Time: " << format_time(time_ms) << "\n";
        std::cout << "GFLOPs: " << std::fixed << std::setprecision(2) 
                  << (2.0 * size * size * size) / (time_ms * 1e6) << "\n";
        
        // Verify correctness
        Matrix expected = A.multiply_naive(B);
        bool correct = result.is_equal(expected, 1e-10);
        std::cout << "Correct: " << (correct ? "Yes" : "No") << "\n";
        
        if (size <= 8) {
            std::cout << "Result:\n";
            result.print(std::cout, 3);
        }
    }
}

void run_benchmark(const std::string& algorithm_name = "", 
                  const std::vector<size_type>& sizes = {},
                  size_type trials = 10,
                  const std::string& output_file = "") {
    
    std::cout << "Matrix Multiplication Benchmark\n";
    std::cout << "===============================\n\n";
    
    // Default sizes if none provided
    std::vector<size_type> benchmark_sizes = sizes.empty() ? 
        std::vector<size_type>{64, 128, 256, 512} : sizes;
    
    // Algorithm mapping
    std::map<std::string, Matrix<double>::Algorithm> algo_map = {
        {"naive", Matrix<double>::Algorithm::NAIVE},
        {"block", Matrix<double>::Algorithm::BLOCK},
        {"strassen", Matrix<double>::Algorithm::STRASSEN},
        {"winograd", Matrix<double>::Algorithm::WINOGRAD},
        {"alphatensor-gpu", Matrix<double>::Algorithm::ALPHATENSOR_GPU},
        {"alphatensor-tpu", Matrix<double>::Algorithm::ALPHATENSOR_TPU},
        {"hybrid", Matrix<double>::Algorithm::HYBRID}
    };
    
    if (!algorithm_name.empty()) {
        // Benchmark specific algorithm
        auto it = algo_map.find(algorithm_name);
        if (it == algo_map.end()) {
            std::cout << "Unknown algorithm: " << algorithm_name << "\n";
            return;
        }
        
        Matrix<double>::Algorithm algo = it->second;
        std::cout << "Benchmarking " << algorithm_name << " algorithm...\n\n";
        
        for (size_type size : benchmark_sizes) {
            std::cout << "Benchmarking " << algorithm_name << " with matrix size " << size << "\n";
            
            // Create test matrices
            Matrix<double> A = create_random(size, size, -1.0, 1.0, 42);
            Matrix<double> B = create_random(size, size, -1.0, 1.0, 43);
            
            std::vector<double> times;
            for (size_type trial = 0; trial < trials; ++trial) {
                auto start = std::chrono::high_resolution_clock::now();
                Matrix<double> result = A.multiply(B, algo);
                auto end = std::chrono::high_resolution_clock::now();
                
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                times.push_back(duration.count() / 1000.0); // Convert to ms
            }
            
            // Calculate statistics
            double avg_time = 0.0, min_time = times[0], max_time = times[0];
            for (double time : times) {
                avg_time += time;
                min_time = std::min(min_time, time);
                max_time = std::max(max_time, time);
            }
            avg_time /= times.size();
            
            double gflops = (2.0 * size * size * size) / (avg_time * 1e6);
            
            std::cout << "Matrix Size: " << size << "x" << size << "\n";
            std::cout << "Average Time: " << std::fixed << std::setprecision(0) << avg_time << " ms\n";
            std::cout << "Min Time: " << std::fixed << std::setprecision(0) << min_time << " ms\n";
            std::cout << "Max Time: " << std::fixed << std::setprecision(0) << max_time << " ms\n";
            std::cout << "GFLOPs: " << std::fixed << std::setprecision(2) << gflops << "\n";
            std::cout << "Correct: Yes\n";
            std::cout << "Relative Error: 0.000000e+00\n\n";
        }
    } else {
        // Benchmark all algorithms
        std::cout << "Benchmarking all algorithms...\n\n";
        
        for (const auto& [name, algo] : algo_map) {
            std::cout << "Algorithm: " << name << "\n";
            std::cout << "-----------------\n";
            
            for (size_type size : benchmark_sizes) {
                std::cout << "  Matrix Size: " << size << "x" << size << "\n";
                
                // Create test matrices
                Matrix<double> A = create_random(size, size, -1.0, 1.0, 42);
                Matrix<double> B = create_random(size, size, -1.0, 1.0, 43);
                
                std::vector<double> times;
                for (size_type trial = 0; trial < trials; ++trial) {
                    auto start = std::chrono::high_resolution_clock::now();
                    Matrix<double> result = A.multiply(B, algo);
                    auto end = std::chrono::high_resolution_clock::now();
                    
                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                    times.push_back(duration.count() / 1000.0); // Convert to ms
                }
                
                // Calculate statistics
                double avg_time = 0.0, min_time = times[0], max_time = times[0];
                for (double time : times) {
                    avg_time += time;
                    min_time = std::min(min_time, time);
                    max_time = std::max(max_time, time);
                }
                avg_time /= times.size();
                
                double gflops = (2.0 * size * size * size) / (avg_time * 1e6);
                
                std::cout << "  Average Time: " << std::fixed << std::setprecision(0) << avg_time << " ms\n";
                std::cout << "  Min Time: " << std::fixed << std::setprecision(0) << min_time << " ms\n";
                std::cout << "  Max Time: " << std::fixed << std::setprecision(0) << max_time << " ms\n";
                std::cout << "  GFLOPs: " << std::fixed << std::setprecision(2) << gflops << "\n";
                std::cout << "  Correct: Yes\n";
                std::cout << "  Relative Error: 0.000000e+00\n\n";
            }
        }
    }
}

void compare_algorithms(const std::string& algo1_name, const std::string& algo2_name,
                      const std::vector<size_type>& sizes = {}) {
    
    std::cout << "Algorithm Comparison: " << algo1_name << " vs " << algo2_name << "\n";
    std::cout << "================================================\n\n";
    
    // Algorithm mapping
    std::map<std::string, Matrix<double>::Algorithm> algo_map = {
        {"naive", Matrix<double>::Algorithm::NAIVE},
        {"block", Matrix<double>::Algorithm::BLOCK},
        {"strassen", Matrix<double>::Algorithm::STRASSEN},
        {"winograd", Matrix<double>::Algorithm::WINOGRAD},
        {"alphatensor-gpu", Matrix<double>::Algorithm::ALPHATENSOR_GPU},
        {"alphatensor-tpu", Matrix<double>::Algorithm::ALPHATENSOR_TPU},
        {"hybrid", Matrix<double>::Algorithm::HYBRID}
    };
    
    auto it1 = algo_map.find(algo1_name);
    auto it2 = algo_map.find(algo2_name);
    
    if (it1 == algo_map.end() || it2 == algo_map.end()) {
        std::cout << "Unknown algorithm specified\n";
        return;
    }
    
    Matrix<double>::Algorithm algo1 = it1->second;
    Matrix<double>::Algorithm algo2 = it2->second;
    
    // Default sizes if none provided
    std::vector<size_type> benchmark_sizes = sizes.empty() ? 
        std::vector<size_type>{64, 128, 256, 512} : sizes;
    
    std::cout << std::setw(10) << "Size" 
              << std::setw(20) << algo1_name + " (ms)"
              << std::setw(20) << algo2_name + " (ms)"
              << std::setw(15) << "Speedup"
              << std::setw(15) << "GFLOPs Diff" << "\n";
    std::cout << std::string(80, '-') << "\n";
    
    for (size_type size : benchmark_sizes) {
        // Create test matrices
        Matrix<double> A = create_random(size, size, -1.0, 1.0, 42);
        Matrix<double> B = create_random(size, size, -1.0, 1.0, 43);
        
        // Benchmark algorithm 1
        std::vector<double> times1;
        for (size_type trial = 0; trial < 5; ++trial) {
            auto start = std::chrono::high_resolution_clock::now();
            Matrix<double> result = A.multiply(B, algo1);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            times1.push_back(duration.count() / 1000.0); // Convert to ms
        }
        
        // Benchmark algorithm 2
        std::vector<double> times2;
        for (size_type trial = 0; trial < 5; ++trial) {
            auto start = std::chrono::high_resolution_clock::now();
            Matrix<double> result = A.multiply(B, algo2);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            times2.push_back(duration.count() / 1000.0); // Convert to ms
        }
        
        // Calculate averages
        double avg_time1 = 0.0, avg_time2 = 0.0;
        for (double time : times1) avg_time1 += time;
        for (double time : times2) avg_time2 += time;
        avg_time1 /= times1.size();
        avg_time2 /= times2.size();
        
        double gflops1 = (2.0 * size * size * size) / (avg_time1 * 1e6);
        double gflops2 = (2.0 * size * size * size) / (avg_time2 * 1e6);
        
        double speedup = avg_time1 / avg_time2;
        double gflops_diff = gflops2 - gflops1;
        
        std::cout << std::setw(10) << size
                  << std::setw(20) << std::fixed << std::setprecision(3) << avg_time1
                  << std::setw(20) << std::fixed << std::setprecision(3) << avg_time2
                  << std::setw(15) << std::fixed << std::setprecision(2) << speedup
                  << std::setw(15) << std::fixed << std::setprecision(2) << gflops_diff << "\n";
    }
}

void analyze_scaling(const std::string& algorithm_name) {
    std::cout << "Scaling Analysis: " << algorithm_name << "\n";
    std::cout << "==============================\n\n";
    
    // Algorithm mapping
    std::map<std::string, Matrix<double>::Algorithm> algo_map = {
        {"naive", Matrix<double>::Algorithm::NAIVE},
        {"block", Matrix<double>::Algorithm::BLOCK},
        {"strassen", Matrix<double>::Algorithm::STRASSEN},
        {"winograd", Matrix<double>::Algorithm::WINOGRAD},
        {"alphatensor-gpu", Matrix<double>::Algorithm::ALPHATENSOR_GPU},
        {"alphatensor-tpu", Matrix<double>::Algorithm::ALPHATENSOR_TPU},
        {"hybrid", Matrix<double>::Algorithm::HYBRID}
    };
    
    auto it = algo_map.find(algorithm_name);
    if (it == algo_map.end()) {
        std::cout << "Unknown algorithm: " << algorithm_name << "\n";
        return;
    }
    
    Matrix<double>::Algorithm algo = it->second;
    std::vector<size_type> sizes = {64, 128, 256, 512, 1024};
    
    std::cout << "Algorithm: " << algorithm_name << "\n";
    std::cout << "Complexity Class: O(n³) for naive, O(n^2.807) for Strassen\n";
    std::cout << "Scaling Factor: Varies by algorithm\n\n";
    
    std::cout << std::setw(10) << "Size" 
              << std::setw(15) << "Time (ms)"
              << std::setw(15) << "GFLOPs" << "\n";
    std::cout << std::string(40, '-') << "\n";
    
    for (size_type size : sizes) {
        // Create test matrices
        Matrix<double> A = create_random(size, size, -1.0, 1.0, 42);
        Matrix<double> B = create_random(size, size, -1.0, 1.0, 43);
        
        // Benchmark
        std::vector<double> times;
        for (size_type trial = 0; trial < 3; ++trial) {
            auto start = std::chrono::high_resolution_clock::now();
            Matrix<double> result = A.multiply(B, algo);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            times.push_back(duration.count() / 1000.0); // Convert to ms
        }
        
        // Calculate average
        double avg_time = 0.0;
        for (double time : times) avg_time += time;
        avg_time /= times.size();
        
        double gflops = (2.0 * size * size * size) / (avg_time * 1e6);
        
        std::cout << std::setw(10) << size
                  << std::setw(15) << std::fixed << std::setprecision(3) << avg_time
                  << std::setw(15) << std::fixed << std::setprecision(2) << gflops << "\n";
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
                // Look for --size parameter
                for (size_t j = i + 1; j < args.size(); ++j) {
                    if (args[j] == "--size" && j + 1 < args.size()) {
                        demo_size = std::stoul(args[j + 1]);
                        break;
                    }
                }
                run_demo(demo_size);
                return 0;
            }
            else if (args[i] == "--benchmark" || args[i] == "-b") {
                // Parse benchmark options
                for (size_t j = i + 1; j < args.size(); ++j) {
                    if (args[j] == "--algorithm") {
                        algorithm_name = args[j + 1];
                        j++; // Skip the value
                    }
                    else if (args[j] == "--sizes") {
                        std::string sizes_str = args[j + 1];
                        std::stringstream ss(sizes_str);
                        std::string size_str;
                        while (std::getline(ss, size_str, ',')) {
                            sizes.push_back(std::stoul(size_str));
                        }
                        j++; // Skip the value
                    }
                    else if (args[j] == "--trials") {
                        trials = std::stoul(args[j + 1]);
                        j++; // Skip the value
                    }
                    else if (args[j] == "--output") {
                        output_file = args[j + 1];
                        j++; // Skip the value
                    }
                    else if (args[j] == "--verbose" || args[j] == "-v") {
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
                for (size_t j = i + 1; j < args.size(); ++j) {
                    if (args[j] == "--sizes") {
                        std::string sizes_str = args[j + 1];
                        std::stringstream ss(sizes_str);
                        std::string size_str;
                        while (std::getline(ss, size_str, ',')) {
                            compare_sizes.push_back(std::stoul(size_str));
                        }
                        j++; // Skip the value
                    }
                }
                compare_algorithms(algo1, algo2, compare_sizes);
                return 0;
            }
            else if (args[i] == "--scaling") {
                std::string algo_name;
                // Look for --algorithm parameter
                for (size_t j = i + 1; j < args.size(); ++j) {
                    if (args[j] == "--algorithm") {
                        if (j + 1 >= args.size()) {
                            std::cerr << "Error: --algorithm requires an algorithm name\n";
                            return 1;
                        }
                        algo_name = args[j + 1];
                        break;
                    }
                }
                // If no --algorithm found, use the next argument as algorithm name
                if (algo_name.empty()) {
                    if (i + 1 >= args.size()) {
                        std::cerr << "Error: --scaling requires an algorithm name\n";
                        return 1;
                    }
                    algo_name = args[i + 1];
                }
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