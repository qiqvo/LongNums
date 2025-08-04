#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// AutoMatrixMultiplicationAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::AutoMatrixMultiplicationAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    auto best_algorithm = Matrix<T>::AutoMatrixMultiplicationAlgorithm::select_best_algorithm(matrix.rows());
    switch (best_algorithm) {
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE:
            return Matrix<T>::NaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::BLOCK:
            return Matrix<T>::BlockMatrixMultiplicationAlgorithm::multiply(matrix, other, Matrix<T>::AutoMatrixMultiplicationAlgorithm::get_thresholds().block_size);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN:
            return Matrix<T>::StrassenMatrixMultiplicationAlgorithm::multiply(matrix, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::WINOGRAD:
            return Matrix<T>::WinogradMatrixMultiplicationAlgorithm::multiply(matrix, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::HYBRID:
            return Matrix<T>::HybridMatrixMultiplicationAlgorithm::multiply(matrix, other);
    }
}


// MatrixMultiplicationAlgorithm selection and configuration
template<typename T>
typename Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType Matrix<T>::AutoMatrixMultiplicationAlgorithm::select_best_algorithm(size_type size) {
    if (size <= Matrix<T>::AutoMatrixMultiplicationAlgorithm::get_thresholds().naive_threshold) {
        return Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE;
    } else if (size <= Matrix<T>::AutoMatrixMultiplicationAlgorithm::get_thresholds().strassen_threshold) {
        return Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::BLOCK;
    } else {
        return Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN;
    }
}

#endif // MATRIX_FUNCTIONS 