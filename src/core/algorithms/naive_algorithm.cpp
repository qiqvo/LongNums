#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// NaiveAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::NaiveAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    Algorithm::validate_dimensions(matrix, other);
    return matrix.multiply_naive(other);
}

#endif // MATRIX_FUNCTIONS 