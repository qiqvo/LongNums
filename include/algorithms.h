#pragma once

#include "matrix.h"
#include <functional>
#include <string>
#include <vector>
#include <memory>

namespace alphatensor {

/**
 * @brief Base class for matrix multiplication algorithms
 */
class MatrixMultiplicationAlgorithm {
public:
    virtual ~MatrixMultiplicationAlgorithm() = default;
    
    /**
     * @brief Multiply two matrices using the algorithm
     * @param A First matrix
     * @param B Second matrix
     * @return Result matrix C = A * B
     */
    virtual Matrix multiply(const Matrix& A, const Matrix& B) const = 0;
    
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
class NaiveAlgorithm : public MatrixMultiplicationAlgorithm {
public:
    Matrix multiply(const Matrix& A, const Matrix& B) const override;
    std::string name() const override { return "Naive"; }
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override;
};

/**
 * @brief Block matrix multiplication algorithm
 * Better cache performance for large matrices
 */
class BlockAlgorithm : public MatrixMultiplicationAlgorithm {
public:
    explicit BlockAlgorithm(size_type block_size = 64);
    Matrix multiply(const Matrix& A, const Matrix& B) const override;
    std::string name() const override { return "Block"; }
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override;
    
private:
    size_type block_size_;
};

/**
 * @brief Strassen's matrix multiplication algorithm
 * Time complexity: O(n^2.807)
 */
class StrassenAlgorithm : public MatrixMultiplicationAlgorithm {
public:
    Matrix multiply(const Matrix& A, const Matrix& B) const override;
    std::string name() const override { return "Strassen"; }
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override;
    
private:
    Matrix strassen_recursive(const Matrix& A, const Matrix& B) const;
    Matrix strassen_2x2(const Matrix& A, const Matrix& B) const;
};

/**
 * @brief Winograd's matrix multiplication algorithm
 * Slightly better than Strassen for some cases
 */
class WinogradAlgorithm : public MatrixMultiplicationAlgorithm {
public:
    Matrix multiply(const Matrix& A, const Matrix& B) const override;
    std::string name() const override { return "Winograd"; }
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override;
    
private:
    Matrix winograd_recursive(const Matrix& A, const Matrix& B) const;
    Matrix winograd_2x2(const Matrix& A, const Matrix& B) const;
};

/**
 * @brief AlphaTensor discovered algorithm for 4x4 matrices
 * Based on the factorizations discovered by AlphaTensor
 */
class AlphaTensorAlgorithm : public MatrixMultiplicationAlgorithm {
public:
    explicit AlphaTensorAlgorithm(const std::string& variant = "gpu");
    Matrix multiply(const Matrix& A, const Matrix& B) const override;
    std::string name() const override { return "AlphaTensor-" + variant_; }
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override;
    
private:
    std::string variant_;
    Matrix alpha_tensor_4x4(const Matrix& A, const Matrix& B) const;
    Matrix alpha_tensor_2x2(const Matrix& A, const Matrix& B) const;
    
    // AlphaTensor factorization coefficients
    struct Factorization {
        std::vector<std::vector<value_type>> u, v, w;
        size_type rank;
    };
    
    Factorization get_factorization() const;
};

/**
 * @brief Hybrid algorithm that chooses the best algorithm based on matrix size
 */
class HybridAlgorithm : public MatrixMultiplicationAlgorithm {
public:
    explicit HybridAlgorithm(size_type naive_threshold = 64,
                           size_type strassen_threshold = 512);
    Matrix multiply(const Matrix& A, const Matrix& B) const override;
    std::string name() const override { return "Hybrid"; }
    bool can_handle(size_type A_rows, size_type A_cols,
                   size_type B_rows, size_type B_cols) const override;
    
private:
    size_type naive_threshold_;
    size_type strassen_threshold_;
    std::shared_ptr<MatrixMultiplicationAlgorithm> choose_algorithm(
        size_type size) const;
};

/**
 * @brief Algorithm factory for creating different multiplication algorithms
 */
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
    
    static std::shared_ptr<MatrixMultiplicationAlgorithm> create(Type type);
    static std::shared_ptr<MatrixMultiplicationAlgorithm> create(const std::string& name);
    
    static std::vector<std::string> available_algorithms();
};

// Utility functions
Matrix multiply_matrices(const Matrix& A, const Matrix& B,
                       const std::shared_ptr<MatrixMultiplicationAlgorithm>& algorithm);

} // namespace alphatensor 