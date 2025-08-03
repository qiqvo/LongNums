#include "algorithms.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cstring>

namespace alphatensor {

// NaiveAlgorithm implementation
Matrix NaiveAlgorithm::multiply(const Matrix& A, const Matrix& B) const {
    if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
        throw std::invalid_argument("Matrix dimensions incompatible for naive multiplication");
    }
    
    Matrix result(A.rows(), B.cols());
    
    for (size_type i = 0; i < A.rows(); ++i) {
        for (size_type j = 0; j < B.cols(); ++j) {
            value_type sum = 0.0;
            for (size_type k = 0; k < A.cols(); ++k) {
                sum += A(i, k) * B(k, j);
            }
            result(i, j) = sum;
        }
    }
    
    return result;
}

bool NaiveAlgorithm::can_handle(size_type A_rows, size_type A_cols,
                               size_type B_rows, size_type B_cols) const {
    return A_cols == B_rows;
}

// BlockAlgorithm implementation
BlockAlgorithm::BlockAlgorithm(size_type block_size) : block_size_(block_size) {}

Matrix BlockAlgorithm::multiply(const Matrix& A, const Matrix& B) const {
    if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
        throw std::invalid_argument("Matrix dimensions incompatible for block multiplication");
    }
    
    Matrix result(A.rows(), B.cols());
    result.zero();
    
    size_type n = A.rows();
    size_type m = A.cols();
    size_type p = B.cols();
    
    // Block matrix multiplication
    for (size_type i = 0; i < n; i += block_size_) {
        size_type i_end = std::min(i + block_size_, n);
        for (size_type j = 0; j < p; j += block_size_) {
            size_type j_end = std::min(j + block_size_, p);
            for (size_type k = 0; k < m; k += block_size_) {
                size_type k_end = std::min(k + block_size_, m);
                
                // Multiply block (i,j) with block (k,j)
                for (size_type ii = i; ii < i_end; ++ii) {
                    for (size_type jj = j; jj < j_end; ++jj) {
                        value_type sum = result(ii, jj);
                        for (size_type kk = k; kk < k_end; ++kk) {
                            sum += A(ii, kk) * B(kk, jj);
                        }
                        result(ii, jj) = sum;
                    }
                }
            }
        }
    }
    
    return result;
}

bool BlockAlgorithm::can_handle(size_type A_rows, size_type A_cols,
                               size_type B_rows, size_type B_cols) const {
    return A_cols == B_rows;
}

// StrassenAlgorithm implementation
Matrix StrassenAlgorithm::multiply(const Matrix& A, const Matrix& B) const {
    if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
        throw std::invalid_argument("Matrix dimensions incompatible for Strassen multiplication");
    }
    
    return strassen_recursive(A, B);
}

bool StrassenAlgorithm::can_handle(size_type A_rows, size_type A_cols,
                                  size_type B_rows, size_type B_cols) const {
    return A_cols == B_rows && A_rows == A_cols && B_rows == B_cols;
}

Matrix StrassenAlgorithm::strassen_recursive(const Matrix& A, const Matrix& B) const {
    size_type n = A.rows();
    
    // Base case: use naive multiplication for small matrices
    if (n <= 64) {
        return NaiveAlgorithm().multiply(A, B);
    }
    
    // Ensure matrix size is power of 2
    if (n % 2 != 0) {
        // Pad matrices to next power of 2
        size_type new_size = 1;
        while (new_size < n) new_size *= 2;
        
        Matrix A_padded(new_size, new_size);
        Matrix B_padded(new_size, new_size);
        A_padded.zero();
        B_padded.zero();
        
        // Copy original matrices
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                A_padded(i, j) = A(i, j);
                B_padded(i, j) = B(i, j);
            }
        }
        
        Matrix C_padded = strassen_recursive(A_padded, B_padded);
        
        // Extract result
        Matrix result(n, n);
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                result(i, j) = C_padded(i, j);
            }
        }
        return result;
    }
    
    // Split matrices into quadrants
    size_type half = n / 2;
    
    Matrix A11 = A.submatrix(0, 0, half, half);
    Matrix A12 = A.submatrix(0, half, half, n);
    Matrix A21 = A.submatrix(half, 0, n, half);
    Matrix A22 = A.submatrix(half, half, n, n);
    
    Matrix B11 = B.submatrix(0, 0, half, half);
    Matrix B12 = B.submatrix(0, half, half, n);
    Matrix B21 = B.submatrix(half, 0, n, half);
    Matrix B22 = B.submatrix(half, half, n, n);
    
    // Strassen's seven multiplications
    Matrix P1 = strassen_recursive(A11, B12 - B22);
    Matrix P2 = strassen_recursive(A11 + A12, B22);
    Matrix P3 = strassen_recursive(A21 + A22, B11);
    Matrix P4 = strassen_recursive(A22, B21 - B11);
    Matrix P5 = strassen_recursive(A11 + A22, B11 + B22);
    Matrix P6 = strassen_recursive(A12 - A22, B21 + B22);
    Matrix P7 = strassen_recursive(A11 - A21, B11 + B12);
    
    // Combine results
    Matrix C11 = P5 + P4 - P2 + P6;
    Matrix C12 = P1 + P2;
    Matrix C21 = P3 + P4;
    Matrix C22 = P5 + P1 - P3 - P7;
    
    // Combine quadrants into result
    Matrix result(n, n);
    result.set_submatrix(0, 0, C11);
    result.set_submatrix(0, half, C12);
    result.set_submatrix(half, 0, C21);
    result.set_submatrix(half, half, C22);
    
    return result;
}

Matrix StrassenAlgorithm::strassen_2x2(const Matrix& A, const Matrix& B) const {
    Matrix result(2, 2);
    
    // Direct 2x2 multiplication
    result(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0);
    result(0, 1) = A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1);
    result(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0);
    result(1, 1) = A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1);
    
    return result;
}

// WinogradAlgorithm implementation
Matrix WinogradAlgorithm::multiply(const Matrix& A, const Matrix& B) const {
    if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
        throw std::invalid_argument("Matrix dimensions incompatible for Winograd multiplication");
    }
    
    return winograd_recursive(A, B);
}

bool WinogradAlgorithm::can_handle(size_type A_rows, size_type A_cols,
                                  size_type B_rows, size_type B_cols) const {
    return A_cols == B_rows && A_rows == A_cols && B_rows == B_cols;
}

Matrix WinogradAlgorithm::winograd_recursive(const Matrix& A, const Matrix& B) const {
    size_type n = A.rows();
    
    // Base case: use naive multiplication for small matrices
    if (n <= 64) {
        return NaiveAlgorithm().multiply(A, B);
    }
    
    // Ensure matrix size is power of 2
    if (n % 2 != 0) {
        // Pad matrices to next power of 2
        size_type new_size = 1;
        while (new_size < n) new_size *= 2;
        
        Matrix A_padded(new_size, new_size);
        Matrix B_padded(new_size, new_size);
        A_padded.zero();
        B_padded.zero();
        
        // Copy original matrices
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                A_padded(i, j) = A(i, j);
                B_padded(i, j) = B(i, j);
            }
        }
        
        Matrix C_padded = winograd_recursive(A_padded, B_padded);
        
        // Extract result
        Matrix result(n, n);
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                result(i, j) = C_padded(i, j);
            }
        }
        return result;
    }
    
    // Split matrices into quadrants
    size_type half = n / 2;
    
    Matrix A11 = A.submatrix(0, 0, half, half);
    Matrix A12 = A.submatrix(0, half, half, n);
    Matrix A21 = A.submatrix(half, 0, n, half);
    Matrix A22 = A.submatrix(half, half, n, n);
    
    Matrix B11 = B.submatrix(0, 0, half, half);
    Matrix B12 = B.submatrix(0, half, half, n);
    Matrix B21 = B.submatrix(half, 0, n, half);
    Matrix B22 = B.submatrix(half, half, n, n);
    
    // Winograd's algorithm (similar to Strassen but with different combinations)
    Matrix S1 = A21 + A22;
    Matrix S2 = S1 - A11;
    Matrix S3 = A11 - A21;
    Matrix S4 = A12 - S2;
    
    Matrix T1 = B12 - B11;
    Matrix T2 = B22 - T1;
    Matrix T3 = B22 - B12;
    Matrix T4 = T2 - B21;
    
    Matrix P1 = winograd_recursive(A11, B11);
    Matrix P2 = winograd_recursive(A12, B21);
    Matrix P3 = winograd_recursive(S1, T1);
    Matrix P4 = winograd_recursive(S2, T2);
    Matrix P5 = winograd_recursive(S3, T3);
    Matrix P6 = winograd_recursive(S4, B22);
    Matrix P7 = winograd_recursive(A22, T4);
    
    // Combine results
    Matrix U1 = P1 + P2;
    Matrix U2 = P1 + P4;
    Matrix U3 = U2 + P5;
    Matrix U4 = U3 + P7;
    Matrix U5 = U3 + P3;
    Matrix U6 = U2 + P3;
    Matrix U7 = U6 + P6;
    
    Matrix C11 = U1;
    Matrix C12 = U7;
    Matrix C21 = U4;
    Matrix C22 = U5;
    
    // Combine quadrants into result
    Matrix result(n, n);
    result.set_submatrix(0, 0, C11);
    result.set_submatrix(0, half, C12);
    result.set_submatrix(half, 0, C21);
    result.set_submatrix(half, half, C22);
    
    return result;
}

Matrix WinogradAlgorithm::winograd_2x2(const Matrix& A, const Matrix& B) const {
    Matrix result(2, 2);
    
    // Direct 2x2 multiplication
    result(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0);
    result(0, 1) = A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1);
    result(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0);
    result(1, 1) = A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1);
    
    return result;
}

// HybridAlgorithm implementation
HybridAlgorithm::HybridAlgorithm(size_type naive_threshold, size_type strassen_threshold)
    : naive_threshold_(naive_threshold), strassen_threshold_(strassen_threshold) {}

Matrix HybridAlgorithm::multiply(const Matrix& A, const Matrix& B) const {
    if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
        throw std::invalid_argument("Matrix dimensions incompatible for hybrid multiplication");
    }
    
    size_type size = A.rows();
    auto algorithm = choose_algorithm(size);
    return algorithm->multiply(A, B);
}

bool HybridAlgorithm::can_handle(size_type A_rows, size_type A_cols,
                                size_type B_rows, size_type B_cols) const {
    return A_cols == B_rows && A_rows == A_cols && B_rows == B_cols;
}

std::shared_ptr<MatrixMultiplicationAlgorithm> HybridAlgorithm::choose_algorithm(size_type size) const {
    if (size <= naive_threshold_) {
        return std::make_shared<NaiveAlgorithm>();
    } else if (size <= strassen_threshold_) {
        return std::make_shared<BlockAlgorithm>();
    } else {
        return std::make_shared<StrassenAlgorithm>();
    }
}

// AlgorithmFactory implementation
std::shared_ptr<MatrixMultiplicationAlgorithm> AlgorithmFactory::create(Type type) {
    switch (type) {
        case Type::NAIVE:
            return std::make_shared<NaiveAlgorithm>();
        case Type::BLOCK:
            return std::make_shared<BlockAlgorithm>();
        case Type::STRASSEN:
            return std::make_shared<StrassenAlgorithm>();
        case Type::WINOGRAD:
            return std::make_shared<WinogradAlgorithm>();
        case Type::ALPHATENSOR_GPU:
            return std::make_shared<AlphaTensorAlgorithm>("gpu");
        case Type::ALPHATENSOR_TPU:
            return std::make_shared<AlphaTensorAlgorithm>("tpu");
        case Type::HYBRID:
            return std::make_shared<HybridAlgorithm>();
        default:
            throw std::invalid_argument("Unknown algorithm type");
    }
}

std::shared_ptr<MatrixMultiplicationAlgorithm> AlgorithmFactory::create(const std::string& name) {
    if (name == "naive" || name == "Naive") {
        return create(Type::NAIVE);
    } else if (name == "block" || name == "Block") {
        return create(Type::BLOCK);
    } else if (name == "strassen" || name == "Strassen") {
        return create(Type::STRASSEN);
    } else if (name == "winograd" || name == "Winograd") {
        return create(Type::WINOGRAD);
    } else if (name == "alphatensor-gpu" || name == "AlphaTensor-GPU") {
        return create(Type::ALPHATENSOR_GPU);
    } else if (name == "alphatensor-tpu" || name == "AlphaTensor-TPU") {
        return create(Type::ALPHATENSOR_TPU);
    } else if (name == "hybrid" || name == "Hybrid") {
        return create(Type::HYBRID);
    } else {
        throw std::invalid_argument("Unknown algorithm name: " + name);
    }
}

std::vector<std::string> AlgorithmFactory::available_algorithms() {
    return {
        "naive", "block", "strassen", "winograd", 
        "alphatensor-gpu", "alphatensor-tpu", "hybrid"
    };
}

// Utility functions
Matrix multiply_matrices(const Matrix& A, const Matrix& B,
                        const std::shared_ptr<MatrixMultiplicationAlgorithm>& algorithm) {
    return algorithm->multiply(A, B);
}

} // namespace alphatensor 