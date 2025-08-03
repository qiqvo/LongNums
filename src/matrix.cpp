#ifndef ALPHATENSOR_MATRIX_FUNCTIONS
#include "matrix.h"

// Non-template (regular) function implementations go here
// Currently empty since all functions are templates

#else

namespace alphatensor {

// Template function implementations

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
    for (auto& element : data_) {
        element *= scalar;
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
typename Matrix<T>::size_type Matrix<T>::rows() const { return rows_; }

template<typename T>
typename Matrix<T>::size_type Matrix<T>::cols() const { return cols_; }

template<typename T>
bool Matrix<T>::is_square() const { return rows_ == cols_; }

template<typename T>
bool Matrix<T>::is_empty() const { return data_.empty(); }

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

template<typename T>
void Matrix<T>::randomize(value_type min, value_type max, unsigned seed) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<value_type> dist(min, max);
    
    for (auto& element : data_) {
        element = dist(gen);
    }
}

template<typename T>
void Matrix<T>::randomize_normal(value_type mean, value_type stddev, unsigned seed) {
    std::mt19937 gen(seed);
    std::normal_distribution<value_type> dist(mean, stddev);
    
    for (auto& element : data_) {
        element = dist(gen);
    }
}

template<typename T>
void Matrix<T>::fill(value_type value) {
    std::fill(data_.begin(), data_.end(), value);
}

template<typename T>
void Matrix<T>::zero() {
    fill(0.0);
}

template<typename T>
void Matrix<T>::identity() {
    if (!is_square()) {
        throw std::invalid_argument("Identity matrix must be square");
    }
    zero();
    for (size_type i = 0; i < rows_; ++i) {
        (*this)(i, i) = 1.0;
    }
}

template<typename T>
typename Matrix<T>::value_type Matrix<T>::trace() const {
    if (!is_square()) {
        throw std::invalid_argument("Trace only defined for square matrices");
    }
    value_type sum = 0.0;
    for (size_type i = 0; i < rows_; ++i) {
        sum += (*this)(i, i);
    }
    return sum;
}

template<typename T>
typename Matrix<T>::value_type Matrix<T>::norm() const {
    value_type sum = 0.0;
    for (const auto& element : data_) {
        sum += element * element;
    }
    return std::sqrt(sum);
}

template<typename T>
void Matrix<T>::print(std::ostream& os, int precision) const {
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

template<typename T>
void Matrix<T>::save_to_file(const std::string& filename) const {
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

template<typename T>
Matrix<T> Matrix<T>::load_from_file(const std::string& filename) {
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

template<typename T>
typename Matrix<T>::value_type* Matrix<T>::data() { return data_.data(); }

template<typename T>
const typename Matrix<T>::value_type* Matrix<T>::data() const { return data_.data(); }

template<typename T>
const std::vector<typename Matrix<T>::value_type>& Matrix<T>::get_data() const { return data_; }

template<typename T>
bool Matrix<T>::is_equal(const Matrix& other, value_type tolerance) const {
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

template<typename T>
typename Matrix<T>::size_type Matrix<T>::index(size_type row, size_type col) const {
    return row * cols_ + col;
}

template<typename T>
void Matrix<T>::check_dimensions(const Matrix& other, const std::string& operation) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions incompatible for " + operation);
    }
}

template<typename T>
void Matrix<T>::check_bounds(size_type row, size_type col) const {
    if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("Matrix index out of bounds");
    }
}

// Free functions
template<typename T>
Matrix<T> operator*(typename Matrix<T>::value_type scalar, const Matrix<T>& matrix) {
    return matrix * scalar;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
    matrix.print(os);
    return os;
}

// Utility functions
template<typename T>
Matrix<T> create_identity(size_type size) {
    Matrix<T> result(size, size);
    result.identity();
    return result;
}

template<typename T>
Matrix<T> create_random(size_type rows, size_type cols, 
                    T min, T max, unsigned seed) {
    Matrix<T> result(rows, cols);
    result.randomize(min, max, seed);
    return result;
}

template<typename T>
Matrix<T> create_random_normal(size_type rows, size_type cols,
                          T mean, T stddev, unsigned seed) {
    Matrix<T> result(rows, cols);
    result.randomize_normal(mean, stddev, seed);
    return result;
}

} // namespace alphatensor

#endif // ALPHATENSOR_MATRIX_FUNCTIONS 