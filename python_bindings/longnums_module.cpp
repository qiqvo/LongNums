#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include "core/long.h"
#include "core/matrix.h"
#include "core/real.h"
#include "core/math.h"
#include "prime/prime_test.h"
#include "random/quad_con_gen.h"
#include "utils/utils.h"

namespace py = pybind11;

// Helper function to convert Python string to Long
Long py_string_to_long(const std::string& s) {
    return Long(s.c_str());
}

// Helper function to convert Long to Python string
std::string long_to_py_string(const Long& l) {
    std::ostringstream oss;
    l.print(oss);
    return oss.str();
}

// Helper function to create Matrix from numpy array
template<typename T>
Matrix<T> numpy_to_matrix(py::array_t<T> arr) {
    py::buffer_info buf = arr.request();
    
    if (buf.ndim != 2) {
        throw std::runtime_error("Number of dimensions must be 2");
    }
    
    size_t rows = buf.shape[0];
    size_t cols = buf.shape[1];
    
    Matrix<T> matrix(rows, cols);
    
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            T* ptr = static_cast<T*>(buf.ptr);
            matrix(i, j) = ptr[i * buf.strides[0] / sizeof(T) + j * buf.strides[1] / sizeof(T)];
        }
    }
    
    return matrix;
}

// Helper function to convert Matrix to numpy array
template<typename T>
py::array_t<T> matrix_to_numpy(const Matrix<T>& matrix) {
    size_t rows = matrix.rows();
    size_t cols = matrix.cols();
    
    auto result = py::array_t<T>({rows, cols});
    py::buffer_info buf = result.request();
    
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            T* ptr = static_cast<T*>(buf.ptr);
            ptr[i * buf.strides[0] / sizeof(T) + j * buf.strides[1] / sizeof(T)] = matrix(i, j);
        }
    }
    
    return result;
}

PYBIND11_MODULE(longnums, m) {
    m.doc() = "LongNums: A Python module for long number arithmetic with multiple multiplication algorithms";
    
    // Long class bindings
    py::class_<Long>(m, "Long")
        .def(py::init<unsigned int>())
        .def(py::init<int>())
        .def(py::init<long>())
        .def(py::init<long long>())
        .def(py::init<unsigned long>())
        .def(py::init<unsigned long long>())
        .def(py::init<const char*>())
        .def(py::init<const Long&>())
        
        // String conversion
        .def("__str__", &long_to_py_string)
        .def("__repr__", &long_to_py_string)
        
        // Arithmetic operators (only the ones that exist)
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(py::self % py::self)
        
        // Comparison operators
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self < py::self)
        .def(py::self <= py::self)
        .def(py::self > py::self)
        .def(py::self >= py::self)
        
        // Advanced multiplication methods
        .def("karatsuba_mul", &Long::karac_mul)
        .def("toomcook_mul", &Long::toomcook_mul)
        .def("strassen_mul", &Long::strassen_mul)
        
        // Utility methods
        .def("size", &Long::size)
        .def("real_size", &Long::real_size)
        .def("change_sign", &Long::changeSign)
        .def("shift", &Long::shift)
        .def("shift_accurate", &Long::shiftaccurate)
        .def("shift_back", &Long::shiftback)
        
        // Division with remainder
        .def("divide", [](const Long& self, int b) {
            Long rem(0);
            Long result = self.divide(b, rem);
            return py::make_tuple(result, rem);
        })
        .def("divide", [](const Long& self, const Long& b) {
            Long rem(0);
            Long result = self.divide(b, rem);
            return py::make_tuple(result, rem);
        });
    
    // Real class bindings
    py::class_<Real>(m, "Real")
        .def(py::init<unsigned long long, int>())
        .def(py::init<const Real&>());
    
    // Matrix class bindings
    py::class_<Matrix<double>>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<size_t, size_t, double>())
        .def(py::init<const Matrix<double>&>())
        
        // Access operators
        .def("__getitem__", [](const Matrix<double>& self, py::tuple indices) {
            if (indices.size() != 2) {
                throw std::runtime_error("Matrix indices must be 2D");
            }
            size_t i = indices[0].cast<size_t>();
            size_t j = indices[1].cast<size_t>();
            return self(i, j);
        })
        .def("__setitem__", [](Matrix<double>& self, py::tuple indices, double value) {
            if (indices.size() != 2) {
                throw std::runtime_error("Matrix indices must be 2D");
            }
            size_t i = indices[0].cast<size_t>();
            size_t j = indices[1].cast<size_t>();
            self(i, j) = value;
        })
        
        // Arithmetic operators
        .def(py::self + py::self)
        .def(py::self + double())
        .def(py::self - py::self)
        .def(py::self - double())
        .def(py::self * py::self)
        .def(py::self * double())
        
        // Comparison operators
        .def(py::self == py::self)
        .def(py::self != py::self)
        
        // Matrix multiplication algorithms
        .def("multiply", [](const Matrix<double>& self, const Matrix<double>& other, 
                           Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType algo) {
            return self.multiply(other, algo);
        })
        .def("multiply", [](const Matrix<double>& self, const Matrix<double>& other) {
            return self.multiply(other);
        })
        
        // Utility methods
        .def("rows", &Matrix<double>::rows)
        .def("cols", &Matrix<double>::cols)
        .def("is_square", &Matrix<double>::is_square)
        .def("is_empty", &Matrix<double>::is_empty)
        .def("transpose", &Matrix<double>::transpose)
        .def("trace", &Matrix<double>::trace)
        .def("norm", &Matrix<double>::norm)
        .def("fill", &Matrix<double>::fill)
        .def("zero", &Matrix<double>::zero)
        .def("identity", &Matrix<double>::identity)
        .def("randomize", &Matrix<double>::randomize)
        .def("print", &Matrix<double>::print);
    
    // Matrix multiplication algorithm types
    py::enum_<Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType>(
        m, "MatrixAlgorithm")
        .value("NAIVE", Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE)
        .value("NAIVE_TRANSPOSED", Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::NAIVE_TRANSPOSED)
        .value("SIMD_NAIVE", Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::SIMD_NAIVE)
        .value("ARM_NEON", Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::ARM_NEON)
        .value("STRASSEN", Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::STRASSEN)
        .value("WINOGRAD", Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::WINOGRAD)
        .value("ALPHA_TENSOR", Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::ALPHA_TENSOR)
        .value("AUTO", Matrix<double>::MatrixMultiplicationAlgorithm::AlgorithmType::AUTO);
    
    // SIMD availability check (simplified)
    m.def("is_simd_available", []() {
        return false; // Simplified for now
    });
    
    // Prime testing functions
    m.def("solovay_strassen_test", &prtest_SolovStras);
    m.def("lehmann_test", &prtest_Lehmann);
    m.def("rabin_miller_test", &prtest_RabinMiller);
    
    // Mathematical functions
    m.def("gcd", &gcd);
    m.def("factorial", &factorial);
    m.def("pow_mod", &pow_mod);
    m.def("pow_long", static_cast<Long(*)(const Long&, const Long&)>(&pow));
    m.def("abs_long", static_cast<Long(*)(const Long&)>(&abs));
    
    // Random number generation
    m.def("rand", [](uint s) { return rand(s); });
    
    // Utility functions
    m.def("create_random_matrix", [](size_t rows, size_t cols, double min_val, double max_val) {
        return create_random<double>(rows, cols, min_val, max_val);
    });
    
    // Numpy array conversion functions
    m.def("matrix_from_numpy", &numpy_to_matrix<double>);
    m.def("matrix_to_numpy", &matrix_to_numpy<double>);
    
    // String conversion functions
    m.def("long_from_string", &py_string_to_long);
    m.def("long_to_string", &long_to_py_string);
}
