#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// Individual algorithm implementations

template<typename T>
Matrix<T> Matrix<T>::multiply_naive(const Matrix& other) const {
    Matrix<T>::Algorithm::validate_dimensions(*this, other);
    Matrix<T> result = Matrix<T>::Algorithm::construct_result(*this, other);
    
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
Matrix<T> Matrix<T>::multiply_block(const Matrix& other, size_type block_size) const {
    Matrix<T>::Algorithm::validate_dimensions(*this, other);
    Matrix<T> result = Matrix<T>::Algorithm::construct_result(*this, other);
    
    for (size_type i = 0; i < rows_; i += block_size) {
        for (size_type j = 0; j < other.cols_; j += block_size) {
            for (size_type k = 0; k < cols_; k += block_size) {
                // Process block
                size_type i_end = std::min(i + block_size, rows_);
                size_type j_end = std::min(j + block_size, other.cols_);
                size_type k_end = std::min(k + block_size, cols_);
                
                for (size_type ii = i; ii < i_end; ++ii) {
                    for (size_type jj = j; jj < j_end; ++jj) {
                        for (size_type kk = k; kk < k_end; ++kk) {
                            result(ii, jj) += (*this)(ii, kk) * other(kk, jj);
                        }
                    }
                }
            }
        }
    }
    
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::multiply_winograd(const Matrix& other) const {
    Matrix<T>::Algorithm::validate_dimensions(*this, other);
    
    if (!is_square() || !other.is_square() || rows_ != other.rows_) {
        // Fall back to naive for non-square matrices
        return Matrix<T>::NaiveAlgorithm::multiply(*this, other);
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
    
    // Winograd's algorithm
    Matrix S1 = A21 + A22;
    Matrix S2 = S1 - A11;
    Matrix S3 = A11 - A21;
    Matrix S4 = A12 - S2;
    
    Matrix T1 = B12 - B11;
    Matrix T2 = B22 - T1;
    Matrix T3 = B22 - B12;
    Matrix T4 = T2 - B21;
    
    Matrix P1 = A11 * B11;
    Matrix P2 = A12 * B21;
    Matrix P3 = S4 * B22;
    Matrix P4 = A22 * T4;
    Matrix P5 = S1 * T1;
    Matrix P6 = S2 * T2;
    Matrix P7 = S3 * T3;
    
    Matrix U1 = P1 + P2;
    Matrix U2 = P1 + P6;
    Matrix U3 = U2 + P7;
    Matrix U4 = U2 + P5;
    Matrix U5 = U4 + P3;
    Matrix U6 = U3 - P4;
    Matrix U7 = U3 + P5;
    
    // Combine quadrants into result
    Matrix result(n, n);
    result.set_submatrix(0, 0, U1);
    result.set_submatrix(0, half, U5);
    result.set_submatrix(half, 0, U6);
    result.set_submatrix(half, half, U7);
    
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::winograd_2x2(const Matrix& A, const Matrix& B) const {
    Matrix result(2, 2);
    
    // Winograd's algorithm for 2x2 matrices
    T S1 = A(1, 0) + A(1, 1);
    T S2 = S1 - A(0, 0);
    T S3 = A(0, 0) - A(1, 0);
    T S4 = A(0, 1) - S2;
    
    T T1 = B(0, 1) - B(0, 0);
    T T2 = B(1, 1) - T1;
    T T3 = B(1, 1) - B(0, 1);
    T T4 = T2 - B(1, 0);
    
    T P1 = A(0, 0) * B(0, 0);
    T P2 = A(0, 1) * B(1, 0);
    T P3 = S4 * B(1, 1);
    T P4 = A(1, 1) * T4;
    T P5 = S1 * T1;
    T P6 = S2 * T2;
    T P7 = S3 * T3;
    
    T U1 = P1 + P2;
    T U2 = P1 + P6;
    T U3 = U2 + P7;
    T U4 = U2 + P5;
    T U5 = U4 + P3;
    T U6 = U3 - P4;
    T U7 = U3 + P5;
    
    result(0, 0) = U1;
    result(0, 1) = U5;
    result(1, 0) = U6;
    result(1, 1) = U7;
    
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::alpha_tensor_4x4(const Matrix& A, const Matrix& B) const {
    // AlphaTensor factorization for 4x4 matrices
    // This is a simplified implementation
    Matrix result(4, 4);
    
    // For now, fall back to Strassen's algorithm
    return StrassenAlgorithm::multiply(A, B);
}

template<typename T>
Matrix<T> Matrix<T>::alpha_tensor_2x2(const Matrix& A, const Matrix& B) const {
    // AlphaTensor factorization for 2x2 matrices
    // This is a simplified implementation
    Matrix result(2, 2);
    
    // For now, fall back to Strassen's algorithm
    return StrassenAlgorithm::multiply(A, B);
}

#endif // MATRIX_FUNCTIONS 