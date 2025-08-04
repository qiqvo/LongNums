#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// AlphaTensorGPUAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::AlphaTensorGPUAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    Algorithm::validate_dimensions(matrix, other);
    return matrix.multiply_alphatensor(other, "gpu");
}

#endif // MATRIX_FUNCTIONS 