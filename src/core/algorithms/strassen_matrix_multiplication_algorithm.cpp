#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// StrassenMatrixMultiplicationAlgorithm class implementation
template<typename T>
Matrix<T> Matrix<T>::StrassenMatrixMultiplicationAlgorithm::multiply(const Matrix<T>& matrix, const Matrix<T>& other) {
    MatrixMultiplicationAlgorithm::validate_dimensions(matrix, other);
    
    if (!matrix.is_square() || !other.is_square() || matrix.rows() != other.rows()) {
        // Fall back to naive for non-square matrices
        return NaiveMatrixMultiplicationAlgorithm::multiply(matrix, other);
    }
    
    return strassen_recursive(matrix, other);
}

template<typename T>
Matrix<T> Matrix<T>::StrassenMatrixMultiplicationAlgorithm::strassen_recursive(const Matrix<T>& A, const Matrix<T>& B) {
    size_type n = A.rows();
    
    if (n <= 64) {  // Use naive for small matrices to avoid overhead
        return NaiveMatrixMultiplicationAlgorithm::multiply(A, B);
    }
    
    // Pad matrices to next power of 2 if necessary
    size_type new_size = 1;
    while (new_size < n) {
        new_size *= 2;
    }
    
    if (new_size != n) {
        Matrix A_padded(new_size, new_size);
        Matrix B_padded(new_size, new_size);
        
        // Copy original matrices
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                A_padded(i, j) = A(i, j);
                B_padded(i, j) = B(i, j);
            }
        }
        
        Matrix result_padded = strassen_recursive(A_padded, B_padded);
        
        // Extract result
        Matrix result(n, n);
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                result(i, j) = result_padded(i, j);
            }
        }
        
        return result;
    }
    
    // Split matrices into quadrants using direct indexing
    size_type half = n / 2;
    
    // Strassen's seven multiplications with optimized submatrix operations
    Matrix P1 = strassen_recursive_quadrant(A, B, 0, 0, 0, half, 0, half, half, n, 0, 1, 1, 1);  // A11 * (B12 - B22)
    Matrix P2 = strassen_recursive_quadrant(A, B, 0, 0, 0, half, 0, half, 0, half, 0, 0, 1, 1);  // (A11 + A12) * B22
    Matrix P3 = strassen_recursive_quadrant(A, B, half, 0, n, half, half, half, n, n, 0, 0, 0, 0);  // (A21 + A22) * B11
    Matrix P4 = strassen_recursive_quadrant(A, B, half, half, n, n, half, 0, n, half, 1, 0, 0, 0);  // A22 * (B21 - B11)
    Matrix P5 = strassen_recursive_quadrant(A, B, 0, 0, 0, half, half, half, n, n, 0, 0, 0, 0);  // (A11 + A22) * (B11 + B22)
    Matrix P6 = strassen_recursive_quadrant(A, B, 0, half, 0, half, half, half, n, n, 0, 1, 1, 1);  // (A12 - A22) * (B21 + B22)
    Matrix P7 = strassen_recursive_quadrant(A, B, 0, 0, 0, half, 0, 0, half, half, 0, 0, 0, 1);  // (A11 - A21) * (B11 + B12)
    
    // Combine results
    Matrix C11 = P5 + P4 - P2 + P6;
    Matrix C12 = P1 + P2;
    Matrix C21 = P3 + P4;
    Matrix C22 = P5 + P1 - P3 - P7;
    
    // Combine quadrants into result
    Matrix result(n, n);
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

template<typename T>
Matrix<T> Matrix<T>::StrassenMatrixMultiplicationAlgorithm::strassen_recursive_quadrant(
    const Matrix<T>& A, const Matrix<T>& B,
    size_type a_start_row, size_type a_start_col, size_type a_end_row, size_type a_end_col,
    size_type b_start_row, size_type b_start_col, size_type b_end_row, size_type b_end_col,
    int a_op, int b_op) {
    
    size_type a_rows = a_end_row - a_start_row;
    size_type a_cols = a_end_col - a_start_col;
    size_type b_rows = b_end_row - b_start_row;
    size_type b_cols = b_end_col - b_start_col;
    
    // Create temporary matrices for the operation
    Matrix<T> A_temp(a_rows, a_cols);
    Matrix<T> B_temp(b_rows, b_cols);
    
    // Copy data with operation
    for (size_type i = 0; i < a_rows; ++i) {
        for (size_type j = 0; j < a_cols; ++j) {
            A_temp(i, j) = A(a_start_row + i, a_start_col + j);
        }
    }
    
    for (size_type i = 0; i < b_rows; ++i) {
        for (size_type j = 0; j < b_cols; ++j) {
            B_temp(i, j) = B(b_start_row + i, b_start_col + j);
        }
    }
    
    // Apply operations based on op codes
    if (a_op == 1) {  // Addition
        for (size_type i = 0; i < a_rows; ++i) {
            for (size_type j = 0; j < a_cols; ++j) {
                A_temp(i, j) += A(a_start_row + i, a_start_col + j + a_cols);
            }
        }
    } else if (a_op == 2) {  // Subtraction
        for (size_type i = 0; i < a_rows; ++i) {
            for (size_type j = 0; j < a_cols; ++j) {
                A_temp(i, j) -= A(a_start_row + i + a_rows, a_start_col + j + a_cols);
            }
        }
    }
    
    if (b_op == 1) {  // Addition
        for (size_type i = 0; i < b_rows; ++i) {
            for (size_type j = 0; j < b_cols; ++j) {
                B_temp(i, j) += B(b_start_row + i + b_rows, b_start_col + j);
            }
        }
    } else if (b_op == 2) {  // Subtraction
        for (size_type i = 0; i < b_rows; ++i) {
            for (size_type j = 0; j < b_cols; ++j) {
                B_temp(i, j) -= B(b_start_row + i, b_start_col + j);
            }
        }
    }
    
    return strassen_recursive(A_temp, B_temp);
}

template<typename T>
Matrix<T> Matrix<T>::StrassenMatrixMultiplicationAlgorithm::strassen_2x2(const Matrix<T>& A, const Matrix<T>& B) {
    Matrix result(2, 2);
    
    // Direct 2x2 multiplication (Strassen's algorithm for 2x2)
    T P1 = A(0, 0) * (B(0, 1) - B(1, 1));
    T P2 = (A(0, 0) + A(0, 1)) * B(1, 1);
    T P3 = (A(1, 0) + A(1, 1)) * B(0, 0);
    T P4 = A(1, 1) * (B(1, 0) - B(0, 0));
    T P5 = (A(0, 0) + A(1, 1)) * (B(0, 0) + B(1, 1));
    T P6 = (A(0, 1) - A(1, 1)) * (B(1, 0) + B(1, 1));
    T P7 = (A(0, 0) - A(1, 0)) * (B(0, 0) + B(0, 1));
    
    result(0, 0) = P5 + P4 - P2 + P6;
    result(0, 1) = P1 + P2;
    result(1, 0) = P3 + P4;
    result(1, 1) = P5 + P1 - P3 - P7;
    
    return result;
}

#endif // MATRIX_FUNCTIONS

