#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

#include <arm_neon.h>  // For ARM NEON intrinsics
#include <cstring>     // For memcpy

// ARM NEON-optimized Matrix Multiplication Algorithm
template<typename T>
Matrix<T> Matrix<T>::ArmNeonMatrixMultiplicationAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    Matrix<T>::MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    Matrix<T> result = Matrix<T>::MatrixMultiplicationAlgorithm::construct_result(matrix, other);
    
    const size_type rows = matrix.rows();
    const size_type cols = other.cols();
    const size_type inner_dim = matrix.cols();
    
    // For double precision, we can process 2 elements at once with NEON
    constexpr size_type SIMD_WIDTH = 2;  // 128-bit / 64-bit = 2 doubles
    
    // Ensure matrices are aligned for SIMD operations
    if (inner_dim % SIMD_WIDTH == 0) {
        // Optimized path for aligned data
        neon_multiply_aligned(matrix, other, result);
    } else {
        // Fallback for unaligned data
        neon_multiply_unaligned(matrix, other, result);
    }
    
    return result;
}

template<typename T>
void Matrix<T>::ArmNeonMatrixMultiplicationAlgorithm::neon_multiply_aligned(
    const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C) {
    
    const size_type rows = A.rows();
    const size_type cols = B.cols();
    const size_type inner_dim = A.cols();
    const size_type simd_width = 2;  // NEON for doubles
    
    // Transpose B for better cache locality and SIMD efficiency
    Matrix<T> B_transposed = B.transpose();
    
    for (size_type i = 0; i < rows; ++i) {
        for (size_type j = 0; j < cols; ++j) {
            float64x2_t sum = vdupq_n_f64(0.0);  // Initialize sum to zero
            
            // Process 2 elements at a time
            for (size_type k = 0; k < inner_dim; k += simd_width) {
                // Load 2 elements from A[i, k:k+1]
                float64x2_t a_vec = vld1q_f64(&A(i, k));
                
                // Load 2 elements from B_transposed[j, k:k+1]
                float64x2_t b_vec = vld1q_f64(&B_transposed(j, k));
                
                // Multiply and accumulate: sum += a_vec * b_vec
                float64x2_t product = vmulq_f64(a_vec, b_vec);
                sum = vaddq_f64(sum, product);
            }
            
            // Horizontal sum of the 2 elements in sum
            double result_array[2];
            vst1q_f64(result_array, sum);
            C(i, j) = result_array[0] + result_array[1];
        }
    }
}

template<typename T>
void Matrix<T>::ArmNeonMatrixMultiplicationAlgorithm::neon_multiply_unaligned(
    const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C) {
    
    const size_type rows = A.rows();
    const size_type cols = B.cols();
    const size_type inner_dim = A.cols();
    const size_type simd_width = 2;
    const size_type aligned_inner = (inner_dim / simd_width) * simd_width;
    
    // Transpose B for better cache locality
    Matrix<T> B_transposed = B.transpose();
    
    for (size_type i = 0; i < rows; ++i) {
        for (size_type j = 0; j < cols; ++j) {
            float64x2_t sum = vdupq_n_f64(0.0);
            
            // Process aligned portion with SIMD
            for (size_type k = 0; k < aligned_inner; k += simd_width) {
                float64x2_t a_vec = vld1q_f64(&A(i, k));
                float64x2_t b_vec = vld1q_f64(&B_transposed(j, k));
                float64x2_t product = vmulq_f64(a_vec, b_vec);
                sum = vaddq_f64(sum, product);
            }
            
            // Handle remaining elements with scalar operations
            T scalar_sum = 0.0;
            for (size_type k = aligned_inner; k < inner_dim; ++k) {
                scalar_sum += A(i, k) * B_transposed(j, k);
            }
            
            // Combine SIMD and scalar results
            double result_array[2];
            vst1q_f64(result_array, sum);
            C(i, j) = result_array[0] + result_array[1] + scalar_sum;
        }
    }
}

// Alternative implementation with loop reordering for better cache performance
template<typename T>
Matrix<T> Matrix<T>::ArmNeonMatrixMultiplicationAlgorithm::multiply_optimized(
    const Matrix<T>& matrix, const Matrix<T>& other) {
    
    Matrix<T>::MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    Matrix<T> result = Matrix<T>::MatrixMultiplicationAlgorithm::construct_result(matrix, other);
    
    const size_type rows = matrix.rows();
    const size_type cols = other.cols();
    const size_type inner_dim = matrix.cols();
    const size_type simd_width = 2;
    
    // Transpose the second matrix for better cache locality
    Matrix<T> B_transposed = other.transpose();
    
    // Loop reordering: i -> k -> j for better cache performance
    for (size_type i = 0; i < rows; ++i) {
        for (size_type k = 0; k < inner_dim; ++k) {
            T a_ik = matrix(i, k);
            
            // Process multiple columns at once with SIMD
            for (size_type j = 0; j < cols; j += simd_width) {
                size_type remaining_cols = std::min(simd_width, cols - j);
                
                if (remaining_cols == simd_width) {
                    // Full SIMD path
                    float64x2_t a_vec = vdupq_n_f64(a_ik);  // Broadcast a_ik to all lanes
                    float64x2_t b_vec = vld1q_f64(&B_transposed(j, k));
                    float64x2_t c_vec = vld1q_f64(&result(i, j));
                    float64x2_t product = vmulq_f64(a_vec, b_vec);
                    float64x2_t new_c = vaddq_f64(c_vec, product);
                    vst1q_f64(&result(i, j), new_c);
                } else {
                    // Scalar path for remaining elements
                    for (size_type jj = 0; jj < remaining_cols; ++jj) {
                        result(i, j + jj) += a_ik * B_transposed(j + jj, k);
                    }
                }
            }
        }
    }
    
    return result;
}

// Check if ARM NEON is available at runtime
template<typename T>
bool Matrix<T>::ArmNeonMatrixMultiplicationAlgorithm::is_neon_available() {
    // Check for ARM NEON support
    #ifdef __ARM_NEON
        return true;
    #else
        return false;
    #endif
}

// Fallback to scalar implementation if NEON is not available
template<typename T>
Matrix<T> Matrix<T>::ArmNeonMatrixMultiplicationAlgorithm::multiply_fallback(
    const Matrix<T>& matrix, const Matrix<T>& other) {
    
    return Matrix<T>::NaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
}

#endif // MATRIX_FUNCTIONS
