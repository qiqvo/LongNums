#include "include/core/matrix.h"
#include <iostream>
#include <chrono>
#include <iomanip>

// Helper function to print matrix
template<typename T>
void print_matrix(const Matrix<T>& matrix, const std::string& name) {
    std::cout << name << " (" << matrix.rows() << "x" << matrix.cols() << "):\n";
    std::cout << matrix << std::endl;
}

// Helper function to measure execution time
template<typename Func>
double measure_time(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1000.0; // Convert to milliseconds
}

int main() {
    std::cout << "=== Matrix Multiplication Algorithm Comparison ===\n\n";
    
    // Create test matrices
    const int size = 4;
    Matrix<double> A(size, size);
    Matrix<double> B(size, size);
    
    // Initialize matrices with some values
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            A(i, j) = i + j + 1;  // Simple pattern
            B(i, j) = (i + 1) * (j + 1);  // Another pattern
        }
    }
    
    print_matrix(A, "Matrix A");
    print_matrix(B, "Matrix B");
    
    std::cout << "Testing different multiplication algorithms:\n";
    std::cout << std::string(50, '-') << std::endl;
    
    Matrix<double> result;
    double execution_time;
    
    // Test Naive Algorithm
    execution_time = measure_time([&]() {
        result = Matrix<double>::NaiveAlgorithm::multiply(A, B);
    });
    std::cout << std::left << std::setw(25) << "Naive Algorithm:";
    std::cout << std::fixed << std::setprecision(3) << execution_time << " ms\n";
    print_matrix(result, "Result (Naive)");
    
    // Test Block Algorithm
    execution_time = measure_time([&]() {
        result = Matrix<double>::BlockAlgorithm::multiply(A, B, 2);
    });
    std::cout << std::left << std::setw(25) << "Block Algorithm (block=2):";
    std::cout << std::fixed << std::setprecision(3) << execution_time << " ms\n";
    print_matrix(result, "Result (Block)");
    
    // Test Strassen Algorithm
    execution_time = measure_time([&]() {
        result = Matrix<double>::StrassenAlgorithm::multiply(A, B);
    });
    std::cout << std::left << std::setw(25) << "Strassen Algorithm:";
    std::cout << std::fixed << std::setprecision(3) << execution_time << " ms\n";
    print_matrix(result, "Result (Strassen)");
    
    // Test Winograd Algorithm
    execution_time = measure_time([&]() {
        result = Matrix<double>::WinogradAlgorithm::multiply(A, B);
    });
    std::cout << std::left << std::setw(25) << "Winograd Algorithm:";
    std::cout << std::fixed << std::setprecision(3) << execution_time << " ms\n";
    print_matrix(result, "Result (Winograd)");
    
    // Test Hybrid Algorithm
    execution_time = measure_time([&]() {
        result = Matrix<double>::HybridAlgorithm::multiply(A, B);
    });
    std::cout << std::left << std::setw(25) << "Hybrid Algorithm:";
    std::cout << std::fixed << std::setprecision(3) << execution_time << " ms\n";
    print_matrix(result, "Result (Hybrid)");
    
    // Test Auto Algorithm
    execution_time = measure_time([&]() {
        result = Matrix<double>::AutoAlgorithm::multiply(A, B);
    });
    std::cout << std::left << std::setw(25) << "Auto Algorithm:";
    std::cout << std::fixed << std::setprecision(3) << execution_time << " ms\n";
    print_matrix(result, "Result (Auto)");
    
    // Test AlphaTensor Algorithm
    execution_time = measure_time([&]() {
        result = Matrix<double>::AlphaTensorAlgorithm::multiply(A, B);
    });
    std::cout << std::left << std::setw(25) << "AlphaTensor:";
    std::cout << std::fixed << std::setprecision(3) << execution_time << " ms\n";
    print_matrix(result, "Result (AlphaTensor)");
    
    
    std::cout << "\n=== Algorithm Comparison Complete ===\n";
    
    return 0;
} 