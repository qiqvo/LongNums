#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// Base Algorithm class implementations
template<typename T>
Matrix<T> Matrix<T>::Algorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    // This is a static method that should be overridden by specific algorithms
    throw std::runtime_error("Algorithm::multiply() called on base class");
}

template<typename T>
Matrix<T> Matrix<T>::Algorithm::construct_result(const Matrix<T>& matrix, const Matrix<T>& other) {
    return Matrix<T>(matrix.rows(), other.cols()).zero();
}

template<typename T>
void Matrix<T>::Algorithm::validate_dimensions(const Matrix<T>& matrix, const Matrix<T>& other) {
    if (matrix.cols() != other.rows()) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
    }
}

#endif // MATRIX_FUNCTIONS 