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

int main() {
    cout << "=== Strassen vs New Strassen Algorithm Comparison ===" << endl;
    
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
    
    // Compute results using both algorithms
    Matrix<double> result_naive, result_strassen, result_new_strassen;
    
    cout << "Testing Naive algorithm..." << endl;
    double naive_time = measure_time([&]() {
        result_naive = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE);
    });
    
    cout << "Testing Strassen algorithm..." << endl;
    double strassen_time = measure_time([&]() {
        result_strassen = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN);
    });
    
    cout << "Testing New Strassen algorithm..." << endl;
    double new_strassen_time = measure_time([&]() {
        result_new_strassen = A.multiply(B, Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::NEW_STRASSEN);
    });
    
    // Check correctness
    bool strassen_correct = matrices_equal(result_naive, result_strassen);
    bool new_strassen_correct = matrices_equal(result_naive, result_new_strassen);
    bool both_equal = matrices_equal(result_strassen, result_new_strassen);
    
    cout << "\nResults:" << endl;
    cout << "  Correctness:" << endl;
    cout << "    Strassen:     " << (strassen_correct ? "✓ PASS" : "✗ FAIL") << endl;
    cout << "    New Strassen: " << (new_strassen_correct ? "✓ PASS" : "✗ FAIL") << endl;
    cout << "    Both equal:   " << (both_equal ? "✓ PASS" : "✗ FAIL") << endl;
    
    cout << "  Performance:" << endl;
    cout << "    Naive:        " << setw(8) << fixed << setprecision(3) << naive_time << " ms" << endl;
    cout << "    Strassen:     " << setw(8) << fixed << setprecision(3) << strassen_time << " ms" << endl;
    cout << "    New Strassen: " << setw(8) << fixed << setprecision(3) << new_strassen_time << " ms" << endl;
    
    cout << "  Speedup vs Naive:" << endl;
    cout << "    Strassen:     " << setw(8) << fixed << setprecision(3) << (naive_time / strassen_time) << "x" << endl;
    cout << "    New Strassen: " << setw(8) << fixed << setprecision(3) << (naive_time / new_strassen_time) << "x" << endl;
    
    cout << "  New vs Original Strassen:" << endl;
    cout << "    Speedup:      " << setw(8) << fixed << setprecision(3) << (strassen_time / new_strassen_time) << "x" << endl;
    
    if (!strassen_correct || !new_strassen_correct) {
        cout << "  ERROR: Results don't match!" << endl;
    }
    
    cout << "\n=== Test Complete ===" << endl;
    
    return 0;
}
