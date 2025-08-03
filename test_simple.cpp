#include "matrix.h"
#include "algorithms.h"
#include <iostream>

using namespace alphatensor;

int main() {
    std::cout << "Simple Matrix Multiplication Test\n";
    std::cout << "================================\n\n";
    
    // Create simple test matrices
    Matrix A(2, 2);
    Matrix B(2, 2);
    
    A(0, 0) = 1.0; A(0, 1) = 2.0;
    A(1, 0) = 3.0; A(1, 1) = 4.0;
    
    B(0, 0) = 5.0; B(0, 1) = 6.0;
    B(1, 0) = 7.0; B(1, 1) = 8.0;
    
    std::cout << "Matrix A:\n";
    A.print(std::cout, 1);
    
    std::cout << "\nMatrix B:\n";
    B.print(std::cout, 1);
    
    // Test naive algorithm
    auto naive = std::make_shared<NaiveAlgorithm<double>>();
    Matrix result = naive->multiply(A, B);
    
    std::cout << "\nResult (A * B):\n";
    result.print(std::cout, 1);
    
    // Expected result: [19, 22]
    //                  [43, 50]
    std::cout << "\nExpected result:\n";
    std::cout << "[19.0, 22.0]\n";
    std::cout << "[43.0, 50.0]\n";
    
    // Verify correctness
    bool correct = (result(0, 0) == 19.0 && result(0, 1) == 22.0 &&
                   result(1, 0) == 43.0 && result(1, 1) == 50.0);
    
    std::cout << "\nCorrect: " << (correct ? "Yes" : "No") << "\n";
    
    return 0;
} 