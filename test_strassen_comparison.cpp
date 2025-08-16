#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include "core/matrix.h"

using namespace std;

// Function to measure execution time
template<typename Func>
double measure_time(Func func) {
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
}

// Function to check if two matrices are approximately equal
template<typename T>
bool matrices_equal(const Matrix<T>& A, const Matrix<T>& B, T tolerance = 1e-10) {
    if (A.rows() != B.rows() || A.cols() != B.cols()) {
        return false;
    }
    
    for (size_t i = 0; i < A.rows(); ++i) {
        for (size_t j = 0; j < A.cols(); ++j) {
            if (abs(A(i, j) - B(i, j)) > tolerance) {
                return false;
            }
        }
    }
    return true;
}

// Test function for original Strassen
template<typename T>
Matrix<T> test_original_strassen(const Matrix<T>& A, const Matrix<T>& B) {
    // We need to temporarily modify the multiply method to use original implementation
    // For now, we'll test the submatrix operations directly
    return A.multiply(B, Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN);
}

// Test function for optimized Strassen (bypassing the main multiply method)
template<typename T>
Matrix<T> test_optimized_strassen(const Matrix<T>& A, const Matrix<T>& B) {
    // We'll need to call the optimized method directly
    // For now, let's use the current implementation which uses optimized version
    return A.multiply(B, Matrix<T>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN);
}

int main() {
    cout << "=== Strassen Algorithm Comparison Test ===" << endl;
    
    // Test different matrix sizes
    vector<size_t> sizes = {64, 128, 256, 512};
    
    for (size_t size : sizes) {
        cout << "\nTesting matrix size: " << size << "x" << size << endl;
        
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
        
        // Compute results using both algorithms
        Matrix<double> result_naive, result_strassen;
        
        double naive_time = measure_time([&]() {
            result_naive = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE);
        });
        
        double strassen_time = measure_time([&]() {
            result_strassen = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN);
        });
        
        // Check correctness
        bool correct = matrices_equal(result_naive, result_strassen);
        
        cout << "  Correctness: " << (correct ? "✓ PASS" : "✗ FAIL") << endl;
        cout << "  Naive time:   " << setw(8) << fixed << setprecision(3) << naive_time << " ms" << endl;
        cout << "  Strassen time: " << setw(8) << fixed << setprecision(3) << strassen_time << " ms" << endl;
        cout << "  Speedup:      " << setw(8) << fixed << setprecision(3) << (naive_time / strassen_time) << "x" << endl;
        
        if (!correct) {
            cout << "  ERROR: Results don't match!" << endl;
            
            // Show some sample differences
            cout << "  Sample differences:" << endl;
            int count = 0;
            for (size_t i = 0; i < min(size, size_t(3)) && count < 5; ++i) {
                for (size_t j = 0; j < min(size, size_t(3)) && count < 5; ++j) {
                    if (abs(result_naive(i, j) - result_strassen(i, j)) > 1e-10) {
                        cout << "    [" << i << "," << j << "] Naive: " << result_naive(i, j) 
                             << " Strassen: " << result_strassen(i, j) 
                             << " Diff: " << abs(result_naive(i, j) - result_strassen(i, j)) << endl;
                        count++;
                    }
                }
            }
        }
    }
    
    cout << "\n=== Test Complete ===" << endl;
    cout << "\nNote: This test compares the current optimized Strassen implementation" << endl;
    cout << "against the naive algorithm. The optimization includes:" << endl;
    cout << "1. Using naive algorithm for matrices <= 64x64" << endl;
    cout << "2. Direct indexing instead of submatrix operations" << endl;
    cout << "3. Reduced memory allocations" << endl;
    
    return 0;
}
