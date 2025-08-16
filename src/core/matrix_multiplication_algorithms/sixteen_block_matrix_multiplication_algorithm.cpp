#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// SixteenBlockMatrixMultiplicationAlgorithm class implementation (CRTP)
// Note: The multiply method is now implemented in the header as a template method



template<typename T>
template<typename Derived>
std::pair<Matrix<T>, Matrix<T>> Matrix<T>::SixteenBlockMatrixMultiplicationAlgorithm<Derived>::pad_matrices_for_16_blocks(const Matrix<T>& A, const Matrix<T>& B) {
    size_type n = A.rows();
    
    // If size is divisible by 4, no padding needed
    if (n % 4 == 0) {
        return std::make_pair(A, B);
    }
    
    // For sizes not divisible by 4, pad to the next multiple of 4
    size_type new_size = ((n + 3) / 4) * 4;  // Round up to next multiple of 4
    
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
bool Matrix<T>::SixteenBlockMatrixMultiplicationAlgorithm<Derived>::is_divisible_by_4(size_type n) {
    return n % 4 == 0;
}

template<typename T>
template<typename Derived>
Matrix<T> Matrix<T>::SixteenBlockMatrixMultiplicationAlgorithm<Derived>::combine_16_blocks(const Matrix<T> blocks[4][4], size_type n) {
    // Combine 16 blocks (4x4 grid) into result using direct indexing
    Matrix result(n, n);
    size_type block_size = n / 4;
    
    for (size_type block_i = 0; block_i < 4; ++block_i) {
        for (size_type block_j = 0; block_j < 4; ++block_j) {
            const Matrix<T>& block = blocks[block_i][block_j];
            size_type start_i = block_i * block_size;
            size_type start_j = block_j * block_size;
            
            for (size_type i = 0; i < block_size; ++i) {
                for (size_type j = 0; j < block_size; ++j) {
                    result(start_i + i, start_j + j) = block(i, j);
                }
            }
        }
    }
    
    return result;
}

#endif // MATRIX_FUNCTIONS
