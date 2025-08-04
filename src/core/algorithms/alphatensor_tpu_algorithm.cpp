#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// AlphaTensorTPUAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::AlphaTensorTPUAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    Algorithm::validate_dimensions(matrix, other);
    return matrix.multiply_alphatensor(other, "tpu");
}

#endif // MATRIX_FUNCTIONS 