#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include "core/matrix.h"

using namespace std;

int main() {
    cout << "=== Simple Strassen Test ===" << endl;
    
    size_t size = 128;
    cout << "Testing matrix size: " << size << "x" << size << endl;
    
    // Create random matrices
    Matrix<double> A(size, size);
    Matrix<double> B(size, size);
    
    // Fill with random values
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(-10.0, 10.0);
    
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            A(i, j) = dis(gen);
            B(i, j) = dis(gen);
        }
    }
    
    cout << "Matrices created successfully" << endl;
    
    // Test each algorithm separately
    try {
        cout << "\n1. Testing Naive..." << endl;
        Matrix<double> result_naive = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE);
        cout << "   Naive: ✓ PASS" << endl;
        
        cout << "\n2. Testing Strassen..." << endl;
        Matrix<double> result_strassen = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN);
        cout << "   Strassen: ✓ PASS" << endl;
        
        cout << "\n3. Testing New Strassen..." << endl;
        Matrix<double> result_new_strassen = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::NEW_STRASSEN);
        cout << "   New Strassen: ✓ PASS" << endl;
        
        cout << "\nAll algorithms completed successfully!" << endl;
        
        // Check if results are equal
        bool strassen_equal = result_naive.is_equal(result_strassen);
        bool new_strassen_equal = result_naive.is_equal(result_new_strassen);
        
        cout << "Results comparison:" << endl;
        cout << "  Naive == Strassen: " << (strassen_equal ? "✓" : "✗") << endl;
        cout << "  Naive == New Strassen: " << (new_strassen_equal ? "✓" : "✗") << endl;
        
    } catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
