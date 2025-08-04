#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// Core Matrix class implementations

template<typename T>
Matrix<T>::Matrix() : rows_(0), cols_(0) {}

template<typename T>
Matrix<T>::Matrix(size_type rows, size_type cols) 
    : rows_(rows), cols_(cols), data_(rows * cols, T{}) {}

template<typename T>
Matrix<T>::Matrix(size_type rows, size_type cols, value_type value)
    : rows_(rows), cols_(cols), data_(rows * cols, value) {}

template<typename T>
Matrix<T>::Matrix(const Matrix& other) = default;

template<typename T>
Matrix<T>::Matrix(Matrix&& other) noexcept = default;

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other) = default;

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix&& other) noexcept = default;

template<typename T>
Matrix<T>::~Matrix() = default;

template<typename T>
typename Matrix<T>::value_type& Matrix<T>::operator()(size_type row, size_type col) {
    check_bounds(row, col);
    return data_[index(row, col)];
}

template<typename T>
const typename Matrix<T>::value_type& Matrix<T>::operator()(size_type row, size_type col) const {
    check_bounds(row, col);
    return data_[index(row, col)];
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix& other) const {
    check_dimensions(other, "addition");
    Matrix result(rows_, cols_);
    for (size_type i = 0; i < data_.size(); ++i) {
        result.data_[i] = data_[i] + other.data_[i];
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix& other) const {
    check_dimensions(other, "subtraction");
    Matrix result(rows_, cols_);
    for (size_type i = 0; i < data_.size(); ++i) {
        result.data_[i] = data_[i] - other.data_[i];
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix& other) const {
    return this->multiply(other);
}

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix& other) {
    check_dimensions(other, "addition");
    for (size_type i = 0; i < data_.size(); ++i) {
        data_[i] += other.data_[i];
    }
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix& other) {
    check_dimensions(other, "subtraction");
    for (size_type i = 0; i < data_.size(); ++i) {
        data_[i] -= other.data_[i];
    }
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(value_type scalar) {
    for (size_type i = 0; i < data_.size(); ++i) {
        data_[i] *= scalar;
    }
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(value_type scalar) const {
    Matrix result(rows_, cols_);
    for (size_type i = 0; i < data_.size(); ++i) {
        result.data_[i] = data_[i] * scalar;
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix result(cols_, rows_);
    for (size_type i = 0; i < rows_; ++i) {
        for (size_type j = 0; j < cols_; ++j) {
            result(j, i) = (*this)(i, j);
        }
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::submatrix(size_type start_row, size_type start_col, 
                    size_type end_row, size_type end_col) const {
    if (start_row >= end_row || start_col >= end_col || 
        end_row > rows_ || end_col > cols_) {
        throw std::out_of_range("Invalid submatrix range");
    }
    
    size_type sub_rows = end_row - start_row;
    size_type sub_cols = end_col - start_col;
    Matrix result(sub_rows, sub_cols);
    
    for (size_type i = 0; i < sub_rows; ++i) {
        for (size_type j = 0; j < sub_cols; ++j) {
            result(i, j) = (*this)(start_row + i, start_col + j);
        }
    }
    
    return result;
}

template<typename T>
void Matrix<T>::set_submatrix(size_type start_row, size_type start_col, const Matrix& sub) {
    if (start_row + sub.rows() > rows_ || start_col + sub.cols() > cols_) {
        throw std::out_of_range("Submatrix too large for target position");
    }
    
    for (size_type i = 0; i < sub.rows(); ++i) {
        for (size_type j = 0; j < sub.cols(); ++j) {
            (*this)(start_row + i, start_col + j) = sub(i, j);
        }
    }
}

// Utility methods
template<typename T>
size_type Matrix<T>::rows() const { return rows_; }

template<typename T>
size_type Matrix<T>::cols() const { return cols_; }

template<typename T>
bool Matrix<T>::is_square() const { return rows_ == cols_; }

template<typename T>
bool Matrix<T>::is_empty() const { return rows_ == 0 || cols_ == 0; }

template<typename T>
typename Matrix<T>::size_type Matrix<T>::index(size_type row, size_type col) const {
    return row * cols_ + col;
}

template<typename T>
void Matrix<T>::check_bounds(size_type row, size_type col) const {
    if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("Matrix index out of bounds");
    }
}

// Matrix multiplication with algorithm selection (legacy method)
template<typename T>
Matrix<T> Matrix<T>::multiply(const Matrix& other, Algorithm algo=Algorithm::AUTO) const {
    switch (algo) {
        case Algorithm::NAIVE:
            return Matrix<T>::NaiveAlgorithm::multiply(*this, other);
        case Algorithm::BLOCK:
            return Matrix<T>::BlockAlgorithm::multiply(*this, other);
        case Algorithm::STRASSEN:
            return Matrix<T>::StrassenAlgorithm::multiply(*this, other);
        case Algorithm::WINOGRAD:
            return Matrix<T>::WinogradAlgorithm::multiply(*this, other);
        case Algorithm::HYBRID:
            return Matrix<T>::HybridAlgorithm::multiply(*this, other);
    }
}

#endif // MATRIX_FUNCTIONS 