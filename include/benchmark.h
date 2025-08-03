#pragma once

#include "algorithms.h"
#include <chrono>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <functional>

namespace alphatensor {

/**
 * @brief Benchmark result structure
 */
struct BenchmarkResult {
    std::string algorithm_name;
    size_type matrix_size;
    double average_time_ms;
    double min_time_ms;
    double max_time_ms;
    double std_dev_ms;
    size_type num_trials;
    bool is_correct;
    double relative_error;
    
    // Memory usage (if available)
    size_type memory_usage_bytes;
    
    // Performance metrics
    double gflops;
    double memory_bandwidth_gbps;
};

/**
 * @brief Benchmark configuration
 */
struct BenchmarkConfig {
    std::vector<size_type> matrix_sizes = {64, 128, 256, 512, 1024, 2048};
    size_type num_trials = 10;
    size_type warmup_trials = 3;
    bool check_correctness = true;
    double tolerance = 1e-10;
    bool use_random_matrices = true;
    unsigned random_seed = 42;
    bool verbose = false;
    bool save_results = false;
    std::string output_file = "benchmark_results.csv";
};

/**
 * @brief Matrix multiplication benchmarker
 */
class MatrixBenchmarker {
public:
    explicit MatrixBenchmarker(const BenchmarkConfig& config = BenchmarkConfig{});
    
    /**
     * @brief Benchmark a single algorithm
     * @param algorithm The algorithm to benchmark
     * @param matrix_size Size of square matrices to test
     * @return Benchmark result
     */
    BenchmarkResult benchmark_algorithm(
        const std::shared_ptr<MatrixMultiplicationAlgorithm>& algorithm,
        size_type matrix_size);
    
    /**
     * @brief Benchmark multiple algorithms
     * @param algorithms Vector of algorithms to benchmark
     * @param matrix_sizes Vector of matrix sizes to test
     * @return Vector of benchmark results
     */
    std::vector<BenchmarkResult> benchmark_algorithms(
        const std::vector<std::shared_ptr<MatrixMultiplicationAlgorithm>>& algorithms,
        const std::vector<size_type>& matrix_sizes = {});
    
    /**
     * @brief Benchmark all available algorithms
     * @return Vector of benchmark results
     */
    std::vector<BenchmarkResult> benchmark_all_algorithms();
    
    /**
     * @brief Compare two algorithms
     * @param algo1 First algorithm
     * @param algo2 Second algorithm
     * @param matrix_sizes Matrix sizes to test
     * @return Comparison results
     */
    std::map<size_type, std::pair<BenchmarkResult, BenchmarkResult>> compare_algorithms(
        const std::shared_ptr<MatrixMultiplicationAlgorithm>& algo1,
        const std::shared_ptr<MatrixMultiplicationAlgorithm>& algo2,
        const std::vector<size_type>& matrix_sizes = {});
    
    /**
     * @brief Generate performance report
     * @param results Benchmark results
     * @return Formatted report string
     */
    std::string generate_report(const std::vector<BenchmarkResult>& results) const;
    
    /**
     * @brief Save results to CSV file
     * @param results Benchmark results
     * @param filename Output filename
     */
    void save_results_to_csv(const std::vector<BenchmarkResult>& results,
                           const std::string& filename = "") const;
    
    /**
     * @brief Plot results (if matplotlib is available)
     * @param results Benchmark results
     * @param filename Output filename
     */
    void plot_results(const std::vector<BenchmarkResult>& results,
                     const std::string& filename = "benchmark_plot.png") const;
    
    /**
     * @brief Get the benchmark configuration
     * @return Current configuration
     */
    const BenchmarkConfig& get_config() const { return config_; }
    
    /**
     * @brief Set the benchmark configuration
     * @param config New configuration
     */
    void set_config(const BenchmarkConfig& config) { config_ = config; }
    
private:
    BenchmarkConfig config_;
    
    // Helper methods
    Matrix generate_test_matrix(size_type size) const;
    std::pair<Matrix, Matrix> generate_test_matrices(size_type size) const;
    double measure_execution_time(const std::function<void()>& func) const;
    bool verify_correctness(const Matrix& A, const Matrix& B, const Matrix& result) const;
    double calculate_gflops(size_type matrix_size, double time_ms) const;
    double calculate_memory_bandwidth(size_type matrix_size, double time_ms) const;
    
    // Statistical calculations
    double calculate_mean(const std::vector<double>& values) const;
    double calculate_std_dev(const std::vector<double>& values, double mean) const;
    double calculate_min(const std::vector<double>& values) const;
    double calculate_max(const std::vector<double>& values) const;
};

/**
 * @brief Performance profiler for detailed algorithm analysis
 */
class PerformanceProfiler {
public:
    struct ProfileResult {
        std::string algorithm_name;
        size_type matrix_size;
        double total_time_ms;
        double computation_time_ms;
        double memory_allocation_time_ms;
        double data_transfer_time_ms;
        size_type memory_allocations;
        size_type cache_misses;
        double cpu_utilization;
    };
    
    explicit PerformanceProfiler(bool enable_detailed_profiling = false);
    
    /**
     * @brief Profile an algorithm
     * @param algorithm Algorithm to profile
     * @param matrix_size Matrix size to test
     * @return Detailed profile result
     */
    ProfileResult profile_algorithm(
        const std::shared_ptr<MatrixMultiplicationAlgorithm>& algorithm,
        size_type matrix_size);
    
    /**
     * @brief Get memory usage statistics
     * @return Memory usage in bytes
     */
    size_type get_memory_usage() const;
    
    /**
     * @brief Reset profiling counters
     */
    void reset_counters();
    
private:
    bool enable_detailed_profiling_;
    size_type memory_allocations_;
    size_type cache_misses_;
    
    // Platform-specific profiling methods
    double get_cpu_utilization() const;
    size_type get_cache_misses() const;
};

/**
 * @brief Scalability analyzer for algorithm performance scaling
 */
class ScalabilityAnalyzer {
public:
    struct ScalingResult {
        std::string algorithm_name;
        std::vector<size_type> matrix_sizes;
        std::vector<double> execution_times;
        std::vector<double> gflops;
        double scaling_factor;
        std::string complexity_class;
    };
    
    /**
     * @brief Analyze algorithm scaling
     * @param algorithm Algorithm to analyze
     * @param matrix_sizes Range of matrix sizes
     * @return Scaling analysis result
     */
    ScalingResult analyze_scaling(
        const std::shared_ptr<MatrixMultiplicationAlgorithm>& algorithm,
        const std::vector<size_type>& matrix_sizes);
    
    /**
     * @brief Determine algorithm complexity class
     * @param matrix_sizes Matrix sizes tested
     * @param execution_times Corresponding execution times
     * @return Complexity class string
     */
    std::string determine_complexity_class(
        const std::vector<size_type>& matrix_sizes,
        const std::vector<double>& execution_times) const;
    
    /**
     * @brief Calculate scaling factor
     * @param matrix_sizes Matrix sizes
     * @param execution_times Execution times
     * @return Scaling factor
     */
    double calculate_scaling_factor(
        const std::vector<size_type>& matrix_sizes,
        const std::vector<double>& execution_times) const;
};

// Utility functions
std::string format_time(double time_ms);
std::string format_size(size_type bytes);
std::string format_gflops(double gflops);

} // namespace alphatensor 