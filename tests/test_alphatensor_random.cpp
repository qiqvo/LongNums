#include <iostream>
#include <random>
#include "../include/core/matrix.h"

int main() {
    using Mat = Matrix<double>;
    using Algo = Mat::MatrixMultiplicationAlgorithm::AlgorithmType;

    std::cout << "AlphaTensor random 4x4 multiplication test" << std::endl;

    const std::size_t num_tests = 100;
    std::size_t passed = 0;
    std::size_t failed = 0;

    // Deterministic RNG for reproducibility
    // std::mt19937 rng(12345);
    // std::uniform_real_distribution<double> dist(-5.0, 5.0);

    for (std::size_t t = 0; t < num_tests; ++t) {
        Mat A = create_random<double>(4, 4);
        Mat B = create_random<double>(4, 4);

        Mat C = A.multiply(B, Algo::ALPHATENSOR);
        Mat C_ref = A.multiply(B, Algo::STRASSEN);
        Mat C_ref_naive = A.multiply(B, Algo::NAIVE);

        if (C.is_equal(C_ref, 1e-9)) {
            ++passed;
        } else {
            ++failed;
            std::cout << "Test " << t + 1 << " FAILED" << std::endl;
            std::cout << "A:\n"; A.print(std::cout, 6); std::cout << '\n';
            std::cout << "B:\n"; B.print(std::cout, 6); std::cout << '\n';
            std::cout << "C:\n"; C.print(std::cout, 6); std::cout << '\n';
            std::cout << "C_ref:\n"; C_ref.print(std::cout, 6); std::cout << '\n';
            std::cout << "C_ref_naive:\n"; C_ref_naive.print(std::cout, 6); std::cout << '\n';
        }
    }

    std::cout << "\nSummary: " << passed << "/" << num_tests << " passed, " << failed << " failed" << std::endl;
    return failed == 0 ? 0 : 1;
}

