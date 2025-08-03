#ifndef ALPHATENSOR_ALGORITHMS_H
#define ALPHATENSOR_ALGORITHMS_H

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
    
    MatrixType multiply(const MatrixType& A, const MatrixType& B) const override;
    std::string name() const override;
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override;
};

/**
 * @brief Block matrix multiplication algorithm
 * Better cache performance for large matrices
 */
template<typename T = double>
class BlockAlgorithm : public MatrixMultiplicationAlgorithm<T> {
public:
    using MatrixType = typename MatrixMultiplicationAlgorithm<T>::MatrixType;
    
    explicit BlockAlgorithm(size_type block_size = 64);
    MatrixType multiply(const MatrixType& A, const MatrixType& B) const override;
    std::string name() const override;
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override;
    
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
    
    MatrixType multiply(const MatrixType& A, const MatrixType& B) const override;
    std::string name() const override;
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override;
    
private:
    MatrixType strassen_recursive(const MatrixType& A, const MatrixType& B) const;
    MatrixType strassen_2x2(const MatrixType& A, const MatrixType& B) const;
};

/**
 * @brief Winograd's matrix multiplication algorithm
 * Slightly better than Strassen for some cases
 */
template<typename T = double>
class WinogradAlgorithm : public MatrixMultiplicationAlgorithm<T> {
public:
    using MatrixType = typename MatrixMultiplicationAlgorithm<T>::MatrixType;
    
    MatrixType multiply(const MatrixType& A, const MatrixType& B) const override;
    std::string name() const override;
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override;
    
private:
    MatrixType winograd_recursive(const MatrixType& A, const MatrixType& B) const;
    MatrixType winograd_2x2(const MatrixType& A, const MatrixType& B) const;
};

/**
 * @brief AlphaTensor discovered algorithm for 4x4 matrices
 * Based on the factorizations discovered by AlphaTensor
 */
template<typename T = double>
class AlphaTensorAlgorithm : public MatrixMultiplicationAlgorithm<T> {
public:
    using MatrixType = typename MatrixMultiplicationAlgorithm<T>::MatrixType;
    
    explicit AlphaTensorAlgorithm(const std::string& variant = "gpu");
    MatrixType multiply(const MatrixType& A, const MatrixType& B) const override;
    std::string name() const override;
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override;
    
private:
    std::string variant_;
    MatrixType alpha_tensor_4x4(const MatrixType& A, const MatrixType& B) const;
    MatrixType alpha_tensor_2x2(const MatrixType& A, const MatrixType& B) const;
    
    // AlphaTensor factorization coefficients
    struct Factorization {
        std::vector<std::vector<T>> u, v, w;
        size_type rank;
    };
    
    Factorization get_factorization() const;
};

/**
 * @brief Hybrid algorithm that chooses the best algorithm based on matrix size
 */
template<typename T = double>
class HybridAlgorithm : public MatrixMultiplicationAlgorithm<T> {
public:
    using MatrixType = typename MatrixMultiplicationAlgorithm<T>::MatrixType;
    
    explicit HybridAlgorithm(size_type naive_threshold = 64,
                           size_type strassen_threshold = 512);
    MatrixType multiply(const MatrixType& A, const MatrixType& B) const override;
    std::string name() const override;
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override;
    
private:
    size_type naive_threshold_;
    size_type strassen_threshold_;
    std::shared_ptr<MatrixMultiplicationAlgorithm<T>> choose_algorithm(size_type size) const;
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
    
    static std::shared_ptr<MatrixMultiplicationAlgorithm<T>> create(Type type);
    static std::shared_ptr<MatrixMultiplicationAlgorithm<T>> create(const std::string& name);
    static std::vector<std::string> available_algorithms();
};

// Utility functions
template<typename T = double>
Matrix<T> multiply_matrices(const Matrix<T>& A, const Matrix<T>& B,
                           const std::shared_ptr<MatrixMultiplicationAlgorithm<T>>& algorithm);

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

// The magic: include the implementation file for templates
#define ALPHATENSOR_ALGORITHMS_FUNCTIONS
#include "../src/algorithms.cpp"

#endif // ALPHATENSOR_ALGORITHMS_H 