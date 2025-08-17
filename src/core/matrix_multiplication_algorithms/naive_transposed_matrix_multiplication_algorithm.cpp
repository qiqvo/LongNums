#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// NaiveTransposedMatrixMultiplicationAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::NaiveTransposedMatrixMultiplicationAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    Matrix<T>::MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    Matrix<T> result = Matrix<T>::MatrixMultiplicationAlgorithm::construct_result(matrix, other);
    Matrix<T> other_transposed = other.transpose();

    for (size_type i = 0; i < matrix.rows(); ++i) {
        for (size_type k = 0; k < matrix.cols(); ++k) {
            T a_ik = matrix(i, k);
            for (size_type j = 0; j < other_transposed.cols(); ++j) {
                result(i, j) += a_ik * other_transposed(j, k);
            }
        }
    }
    
    return result;
}
#endif // MATRIX_FUNCTIONS

