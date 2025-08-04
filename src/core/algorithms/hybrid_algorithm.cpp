#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// HybridAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::HybridAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    Algorithm::validate_dimensions(matrix, other);
    
    if (!matrix.is_square() || !other.is_square() || matrix.rows() != other.rows()) {
        // Fall back to naive for non-square matrices
        return matrix.multiply_naive(other);
    }
    
    return matrix.multiply_hybrid(other);
}

#endif // MATRIX_FUNCTIONS 