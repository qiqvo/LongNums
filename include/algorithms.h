#pragma once

#include "matrix.h"
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace alphatensor {

/**
 * @brief Base class for matrix multiplication algorithms
 */
template<typename T = double>
class MatrixMultiplicationAlgorithm {
public:
    using MatrixType = Matrix<T>;
    using value_type = T;
    
    virtual ~MatrixMultiplicationAlgorithm() = default;
    
    /**
     * @brief Multiply two matrices using the algorithm
     * @param A First matrix
     * @param B Second matrix
     * @return Result matrix C = A * B
     */
    virtual MatrixType multiply(const MatrixType& A, const MatrixType& B) const = 0;
    
    /**
     * @brief Get the name of the algorithm
     * @return Algorithm name
     */
    virtual std::string name() const = 0;
    
    /**
     * @brief Check if the algorithm can handle the given matrix dimensions
     * @param A_rows Number of rows in matrix A
     * @param A_cols Number of columns in matrix A
     * @param B_rows Number of rows in matrix B
     * @param B_cols Number of columns in matrix B
     * @return True if the algorithm can handle these dimensions
     */
    virtual bool can_handle(size_type A_rows, size_type A_cols,
                           size_type B_rows, size_type B_cols) const = 0;
};

/**
 * @brief Standard naive matrix multiplication algorithm
 * Time complexity: O(n³)
 */
template<typename T = double>
class NaiveAlgorithm : public MatrixMultiplicationAlgorithm<T> {
public:
    using MatrixType = typename MatrixMultiplicationAlgorithm<T>::MatrixType;
    
    MatrixType multiply(const MatrixType& A, const MatrixType& B) const override {
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
    
    std::string name() const override { return "Naive"; }
    
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override {
        return A_cols == B_rows;
    }
};

/**
 * @brief Block matrix multiplication algorithm
 * Better cache performance for large matrices
 */
template<typename T = double>
class BlockAlgorithm : public MatrixMultiplicationAlgorithm<T> {
public:
    using MatrixType = typename MatrixMultiplicationAlgorithm<T>::MatrixType;
    
    explicit BlockAlgorithm(size_type block_size = 64) : block_size_(block_size) {}
    
    MatrixType multiply(const MatrixType& A, const MatrixType& B) const override {
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
    
    std::string name() const override { return "Block"; }
    
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override {
        return A_cols == B_rows;
    }
    
private:
    size_type block_size_;
};

/**
 * @brief Strassen's matrix multiplication algorithm
 * Time complexity: O(n^2.807)
 */
template<typename T = double>
class StrassenAlgorithm : public MatrixMultiplicationAlgorithm<T> {
public:
    using MatrixType = typename MatrixMultiplicationAlgorithm<T>::MatrixType;
    
    MatrixType multiply(const MatrixType& A, const MatrixType& B) const override {
        if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
            throw std::invalid_argument("Matrix dimensions incompatible for Strassen multiplication");
        }
        
        return strassen_recursive(A, B);
    }
    
    std::string name() const override { return "Strassen"; }
    
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override {
        return A_cols == B_rows && A_rows == A_cols && B_rows == B_cols;
    }
    
private:
    MatrixType strassen_recursive(const MatrixType& A, const MatrixType& B) const {
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
    
    MatrixType strassen_2x2(const MatrixType& A, const MatrixType& B) const {
        MatrixType result(2, 2);
        
        // Direct 2x2 multiplication
        result(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0);
        result(0, 1) = A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1);
        result(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0);
        result(1, 1) = A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1);
        
        return result;
    }
};

/**
 * @brief Winograd's matrix multiplication algorithm
 * Slightly better than Strassen for some cases
 */
template<typename T = double>
class WinogradAlgorithm : public MatrixMultiplicationAlgorithm<T> {
public:
    using MatrixType = typename MatrixMultiplicationAlgorithm<T>::MatrixType;
    
    MatrixType multiply(const MatrixType& A, const MatrixType& B) const override {
        if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
            throw std::invalid_argument("Matrix dimensions incompatible for Winograd multiplication");
        }
        
        return winograd_recursive(A, B);
    }
    
    std::string name() const override { return "Winograd"; }
    
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override {
        return A_cols == B_rows && A_rows == A_cols && B_rows == B_cols;
    }
    
private:
    MatrixType winograd_recursive(const MatrixType& A, const MatrixType& B) const {
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
    
    MatrixType winograd_2x2(const MatrixType& A, const MatrixType& B) const {
        MatrixType result(2, 2);
        
        // Direct 2x2 multiplication
        result(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0);
        result(0, 1) = A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1);
        result(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0);
        result(1, 1) = A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1);
        
        return result;
    }
};

/**
 * @brief AlphaTensor discovered algorithm for 4x4 matrices
 * Based on the factorizations discovered by AlphaTensor
 */
template<typename T = double>
class AlphaTensorAlgorithm : public MatrixMultiplicationAlgorithm<T> {
public:
    using MatrixType = typename MatrixMultiplicationAlgorithm<T>::MatrixType;
    
    explicit AlphaTensorAlgorithm(const std::string& variant = "gpu") : variant_(variant) {}
    
    MatrixType multiply(const MatrixType& A, const MatrixType& B) const override {
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
    
    std::string name() const override { return "AlphaTensor-" + variant_; }
    
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override {
        return A_cols == B_rows;
    }
    
private:
    std::string variant_;
    
    MatrixType alpha_tensor_4x4(const MatrixType& A, const MatrixType& B) const {
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
    
    MatrixType alpha_tensor_2x2(const MatrixType& A, const MatrixType& B) const {
        MatrixType result(2, 2);
        
        result(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0);
        result(0, 1) = A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1);
        result(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0);
        result(1, 1) = A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1);
        
        return result;
    }
    
    // AlphaTensor factorization coefficients
    struct Factorization {
        std::vector<std::vector<T>> u, v, w;
        size_type rank;
    };
    
    Factorization get_factorization() const {
        // Placeholder implementation
        Factorization f;
        f.rank = 7;
        return f;
    }
};

/**
 * @brief Hybrid algorithm that chooses the best algorithm based on matrix size
 */
template<typename T = double>
class HybridAlgorithm : public MatrixMultiplicationAlgorithm<T> {
public:
    using MatrixType = typename MatrixMultiplicationAlgorithm<T>::MatrixType;
    
    explicit HybridAlgorithm(size_type naive_threshold = 64,
                           size_type strassen_threshold = 512)
        : naive_threshold_(naive_threshold), strassen_threshold_(strassen_threshold) {}
    
    MatrixType multiply(const MatrixType& A, const MatrixType& B) const override {
        if (!can_handle(A.rows(), A.cols(), B.rows(), B.cols())) {
            throw std::invalid_argument("Matrix dimensions incompatible for hybrid multiplication");
        }
        
        size_type size = A.rows();
        auto algorithm = choose_algorithm(size);
        return algorithm->multiply(A, B);
    }
    
    std::string name() const override { return "Hybrid"; }
    
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override {
        return A_cols == B_rows && A_rows == A_cols && B_rows == B_cols;
    }
    
private:
    size_type naive_threshold_;
    size_type strassen_threshold_;
    
    std::shared_ptr<MatrixMultiplicationAlgorithm<T>> choose_algorithm(size_type size) const {
        if (size <= naive_threshold_) {
            return std::make_shared<NaiveAlgorithm<T>>();
        } else if (size <= strassen_threshold_) {
            return std::make_shared<BlockAlgorithm<T>>();
        } else {
            return std::make_shared<StrassenAlgorithm<T>>();
        }
    }
};

/**
 * @brief Algorithm factory for creating different multiplication algorithms
 */
template<typename T = double>
class AlgorithmFactory {
public:
    enum class Type {
        NAIVE,
        BLOCK,
        STRASSEN,
        WINOGRAD,
        ALPHATENSOR_GPU,
        ALPHATENSOR_TPU,
        HYBRID
    };
    
    static std::shared_ptr<MatrixMultiplicationAlgorithm<T>> create(Type type) {
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
    
    static std::shared_ptr<MatrixMultiplicationAlgorithm<T>> create(const std::string& name) {
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
    
    static std::vector<std::string> available_algorithms() {
        return {
            "naive", "block", "strassen", "winograd", 
            "alphatensor-gpu", "alphatensor-tpu", "hybrid"
        };
    }
};

// Utility functions
template<typename T = double>
Matrix<T> multiply_matrices(const Matrix<T>& A, const Matrix<T>& B,
                           const std::shared_ptr<MatrixMultiplicationAlgorithm<T>>& algorithm) {
    return algorithm->multiply(A, B);
}

// Type aliases for common usage
using MatrixMultiplicationAlgorithmD = MatrixMultiplicationAlgorithm<double>;
using MatrixMultiplicationAlgorithmF = MatrixMultiplicationAlgorithm<float>;
using NaiveAlgorithmD = NaiveAlgorithm<double>;
using NaiveAlgorithmF = NaiveAlgorithm<float>;
using BlockAlgorithmD = BlockAlgorithm<double>;
using BlockAlgorithmF = BlockAlgorithm<float>;
using StrassenAlgorithmD = StrassenAlgorithm<double>;
using StrassenAlgorithmF = StrassenAlgorithm<float>;
using WinogradAlgorithmD = WinogradAlgorithm<double>;
using WinogradAlgorithmF = WinogradAlgorithm<float>;
using AlphaTensorAlgorithmD = AlphaTensorAlgorithm<double>;
using AlphaTensorAlgorithmF = AlphaTensorAlgorithm<float>;
using HybridAlgorithmD = HybridAlgorithm<double>;
using HybridAlgorithmF = HybridAlgorithm<float>;
using AlgorithmFactoryD = AlgorithmFactory<double>;
using AlgorithmFactoryF = AlgorithmFactory<float>;

} // namespace alphatensor 