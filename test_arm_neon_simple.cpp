#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>

// Simple matrix class for testing
template<typename T>
class SimpleMatrix {
public:
    SimpleMatrix(size_t rows, size_t cols) : rows_(rows), cols_(cols), data_(rows * cols) {}
    
    T& operator()(size_t i, size_t j) { return data_[i * cols_ + j]; }
    const T& operator()(size_t i, size_t j) const { return data_[i * cols_ + j]; }
    
    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }
    
    void randomize() {
        for (auto& val : data_) {
            val = static_cast<T>(rand()) / RAND_MAX * 2.0 - 1.0;
        }
    }
    
    bool is_equal(const SimpleMatrix& other, T tolerance = 1e-10) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) return false;
        for (size_t i = 0; i < data_.size(); ++i) {
            if (std::abs(data_[i] - other.data_[i]) > tolerance) return false;
        }
        return true;
    }
    
private:
    size_t rows_, cols_;
    std::vector<T> data_;
};

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

// Naive matrix multiplication
template<typename T>
SimpleMatrix<T> naive_multiply(const SimpleMatrix<T>& A, const SimpleMatrix<T>& B) {
    SimpleMatrix<T> C(A.rows(), B.cols());
    
    for (size_t i = 0; i < A.rows(); ++i) {
        for (size_t j = 0; j < B.cols(); ++j) {
            T sum = 0.0;
            for (size_t k = 0; k < A.cols(); ++k) {
                sum += A(i, k) * B(k, j);
            }
            C(i, j) = sum;
        }
    }
    
    return C;
}

// ARM NEON optimized multiplication
#ifdef __ARM_NEON
#include <arm_neon.h>

template<typename T>
SimpleMatrix<T> neon_multiply(const SimpleMatrix<T>& A, const SimpleMatrix<T>& B) {
    SimpleMatrix<T> C(A.rows(), B.cols());
    
    // Transpose B for better cache locality
    SimpleMatrix<T> B_transposed(B.cols(), B.rows());
    for (size_t i = 0; i < B.rows(); ++i) {
        for (size_t j = 0; j < B.cols(); ++j) {
            B_transposed(j, i) = B(i, j);
        }
    }
    
    const size_t simd_width = 2;  // NEON processes 2 doubles at once
    
    for (size_t i = 0; i < A.rows(); ++i) {
        for (size_t j = 0; j < B.cols(); ++j) {
            float64x2_t sum = vdupq_n_f64(0.0);
            
            // Process aligned portion with SIMD
            size_t aligned_k = (A.cols() / simd_width) * simd_width;
            for (size_t k = 0; k < aligned_k; k += simd_width) {
                float64x2_t a_vec = vld1q_f64(&A(i, k));
                float64x2_t b_vec = vld1q_f64(&B_transposed(j, k));
                float64x2_t product = vmulq_f64(a_vec, b_vec);
                sum = vaddq_f64(sum, product);
            }
            
            // Handle remaining elements with scalar operations
            T scalar_sum = 0.0;
            for (size_t k = aligned_k; k < A.cols(); ++k) {
                scalar_sum += A(i, k) * B_transposed(j, k);
            }
            
            // Combine SIMD and scalar results
            double result_array[2];
            vst1q_f64(result_array, sum);
            C(i, j) = result_array[0] + result_array[1] + scalar_sum;
        }
    }
    
    return C;
}
#else
template<typename T>
SimpleMatrix<T> neon_multiply(const SimpleMatrix<T>& A, const SimpleMatrix<T>& B) {
    // Fallback to naive implementation if NEON is not available
    return naive_multiply(A, B);
}
#endif

int main() {
    std::cout << "=== ARM NEON Matrix Multiplication Test ===" << std::endl;
    
#ifdef __ARM_NEON
    std::cout << "ARM NEON support: ✓ Available" << std::endl;
#else
    std::cout << "ARM NEON support: ✗ Not available" << std::endl;
#endif
    
    // Test different matrix sizes
    std::vector<size_t> sizes = {64, 128, 256, 512};
    
    for (size_t size : sizes) {
        std::cout << "\n--- Testing " << size << "x" << size << " matrices ---" << std::endl;
        
        // Create test matrices
        SimpleMatrix<double> A(size, size);
        SimpleMatrix<double> B(size, size);
        A.randomize();
        B.randomize();
        
        SimpleMatrix<double> result_naive(size, size), result_neon(size, size);
        
        // Test naive algorithm
        double naive_time = measure_time([&]() {
            result_naive = naive_multiply(A, B);
        }, "Naive algorithm");
        
        // Test NEON algorithm
        double neon_time = measure_time([&]() {
            result_neon = neon_multiply(A, B);
        }, "ARM NEON algorithm");
        
        // Verify results are the same
        if (result_naive.is_equal(result_neon, 1e-10)) {
            std::cout << "✓ Results match" << std::endl;
        } else {
            std::cout << "✗ Results differ!" << std::endl;
        }
        
        // Calculate speedup
        if (naive_time > 0) {
            double speedup = naive_time / neon_time;
            std::cout << "Speedup: " << std::fixed << std::setprecision(2) 
                      << speedup << "x" << std::endl;
        }
    }
    
    std::cout << "\n=== Performance Summary ===" << std::endl;
    std::cout << "Expected speedup: 1.5-2x for double precision" << std::endl;
    std::cout << "ARM NEON processes 2 double-precision elements at once" << std::endl;
    
    return 0;
}
