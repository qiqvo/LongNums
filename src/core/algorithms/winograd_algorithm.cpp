#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// WinogradMatrixMultiplicationAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::WinogradMatrixMultiplicationAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    
    if (!matrix.is_square() || !other.is_square() || matrix.rows() != other.rows()) {
        // Fall back to naive for non-square matrices
        return Matrix<T>::NaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
    }
    
    return WinogradMatrixMultiplicationAlgorithm::winograd_recursive(matrix, other);
}


// Private algorithm helper methods
template<typename T>
Matrix<T> Matrix<T>::WinogradMatrixMultiplicationAlgorithm::winograd_recursive(const Matrix<T>& A, const Matrix<T>& B) {
    size_type n = A.rows();
    
    if (n <= 2) {
        return WinogradMatrixMultiplicationAlgorithm::winograd_2x2(A, B);
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
        
        Matrix result_padded = WinogradMatrixMultiplicationAlgorithm::winograd_recursive(A_padded, B_padded);
        
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
    
    Matrix P1 = WinogradMatrixMultiplicationAlgorithm::multiply(A11, B11);
    Matrix P2 = WinogradMatrixMultiplicationAlgorithm::multiply(A12, B21);
    Matrix P3 = WinogradMatrixMultiplicationAlgorithm::multiply(S4, B22);
    Matrix P4 = WinogradMatrixMultiplicationAlgorithm::multiply(A22, T4);
    Matrix P5 = WinogradMatrixMultiplicationAlgorithm::multiply(S1, T1);
    Matrix P6 = WinogradMatrixMultiplicationAlgorithm::multiply(S2, T2);
    Matrix P7 = WinogradMatrixMultiplicationAlgorithm::multiply(S3, T3);
    
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
Matrix<T> Matrix<T>::WinogradMatrixMultiplicationAlgorithm::winograd_2x2(const Matrix<T>& A, const Matrix<T>& B) {
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

#endif // MATRIX_FUNCTIONS 