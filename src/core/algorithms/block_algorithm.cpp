#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// BlockAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::BlockAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other, size_type block_size) {
    Algorithm::validate_dimensions(matrix, other);
    return matrix.multiply_block(other, block_size);
}

#endif // MATRIX_FUNCTIONS 