#include "matrix.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>

namespace alphatensor {

// Constructors
Matrix::Matrix(size_type rows, size_type cols) 
    : rows_(rows), cols_(cols), data_(rows * cols, 0.0) {}

Matrix::Matrix(size_type rows, size_type cols, value_type value)
    : rows_(rows), cols_(cols), data_(rows * cols, value) {}

Matrix::Matrix(const Matrix& other)
    : rows_(other.rows_), cols_(other.cols_), data_(other.data_) {}

Matrix::Matrix(Matrix&& other) noexcept
    : rows_(other.rows_), cols_(other.cols_), data_(std::move(other.data_)) {
    other.rows_ = 0;
    other.cols_ = 0;
}

// Assignment operators
Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = other.data_;
    }
    return *this;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this != &other) {
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = std::move(other.data_);
        other.rows_ = 0;
        other.cols_ = 0;
    }
    return *this;
}

// Access operators
value_type& Matrix::operator()(size_type row, size_type col) {
    check_bounds(row, col);
    return data_[index(row, col)];
}

const value_type& Matrix::operator()(size_type row, size_type col) const {
    check_bounds(row, col);
    return data_[index(row, col)];
}

// Arithmetic operators
Matrix Matrix::operator+(const Matrix& other) const {
    check_dimensions(other, "addition");
    Matrix result(rows_, cols_);
    for (size_type i = 0; i < data_.size(); ++i) {
        result.data_[i] = data_[i] + other.data_[i];
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    check_dimensions(other, "subtraction");
    Matrix result(rows_, cols_);
    for (size_type i = 0; i < data_.size(); ++i) {
        result.data_[i] = data_[i] - other.data_[i];
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
    }
    
    Matrix result(rows_, other.cols_);
    
    // Naive matrix multiplication
    for (size_type i = 0; i < rows_; ++i) {
        for (size_type j = 0; j < other.cols_; ++j) {
            value_type sum = 0.0;
            for (size_type k = 0; k < cols_; ++k) {
                sum += (*this)(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }
    
    return result;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    check_dimensions(other, "addition");
    for (size_type i = 0; i < data_.size(); ++i) {
        data_[i] += other.data_[i];
    }
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    check_dimensions(other, "subtraction");
    for (size_type i = 0; i < data_.size(); ++i) {
        data_[i] -= other.data_[i];
    }
    return *this;
}

Matrix& Matrix::operator*=(value_type scalar) {
    for (auto& element : data_) {
        element *= scalar;
    }
    return *this;
}

Matrix Matrix::operator*(value_type scalar) const {
    Matrix result(rows_, cols_);
    for (size_type i = 0; i < data_.size(); ++i) {
        result.data_[i] = data_[i] * scalar;
    }
    return result;
}

// Matrix operations
Matrix Matrix::transpose() const {
    Matrix result(cols_, rows_);
    for (size_type i = 0; i < rows_; ++i) {
        for (size_type j = 0; j < cols_; ++j) {
            result(j, i) = (*this)(i, j);
        }
    }
    return result;
}

Matrix Matrix::submatrix(size_type start_row, size_type start_col,
                        size_type end_row, size_type end_col) const {
    if (start_row >= end_row || start_col >= end_col ||
        end_row > rows_ || end_col > cols_) {
        throw std::out_of_range("Invalid submatrix bounds");
    }
    
    Matrix result(end_row - start_row, end_col - start_col);
    for (size_type i = start_row; i < end_row; ++i) {
        for (size_type j = start_col; j < end_col; ++j) {
            result(i - start_row, j - start_col) = (*this)(i, j);
        }
    }
    return result;
}

void Matrix::set_submatrix(size_type start_row, size_type start_col, const Matrix& sub) {
    if (start_row + sub.rows() > rows_ || start_col + sub.cols() > cols_) {
        throw std::out_of_range("Submatrix too large for target position");
    }
    
    for (size_type i = 0; i < sub.rows(); ++i) {
        for (size_type j = 0; j < sub.cols(); ++j) {
            (*this)(start_row + i, start_col + j) = sub(i, j);
        }
    }
}

// Random initialization
void Matrix::randomize(value_type min, value_type max, unsigned seed) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<value_type> dist(min, max);
    
    for (auto& element : data_) {
        element = dist(gen);
    }
}

void Matrix::randomize_normal(value_type mean, value_type stddev, unsigned seed) {
    std::mt19937 gen(seed);
    std::normal_distribution<value_type> dist(mean, stddev);
    
    for (auto& element : data_) {
        element = dist(gen);
    }
}

// Utility methods
void Matrix::fill(value_type value) {
    std::fill(data_.begin(), data_.end(), value);
}

void Matrix::zero() {
    fill(0.0);
}

void Matrix::identity() {
    if (!is_square()) {
        throw std::invalid_argument("Identity matrix must be square");
    }
    zero();
    for (size_type i = 0; i < rows_; ++i) {
        (*this)(i, i) = 1.0;
    }
}

value_type Matrix::trace() const {
    if (!is_square()) {
        throw std::invalid_argument("Trace only defined for square matrices");
    }
    value_type sum = 0.0;
    for (size_type i = 0; i < rows_; ++i) {
        sum += (*this)(i, i);
    }
    return sum;
}

value_type Matrix::norm() const {
    value_type sum = 0.0;
    for (const auto& element : data_) {
        sum += element * element;
    }
    return std::sqrt(sum);
}

// I/O
void Matrix::print(std::ostream& os, int precision) const {
    os << std::fixed << std::setprecision(precision);
    for (size_type i = 0; i < rows_; ++i) {
        os << "[";
        for (size_type j = 0; j < cols_; ++j) {
            os << (*this)(i, j);
            if (j < cols_ - 1) os << ", ";
        }
        os << "]" << std::endl;
    }
}

void Matrix::save_to_file(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    file << rows_ << " " << cols_ << std::endl;
    for (size_type i = 0; i < rows_; ++i) {
        for (size_type j = 0; j < cols_; ++j) {
            file << (*this)(i, j);
            if (j < cols_ - 1) file << " ";
        }
        file << std::endl;
    }
}

Matrix Matrix::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    size_type rows, cols;
    file >> rows >> cols;
    
    Matrix result(rows, cols);
    for (size_type i = 0; i < rows; ++i) {
        for (size_type j = 0; j < cols; ++j) {
            file >> result(i, j);
        }
    }
    
    return result;
}

// Verification
bool Matrix::is_equal(const Matrix& other, value_type tolerance) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        return false;
    }
    
    for (size_type i = 0; i < data_.size(); ++i) {
        if (std::abs(data_[i] - other.data_[i]) > tolerance) {
            return false;
        }
    }
    return true;
}

// Helper methods
size_type Matrix::index(size_type row, size_type col) const {
    return row * cols_ + col;
}

void Matrix::check_dimensions(const Matrix& other, const std::string& operation) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions incompatible for " + operation);
    }
}

void Matrix::check_bounds(size_type row, size_type col) const {
    if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("Matrix index out of bounds");
    }
}

// Free functions
Matrix operator*(Matrix::value_type scalar, const Matrix& matrix) {
    return matrix * scalar;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    matrix.print(os);
    return os;
}

// Utility functions
Matrix create_identity(size_type size) {
    Matrix result(size, size);
    result.identity();
    return result;
}

Matrix create_random(size_type rows, size_type cols, 
                    Matrix::value_type min, Matrix::value_type max,
                    unsigned seed) {
    Matrix result(rows, cols);
    result.randomize(min, max, seed);
    return result;
}

Matrix create_random_normal(size_type rows, size_type cols,
                          Matrix::value_type mean, Matrix::value_type stddev,
                          unsigned seed) {
    Matrix result(rows, cols);
    result.randomize_normal(mean, stddev, seed);
    return result;
}

} // namespace alphatensor 