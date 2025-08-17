#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

template<typename T>
Matrix<T>* Matrix<T>::WinogradMatrixMultiplicationAlgorithm::compute_from_blocks(const Matrix<T>* A_blocks, const Matrix<T>* B_blocks) {
    // Winograd's algorithm
    Matrix S1 = A_blocks[2] + A_blocks[3];
    Matrix S2 = S1 - A_blocks[0];
    Matrix S3 = A_blocks[0] - A_blocks[2];
    Matrix S4 = A_blocks[1] - S2;
    
    Matrix T1 = B_blocks[1] - B_blocks[0];
    Matrix T2 = B_blocks[3] - T1;
    Matrix T3 = B_blocks[3] - B_blocks[1];
    Matrix T4 = T2 - B_blocks[2];
    
    Matrix P1 = WinogradMatrixMultiplicationAlgorithm::split_and_multiply(A_blocks[0], B_blocks[0]);
    Matrix P2 = WinogradMatrixMultiplicationAlgorithm::split_and_multiply(A_blocks[1], B_blocks[2]);
    Matrix P3 = WinogradMatrixMultiplicationAlgorithm::split_and_multiply(S4, B_blocks[3]);
    Matrix P4 = WinogradMatrixMultiplicationAlgorithm::split_and_multiply(A_blocks[3], T4);
    Matrix P5 = WinogradMatrixMultiplicationAlgorithm::split_and_multiply(S1, T1);
    Matrix P6 = WinogradMatrixMultiplicationAlgorithm::split_and_multiply(S2, T2);
    Matrix P7 = WinogradMatrixMultiplicationAlgorithm::split_and_multiply(S3, T3);
    
    Matrix U1 = P1 + P2;
    Matrix U2 = P1 + P6;
    Matrix U3 = U2 + P7;
    Matrix U4 = U2 + P5;
    Matrix U5 = U4 + P3;
    Matrix U6 = U3 - P4;
    Matrix U7 = U3 + P5;
    
    Matrix<T>* blocks = new Matrix<T>[4] {U1, U5, U6, U7};
    return blocks;
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

