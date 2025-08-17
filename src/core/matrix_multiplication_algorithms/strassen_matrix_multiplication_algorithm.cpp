#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// StrassenMatrixMultiplicationAlgorithm class implementation
template<typename T>
Matrix<T>* Matrix<T>::StrassenMatrixMultiplicationAlgorithm::compute_from_blocks(const Matrix<T>* A_blocks, const Matrix<T>* B_blocks) {
    Matrix<T> P1 = StrassenMatrixMultiplicationAlgorithm::split_and_multiply(A_blocks[0], B_blocks[1] - B_blocks[3]);
    Matrix<T> P2 = StrassenMatrixMultiplicationAlgorithm::split_and_multiply(A_blocks[0] + A_blocks[1], B_blocks[3]);
    Matrix<T> P3 = StrassenMatrixMultiplicationAlgorithm::split_and_multiply(A_blocks[2] + A_blocks[3], B_blocks[0]);
    Matrix<T> P4 = StrassenMatrixMultiplicationAlgorithm::split_and_multiply(A_blocks[3], B_blocks[2] - B_blocks[0]);
    Matrix<T> P5 = StrassenMatrixMultiplicationAlgorithm::split_and_multiply(A_blocks[0] + A_blocks[3], B_blocks[0] + B_blocks[3]);
    Matrix<T> P6 = StrassenMatrixMultiplicationAlgorithm::split_and_multiply(A_blocks[1] - A_blocks[3], B_blocks[2] + B_blocks[3]);
    Matrix<T> P7 = StrassenMatrixMultiplicationAlgorithm::split_and_multiply(A_blocks[0] - A_blocks[2], B_blocks[0] + B_blocks[1]);
    
    // Combine results
    Matrix<T> C11 = P5 + P4 - P2 + P6;
    Matrix<T> C12 = P1 + P2;
    Matrix<T> C21 = P3 + P4;
    Matrix<T> C22 = P5 + P1 - P3 - P7;
    
    Matrix<T>* blocks = new Matrix<T>[4] {C11, C12, C21, C22};
    return blocks;
}

template<typename T>
Matrix<T> Matrix<T>::StrassenMatrixMultiplicationAlgorithm::multiply_2x2(const Matrix<T>& A, const Matrix<T>& B) {
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

