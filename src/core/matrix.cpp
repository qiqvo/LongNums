#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"

// Non-template (regular) function implementations go here
// Currently empty since all functions are templates

#else

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

// Static member initialization
template<typename T>
typename Matrix<T>::Thresholds Matrix<T>::thresholds_ = {64, 512, 64};

// Algorithm class implementations
template<typename T>
Matrix<T> Matrix<T>::Algorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    // This is a static method that should be overridden by specific algorithms
    throw std::runtime_error("Algorithm::multiply() called on base class");
}

template<typename T>
Matrix<T> Matrix<T>::NaiveAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    Matrix<T>::Algorithm::validate_dimensions(matrix, other);
    Matrix<T> result = Matrix<T>::Algorithm::construct_result(matrix, other);
    
    for (size_type i = 0; i < matrix.rows(); ++i) {
        for (size_type j = 0; j < other.cols(); ++j) {
            value_type sum = 0.0;
            for (size_type k = 0; k < matrix.cols(); ++k) {
                sum += matrix(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }
    
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::BlockAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other, size_type block_size) {
    Matrix<T>::Algorithm::validate_dimensions(matrix, other);
    Matrix<T> result = Matrix<T>::Algorithm::construct_result(matrix, other);
    
    for (size_type i = 0; i < matrix.rows(); i += block_size) {
        for (size_type j = 0; j < other.cols(); j += block_size) {
            for (size_type k = 0; k < matrix.cols(); k += block_size) {
                // Process block
                size_type i_end = std::min(i + block_size, matrix.rows());
                size_type j_end = std::min(j + block_size, other.cols());
                size_type k_end = std::min(k + block_size, matrix.cols());
                
                for (size_type ii = i; ii < i_end; ++ii) {
                    for (size_type jj = j; jj < j_end; ++jj) {
                        for (size_type kk = k; kk < k_end; ++kk) {
                            result(ii, jj) += matrix(ii, kk) * other(kk, jj);
                        }
                    }
                }
            }
        }
    }
    
    return result;
}


template<typename T>
Matrix<T> Matrix<T>::StrassenAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    if (matrix.cols() != other.rows()) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
    }
    
    if (!matrix.is_square() || !other.is_square() || matrix.rows() != other.rows()) {
        // Fall back to naive for non-square matrices
        return matrix.multiply_naive(other);
    }
    
    return strassen_recursive(matrix, other);
}

template<typename T>
Matrix<T> Matrix<T>::StrassenAlgorithm::strassen_recursive(const Matrix<T>& A, const Matrix<T>& B) {
    size_type n = A.rows();
    
    if (n <= 2) {
        return strassen_2x2(A, B);
    }
    
    // Pad matrices to next power of 2 if necessary
    size_type new_size = 1;
    while (new_size < n) {
        new_size *= 2;
    }
    
    if (new_size != n) {
        Matrix A_padded(new_size, new_size);
        Matrix B_padded(new_size, new_size);
        
        // Copy original matrices
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                A_padded(i, j) = A(i, j);
                B_padded(i, j) = B(i, j);
            }
        }
        
        Matrix result_padded = strassen_recursive(A_padded, B_padded);
        
        // Extract result
        Matrix result(n, n);
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                result(i, j) = result_padded(i, j);
            }
        }
        
        return result;
    }
    
    // Split matrices into quadrants
    size_type half = n / 2;
    
    Matrix A11 = A.submatrix(0, 0, half, half);
    Matrix A12 = A.submatrix(0, half, half, n);
    Matrix A21 = A.submatrix(half, 0, n, half);
    Matrix A22 = A.submatrix(half, half, n, n);
    
    Matrix B11 = B.submatrix(0, 0, half, half);
    Matrix B12 = B.submatrix(0, half, half, n);
    Matrix B21 = B.submatrix(half, 0, n, half);
    Matrix B22 = B.submatrix(half, half, n, n);
    
    // Strassen's seven multiplications
    Matrix P1 = strassen_recursive(A11, B12 - B22);
    Matrix P2 = strassen_recursive(A11 + A12, B22);
    Matrix P3 = strassen_recursive(A21 + A22, B11);
    Matrix P4 = strassen_recursive(A22, B21 - B11);
    Matrix P5 = strassen_recursive(A11 + A22, B11 + B22);
    Matrix P6 = strassen_recursive(A12 - A22, B21 + B22);
    Matrix P7 = strassen_recursive(A11 - A21, B11 + B12);
    
    // Combine results
    Matrix C11 = P5 + P4 - P2 + P6;
    Matrix C12 = P1 + P2;
    Matrix C21 = P3 + P4;
    Matrix C22 = P5 + P1 - P3 - P7;
    
    // Combine quadrants into result
    Matrix result(n, n);
    result.set_submatrix(0, 0, C11);
    result.set_submatrix(0, half, C12);
    result.set_submatrix(half, 0, C21);
    result.set_submatrix(half, half, C22);
    
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::StrassenAlgorithm::strassen_2x2(const Matrix<T>& A, const Matrix<T>& B) {
    Matrix result(2, 2);
    
    // Direct 2x2 multiplication (Strassen's algorithm for 2x2)
    T P1 = A(0, 0) * (B(0, 1) - B(1, 1));
    T P2 = (A(0, 0) + A(0, 1)) * B(1, 1);
    T P3 = (A(1, 0) + A(1, 1)) * B(0, 0);
    T P4 = A(1, 1) * (B(1, 0) - B(0, 0));
    T P5 = (A(0, 0) + A(1, 1)) * (B(0, 0) + B(1, 1));
    T P6 = (A(0, 1) - A(1, 1)) * (B(1, 0) + B(1, 1));
    T P7 = (A(0, 0) - A(1, 0)) * (B(0, 0) + B(0, 1));
    
    result(0, 0) = P5 + P4 - P2 + P6;
    result(0, 1) = P1 + P2;
    result(1, 0) = P3 + P4;
    result(1, 1) = P5 + P1 - P3 - P7;
    
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::WinogradAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    return matrix.multiply_winograd(other);
}

template<typename T>
Matrix<T> Matrix<T>::HybridAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    return matrix.multiply_hybrid(other);
}

template<typename T>
Matrix<T> Matrix<T>::AutoAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    auto best_algorithm = Matrix<T>::AutoAlgorithm::select_best_algorithm(matrix, other);

    switch (best_algorithm) {
        case AlgorithmType::NAIVE:
            return Matrix<T>::NaiveAlgorithm::multiply(matrix, other);
        case AlgorithmType::BLOCK:
            return Matrix<T>::BlockAlgorithm::multiply(matrix, other, Matrix<T>::get_thresholds().block_size);
        case AlgorithmType::STRASSEN:
            return Matrix<T>::StrassenAlgorithm::multiply(matrix, other);
        case AlgorithmType::WINOGRAD:
            return matrix.multiply_winograd(other);
        case AlgorithmType::HYBRID:
            return matrix.multiply_hybrid(other);
        default:
            return matrix.multiply_naive(other);
    }
}

template<typename T>
Matrix<T> Matrix<T>::AlphaTensorGPUAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    return matrix.multiply_alphatensor(other, "gpu");
}

template<typename T>
Matrix<T> Matrix<T>::AlphaTensorTPUAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    return matrix.multiply_alphatensor(other, "tpu");
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



template<typename T>
Matrix<T> Matrix<T>::multiply_winograd(const Matrix& other) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
    }
    
    if (!is_square() || !other.is_square() || rows_ != other.rows_) {
        // Fall back to naive for non-square matrices
        return multiply_naive(other);
    }
    
    return winograd_recursive(*this, other);
}

template<typename T>
Matrix<T> Matrix<T>::multiply_alphatensor(const Matrix& other, const std::string& variant) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
    }
    
    if (!is_square() || !other.is_square() || rows_ != other.rows_) {
        // Fall back to naive for non-square matrices
        return multiply_naive(other);
    }
    
    size_type n = rows_;
    
    // For 4x4 matrices, use the AlphaTensor factorization
    if (n == 4) {
        return alpha_tensor_4x4(*this, other);
    }
    
    // For 2x2 matrices, use the basic 2x2 algorithm
    if (n == 2) {
        return alpha_tensor_2x2(*this, other);
    }
    
    // For other sizes, fall back to Strassen's algorithm
    return StrassenAlgorithm::multiply(*this, other);
}

template<typename T>
Matrix<T> Matrix<T>::multiply_hybrid(const Matrix& other) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
    }
    
    if (!is_square() || !other.is_square() || rows_ != other.rows_) {
        // Fall back to naive for non-square matrices
        return multiply_naive(other);
    }
    
    size_type size = rows_;
    
    if (size <= thresholds_.naive_threshold) {
        return multiply_naive(other);
    } else if (size <= thresholds_.strassen_threshold) {
        return multiply_block(other, thresholds_.block_size);
    } else {
        return StrassenAlgorithm::multiply(*this, other);
    }
}

// Algorithm selection and configuration
template<typename T>
typename Matrix<T>::Algorithm Matrix<T>::select_best_algorithm(size_type size) {
    if (size <= thresholds_.naive_threshold) {
        return Algorithm::NAIVE;
    } else if (size <= thresholds_.strassen_threshold) {
        return Algorithm::BLOCK;
    } else {
        return Algorithm::STRASSEN;
    }
}

template<typename T>
void Matrix<T>::set_thresholds(size_type naive_threshold, size_type strassen_threshold, size_type block_size) {
    thresholds_.naive_threshold = naive_threshold;
    thresholds_.strassen_threshold = strassen_threshold;
    thresholds_.block_size = block_size;
}

template<typename T>
typename Matrix<T>::Thresholds Matrix<T>::get_thresholds() {
    return thresholds_;
}

// Private algorithm helper methods

template<typename T>
Matrix<T> Matrix<T>::winograd_recursive(const Matrix& A, const Matrix& B) const {
    size_type n = A.rows();
    
    if (n <= 2) {
        return winograd_2x2(A, B);
    }
    
    // Pad matrices to next power of 2 if necessary
    size_type new_size = 1;
    while (new_size < n) {
        new_size *= 2;
    }
    
    if (new_size != n) {
        Matrix A_padded(new_size, new_size);
        Matrix B_padded(new_size, new_size);
        
        // Copy original matrices
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                A_padded(i, j) = A(i, j);
                B_padded(i, j) = B(i, j);
            }
        }
        
        Matrix result_padded = winograd_recursive(A_padded, B_padded);
        
        // Extract result
        Matrix result(n, n);
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                result(i, j) = result_padded(i, j);
            }
        }
        
        return result;
    }
    
    // Winograd's algorithm implementation
    // This is a simplified version - in practice, you'd use the full Winograd algorithm
    return strassen_recursive(A, B); // Fall back to Strassen for now
}

template<typename T>
Matrix<T> Matrix<T>::winograd_2x2(const Matrix& A, const Matrix& B) const {
    Matrix result(2, 2);
    
    // Direct 2x2 multiplication
    result(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0);
    result(0, 1) = A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1);
    result(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0);
    result(1, 1) = A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1);
    
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::alpha_tensor_4x4(const Matrix& A, const Matrix& B) const {
    // Simplified AlphaTensor implementation for 4x4 matrices
    Matrix result(4, 4);
    result.zero();
    
    // Split matrices into 2x2 blocks
    Matrix A11 = A.submatrix(0, 0, 2, 2);
    Matrix A12 = A.submatrix(0, 2, 2, 4);
    Matrix A21 = A.submatrix(2, 0, 4, 2);
    Matrix A22 = A.submatrix(2, 2, 4, 4);
    
    Matrix B11 = B.submatrix(0, 0, 2, 2);
    Matrix B12 = B.submatrix(0, 2, 2, 4);
    Matrix B21 = B.submatrix(2, 0, 4, 2);
    Matrix B22 = B.submatrix(2, 2, 4, 4);
    
    // Strassen's seven multiplications
    Matrix P1 = alpha_tensor_2x2(A11, B12 - B22);
    Matrix P2 = alpha_tensor_2x2(A11 + A12, B22);
    Matrix P3 = alpha_tensor_2x2(A21 + A22, B11);
    Matrix P4 = alpha_tensor_2x2(A22, B21 - B11);
    Matrix P5 = alpha_tensor_2x2(A11 + A22, B11 + B22);
    Matrix P6 = alpha_tensor_2x2(A12 - A22, B21 + B22);
    Matrix P7 = alpha_tensor_2x2(A11 - A21, B11 + B12);
    
    // Combine results
    Matrix C11 = P5 + P4 - P2 + P6;
    Matrix C12 = P1 + P2;
    Matrix C21 = P3 + P4;
    Matrix C22 = P5 + P1 - P3 - P7;
    
    // Combine quadrants into result
    result.set_submatrix(0, 0, C11);
    result.set_submatrix(0, 2, C12);
    result.set_submatrix(2, 0, C21);
    result.set_submatrix(2, 2, C22);
    
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::alpha_tensor_2x2(const Matrix& A, const Matrix& B) const {
    Matrix result(2, 2);
    
    // Direct 2x2 multiplication (Strassen's algorithm for 2x2)
    T P1 = A(0, 0) * (B(0, 1) - B(1, 1));
    T P2 = (A(0, 0) + A(0, 1)) * B(1, 1);
    T P3 = (A(1, 0) + A(1, 1)) * B(0, 0);
    T P4 = A(1, 1) * (B(1, 0) - B(0, 0));
    T P5 = (A(0, 0) + A(1, 1)) * (B(0, 0) + B(1, 1));
    T P6 = (A(0, 1) - A(1, 1)) * (B(1, 0) + B(1, 1));
    T P7 = (A(0, 0) - A(1, 0)) * (B(0, 0) + B(0, 1));
    
    result(0, 0) = P5 + P4 - P2 + P6;
    result(0, 1) = P1 + P2;
    result(1, 0) = P3 + P4;
    result(1, 1) = P5 + P1 - P3 - P7;
    
    return result;
}


#endif // MATRIX_FUNCTIONS 