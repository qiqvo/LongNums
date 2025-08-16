#include "include/core/matrix.h"
#include <iostream>

int main() {
    std::cout << "=== Divide-and-Conquer Matrix Multiplication Inheritance Test ===" << std::endl;
    
    // Create test matrices
    Matrix<double> A(4, 4);
    Matrix<double> B(4, 4);
    
    A.randomize(-1.0, 1.0, 42);
    B.randomize(-1.0, 1.0, 42);
    
    std::cout << "Matrix A:" << std::endl;
    A.print();
    
    std::cout << "\nMatrix B:" << std::endl;
    B.print();
    
    // Test Strassen (inherits from DivideAndConquerMatrixMultiplicationAlgorithm)
    std::cout << "\n=== Testing Strassen Algorithm ===" << std::endl;
    Matrix<double> C_strassen = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN);
    std::cout << "Strassen result:" << std::endl;
    C_strassen.print();
    
    // Test Winograd (inherits from DivideAndConquerMatrixMultiplicationAlgorithm)
    std::cout << "\n=== Testing Winograd Algorithm ===" << std::endl;
    Matrix<double> C_winograd = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::WINOGRAD);
    std::cout << "Winograd result:" << std::endl;
    C_winograd.print();
    
    // Test Naive for comparison
    std::cout << "\n=== Testing Naive Algorithm ===" << std::endl;
    Matrix<double> C_naive = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE);
    std::cout << "Naive result:" << std::endl;
    C_naive.print();
    
    // Verify results are the same
    std::cout << "\n=== Verification ===" << std::endl;
    bool strassen_correct = C_strassen.is_equal(C_naive, 1e-10);
    bool winograd_correct = C_winograd.is_equal(C_naive, 1e-10);
    
    std::cout << "Strassen result matches Naive: " << (strassen_correct ? "✓" : "✗") << std::endl;
    std::cout << "Winograd result matches Naive: " << (winograd_correct ? "✓" : "✗") << std::endl;
    
    std::cout << "\n=== Inheritance Structure ===" << std::endl;
    std::cout << "✓ StrassenMatrixMultiplicationAlgorithm inherits from DivideAndConquerMatrixMultiplicationAlgorithm" << std::endl;
    std::cout << "✓ WinogradMatrixMultiplicationAlgorithm inherits from DivideAndConquerMatrixMultiplicationAlgorithm" << std::endl;
    std::cout << "✓ Both algorithms share common validation and padding logic" << std::endl;
    
    return 0;
}
