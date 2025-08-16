#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

#ifdef __x86_64__
#include <immintrin.h>  // For AVX2 intrinsics
#endif
#include <cstring>      // For memcpy

// SIMD-optimized Naive Matrix Multiplication Algorithm
template<typename T>
Matrix<T> Matrix<T>::SimdNaiveMatrixMultiplicationAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
#ifdef __x86_64__
    Matrix<T>::MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    Matrix<T> result = Matrix<T>::MatrixMultiplicationAlgorithm::construct_result(matrix, other);
    
    const size_type rows = matrix.rows();
    const size_type cols = other.cols();
    const size_type inner_dim = matrix.cols();
    
    // For double precision, we can process 4 elements at once with AVX2
    constexpr size_type SIMD_WIDTH = 4;  // 256-bit / 64-bit = 4 doubles
    
    // Ensure matrices are aligned for SIMD operations
    if (inner_dim % SIMD_WIDTH == 0) {
        // Optimized path for aligned data
        simd_multiply_aligned(matrix, other, result);
    } else {
        // Fallback for unaligned data
        simd_multiply_unaligned(matrix, other, result);
    }
    
    return result;
#else
    // Fallback to naive implementation on non-x86 architectures
    return Matrix<T>::NaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
#endif
}

#ifdef __x86_64__
template<typename T>
void Matrix<T>::SimdNaiveMatrixMultiplicationAlgorithm::simd_multiply_aligned(
    const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C) {
    
    const size_type rows = A.rows();
    const size_type cols = B.cols();
    const size_type inner_dim = A.cols();
    const size_type simd_width = 4;  // AVX2 for doubles
    
    // Transpose B for better cache locality and SIMD efficiency
    Matrix<T> B_transposed = B.transpose();
    
    for (size_type i = 0; i < rows; ++i) {
        for (size_type j = 0; j < cols; ++j) {
            __m256d sum = _mm256_setzero_pd();  // Initialize sum to zero
            
            // Process 4 elements at a time
            for (size_type k = 0; k < inner_dim; k += simd_width) {
                // Load 4 elements from A[i, k:k+3]
                __m256d a_vec = _mm256_loadu_pd(&A(i, k));
                
                // Load 4 elements from B_transposed[j, k:k+3]
                __m256d b_vec = _mm256_loadu_pd(&B_transposed(j, k));
                
                // Multiply and accumulate: sum += a_vec * b_vec
                __m256d product = _mm256_mul_pd(a_vec, b_vec);
                sum = _mm256_add_pd(sum, product);
            }
            
            // Horizontal sum of the 4 elements in sum
            double result_array[4];
            _mm256_storeu_pd(result_array, sum);
            C(i, j) = result_array[0] + result_array[1] + result_array[2] + result_array[3];
        }
    }
}

template<typename T>
void Matrix<T>::SimdNaiveMatrixMultiplicationAlgorithm::simd_multiply_unaligned(
    const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C) {
    
    const size_type rows = A.rows();
    const size_type cols = B.cols();
    const size_type inner_dim = A.cols();
    const size_type simd_width = 4;
    const size_type aligned_inner = (inner_dim / simd_width) * simd_width;
    
    // Transpose B for better cache locality
    Matrix<T> B_transposed = B.transpose();
    
    for (size_type i = 0; i < rows; ++i) {
        for (size_type j = 0; j < cols; ++j) {
            __m256d sum = _mm256_setzero_pd();
            
            // Process aligned portion with SIMD
            for (size_type k = 0; k < aligned_inner; k += simd_width) {
                __m256d a_vec = _mm256_loadu_pd(&A(i, k));
                __m256d b_vec = _mm256_loadu_pd(&B_transposed(j, k));
                __m256d product = _mm256_mul_pd(a_vec, b_vec);
                sum = _mm256_add_pd(sum, product);
            }
            
            // Handle remaining elements with scalar operations
            T scalar_sum = 0.0;
            for (size_type k = aligned_inner; k < inner_dim; ++k) {
                scalar_sum += A(i, k) * B_transposed(j, k);
            }
            
            // Combine SIMD and scalar results
            double result_array[4];
            _mm256_storeu_pd(result_array, sum);
            C(i, j) = result_array[0] + result_array[1] + result_array[2] + result_array[3] + scalar_sum;
        }
    }
}

// Alternative implementation with loop reordering for better cache performance
template<typename T>
Matrix<T> Matrix<T>::SimdNaiveMatrixMultiplicationAlgorithm::multiply_optimized(
    const Matrix<T>& matrix, const Matrix<T>& other) {
    
    Matrix<T>::MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    Matrix<T> result = Matrix<T>::MatrixMultiplicationAlgorithm::construct_result(matrix, other);
    
    const size_type rows = matrix.rows();
    const size_type cols = other.cols();
    const size_type inner_dim = matrix.cols();
    const size_type simd_width = 4;
    
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
                    __m256d a_vec = _mm256_set1_pd(a_ik);  // Broadcast a_ik to all lanes
                    __m256d b_vec = _mm256_loadu_pd(&B_transposed(j, k));
                    __m256d c_vec = _mm256_loadu_pd(&result(i, j));
                    __m256d product = _mm256_mul_pd(a_vec, b_vec);
                    __m256d new_c = _mm256_add_pd(c_vec, product);
                    _mm256_storeu_pd(&result(i, j), new_c);
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

// Check if SIMD is available at runtime
template<typename T>
bool Matrix<T>::SimdNaiveMatrixMultiplicationAlgorithm::is_simd_available() {
    // Check for AVX2 support
    #ifdef __AVX2__
        return true;
    #else
        return false;
    #endif
}

// Fallback to scalar implementation if SIMD is not available
template<typename T>
Matrix<T> Matrix<T>::SimdNaiveMatrixMultiplicationAlgorithm::multiply_fallback(
    const Matrix<T>& matrix, const Matrix<T>& other) {
    
    return Matrix<T>::NaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
}

#endif // MATRIX_FUNCTIONS
