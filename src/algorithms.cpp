#include "algorithms.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cstring>

namespace alphatensor {

// Explicit template instantiations for common types
template class NaiveAlgorithm<double>;
template class NaiveAlgorithm<float>;

template class BlockAlgorithm<double>;
template class BlockAlgorithm<float>;

template class StrassenAlgorithm<double>;
template class StrassenAlgorithm<float>;

template class WinogradAlgorithm<double>;
template class WinogradAlgorithm<float>;

template class AlphaTensorAlgorithm<double>;
template class AlphaTensorAlgorithm<float>;

template class HybridAlgorithm<double>;
template class HybridAlgorithm<float>;

template class AlgorithmFactory<double>;
template class AlgorithmFactory<float>;

// Explicit instantiations for utility functions
template Matrix<double> multiply_matrices<double>(const Matrix<double>& A, const Matrix<double>& B,
                                                const std::shared_ptr<MatrixMultiplicationAlgorithm<double>>& algorithm);
template Matrix<float> multiply_matrices<float>(const Matrix<float>& A, const Matrix<float>& B,
                                              const std::shared_ptr<MatrixMultiplicationAlgorithm<float>>& algorithm);

} // namespace alphatensor 