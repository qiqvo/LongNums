#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// FourBlockMatrixMultiplicationAlgorithm class implementation (CRTP)
// Note: The multiply method is now implemented in the header as a template method



template<typename T>
template<typename Derived>
std::pair<Matrix<T>, Matrix<T>> Matrix<T>::FourBlockMatrixMultiplicationAlgorithm<Derived>::pad_odd_matrices(const Matrix<T>& A, const Matrix<T>& B) {
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
Matrix<T> Matrix<T>::FourBlockMatrixMultiplicationAlgorithm<Derived>::combine_quadrants(const Matrix<T>& C11, const Matrix<T>& C12, 
                                                                                               const Matrix<T>& C21, const Matrix<T>& C22, size_type n) {
    // Combine quadrants into result using direct indexing
    Matrix result(n, n);
    size_type half = n / 2;
    
    for (size_type i = 0; i < half; ++i) {
        for (size_type j = 0; j < half; ++j) {
            result(i, j) = C11(i, j);
            result(i, j + half) = C12(i, j);
            result(i + half, j) = C21(i, j);
            result(i + half, j + half) = C22(i, j);
        }
    }
    
    return result;
}

#endif // MATRIX_FUNCTIONS
