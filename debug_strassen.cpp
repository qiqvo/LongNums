#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include "core/matrix.h"

using namespace std;

int main() {
    cout << "=== Debug Strassen Algorithm ===" << endl;
    
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
    cout << "A dimensions: " << A.rows() << "x" << A.cols() << endl;
    cout << "B dimensions: " << B.rows() << "x" << B.cols() << endl;
    
    try {
        cout << "Attempting naive multiplication..." << endl;
        Matrix<double> result_naive = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE);
        cout << "Naive multiplication successful" << endl;
        
        cout << "Attempting Strassen multiplication..." << endl;
        Matrix<double> result_strassen = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN);
        cout << "Strassen multiplication successful" << endl;
        
        cout << "Attempting New Strassen multiplication..." << endl;
        Matrix<double> result_new_strassen = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::NEW_STRASSEN);
        cout << "New Strassen multiplication successful" << endl;
        
        cout << "All multiplications completed successfully!" << endl;
        
    } catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    
    return 0;
}
