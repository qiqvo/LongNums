#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// AutoAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::AutoAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    auto best_algorithm = Matrix<T>::select_best_algorithm(matrix.rows());
    switch (best_algorithm) {
        case AlgorithmType::NAIVE:
            return matrix.multiply_naive(other);
        case AlgorithmType::BLOCK:
            return matrix.multiply_block(other, Matrix<T>::get_thresholds().block_size);
        case AlgorithmType::STRASSEN:
            return Matrix<T>::StrassenAlgorithm::multiply(matrix, other);
        case AlgorithmType::WINOGRAD:
            return matrix.multiply_winograd(other);
        case AlgorithmType::HYBRID:
            return matrix.multiply_hybrid(other);
        default:
            return matrix.multiply_naive(other);
    }
}

#endif // MATRIX_FUNCTIONS 