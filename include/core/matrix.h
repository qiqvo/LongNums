#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <memory>
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>

// Type definitions for matrix operations
using size_type = std::size_t;

template<typename T = double>
class Matrix {
public:
    using value_type = T;
    using size_type = std::size_t;

    // Constructors
    Matrix();
    Matrix(size_type rows, size_type cols);
    Matrix(size_type rows, size_type cols, value_type value);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    
    // Assignment operators
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;
    
    // Destructor
    ~Matrix();
    
    // Access operators
    value_type& operator()(size_type row, size_type col);
    const value_type& operator()(size_type row, size_type col) const;
    
    // Arithmetic operators
    Matrix operator+(const Matrix& other) const;
    Matrix operator+(value_type scalar) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator-(value_type scalar) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(value_type scalar) const;
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(value_type scalar);
    
    // Comparison operators
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;
    
    class MatrixMultiplicationAlgorithm {
        public:
        static Matrix construct_result(const Matrix& matrix, const Matrix& other);
        static void validate_dimensions(const Matrix& matrix, const Matrix& other);
        
        // MatrixMultiplicationAlgorithm types for matrix multiplication
        enum class AlgorithmType {
            NAIVE,
            NAIVE_TRANSPOSED,
            SIMD_NAIVE,
            ARM_NEON,
            STRASSEN,
            WINOGRAD,
            ALPHA_TENSOR,
            AUTO 
        };

        static Matrix multiply(const Matrix& matrix, const Matrix& other);
    };

    class NaiveMatrixMultiplicationAlgorithm {
        public:
        static Matrix multiply(const Matrix& matrix, const Matrix& other);
    };

    class NaiveTransposedMatrixMultiplicationAlgorithm {
        public:
        static Matrix multiply(const Matrix& matrix, const Matrix& other);
    };

    class SimdNaiveMatrixMultiplicationAlgorithm {
        public:
        static Matrix multiply(const Matrix& matrix, const Matrix& other);
        static Matrix multiply_optimized(const Matrix& matrix, const Matrix& other);
        static bool is_simd_available();
        static Matrix multiply_fallback(const Matrix& matrix, const Matrix& other);
        
        private:
        static void simd_multiply_aligned(const Matrix& A, const Matrix& B, Matrix& C);
        static void simd_multiply_unaligned(const Matrix& A, const Matrix& B, Matrix& C);
    };

    class ArmNeonMatrixMultiplicationAlgorithm {
        public:
        static Matrix multiply(const Matrix& matrix, const Matrix& other);
        static Matrix multiply_optimized(const Matrix& matrix, const Matrix& other);
        static bool is_neon_available();
        static Matrix multiply_fallback(const Matrix& matrix, const Matrix& other);
        
        private:
        static void neon_multiply_aligned(const Matrix& A, const Matrix& B, Matrix& C);
        static void neon_multiply_unaligned(const Matrix& A, const Matrix& B, Matrix& C);
    };

    // Superbase class for block-based matrix multiplication algorithms using CRTP
    template<int block_num, typename Derived>
    class BlockMatrixMultiplicationAlgorithm : public MatrixMultiplicationAlgorithm {
        public:
        static Matrix multiply(const Matrix& matrix, const Matrix& other) {
            try {
                validate_divide_and_conquer_inputs(matrix, other);
                return split_and_multiply(matrix, other);
            } catch (const std::invalid_argument&) {
                // Fall back to naive for non-square matrices
                return NaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
            }
        }
        
        protected:
        static Matrix split_and_multiply(const Matrix& matrix, const Matrix& other);

        static Matrix* compute_from_blocks(const Matrix* A_blocks, const Matrix* B_blocks){
            (void)A_blocks; (void)B_blocks; // Suppress unused parameter warnings
            throw std::runtime_error("Not implemented; provides an interface for the derived class to implement.");
        }

        // Helper method to pad matrices to be divisible by 4
        // Common validation for divide-and-conquer algorithms
        static void validate_divide_and_conquer_inputs(const Matrix& matrix, const Matrix& other);
        
        // Helper method to pad odd-sized matrices with zeros
        static std::pair<Matrix, Matrix> pad_matrices(const Matrix& A, const Matrix& B);
        
        // Helper method to combine quadrants into result using direct indexing
        static Matrix combine_blocks(const Matrix* blocks, size_type n);
    };

    class StrassenMatrixMultiplicationAlgorithm : public BlockMatrixMultiplicationAlgorithm<4, StrassenMatrixMultiplicationAlgorithm> {
        public:
        static Matrix* compute_from_blocks(const Matrix* A_blocks, const Matrix* B_blocks);

        private:
        static Matrix multiply_2x2(const Matrix& A, const Matrix& B);
    };

    class WinogradMatrixMultiplicationAlgorithm : public BlockMatrixMultiplicationAlgorithm<4, WinogradMatrixMultiplicationAlgorithm> {
        public:
        static Matrix* compute_from_blocks(const Matrix* A_blocks, const Matrix* B_blocks);

        private:
        static Matrix multiply_2x2(const Matrix& A, const Matrix& B);
    };

    class AlphaTensorMatrixMultiplicationAlgorithm : public BlockMatrixMultiplicationAlgorithm<16, AlphaTensorMatrixMultiplicationAlgorithm> {
        public:
        static Matrix* compute_from_blocks(const Matrix* A_blocks, const Matrix* B_blocks);
        
        private:
        static const int p_size_n = 16;
        static const int p_size_m = 49;

        static int u[p_size_n * p_size_m];
        static int v[p_size_n * p_size_m];
        static int w[p_size_n * p_size_m];

        static Matrix alpha_tensor_4x4(const Matrix& A, const Matrix& B);
    };

    class AutoMatrixMultiplicationAlgorithm {
    
        public:

        // MatrixMultiplicationAlgorithm selection and configuration
        static void set_thresholds(size_type naive_threshold, size_type strassen_threshold, size_type block_num);
        
        // Thresholds structure for algorithm configuration
        struct AutoMatrixMultiplicationAlgorithmThresholds {
            size_type naive_threshold = 64;
            size_type strassen_threshold = 512;
            size_type alpha_tensor_threshold = 1024;
        };
        
        static AutoMatrixMultiplicationAlgorithmThresholds get_thresholds();
        static inline AutoMatrixMultiplicationAlgorithmThresholds thresholds_{};
        
        static typename Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType select_best_algorithm(size_type size);
        static Matrix multiply(const Matrix& matrix, const Matrix& other);
    };

    // Matrix multiplication with algorithm selection
    Matrix multiply(const Matrix& other, typename Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType algo = Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::AUTO) const;

    // Utility methods
    size_type rows() const;
    size_type cols() const;
    bool is_square() const;
    bool is_empty() const;
    
    // Matrix operations
    Matrix transpose() const;
    Matrix submatrix(size_type start_row, size_type start_col, 
                    size_type end_row, size_type end_col) const;
    void set_submatrix(size_type start_row, size_type start_col, const Matrix& sub);
    
    // Random initialization
    void randomize(value_type min = -1.0, value_type max = 1.0, unsigned seed = 42);
    void randomize_normal(value_type mean = 0.0, value_type stddev = 1.0, unsigned seed = 42);
    
    // Utility methods
    void fill(value_type value);
    void zero();
    void identity();
    value_type trace() const;
    value_type norm() const;
    
    // I/O
    void print(std::ostream& os = std::cout, int precision = 6) const;
    void save_to_file(const std::string& filename) const;
    static Matrix load_from_file(const std::string& filename);
    
    // Data access (for advanced users)
    value_type* data();
    const value_type* data() const;
    const std::vector<value_type>& get_data() const;
    
    // Verification
    bool is_equal(const Matrix& other, value_type tolerance = 1e-10) const;
    
private:
    size_type rows_;
    size_type cols_;
    std::vector<value_type> data_;
    
    // Helper methods
    size_type index(size_type row, size_type col) const;
    void check_bounds(size_type row, size_type col) const;
    void check_dimensions(const Matrix& other, const std::string& operation) const;
};

// Free functions
template<typename T>
Matrix<T> operator+(typename Matrix<T>::value_type scalar, const Matrix<T>& matrix);
template<typename T>
Matrix<T> operator-(typename Matrix<T>::value_type scalar, const Matrix<T>& matrix);
template<typename T>
Matrix<T> operator*(typename Matrix<T>::value_type scalar, const Matrix<T>& matrix);

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix);

// Utility functions
template<typename T = double>
Matrix<T> create_identity(size_type size);

template<typename T = double>
Matrix<T> create_random(size_type rows, size_type cols, 
                    T min = -1.0, T max = 1.0,
                    unsigned seed = 42);

template<typename T = double>
Matrix<T> create_random_normal(size_type rows, size_type cols,
                          T mean = 0.0, T stddev = 1.0,
                          unsigned seed = 42);

// The magic: include the implementation files for templates
#define MATRIX_FUNCTIONS
#include "../../src/core/matrix_core.cpp"
#include "../../src/core/matrix_multiplication_algorithms/matrix_multiplication_algorithm.cpp"
#include "../../src/core/matrix_multiplication_algorithms/naive_matrix_multiplication_algorithm.cpp"
#include "../../src/core/matrix_multiplication_algorithms/naive_transposed_matrix_multiplication_algorithm.cpp"
#include "../../src/core/matrix_multiplication_algorithms/simd_naive_matrix_multiplication_algorithm.cpp"
#include "../../src/core/matrix_multiplication_algorithms/arm_neon_matrix_multiplication_algorithm.cpp"
#include "../../src/core/matrix_multiplication_algorithms/block_matrix_multiplication_algorithm.cpp"
#include "../../src/core/matrix_multiplication_algorithms/strassen_matrix_multiplication_algorithm.cpp"
#include "../../src/core/matrix_multiplication_algorithms/winograd_matrix_multiplication_algorithm.cpp"
#include "../../src/core/matrix_multiplication_algorithms/auto_matrix_multiplication_algorithm.cpp"
#include "../../src/core/matrix_multiplication_algorithms/alpha_tensor_matrix_multiplication_algorithm.cpp"
#include "../../src/core/matrix_utils.cpp"

#endif // MATRIX_H 