#include "include/core/matrix.h"
#include <iostream>

int main() {
    // Create test matrices
    Matrix<double> A(2, 2);
    Matrix<double> B(2, 2);
    
    // Initialize matrices
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;
    
    B(0, 0) = 5; B(0, 1) = 6;
    B(1, 0) = 7; B(1, 1) = 8;
    
    std::cout << "Matrix A:\n" << A << std::endl;
    std::cout << "Matrix B:\n" << B << std::endl;
    
    // Test different algorithms
    std::cout << "=== Testing Static Algorithm Classes ===" << std::endl;
    
    // Naive algorithm
    auto result_naive = Matrix<double>::NaiveAlgorithm::multiply(A, B);
    std::cout << "Naive Algorithm Result:\n" << result_naive << std::endl;
    
    // Block algorithm
    auto result_block = Matrix<double>::BlockAlgorithm::multiply(A, B, 2);
    std::cout << "Block Algorithm Result:\n" << result_block << std::endl;
    
    // Strassen algorithm
    auto result_strassen = Matrix<double>::StrassenAlgorithm::multiply(A, B);
    std::cout << "Strassen Algorithm Result:\n" << result_strassen << std::endl;
    
    // Winograd algorithm
    auto result_winograd = Matrix<double>::WinogradAlgorithm::multiply(A, B);
    std::cout << "Winograd Algorithm Result:\n" << result_winograd << std::endl;
    
    // Hybrid algorithm
    auto result_hybrid = Matrix<double>::HybridAlgorithm::multiply(A, B);
    std::cout << "Hybrid Algorithm Result:\n" << result_hybrid << std::endl;
    
    // Auto algorithm
    auto result_auto = Matrix<double>::AutoAlgorithm::multiply(A, B);
    std::cout << "Auto Algorithm Result:\n" << result_auto << std::endl;
    
    // AlphaTensor GPU algorithm
    auto result_alphatensor_gpu = Matrix<double>::AlphaTensorGPUAlgorithm::multiply(A, B);
    std::cout << "AlphaTensor GPU Algorithm Result:\n" << result_alphatensor_gpu << std::endl;
    
    // AlphaTensor TPU algorithm
    auto result_alphatensor_tpu = Matrix<double>::AlphaTensorTPUAlgorithm::multiply(A, B);
    std::cout << "AlphaTensor TPU Algorithm Result:\n" << result_alphatensor_tpu << std::endl;
    
    std::cout << "=== All algorithms completed successfully! ===" << std::endl;
    
    return 0;
} 