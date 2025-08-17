#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// BlockMatrixMultiplicationAlgorithm superbase class implementation (CRTP)
// Note: The multiply method is now implemented in the header as a template method

template<typename T>
template<int block_num, typename Derived>
Matrix<T> Matrix<T>::BlockMatrixMultiplicationAlgorithm<block_num, Derived>::split_and_multiply(const Matrix<T>& A, const Matrix<T>& B) {
    size_type n = A.rows();
    size_type blocks_per_row = static_cast<size_type>(std::sqrt(block_num));
    
    // Use naive for small matrices to avoid overhead
    if (n <= 16) {
        return NaiveMatrixMultiplicationAlgorithm::multiply(A, B);
    }
    
    Matrix<T> A_padded;
    Matrix<T> B_padded;
    if (n % blocks_per_row != 0) {
        auto padded_pair = BlockMatrixMultiplicationAlgorithm<block_num, Derived>::pad_matrices(A, B);
        A_padded = padded_pair.first;
        B_padded = padded_pair.second;
    } else {
        A_padded = A;
        B_padded = B;
    }
    size_type padded_size = A_padded.rows();
    size_type block_size = padded_size / blocks_per_row;

    // Create temporary matrices
    Matrix<T> A_blocks[block_num];
    Matrix<T> B_blocks[block_num];

    for (size_type i = 0; i < blocks_per_row; ++i) {
        for (size_type j = 0; j < blocks_per_row; ++j) {
            A_blocks[i * blocks_per_row + j] = A_padded.submatrix(i * block_size, j * block_size, (i + 1) * block_size, (j + 1) * block_size);
            B_blocks[i * blocks_per_row + j] = B_padded.submatrix(i * block_size, j * block_size, (i + 1) * block_size, (j + 1) * block_size);
        }
    }
    
    auto C_blocks = Derived::compute_from_blocks(A_blocks, B_blocks);
    
    // recombine to the original size
    return BlockMatrixMultiplicationAlgorithm<block_num, Derived>::combine_blocks(C_blocks, n);
}
    
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
std::pair<Matrix<T>, Matrix<T>> Matrix<T>::BlockMatrixMultiplicationAlgorithm<block_num, Derived>::pad_matrices(const Matrix<T>& A, const Matrix<T>& B) {
    size_type n = A.rows();
    size_type blocks_per_row = static_cast<size_type>(std::sqrt(block_num));
    
    // If size is divisible by block_num, no padding needed
    if (n % blocks_per_row == 0) {
        return std::make_pair(A, B);
    }
    
    // For sizes not divisible by blocks_per_row, pad to the next multiple
    size_type new_size = ((n + blocks_per_row - 1) / blocks_per_row) * blocks_per_row;  // Round up to next multiple
    
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
    size_type blocks_per_row = static_cast<size_type>(std::sqrt(block_num));
    
    // Check if blocks array is valid
    if (blocks == nullptr) {
        throw std::runtime_error("Blocks array is null in combine_blocks");
    }
    
    // Calculate the actual block size from the first block
    size_type actual_block_size = blocks[0].rows();
    
    for (size_type block_i = 0; block_i < blocks_per_row; ++block_i) {
        for (size_type block_j = 0; block_j < blocks_per_row; ++block_j) {
            const Matrix<T>& block = blocks[block_i * blocks_per_row + block_j];
            size_type start_i = block_i * actual_block_size;
            size_type start_j = block_j * actual_block_size;
            
            // Copy the block to the result, but only up to the original matrix size
            for (size_type i = 0; i < actual_block_size && start_i + i < n; ++i) {
                for (size_type j = 0; j < actual_block_size && start_j + j < n; ++j) {
                    result(start_i + i, start_j + j) = block(i, j);
                }
            }
        }
    }
    
    return result;
}

#endif // MATRIX_FUNCTIONS
