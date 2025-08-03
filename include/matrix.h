#ifndef ALPHATENSOR_MATRIX_H
#define ALPHATENSOR_MATRIX_H

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

namespace alphatensor {

// Type definitions for matrix operations
using size_type = std::size_t;

/**
 * @brief Matrix class for efficient matrix operations
 * 
 * This class provides a modern C++ implementation of matrix operations
 * with support for various matrix multiplication algorithms including
 * those discovered by AlphaTensor.
 */
template<typename T = double>
class Matrix {
public:
    using value_type = T;
    using size_type = std::size_t;

    // Algorithm types for matrix multiplication
    enum class Algorithm {
        NAIVE,
        BLOCK,
        STRASSEN,
        WINOGRAD,
        ALPHATENSOR_GPU,
        ALPHATENSOR_TPU,
        HYBRID,
        AUTO  // Automatically choose the best algorithm
    };

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
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(value_type scalar);
    Matrix operator*(value_type scalar) const;
    
    // Matrix multiplication with algorithm selection
    Matrix multiply(const Matrix& other, Algorithm algo = Algorithm::AUTO) const;
    
    // Individual algorithm implementations
    Matrix multiply_naive(const Matrix& other) const;
    Matrix multiply_block(const Matrix& other, size_type block_size = 64) const;
    Matrix multiply_strassen(const Matrix& other) const;
    Matrix multiply_winograd(const Matrix& other) const;
    Matrix multiply_alphatensor(const Matrix& other, const std::string& variant = "gpu") const;
    Matrix multiply_hybrid(const Matrix& other) const;
    
    // Algorithm selection and configuration
    static Algorithm select_best_algorithm(size_type size);
    static void set_thresholds(size_type naive_threshold, size_type strassen_threshold, size_type block_size);
    
    // Thresholds structure for algorithm configuration
    struct Thresholds {
        size_type naive_threshold = 64;
        size_type strassen_threshold = 512;
        size_type block_size = 64;
    };
    
    static Thresholds get_thresholds();
    
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
    
    // Static configuration for algorithm selection
    static Thresholds thresholds_;
    
    // Helper methods
    size_type index(size_type row, size_type col) const;
    void check_dimensions(const Matrix& other, const std::string& operation) const;
    void check_bounds(size_type row, size_type col) const;
    
    // Private algorithm helper methods
    Matrix strassen_recursive(const Matrix& A, const Matrix& B) const;
    Matrix strassen_2x2(const Matrix& A, const Matrix& B) const;
    Matrix winograd_recursive(const Matrix& A, const Matrix& B) const;
    Matrix winograd_2x2(const Matrix& A, const Matrix& B) const;
    Matrix alpha_tensor_4x4(const Matrix& A, const Matrix& B) const;
    Matrix alpha_tensor_2x2(const Matrix& A, const Matrix& B) const;
};

// Free functions
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

// Type alias for common usage
using MatrixD = Matrix<double>;
using MatrixF = Matrix<float>;

} // namespace alphatensor

// The magic: include the implementation file for templates
#define ALPHATENSOR_MATRIX_FUNCTIONS
#include "../src/matrix.cpp"

#endif // ALPHATENSOR_MATRIX_H 