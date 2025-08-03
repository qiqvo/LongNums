#include "benchmark.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <numeric>

namespace alphatensor {

// Explicit template instantiations for common types
template class MatrixBenchmarker<double>;
template class MatrixBenchmarker<float>;

template class PerformanceProfiler<double>;
template class PerformanceProfiler<float>;

template class ScalabilityAnalyzer<double>;
template class ScalabilityAnalyzer<float>;

// Explicit instantiations for BenchmarkResult
template struct BenchmarkResult<double>;
template struct BenchmarkResult<float>;

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
    int unit = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024.0 && unit < 4) {
        size /= 1024.0;
        unit++;
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << " " << units[unit];
    return oss.str();
}

std::string format_gflops(double gflops) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << gflops << " GFLOPs";
    return oss.str();
}

} // namespace alphatensor 