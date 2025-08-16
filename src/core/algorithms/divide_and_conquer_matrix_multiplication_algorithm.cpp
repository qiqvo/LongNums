#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// DivideAndConquerMatrixMultiplicationAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::DivideAndConquerMatrixMultiplicationAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    // This is a base class - should be overridden by derived classes
    // For now, fall back to naive multiplication
    return NaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
}

template<typename T>
void Matrix<T>::DivideAndConquerMatrixMultiplicationAlgorithm::validate_divide_and_conquer_inputs(const Matrix<T>& matrix, const Matrix<T>& other) {
    MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    
    if (!matrix.is_square() || !other.is_square() || matrix.rows() != other.rows()) {
        throw std::invalid_argument("Divide-and-conquer algorithms require square matrices of the same size");
    }
}

template<typename T>
std::pair<Matrix<T>, Matrix<T>> Matrix<T>::DivideAndConquerMatrixMultiplicationAlgorithm::pad_to_power_of_2(const Matrix<T>& A, const Matrix<T>& B) {
    size_type n = A.rows();
    size_type new_size = next_power_of_2(n);
    
    if (new_size == n) {
        // Already power of 2, return original matrices
        return std::make_pair(A, B);
    }
    
    Matrix<T> A_padded(new_size, new_size);
    Matrix<T> B_padded(new_size, new_size);
    
    // Copy original matrices
    for (size_type i = 0; i < n; ++i) {
        for (size_type j = 0; j < n; ++j) {
            A_padded(i, j) = A(i, j);
            B_padded(i, j) = B(i, j);
        }
    }
    
    return std::make_pair(A_padded, B_padded);
}

template<typename T>
Matrix<T> Matrix<T>::DivideAndConquerMatrixMultiplicationAlgorithm::extract_from_padded(const Matrix<T>& padded_result, size_type original_size) {
    if (padded_result.rows() == original_size) {
        return padded_result;
    }
    
    Matrix<T> result(original_size, original_size);
    for (size_type i = 0; i < original_size; ++i) {
        for (size_type j = 0; j < original_size; ++j) {
            result(i, j) = padded_result(i, j);
        }
    }
    
    return result;
}

template<typename T>
bool Matrix<T>::DivideAndConquerMatrixMultiplicationAlgorithm::is_power_of_2(size_type n) {
    return n > 0 && (n & (n - 1)) == 0;
}

template<typename T>
typename Matrix<T>::size_type Matrix<T>::DivideAndConquerMatrixMultiplicationAlgorithm::next_power_of_2(size_type n) {
    if (n <= 1) return 1;
    
    size_type power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}

#endif // MATRIX_FUNCTIONS
