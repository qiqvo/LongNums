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
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    Matrix result(rows_, cols_);
    for (size_type i = 0; i < data_.size(); ++i) {
        result.data_[i] = data_[i] + other.data_[i];
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(value_type scalar) const {
    Matrix result(rows_, cols_);
    for (size_type i = 0; i < data_.size(); ++i) {
        result.data_[i] = data_[i] + scalar;
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(value_type scalar) const {
    Matrix result(rows_, cols_);
    for (size_type i = 0; i < data_.size(); ++i) {
        result.data_[i] = data_[i] - scalar;
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match for subtraction");
    }
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

template<typename T>
void Matrix<T>::check_dimensions(const Matrix& other, const std::string& operation) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match for " + operation);
    }
}

// Matrix multiplication with algorithm selection (legacy method)
template<typename T>
Matrix<T> Matrix<T>::multiply(const Matrix& other, typename Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType algo) const {
    switch (algo) {
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE:
            return Matrix<T>::NaiveMatrixMultiplicationAlgorithm::multiply(*this, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::SIMD_NAIVE:
            return Matrix<T>::SimdNaiveMatrixMultiplicationAlgorithm::multiply(*this, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::ARM_NEON:
            return Matrix<T>::ArmNeonMatrixMultiplicationAlgorithm::multiply(*this, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::BLOCK:
            return Matrix<T>::BlockMatrixMultiplicationAlgorithm::multiply(*this, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN:
            return Matrix<T>::StrassenMatrixMultiplicationAlgorithm::multiply(*this, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::NEW_STRASSEN:
            return Matrix<T>::NewStrassenMatrixMultiplicationAlgorithm::multiply(*this, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::WINOGRAD:
            return Matrix<T>::WinogradMatrixMultiplicationAlgorithm::multiply(*this, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::HYBRID:
            return Matrix<T>::HybridMatrixMultiplicationAlgorithm::multiply(*this, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::AUTO:
            return Matrix<T>::AutoMatrixMultiplicationAlgorithm::multiply(*this, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::ALPHATENSOR:
            return Matrix<T>::AlphaTensorMatrixMultiplicationAlgorithm::multiply(*this, other);
        default:
            throw std::invalid_argument("Invalid algorithm type");
    }
}

#endif // MATRIX_FUNCTIONS 