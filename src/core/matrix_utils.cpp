#ifndef MATRIX_FUNCTIONS
#include "core/matrix.h"
#else

// Utility methods implementations

template<typename T>
void Matrix<T>::randomize(value_type min, value_type max, unsigned seed) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<value_type> dist(min, max);
    
    for (size_type i = 0; i < data_.size(); ++i) {
        data_[i] = dist(gen);
    }
}

template<typename T>
void Matrix<T>::randomize_normal(value_type mean, value_type stddev, unsigned seed) {
    std::mt19937 gen(seed);
    std::normal_distribution<value_type> dist(mean, stddev);
    
    for (size_type i = 0; i < data_.size(); ++i) {
        data_[i] = dist(gen);
    }
}

template<typename T>
void Matrix<T>::fill(value_type value) {
    for (size_type i = 0; i < data_.size(); ++i) {
        data_[i] = value;
    }
}

template<typename T>
void Matrix<T>::zero() {
    fill(0);
}

template<typename T>
void Matrix<T>::identity() {
    if (!is_square()) {
        throw std::invalid_argument("Identity matrix must be square");
    }
    
    zero();
    for (size_type i = 0; i < rows_; ++i) {
        (*this)(i, i) = 1;
    }
}

template<typename T>
typename Matrix<T>::value_type Matrix<T>::trace() const {
    if (!is_square()) {
        throw std::invalid_argument("Trace only defined for square matrices");
    }
    
    value_type sum = 0;
    for (size_type i = 0; i < rows_; ++i) {
        sum += (*this)(i, i);
    }
    return sum;
}

template<typename T>
typename Matrix<T>::value_type Matrix<T>::norm() const {
    value_type sum = 0;
    for (size_type i = 0; i < data_.size(); ++i) {
        sum += data_[i] * data_[i];
    }
    return std::sqrt(sum);
}

template<typename T>
void Matrix<T>::print(std::ostream& os, int precision) const {
    os << std::fixed << std::setprecision(precision);
    for (size_type i = 0; i < rows_; ++i) {
        os << "[";
        for (size_type j = 0; j < cols_; ++j) {
            os << (*this)(i, j);
            if (j < cols_ - 1) os << ", ";
        }
        os << "]" << std::endl;
    }
}

template<typename T>
void Matrix<T>::save_to_file(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    file << rows_ << " " << cols_ << std::endl;
    for (size_type i = 0; i < rows_; ++i) {
        for (size_type j = 0; j < cols_; ++j) {
            file << (*this)(i, j);
            if (j < cols_ - 1) file << " ";
        }
        file << std::endl;
    }
    file.close();
}

template<typename T>
Matrix<T> Matrix<T>::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    size_type rows, cols;
    file >> rows >> cols;
    
    Matrix result(rows, cols);
    for (size_type i = 0; i < rows; ++i) {
        for (size_type j = 0; j < cols; ++j) {
            file >> result(i, j);
        }
    }
    file.close();
    return result;
}

template<typename T>
bool Matrix<T>::is_equal(const Matrix& other, value_type tolerance) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        return false;
    }
    
    for (size_type i = 0; i < data_.size(); ++i) {
        if (std::abs(data_[i] - other.data_[i]) > tolerance) {
            return false;
        }
    }
    return true;
}

// Data access methods
template<typename T>
typename Matrix<T>::value_type* Matrix<T>::data() {
    return data_.data();
}

template<typename T>
const typename Matrix<T>::value_type* Matrix<T>::data() const {
    return data_.data();
}

template<typename T>
const std::vector<typename Matrix<T>::value_type>& Matrix<T>::get_data() const {
    return data_;
}

// Free functions
template<typename T>
Matrix<T> operator*(typename Matrix<T>::value_type scalar, const Matrix<T>& matrix) {
    return matrix * scalar;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
    matrix.print(os);
    return os;
}

// Utility functions
template<typename T>
Matrix<T> create_identity(size_type size) {
    Matrix<T> result(size, size);
    result.identity();
    return result;
}

template<typename T>
Matrix<T> create_random(size_type rows, size_type cols, 
                    T min, T max, unsigned seed) {
    Matrix<T> result(rows, cols);
    result.randomize(min, max, seed);
    return result;
}

template<typename T>
Matrix<T> create_random_normal(size_type rows, size_type cols,
                          T mean, T stddev, unsigned seed) {
    Matrix<T> result(rows, cols);
    result.randomize_normal(mean, stddev, seed);
    return result;
}

#endif // MATRIX_FUNCTIONS 