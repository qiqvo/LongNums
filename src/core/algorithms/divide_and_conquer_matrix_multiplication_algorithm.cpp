#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// DivideAndConquerMatrixMultiplicationAlgorithm class implementation (CRTP)
// Note: The multiply method is now implemented in the header as a template method

template<typename T>
template<typename Derived>
void Matrix<T>::DivideAndConquerMatrixMultiplicationAlgorithm<Derived>::validate_divide_and_conquer_inputs(const Matrix<T>& matrix, const Matrix<T>& other) {
    MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    
    if (!matrix.is_square() || !other.is_square() || matrix.rows() != other.rows()) {
        throw std::invalid_argument("Divide-and-conquer algorithms require square matrices of the same size");
    }
}

template<typename T>
template<typename Derived>
std::pair<Matrix<T>, Matrix<T>> Matrix<T>::DivideAndConquerMatrixMultiplicationAlgorithm<Derived>::pad_odd_matrices(const Matrix<T>& A, const Matrix<T>& B) {
    size_type n = A.rows();
    
    // If size is even, no padding needed
    if (n % 2 == 0) {
        return std::make_pair(A, B);
    }
    
    // For odd sizes, pad with one additional row and column of zeros
    size_type new_size = n + 1;
    
    Matrix<T> A_padded(new_size, new_size);
    Matrix<T> B_padded(new_size, new_size);
    
    // Copy original matrices
    for (size_type i = 0; i < n; ++i) {
        for (size_type j = 0; j < n; ++j) {
            A_padded(i, j) = A(i, j);
            B_padded(i, j) = B(i, j);
        }
    }
    
    // Pad with zeros (new rows and columns are already initialized to 0)
    
    return std::make_pair(A_padded, B_padded);
}

template<typename T>
template<typename Derived>
Matrix<T> Matrix<T>::DivideAndConquerMatrixMultiplicationAlgorithm<Derived>::extract_from_padded(const Matrix<T>& padded_result, size_type original_size) {
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
bool Matrix<T>::DivideAndConquerMatrixMultiplicationAlgorithm<Derived>::is_even(size_type n) {
    return n % 2 == 0;
}



#endif // MATRIX_FUNCTIONS
