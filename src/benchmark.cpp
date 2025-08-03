#ifndef ALPHATENSOR_BENCHMARK_FUNCTIONS
#include "benchmark.h"

// Non-template (regular) function implementations go here
namespace alphatensor {

// Utility functions
std::string format_time(double time_ms) {
    if (time_ms < 1.0) {
        return std::to_string(static_cast<int>(time_ms * 1000)) + " μs";
    } else if (time_ms < 1000.0) {
        return std::to_string(static_cast<int>(time_ms)) + " ms";
    } else {
        return std::to_string(static_cast<int>(time_ms / 1000)) + " s";
    }
}

std::string format_size(size_type bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit_index = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024.0 && unit_index < 4) {
        size /= 1024.0;
        unit_index++;
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << " " << units[unit_index];
    return oss.str();
}

std::string format_gflops(double gflops) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << gflops << " GFLOPs";
    return oss.str();
}

} // namespace alphatensor

#else

namespace alphatensor {

// Template function implementations

template<typename T>
MatrixBenchmarker<T>::MatrixBenchmarker(const BenchmarkConfig& config) : config_(config) {}

template<typename T>
BenchmarkResult<T> MatrixBenchmarker<T>::benchmark_algorithm(
    const std::shared_ptr<AlgorithmType>& algorithm,
    size_type matrix_size) {
    
    BenchmarkResult<T> result;
    result.algorithm_name = algorithm->name();
    result.matrix_size = matrix_size;
    result.num_trials = config_.num_trials;
    
    std::vector<double> times;
    times.reserve(config_.num_trials);
    
    // Generate test matrices
    auto test_matrices = generate_test_matrices(matrix_size);
    MatrixType& A = test_matrices.first;
    MatrixType& B = test_matrices.second;
    
    // Warmup runs
    for (size_type i = 0; i < config_.warmup_trials; ++i) {
        algorithm->multiply(A, B);
    }
    
    // Actual benchmark runs
    for (size_type i = 0; i < config_.num_trials; ++i) {
        double time = measure_execution_time([&]() {
            algorithm->multiply(A, B);
        });
        times.push_back(time);
    }
    
    // Calculate statistics
    result.average_time_ms = calculate_mean(times);
    result.min_time_ms = calculate_min(times);
    result.max_time_ms = calculate_max(times);
    result.std_dev_ms = calculate_std_dev(times, result.average_time_ms);
    
    // Calculate performance metrics
    result.gflops = calculate_gflops(matrix_size, result.average_time_ms);
    result.memory_bandwidth_gbps = calculate_memory_bandwidth(matrix_size, result.average_time_ms);
    
    // Verify correctness if requested
    if (config_.check_correctness) {
        MatrixType result_matrix = algorithm->multiply(A, B);
        MatrixType expected = NaiveAlgorithm<T>().multiply(A, B);
        result.is_correct = verify_correctness(A, B, result_matrix);
        
        if (result.is_correct) {
            // Calculate relative error
            double max_error = 0.0;
            for (size_type i = 0; i < result_matrix.rows(); ++i) {
                for (size_type j = 0; j < result_matrix.cols(); ++j) {
                    double error = std::abs(result_matrix(i, j) - expected(i, j));
                    if (expected(i, j) != 0.0) {
                        error /= std::abs(expected(i, j));
                    }
                    max_error = std::max(max_error, error);
                }
            }
            result.relative_error = max_error;
        } else {
            result.relative_error = std::numeric_limits<double>::infinity();
        }
    } else {
        result.is_correct = true;
        result.relative_error = 0.0;
    }
    
    return result;
}

template<typename T>
std::vector<BenchmarkResult<T>> MatrixBenchmarker<T>::benchmark_algorithms(
    const std::vector<std::shared_ptr<AlgorithmType>>& algorithms,
    const std::vector<size_type>& matrix_sizes) {
    
    std::vector<size_type> sizes = matrix_sizes.empty() ? config_.matrix_sizes : matrix_sizes;
    std::vector<BenchmarkResult<T>> results;
    
    for (const auto& algorithm : algorithms) {
        for (size_type size : sizes) {
            if (algorithm->can_handle(size, size, size, size)) {
                if (config_.verbose) {
                    std::cout << "Benchmarking " << algorithm->name() 
                              << " with matrix size " << size << std::endl;
                }
                results.push_back(benchmark_algorithm(algorithm, size));
            }
        }
    }
    
    return results;
}

template<typename T>
std::vector<BenchmarkResult<T>> MatrixBenchmarker<T>::benchmark_all_algorithms() {
    std::vector<std::shared_ptr<AlgorithmType>> algorithms = {
        std::make_shared<NaiveAlgorithm<T>>(),
        std::make_shared<BlockAlgorithm<T>>(),
        std::make_shared<StrassenAlgorithm<T>>(),
        std::make_shared<WinogradAlgorithm<T>>(),
        std::make_shared<AlphaTensorAlgorithm<T>>("gpu"),
        std::make_shared<AlphaTensorAlgorithm<T>>("tpu"),
        std::make_shared<HybridAlgorithm<T>>()
    };
    
    return benchmark_algorithms(algorithms);
}

template<typename T>
std::map<size_type, std::pair<BenchmarkResult<T>, BenchmarkResult<T>>> MatrixBenchmarker<T>::compare_algorithms(
    const std::shared_ptr<AlgorithmType>& algo1,
    const std::shared_ptr<AlgorithmType>& algo2,
    const std::vector<size_type>& matrix_sizes) {
    
    std::vector<size_type> sizes = matrix_sizes.empty() ? config_.matrix_sizes : matrix_sizes;
    std::map<size_type, std::pair<BenchmarkResult<T>, BenchmarkResult<T>>> comparison;
    
    for (size_type size : sizes) {
        if (algo1->can_handle(size, size, size, size) && 
            algo2->can_handle(size, size, size, size)) {
            BenchmarkResult<T> result1 = benchmark_algorithm(algo1, size);
            BenchmarkResult<T> result2 = benchmark_algorithm(algo2, size);
            comparison[size] = std::make_pair(result1, result2);
        }
    }
    
    return comparison;
}

template<typename T>
std::string MatrixBenchmarker<T>::generate_report(const std::vector<BenchmarkResult<T>>& results) const {
    std::ostringstream report;
    report << "Matrix Multiplication Benchmark Report\n";
    report << "=====================================\n\n";
    
    // Group results by algorithm
    std::map<std::string, std::vector<BenchmarkResult<T>>> grouped_results;
    for (const auto& result : results) {
        grouped_results[result.algorithm_name].push_back(result);
    }
    
    for (const auto& group : grouped_results) {
        report << "Algorithm: " << group.first << "\n";
        report << "-" << std::string(group.first.length() + 10, '-') << "\n";
        
        for (const auto& result : group.second) {
            report << "  Matrix Size: " << result.matrix_size << "x" << result.matrix_size << "\n";
            report << "  Average Time: " << format_time(result.average_time_ms) << "\n";
            report << "  Min Time: " << format_time(result.min_time_ms) << "\n";
            report << "  Max Time: " << format_time(result.max_time_ms) << "\n";
            report << "  Std Dev: " << format_time(result.std_dev_ms) << "\n";
            report << "  GFLOPs: " << std::fixed << std::setprecision(2) << result.gflops << "\n";
            report << "  Memory Bandwidth: " << std::fixed << std::setprecision(2) 
                   << result.memory_bandwidth_gbps << " GB/s\n";
            report << "  Correct: " << (result.is_correct ? "Yes" : "No") << "\n";
            if (result.is_correct) {
                report << "  Relative Error: " << std::scientific << std::setprecision(6) 
                       << result.relative_error << "\n";
            }
            report << "\n";
        }
    }
    
    return report.str();
}

template<typename T>
void MatrixBenchmarker<T>::save_results_to_csv(const std::vector<BenchmarkResult<T>>& results,
                           const std::string& filename) const {
    std::string output_file = filename.empty() ? config_.output_file : filename;
    std::ofstream file(output_file);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + output_file);
    }
    
    // Write header
    file << "Algorithm,MatrixSize,AverageTime(ms),MinTime(ms),MaxTime(ms),StdDev(ms),"
         << "GFLOPs,MemoryBandwidth(GB/s),Correct,RelativeError\n";
    
    // Write data
    for (const auto& result : results) {
        file << result.algorithm_name << ","
             << result.matrix_size << ","
             << result.average_time_ms << ","
             << result.min_time_ms << ","
             << result.max_time_ms << ","
             << result.std_dev_ms << ","
             << result.gflops << ","
             << result.memory_bandwidth_gbps << ","
             << (result.is_correct ? "Yes" : "No") << ","
             << result.relative_error << "\n";
    }
}

template<typename T>
void MatrixBenchmarker<T>::plot_results(const std::vector<BenchmarkResult<T>>& results,
                      const std::string& filename) const {
    // This would typically use a plotting library like matplotlib
    // For now, we'll just create a simple text-based visualization
    
    std::cout << "Performance Comparison Chart\n";
    std::cout << "===========================\n\n";
    
    // Group by matrix size
    std::map<size_type, std::vector<BenchmarkResult<T>>> size_groups;
    for (const auto& result : results) {
        size_groups[result.matrix_size].push_back(result);
    }
    
    for (const auto& size_group : size_groups) {
        std::cout << "Matrix Size: " << size_group.first << "x" << size_group.first << "\n";
        std::cout << std::string(50, '-') << "\n";
        
        for (const auto& result : size_group.second) {
            std::cout << std::setw(20) << std::left << result.algorithm_name
                      << std::setw(15) << std::right << format_time(result.average_time_ms)
                      << std::setw(10) << std::right << std::fixed << std::setprecision(2) 
                      << result.gflops << " GFLOPs\n";
        }
        std::cout << "\n";
    }
}

template<typename T>
const BenchmarkConfig& MatrixBenchmarker<T>::get_config() const { return config_; }

template<typename T>
void MatrixBenchmarker<T>::set_config(const BenchmarkConfig& config) { config_ = config; }

template<typename T>
typename MatrixBenchmarker<T>::MatrixType MatrixBenchmarker<T>::generate_test_matrix(size_type size) const {
    if (config_.use_random_matrices) {
        return create_random<T>(size, size, -1.0, 1.0, config_.random_seed);
    } else {
        MatrixType matrix(size, size);
        for (size_type i = 0; i < size; ++i) {
            for (size_type j = 0; j < size; ++j) {
                matrix(i, j) = static_cast<T>(i + j);
            }
        }
        return matrix;
    }
}

template<typename T>
std::pair<typename MatrixBenchmarker<T>::MatrixType, typename MatrixBenchmarker<T>::MatrixType> 
MatrixBenchmarker<T>::generate_test_matrices(size_type size) const {
    MatrixType A = generate_test_matrix(size);
    MatrixType B = generate_test_matrix(size);
    return std::make_pair(A, B);
}

template<typename T>
double MatrixBenchmarker<T>::measure_execution_time(const std::function<void()>& func) const {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1000.0; // Convert to milliseconds
}

template<typename T>
bool MatrixBenchmarker<T>::verify_correctness(const MatrixType& A, const MatrixType& B, const MatrixType& result) const {
    MatrixType expected = NaiveAlgorithm<T>().multiply(A, B);
    
    for (size_type i = 0; i < result.rows(); ++i) {
        for (size_type j = 0; j < result.cols(); ++j) {
            if (std::abs(result(i, j) - expected(i, j)) > config_.tolerance) {
                return false;
            }
        }
    }
    return true;
}

template<typename T>
double MatrixBenchmarker<T>::calculate_gflops(size_type matrix_size, double time_ms) const {
    // GFLOPs = (2 * n^3) / (time_ms * 1e6)
    double operations = 2.0 * matrix_size * matrix_size * matrix_size;
    return operations / (time_ms * 1e6);
}

template<typename T>
double MatrixBenchmarker<T>::calculate_memory_bandwidth(size_type matrix_size, double time_ms) const {
    // Memory bandwidth = (3 * n^2 * sizeof(T)) / (time_ms * 1e6)
    double bytes = 3.0 * matrix_size * matrix_size * sizeof(T);
    return bytes / (time_ms * 1e6) / 1e9; // Convert to GB/s
}

template<typename T>
double MatrixBenchmarker<T>::calculate_mean(const std::vector<double>& values) const {
    return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

template<typename T>
double MatrixBenchmarker<T>::calculate_std_dev(const std::vector<double>& values, double mean) const {
    double sum_squares = 0.0;
    for (double value : values) {
        double diff = value - mean;
        sum_squares += diff * diff;
    }
    return std::sqrt(sum_squares / values.size());
}

template<typename T>
double MatrixBenchmarker<T>::calculate_min(const std::vector<double>& values) const {
    return *std::min_element(values.begin(), values.end());
}

template<typename T>
double MatrixBenchmarker<T>::calculate_max(const std::vector<double>& values) const {
    return *std::max_element(values.begin(), values.end());
}

template<typename T>
PerformanceProfiler<T>::PerformanceProfiler(bool enable_detailed_profiling)
    : enable_detailed_profiling_(enable_detailed_profiling),
      memory_allocations_(0), cache_misses_(0) {}

template<typename T>
typename PerformanceProfiler<T>::ProfileResult PerformanceProfiler<T>::profile_algorithm(
    const std::shared_ptr<AlgorithmType>& algorithm,
    size_type matrix_size) {
    
    ProfileResult result;
    result.algorithm_name = algorithm->name();
    result.matrix_size = matrix_size;
    
    // Generate test matrices
    MatrixType A = create_random<T>(matrix_size, matrix_size);
    MatrixType B = create_random<T>(matrix_size, matrix_size);
    
    // Measure total time
    auto start = std::chrono::high_resolution_clock::now();
    MatrixType C = algorithm->multiply(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    result.total_time_ms = duration.count() / 1000.0;
    
    // For detailed profiling, we would measure individual components
    if (enable_detailed_profiling_) {
        result.computation_time_ms = result.total_time_ms * 0.9; // Estimate
        result.memory_allocation_time_ms = result.total_time_ms * 0.05; // Estimate
        result.data_transfer_time_ms = result.total_time_ms * 0.05; // Estimate
        result.memory_allocations = memory_allocations_;
        result.cache_misses = cache_misses_;
        result.cpu_utilization = get_cpu_utilization();
    } else {
        result.computation_time_ms = result.total_time_ms;
        result.memory_allocation_time_ms = 0.0;
        result.data_transfer_time_ms = 0.0;
        result.memory_allocations = 0;
        result.cache_misses = 0;
        result.cpu_utilization = 0.0;
    }
    
    return result;
}

template<typename T>
size_type PerformanceProfiler<T>::get_memory_usage() const {
    // This would typically use platform-specific APIs
    // For now, return a placeholder value
    return 0;
}

template<typename T>
void PerformanceProfiler<T>::reset_counters() {
    memory_allocations_ = 0;
    cache_misses_ = 0;
}

template<typename T>
double PerformanceProfiler<T>::get_cpu_utilization() const {
    // This would typically use platform-specific APIs
    // For now, return a placeholder value
    return 0.0;
}

template<typename T>
size_type PerformanceProfiler<T>::get_cache_misses() const {
    // This would typically use platform-specific APIs
    // For now, return a placeholder value
    return 0;
}

template<typename T>
typename ScalabilityAnalyzer<T>::ScalingResult ScalabilityAnalyzer<T>::analyze_scaling(
    const std::shared_ptr<AlgorithmType>& algorithm,
    const std::vector<size_type>& matrix_sizes) {
    
    ScalingResult result;
    result.algorithm_name = algorithm->name();
    
    for (size_type size : matrix_sizes) {
        if (algorithm->can_handle(size, size, size, size)) {
            // Generate test matrices
            Matrix<T> A = create_random<T>(size, size);
            Matrix<T> B = create_random<T>(size, size);
            
            // Measure execution time
            auto start = std::chrono::high_resolution_clock::now();
            algorithm->multiply(A, B);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            double time_ms = duration.count() / 1000.0;
            
            result.matrix_sizes.push_back(size);
            result.execution_times.push_back(time_ms);
            result.gflops.push_back((2.0 * size * size * size) / (time_ms * 1e6));
        }
    }
    
    result.scaling_factor = calculate_scaling_factor(result.matrix_sizes, result.execution_times);
    result.complexity_class = determine_complexity_class(result.matrix_sizes, result.execution_times);
    
    return result;
}

template<typename T>
std::string ScalabilityAnalyzer<T>::determine_complexity_class(
    const std::vector<size_type>& matrix_sizes,
    const std::vector<double>& execution_times) const {
    
    if (matrix_sizes.size() < 2) return "Unknown";
    
    // Calculate the scaling factor
    double scaling_factor = calculate_scaling_factor(matrix_sizes, execution_times);
    
    if (scaling_factor < 2.5) return "O(n^2)";
    else if (scaling_factor < 2.9) return "O(n^2.807)"; // Strassen
    else if (scaling_factor < 3.1) return "O(n^3)";
    else return "O(n^3+)";
}

template<typename T>
double ScalabilityAnalyzer<T>::calculate_scaling_factor(
    const std::vector<size_type>& matrix_sizes,
    const std::vector<double>& execution_times) const {
    
    if (matrix_sizes.size() < 2) return 0.0;
    
    // Calculate the average scaling factor between consecutive sizes
    double total_scaling = 0.0;
    int count = 0;
    
    for (size_t i = 1; i < matrix_sizes.size(); ++i) {
        double size_ratio = static_cast<double>(matrix_sizes[i]) / matrix_sizes[i-1];
        double time_ratio = execution_times[i] / execution_times[i-1];
        
        if (time_ratio > 0 && size_ratio > 0) {
            total_scaling += std::log(time_ratio) / std::log(size_ratio);
            count++;
        }
    }
    
    return count > 0 ? total_scaling / count : 0.0;
}

} // namespace alphatensor

#endif // ALPHATENSOR_BENCHMARK_FUNCTIONS 