#include "matrix.h"

namespace alphatensor {

// Explicit template instantiations for common types
template class Matrix<double>;
template class Matrix<float>;

// Explicit instantiations for free functions
template Matrix<double> operator*(double scalar, const Matrix<double>& matrix);
template Matrix<float> operator*(float scalar, const Matrix<float>& matrix);

template std::ostream& operator<<(std::ostream& os, const Matrix<double>& matrix);
template std::ostream& operator<<(std::ostream& os, const Matrix<float>& matrix);

// Explicit instantiations for utility functions
template Matrix<double> create_identity<double>(size_type size);
template Matrix<float> create_identity<float>(size_type size);

template Matrix<double> create_random<double>(size_type rows, size_type cols, 
                                           double min, double max, unsigned seed);
template Matrix<float> create_random<float>(size_type rows, size_type cols, 
                                          float min, float max, unsigned seed);

template Matrix<double> create_random_normal<double>(size_type rows, size_type cols,
                                                  double mean, double stddev, unsigned seed);
template Matrix<float> create_random_normal<float>(size_type rows, size_type cols,
                                                 float mean, float stddev, unsigned seed);

} // namespace alphatensor 