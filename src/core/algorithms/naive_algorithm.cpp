#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// NaiveMatrixMultiplicationAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::NaiveMatrixMultiplicationAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    Matrix<T>::MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    Matrix<T> result = Matrix<T>::MatrixMultiplicationAlgorithm::construct_result(matrix, other);
    
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
#endif // MATRIX_FUNCTIONS 