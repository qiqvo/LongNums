#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// BlockMatrixMultiplicationAlgorithm superbase class implementation (CRTP)
// Note: The multiply method is now implemented in the header as a template method

template<typename T>
template<typename Derived>
void Matrix<T>::BlockMatrixMultiplicationAlgorithm<Derived>::validate_divide_and_conquer_inputs(const Matrix<T>& matrix, const Matrix<T>& other) {
    MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    
    if (!matrix.is_square() || !other.is_square() || matrix.rows() != other.rows()) {
        throw std::invalid_argument("Divide-and-conquer algorithms require square matrices of the same size");
    }
}

template<typename T>
template<typename Derived>
Matrix<T> Matrix<T>::BlockMatrixMultiplicationAlgorithm<Derived>::extract_from_padded(const Matrix<T>& padded_result, size_type original_size) {
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
template<typename Derived>
bool Matrix<T>::BlockMatrixMultiplicationAlgorithm<Derived>::is_even(size_type n) {
    return n % 2 == 0;
}

#endif // MATRIX_FUNCTIONS
