#!/usr/bin/env python3
"""
Setup script for LongNums Python module using pybind11
"""

from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext
import pybind11
import os
import sys

# Get the directory containing this setup.py
here = os.path.abspath(os.path.dirname(__file__))

# Define the extension module
ext_modules = [
    Pybind11Extension(
        "longnums",
        [
            "python_bindings/longnums_module.cpp",
            "src/core/long.cpp",
            "src/core/matrix_core.cpp",
            "src/core/matrix_multiplication_algorithms/"
            "matrix_multiplication_algorithm.cpp",
            "src/core/matrix_multiplication_algorithms/"
            "block_matrix_multiplication_algorithm.cpp",
            "src/core/matrix_multiplication_algorithms/"
            "naive_matrix_multiplication_algorithm.cpp",
            "src/core/matrix_multiplication_algorithms/"
            "naive_transposed_matrix_multiplication_algorithm.cpp",
            "src/core/matrix_multiplication_algorithms/"
            "simd_naive_matrix_multiplication_algorithm.cpp",
            "src/core/matrix_multiplication_algorithms/"
            "arm_neon_matrix_multiplication_algorithm.cpp",
            "src/core/matrix_multiplication_algorithms/"
            "strassen_matrix_multiplication_algorithm.cpp",
            "src/core/matrix_multiplication_algorithms/"
            "winograd_matrix_multiplication_algorithm.cpp",
            "src/core/matrix_multiplication_algorithms/"
            "auto_matrix_multiplication_algorithm.cpp",
            "src/core/matrix_multiplication_algorithms/"
            "alpha_tensor_matrix_multiplication_algorithm.cpp",
            "src/core/matrix_multiply.cpp",
            "src/core/matrix_utils.cpp",
            "src/core/real.cpp",
            "src/core/strassen_mul.cpp",
            "src/core/toomcook_mul.cpp",
            "src/core/math.cpp",
            "src/random/generator_general.cpp",
            "src/random/evenly_gen.cpp",
            "src/random/quad_con_gen.cpp",
            "src/prime/prime_test.cpp",
            "src/utils/utils.cpp",
        ],
        include_dirs=[
            "include",
            pybind11.get_include(),
        ],
        language="c++",
        cxx_std=17,
        extra_compile_args=(
            ["-Wall", "-Wextra", "-O2", "-march=native", "-pthread"]
            if sys.platform != "win32"
            else ["/W4", "/O2", "/arch:AVX2"]
        ),
        extra_link_args=["-pthread"] if sys.platform != "win32" else [],
    ),
]

setup(
    name="longnums",
    version="1.0.0",
    author="Your Name",
    author_email="your.email@example.com",
    description="A Python module for long number arithmetic with "
    "multiple multiplication algorithms",
    long_description=open("README.md").read(),
    long_description_content_type="text/markdown",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
    install_requires=[
        "pybind11>=2.6.0",
    ],
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: C++",
        "Topic :: Scientific/Engineering :: Mathematics",
        "Topic :: Software Development :: Libraries :: Python Modules",
    ],
    keywords="mathematics, arithmetic, long numbers, matrix multiplication, "
    "prime testing, SIMD",
    project_urls={
        "Bug Reports": "https://github.com/yourusername/LongNums/issues",
        "Source": "https://github.com/yourusername/LongNums",
    },
)
