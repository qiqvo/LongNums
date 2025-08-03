#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>

namespace alphatensor {

// Type definitions for matrix operations
using value_type = double;
using size_type = std::size_t;

/**
 * @brief Matrix class for efficient matrix operations
 * 
 * This class provides a modern C++ implementation of matrix operations
 * with support for various matrix multiplication algorithms including
 * those discovered by AlphaTensor.
 */
class Matrix {
public:
    using value_type = double;
    using size_type = std::size_t;

    // Constructors
    Matrix() = default;
    Matrix(size_type rows, size_type cols);
    Matrix(size_type rows, size_type cols, value_type value);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    
    // Assignment operators
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;
    
    // Destructor
    ~Matrix() = default;
    
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
    
    // Utility methods
    size_type rows() const { return rows_; }
    size_type cols() const { return cols_; }
    bool is_square() const { return rows_ == cols_; }
    bool is_empty() const { return data_.empty(); }
    
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
    value_type* data() { return data_.data(); }
    const value_type* data() const { return data_.data(); }
    const std::vector<value_type>& get_data() const { return data_; }
    
    // Verification
    bool is_equal(const Matrix& other, value_type tolerance = 1e-10) const;
    
private:
    size_type rows_;
    size_type cols_;
    std::vector<value_type> data_;
    
    // Helper methods
    size_type index(size_type row, size_type col) const;
    void check_dimensions(const Matrix& other, const std::string& operation) const;
    void check_bounds(size_type row, size_type col) const;
};

// Free functions
Matrix operator*(Matrix::value_type scalar, const Matrix& matrix);
std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

// Utility functions
Matrix create_identity(size_type size);
Matrix create_random(size_type rows, size_type cols, 
                    value_type min = -1.0, value_type max = 1.0,
                    unsigned seed = 42);
Matrix create_random_normal(size_type rows, size_type cols,
                          value_type mean = 0.0, value_type stddev = 1.0,
                          unsigned seed = 42);

} // namespace alphatensor 