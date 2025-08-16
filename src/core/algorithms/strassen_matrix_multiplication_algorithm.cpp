#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// StrassenMatrixMultiplicationAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::StrassenMatrixMultiplicationAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    
    if (!matrix.is_square() || !other.is_square() || matrix.rows() != other.rows()) {
        // Fall back to naive for non-square matrices
        return NaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
    }
    
    return strassen_recursive(matrix, other);
}

template<typename T>
Matrix<T> Matrix<T>::StrassenMatrixMultiplicationAlgorithm::strassen_recursive(const Matrix<T>& A, const Matrix<T>& B) {
    size_type n = A.rows();
    
    // Use naive for small matrices to avoid overhead
    if (n <= 16) {
        return NaiveMatrixMultiplicationAlgorithm::multiply(A, B);
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
    
    // Split matrices into quadrants using direct indexing
    size_type half = n / 2;
    
    // Create temporary matrices for the seven multiplications
    Matrix<T> A11(half, half), A12(half, half), A21(half, half), A22(half, half);
    Matrix<T> B11(half, half), B12(half, half), B21(half, half), B22(half, half);
    
    // Copy quadrants using direct indexing (no submatrix overhead)
    for (size_type i = 0; i < half; ++i) {
        for (size_type j = 0; j < half; ++j) {
            A11(i, j) = A(i, j);
            A12(i, j) = A(i, j + half);
            A21(i, j) = A(i + half, j);
            A22(i, j) = A(i + half, j + half);
            
            B11(i, j) = B(i, j);
            B12(i, j) = B(i, j + half);
            B21(i, j) = B(i + half, j);
            B22(i, j) = B(i + half, j + half);
        }
    }
    
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
    
    // Combine quadrants into result using direct indexing
    Matrix result(n, n);
    for (size_type i = 0; i < half; ++i) {
        for (size_type j = 0; j < half; ++j) {
            result(i, j) = C11(i, j);
            result(i, j + half) = C12(i, j);
            result(i + half, j) = C21(i, j);
            result(i + half, j + half) = C22(i, j);
        }
    }
    
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::StrassenMatrixMultiplicationAlgorithm::strassen_2x2(const Matrix<T>& A, const Matrix<T>& B) {
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

