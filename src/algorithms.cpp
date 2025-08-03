#ifndef ALPHATENSOR_ALGORITHMS_FUNCTIONS
#include "algorithms.h"

// Non-template (regular) function implementations go here
// Currently empty since all functions are templates

#else

namespace alphatensor {

// Template function implementations

template<typename T>
typename NaiveAlgorithm<T>::MatrixType NaiveAlgorithm<T>::multiply(const MatrixType& A, const MatrixType& B) const {
    if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
        throw std::invalid_argument("Matrix dimensions incompatible for naive multiplication");
    }
    
    MatrixType result(A.rows(), B.cols());
    
    for (size_type i = 0; i < A.rows(); ++i) {
        for (size_type j = 0; j < B.cols(); ++j) {
            T sum = 0.0;
            for (size_type k = 0; k < A.cols(); ++k) {
                sum += A(i, k) * B(k, j);
            }
            result(i, j) = sum;
        }
    }
    
    return result;
}

template<typename T>
std::string NaiveAlgorithm<T>::name() const { return "Naive"; }

template<typename T>
bool NaiveAlgorithm<T>::can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const {
    return A_cols == B_rows;
}

template<typename T>
BlockAlgorithm<T>::BlockAlgorithm(size_type block_size) : block_size_(block_size) {}

template<typename T>
typename BlockAlgorithm<T>::MatrixType BlockAlgorithm<T>::multiply(const MatrixType& A, const MatrixType& B) const {
    if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
        throw std::invalid_argument("Matrix dimensions incompatible for block multiplication");
    }
    
    MatrixType result(A.rows(), B.cols());
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
                        T sum = result(ii, jj);
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

template<typename T>
std::string BlockAlgorithm<T>::name() const { return "Block"; }

template<typename T>
bool BlockAlgorithm<T>::can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const {
    return A_cols == B_rows;
}

template<typename T>
typename StrassenAlgorithm<T>::MatrixType StrassenAlgorithm<T>::multiply(const MatrixType& A, const MatrixType& B) const {
    if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
        throw std::invalid_argument("Matrix dimensions incompatible for Strassen multiplication");
    }
    
    return strassen_recursive(A, B);
}

template<typename T>
std::string StrassenAlgorithm<T>::name() const { return "Strassen"; }

template<typename T>
bool StrassenAlgorithm<T>::can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const {
    return A_cols == B_rows && A_rows == A_cols && B_rows == B_cols;
}

template<typename T>
typename StrassenAlgorithm<T>::MatrixType StrassenAlgorithm<T>::strassen_recursive(const MatrixType& A, const MatrixType& B) const {
    size_type n = A.rows();
    
    // Base case: use naive multiplication for small matrices
    if (n <= 64) {
        return NaiveAlgorithm<T>().multiply(A, B);
    }
    
    // Ensure matrix size is power of 2
    if (n % 2 != 0) {
        // Pad matrices to next power of 2
        size_type new_size = 1;
        while (new_size < n) new_size *= 2;
        
        MatrixType A_padded(new_size, new_size);
        MatrixType B_padded(new_size, new_size);
        A_padded.zero();
        B_padded.zero();
        
        // Copy original matrices
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                A_padded(i, j) = A(i, j);
                B_padded(i, j) = B(i, j);
            }
        }
        
        MatrixType C_padded = strassen_recursive(A_padded, B_padded);
        
        // Extract result
        MatrixType result(n, n);
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                result(i, j) = C_padded(i, j);
            }
        }
        return result;
    }
    
    // Split matrices into quadrants
    size_type half = n / 2;
    
    MatrixType A11 = A.submatrix(0, 0, half, half);
    MatrixType A12 = A.submatrix(0, half, half, n);
    MatrixType A21 = A.submatrix(half, 0, n, half);
    MatrixType A22 = A.submatrix(half, half, n, n);
    
    MatrixType B11 = B.submatrix(0, 0, half, half);
    MatrixType B12 = B.submatrix(0, half, half, n);
    MatrixType B21 = B.submatrix(half, 0, n, half);
    MatrixType B22 = B.submatrix(half, half, n, n);
    
    // Strassen's seven multiplications
    MatrixType P1 = strassen_recursive(A11, B12 - B22);
    MatrixType P2 = strassen_recursive(A11 + A12, B22);
    MatrixType P3 = strassen_recursive(A21 + A22, B11);
    MatrixType P4 = strassen_recursive(A22, B21 - B11);
    MatrixType P5 = strassen_recursive(A11 + A22, B11 + B22);
    MatrixType P6 = strassen_recursive(A12 - A22, B21 + B22);
    MatrixType P7 = strassen_recursive(A11 - A21, B11 + B12);
    
    // Combine results
    MatrixType C11 = P5 + P4 - P2 + P6;
    MatrixType C12 = P1 + P2;
    MatrixType C21 = P3 + P4;
    MatrixType C22 = P5 + P1 - P3 - P7;
    
    // Combine quadrants into result
    MatrixType result(n, n);
    result.set_submatrix(0, 0, C11);
    result.set_submatrix(0, half, C12);
    result.set_submatrix(half, 0, C21);
    result.set_submatrix(half, half, C22);
    
    return result;
}

template<typename T>
typename StrassenAlgorithm<T>::MatrixType StrassenAlgorithm<T>::strassen_2x2(const MatrixType& A, const MatrixType& B) const {
    MatrixType result(2, 2);
    
    // Direct 2x2 multiplication
    result(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0);
    result(0, 1) = A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1);
    result(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0);
    result(1, 1) = A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1);
    
    return result;
}

template<typename T>
typename WinogradAlgorithm<T>::MatrixType WinogradAlgorithm<T>::multiply(const MatrixType& A, const MatrixType& B) const {
    if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
        throw std::invalid_argument("Matrix dimensions incompatible for Winograd multiplication");
    }
    
    return winograd_recursive(A, B);
}

template<typename T>
std::string WinogradAlgorithm<T>::name() const { return "Winograd"; }

template<typename T>
bool WinogradAlgorithm<T>::can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const {
    return A_cols == B_rows && A_rows == A_cols && B_rows == B_cols;
}

template<typename T>
typename WinogradAlgorithm<T>::MatrixType WinogradAlgorithm<T>::winograd_recursive(const MatrixType& A, const MatrixType& B) const {
    size_type n = A.rows();
    
    // Base case: use naive multiplication for small matrices
    if (n <= 64) {
        return NaiveAlgorithm<T>().multiply(A, B);
    }
    
    // Ensure matrix size is power of 2
    if (n % 2 != 0) {
        // Pad matrices to next power of 2
        size_type new_size = 1;
        while (new_size < n) new_size *= 2;
        
        MatrixType A_padded(new_size, new_size);
        MatrixType B_padded(new_size, new_size);
        A_padded.zero();
        B_padded.zero();
        
        // Copy original matrices
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                A_padded(i, j) = A(i, j);
                B_padded(i, j) = B(i, j);
            }
        }
        
        MatrixType C_padded = winograd_recursive(A_padded, B_padded);
        
        // Extract result
        MatrixType result(n, n);
        for (size_type i = 0; i < n; ++i) {
            for (size_type j = 0; j < n; ++j) {
                result(i, j) = C_padded(i, j);
            }
        }
        return result;
    }
    
    // Split matrices into quadrants
    size_type half = n / 2;
    
    MatrixType A11 = A.submatrix(0, 0, half, half);
    MatrixType A12 = A.submatrix(0, half, half, n);
    MatrixType A21 = A.submatrix(half, 0, n, half);
    MatrixType A22 = A.submatrix(half, half, n, n);
    
    MatrixType B11 = B.submatrix(0, 0, half, half);
    MatrixType B12 = B.submatrix(0, half, half, n);
    MatrixType B21 = B.submatrix(half, 0, n, half);
    MatrixType B22 = B.submatrix(half, half, n, n);
    
    // Winograd's algorithm (similar to Strassen but with different combinations)
    MatrixType S1 = A21 + A22;
    MatrixType S2 = S1 - A11;
    MatrixType S3 = A11 - A21;
    MatrixType S4 = A12 - S2;
    
    MatrixType T1 = B12 - B11;
    MatrixType T2 = B22 - T1;
    MatrixType T3 = B22 - B12;
    MatrixType T4 = T2 - B21;
    
    MatrixType P1 = winograd_recursive(A11, B11);
    MatrixType P2 = winograd_recursive(A12, B21);
    MatrixType P3 = winograd_recursive(S1, T1);
    MatrixType P4 = winograd_recursive(S2, T2);
    MatrixType P5 = winograd_recursive(S3, T3);
    MatrixType P6 = winograd_recursive(S4, B22);
    MatrixType P7 = winograd_recursive(A22, T4);
    
    // Combine results
    MatrixType U1 = P1 + P2;
    MatrixType U2 = P1 + P4;
    MatrixType U3 = U2 + P5;
    MatrixType U4 = U3 + P7;
    MatrixType U5 = U3 + P3;
    MatrixType U6 = U2 + P3;
    MatrixType U7 = U6 + P6;
    
    MatrixType C11 = U1;
    MatrixType C12 = U7;
    MatrixType C21 = U4;
    MatrixType C22 = U5;
    
    // Combine quadrants into result
    MatrixType result(n, n);
    result.set_submatrix(0, 0, C11);
    result.set_submatrix(0, half, C12);
    result.set_submatrix(half, 0, C21);
    result.set_submatrix(half, half, C22);
    
    return result;
}

template<typename T>
typename WinogradAlgorithm<T>::MatrixType WinogradAlgorithm<T>::winograd_2x2(const MatrixType& A, const MatrixType& B) const {
    MatrixType result(2, 2);
    
    // Direct 2x2 multiplication
    result(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0);
    result(0, 1) = A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1);
    result(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0);
    result(1, 1) = A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1);
    
    return result;
}

template<typename T>
AlphaTensorAlgorithm<T>::AlphaTensorAlgorithm(const std::string& variant) : variant_(variant) {}

template<typename T>
typename AlphaTensorAlgorithm<T>::MatrixType AlphaTensorAlgorithm<T>::multiply(const MatrixType& A, const MatrixType& B) const {
    if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
        throw std::invalid_argument("Matrix dimensions incompatible for AlphaTensor multiplication");
    }
    
    if (A.rows() == 4 && B.rows() == 4) {
        return alpha_tensor_4x4(A, B);
    } else {
        // Fall back to naive for non-4x4 matrices
        return NaiveAlgorithm<T>().multiply(A, B);
    }
}

template<typename T>
std::string AlphaTensorAlgorithm<T>::name() const { return "AlphaTensor-" + variant_; }

template<typename T>
bool AlphaTensorAlgorithm<T>::can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const {
    return A_cols == B_rows;
}

template<typename T>
typename AlphaTensorAlgorithm<T>::MatrixType AlphaTensorAlgorithm<T>::alpha_tensor_4x4(const MatrixType& A, const MatrixType& B) const {
    // Simple implementation for 4x4 matrices
    MatrixType result(4, 4);
    
    for (size_type i = 0; i < 4; ++i) {
        for (size_type j = 0; j < 4; ++j) {
            T sum = 0.0;
            for (size_type k = 0; k < 4; ++k) {
                sum += A(i, k) * B(k, j);
            }
            result(i, j) = sum;
        }
    }
    
    return result;
}

template<typename T>
typename AlphaTensorAlgorithm<T>::MatrixType AlphaTensorAlgorithm<T>::alpha_tensor_2x2(const MatrixType& A, const MatrixType& B) const {
    MatrixType result(2, 2);
    
    result(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0);
    result(0, 1) = A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1);
    result(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0);
    result(1, 1) = A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1);
    
    return result;
}

template<typename T>
typename AlphaTensorAlgorithm<T>::Factorization AlphaTensorAlgorithm<T>::get_factorization() const {
    // Placeholder implementation
    Factorization f;
    f.rank = 7;
    return f;
}

template<typename T>
HybridAlgorithm<T>::HybridAlgorithm(size_type naive_threshold, size_type strassen_threshold)
    : naive_threshold_(naive_threshold), strassen_threshold_(strassen_threshold) {}

template<typename T>
typename HybridAlgorithm<T>::MatrixType HybridAlgorithm<T>::multiply(const MatrixType& A, const MatrixType& B) const {
    if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
        throw std::invalid_argument("Matrix dimensions incompatible for hybrid multiplication");
    }
    
    size_type size = A.rows();
    auto algorithm = choose_algorithm(size);
    return algorithm->multiply(A, B);
}

template<typename T>
std::string HybridAlgorithm<T>::name() const { return "Hybrid"; }

template<typename T>
bool HybridAlgorithm<T>::can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const {
    return A_cols == B_rows && A_rows == A_cols && B_rows == B_cols;
}

template<typename T>
std::shared_ptr<MatrixMultiplicationAlgorithm<T>> HybridAlgorithm<T>::choose_algorithm(size_type size) const {
    if (size <= naive_threshold_) {
        return std::make_shared<NaiveAlgorithm<T>>();
    } else if (size <= strassen_threshold_) {
        return std::make_shared<BlockAlgorithm<T>>();
    } else {
        return std::make_shared<StrassenAlgorithm<T>>();
    }
}

template<typename T>
std::shared_ptr<MatrixMultiplicationAlgorithm<T>> AlgorithmFactory<T>::create(Type type) {
    switch (type) {
        case Type::NAIVE:
            return std::make_shared<NaiveAlgorithm<T>>();
        case Type::BLOCK:
            return std::make_shared<BlockAlgorithm<T>>();
        case Type::STRASSEN:
            return std::make_shared<StrassenAlgorithm<T>>();
        case Type::WINOGRAD:
            return std::make_shared<WinogradAlgorithm<T>>();
        case Type::ALPHATENSOR_GPU:
            return std::make_shared<AlphaTensorAlgorithm<T>>("gpu");
        case Type::ALPHATENSOR_TPU:
            return std::make_shared<AlphaTensorAlgorithm<T>>("tpu");
        case Type::HYBRID:
            return std::make_shared<HybridAlgorithm<T>>();
        default:
            throw std::invalid_argument("Unknown algorithm type");
    }
}

template<typename T>
std::shared_ptr<MatrixMultiplicationAlgorithm<T>> AlgorithmFactory<T>::create(const std::string& name) {
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

template<typename T>
std::vector<std::string> AlgorithmFactory<T>::available_algorithms() {
    return {
        "naive", "block", "strassen", "winograd", 
        "alphatensor-gpu", "alphatensor-tpu", "hybrid"
    };
}

template<typename T>
Matrix<T> multiply_matrices(const Matrix<T>& A, const Matrix<T>& B,
                           const std::shared_ptr<MatrixMultiplicationAlgorithm<T>>& algorithm) {
    return algorithm->multiply(A, B);
}

} // namespace alphatensor

#endif // ALPHATENSOR_ALGORITHMS_FUNCTIONS 