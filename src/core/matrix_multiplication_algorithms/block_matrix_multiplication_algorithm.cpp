#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// BlockMatrixMultiplicationAlgorithm superbase class implementation (CRTP)
// Note: The multiply method is now implemented in the header as a template method

template<typename T>
template<int block_num, typename Derived>
void Matrix<T>::BlockMatrixMultiplicationAlgorithm<block_num, Derived>::validate_divide_and_conquer_inputs(const Matrix<T>& matrix, const Matrix<T>& other) {
    MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    
    if (!matrix.is_square() || !other.is_square() || matrix.rows() != other.rows()) {
        throw std::invalid_argument("Divide-and-conquer algorithms require square matrices of the same size");
    }
}

template<typename T>
template<int block_num, typename Derived>
Matrix<T> Matrix<T>::BlockMatrixMultiplicationAlgorithm<block_num, Derived>::extract_from_padded(const Matrix<T>& padded_result, size_type original_size) {
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
template<int block_num, typename Derived>
std::pair<Matrix<T>, Matrix<T>> Matrix<T>::BlockMatrixMultiplicationAlgorithm<block_num, Derived>::pad_matrices(const Matrix<T>& A, const Matrix<T>& B) {
    size_type n = A.rows();
    
    // If size is divisible by block_num, no padding needed
    if (n % block_num == 0) {
        return std::make_pair(A, B);
    }
    
    // For sizes not divisible by block_num, pad to the next multiple
    size_type new_size = ((n + block_num - 1) / block_num) * block_num;  // Round up to next multiple
    
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
template<int block_num, typename Derived>
Matrix<T> Matrix<T>::BlockMatrixMultiplicationAlgorithm<block_num, Derived>::combine_blocks(const Matrix<T>* blocks, size_type n) {
    // Combine blocks into result using direct indexing
    Matrix result(n, n);
    size_type block_size = n / block_num;
    size_type blocks_per_row = static_cast<size_type>(std::sqrt(block_num));
    
    for (size_type block_i = 0; block_i < blocks_per_row; ++block_i) {
        for (size_type block_j = 0; block_j < blocks_per_row; ++block_j) {
            const Matrix<T>& block = blocks[block_i * blocks_per_row + block_j];
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
