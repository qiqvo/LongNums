#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// HybridMatrixMultiplicationAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::HybridMatrixMultiplicationAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    if (!matrix.is_square() || !other.is_square() || matrix.rows() != other.rows()) {
        // Fall back to naive for non-square matrices
        return Matrix<T>::NaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
    }
    
    size_type size = matrix.rows();
    
    if (size <= Matrix<T>::AutoMatrixMultiplicationAlgorithm::get_thresholds().naive_threshold) {
        return Matrix<T>::NaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
    } else if (size <= Matrix<T>::AutoMatrixMultiplicationAlgorithm::get_thresholds().strassen_threshold) {
        return Matrix<T>::BlockMatrixMultiplicationAlgorithm::multiply(matrix, other);
    } else {
        return Matrix<T>::StrassenMatrixMultiplicationAlgorithm::multiply(matrix, other);
    }
}
#endif // MATRIX_FUNCTIONS

