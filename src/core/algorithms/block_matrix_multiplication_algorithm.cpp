#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// BlockMatrixMultiplicationAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::BlockMatrixMultiplicationAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other, size_type block_size) {
    MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    Matrix<T> result = Matrix<T>::MatrixMultiplicationAlgorithm::construct_result(matrix, other);
    
    for (size_type i = 0; i < matrix.rows(); i += block_size) {
        for (size_type j = 0; j < other.cols(); j += block_size) {
            for (size_type k = 0; k < matrix.cols(); k += block_size) {
                // Process block
                size_type i_end = std::min(i + block_size, matrix.rows());
                size_type j_end = std::min(j + block_size, other.cols());
                size_type k_end = std::min(k + block_size, matrix.cols());
                
                for (size_type ii = i; ii < i_end; ++ii) {
                    for (size_type jj = j; jj < j_end; ++jj) {
                        for (size_type kk = k; kk < k_end; ++kk) {
                            result(ii, jj) += matrix(ii, kk) * other(kk, jj);
                        }
                    }
                }
            }
        }
    }
    
    return result;
}

#endif // MATRIX_FUNCTIONS

