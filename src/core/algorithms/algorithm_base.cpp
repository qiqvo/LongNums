#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// Base MatrixMultiplicationAlgorithm class implementations
template<typename T>
Matrix<T> Matrix<T>::MatrixMultiplicationAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    // This is a static method that should be overridden by specific algorithms
    throw std::runtime_error("MatrixMultiplicationAlgorithm::multiply() called on base class");
}

template<typename T>
Matrix<T> Matrix<T>::MatrixMultiplicationAlgorithm::construct_result(const Matrix<T>& matrix, const Matrix<T>& other) {
    return Matrix<T>(matrix.rows(), other.cols()).zero();
}

template<typename T>
void Matrix<T>::MatrixMultiplicationAlgorithm::validate_dimensions(const Matrix<T>& matrix, const Matrix<T>& other) {
    if (matrix.cols() != other.rows()) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
    }
}

#endif // MATRIX_FUNCTIONS 