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
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::SIMD_NAIVE:
            return Matrix<T>::SimdNaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::ARM_NEON:
            return Matrix<T>::ArmNeonMatrixMultiplicationAlgorithm::multiply(matrix, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN:
            return Matrix<T>::StrassenMatrixMultiplicationAlgorithm::multiply(matrix, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::WINOGRAD:
            return Matrix<T>::WinogradMatrixMultiplicationAlgorithm::multiply(matrix, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::ALPHA_TENSOR:
            return Matrix<T>::AlphaTensorMatrixMultiplicationAlgorithm::multiply(matrix, other);
        case Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::AUTO:
            // Fallback to naive for AUTO case to avoid infinite recursion
            return Matrix<T>::NaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
        default:
            return Matrix<T>::NaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
    }
}


// MatrixMultiplicationAlgorithm selection and configuration
template<typename T>
typename Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType Matrix<T>::AutoMatrixMultiplicationAlgorithm::select_best_algorithm(size_type size) {
    if (size <= Matrix<T>::AutoMatrixMultiplicationAlgorithm::get_thresholds().naive_threshold) {
        return Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE;
    } else if (size <= Matrix<T>::AutoMatrixMultiplicationAlgorithm::get_thresholds().strassen_threshold) {
        return Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN;
    } else if (size <= Matrix<T>::AutoMatrixMultiplicationAlgorithm::get_thresholds().alpha_tensor_threshold) {
        return Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::ALPHA_TENSOR;
    } else {
        throw std::invalid_argument("Invalid algorithm type");
    }
}

template<typename T>
void Matrix<T>::AutoMatrixMultiplicationAlgorithm::set_thresholds(
    size_type naive_threshold,
    size_type strassen_threshold,
    size_type alpha_tensor_threshold
) {
    Matrix<T>::AutoMatrixMultiplicationAlgorithm::thresholds_.naive_threshold = naive_threshold;
    Matrix<T>::AutoMatrixMultiplicationAlgorithm::thresholds_.strassen_threshold = strassen_threshold;
    Matrix<T>::AutoMatrixMultiplicationAlgorithm::thresholds_.alpha_tensor_threshold = alpha_tensor_threshold;
}

template<typename T>
typename Matrix<T>::AutoMatrixMultiplicationAlgorithm::AutoMatrixMultiplicationAlgorithmThresholds
Matrix<T>::AutoMatrixMultiplicationAlgorithm::get_thresholds() {
    return Matrix<T>::AutoMatrixMultiplicationAlgorithm::thresholds_;
}

#endif // MATRIX_FUNCTIONS

