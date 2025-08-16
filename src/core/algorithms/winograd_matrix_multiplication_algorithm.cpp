#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

template<typename T>
Matrix<T> Matrix<T>::WinogradMatrixMultiplicationAlgorithm::recursive_multiply_impl(const Matrix<T>& A, const Matrix<T>& B) {
    size_type n = A.rows();
    
    if (n <= 2) {
        return multiply_2x2(A, B);
    }
    
    // Use parent class helper methods for padding
    auto [A_padded, B_padded] = DivideAndConquerMatrixMultiplicationAlgorithm<WinogradMatrixMultiplicationAlgorithm>::pad_odd_matrices(A, B);
    
    if (A_padded.rows() != n) {
        Matrix result_padded = recursive_multiply_impl(A_padded, B_padded);
        return DivideAndConquerMatrixMultiplicationAlgorithm<WinogradMatrixMultiplicationAlgorithm>::extract_from_padded(result_padded, n);
    }
    
    // Split matrices into quadrants using direct indexing
    size_type half = n / 2;
    
    // Create temporary matrices for the multiplications
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
    
    // Winograd's algorithm
    Matrix S1 = A21 + A22;
    Matrix S2 = S1 - A11;
    Matrix S3 = A11 - A21;
    Matrix S4 = A12 - S2;
    
    Matrix T1 = B12 - B11;
    Matrix T2 = B22 - T1;
    Matrix T3 = B22 - B12;
    Matrix T4 = T2 - B21;
    
    Matrix P1 = recursive_multiply_impl(A11, B11);
    Matrix P2 = recursive_multiply_impl(A12, B21);
    Matrix P3 = recursive_multiply_impl(S4, B22);
    Matrix P4 = recursive_multiply_impl(A22, T4);
    Matrix P5 = recursive_multiply_impl(S1, T1);
    Matrix P6 = recursive_multiply_impl(S2, T2);
    Matrix P7 = recursive_multiply_impl(S3, T3);
    
    Matrix U1 = P1 + P2;
    Matrix U2 = P1 + P6;
    Matrix U3 = U2 + P7;
    Matrix U4 = U2 + P5;
    Matrix U5 = U4 + P3;
    Matrix U6 = U3 - P4;
    Matrix U7 = U3 + P5;
    
    // Use base class method to combine quadrants
    return DivideAndConquerMatrixMultiplicationAlgorithm<WinogradMatrixMultiplicationAlgorithm>::combine_quadrants(U1, U5, U6, U7, n);
}

template<typename T>
Matrix<T> Matrix<T>::WinogradMatrixMultiplicationAlgorithm::multiply_2x2(const Matrix<T>& A, const Matrix<T>& B) {
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

