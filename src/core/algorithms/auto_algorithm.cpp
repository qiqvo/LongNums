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
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::ALPHATENSOR:
            return Matrix<T>::AlphaTensorMatrixMultiplicationAlgorithm::multiply(matrix, other);
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

template<typename T>
void Matrix<T>::AutoMatrixMultiplicationAlgorithm::set_thresholds(
    size_type naive_threshold,
    size_type strassen_threshold,
    size_type block_size
) {
    Matrix<T>::AutoMatrixMultiplicationAlgorithm::thresholds_.naive_threshold = naive_threshold;
    Matrix<T>::AutoMatrixMultiplicationAlgorithm::thresholds_.strassen_threshold = strassen_threshold;
    Matrix<T>::AutoMatrixMultiplicationAlgorithm::thresholds_.block_size = block_size;
}

template<typename T>
typename Matrix<T>::AutoMatrixMultiplicationAlgorithm::AutoMatrixMultiplicationAlgorithmThresholds
Matrix<T>::AutoMatrixMultiplicationAlgorithm::get_thresholds() {
    return Matrix<T>::AutoMatrixMultiplicationAlgorithm::thresholds_;
}

#endif // MATRIX_FUNCTIONS 