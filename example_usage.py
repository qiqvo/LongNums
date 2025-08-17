#!/usr/bin/env python3
"""
Example usage of the LongNums Python module
"""

import numpy as np
import longnums as ln


def main():
    print("=== LongNums Python Module Example ===\n")

    # 1. Long number arithmetic
    print("1. Long Number Arithmetic:")
    print("-" * 30)

    # Create large numbers
    a = ln.Long("12345678901234567890")
    b = ln.Long("98765432109876543210")

    print(f"a = {a}")
    print(f"b = {b}")
    print(f"a + b = {a + b}")
    print(f"a * b = {a * b}")

    # Advanced multiplication algorithms
    print(f"\nKaratsuba multiplication: {a.karatsuba_mul(b)}")
    print(f"Toom-Cook multiplication: {a.toomcook_mul(b)}")
    print(f"Strassen multiplication: {a.strassen_mul(b)}")

    # 2. Matrix operations
    print("\n2. Matrix Operations:")
    print("-" * 30)

    # Create matrices
    A = ln.Matrix(3, 3, 1.0)
    B = ln.Matrix(3, 3, 2.0)

    print("Matrix A:")
    for i in range(3):
        for j in range(3):
            print(f"{A[i, j]:.1f}", end=" ")
        print()

    print("\nMatrix B:")
    for i in range(3):
        for j in range(3):
            print(f"{B[i, j]:.1f}", end=" ")
        print()

    # Matrix multiplication with different algorithms
    print("\nMatrix multiplication results:")
    C_naive = A.multiply(B, ln.MatrixAlgorithm.NAIVE)
    C_simd = A.multiply(B, ln.MatrixAlgorithm.SIMD_NAIVE)
    C_strassen = A.multiply(B, ln.MatrixAlgorithm.STRASSEN)

    print("Naive algorithm result:")
    for i in range(3):
        for j in range(3):
            print(f"{C_naive[i, j]:.1f}", end=" ")
        print()

    print("SIMD algorithm result:")
    for i in range(3):
        for j in range(3):
            print(f"{C_simd[i, j]:.1f}", end=" ")
        print()

    print("Strassen algorithm result:")
    for i in range(3):
        for j in range(3):
            print(f"{C_strassen[i, j]:.1f}", end=" ")
        print()

    # 3. Prime testing
    print("\n3. Prime Testing:")
    print("-" * 30)

    test_numbers = [17, 25, 97, 100]

    for num in test_numbers:
        n = ln.Long(str(num))
        solovay = ln.solovay_strassen_test(n, 10)
        lehmann = ln.lehmann_test(n, 10)
        rabin = ln.rabin_miller_test(n, 10)

        print(f"Number {num}:")
        print(f"  Solovay-Strassen: {solovay:.6f}")
        print(f"  Lehmann: {lehmann:.6f}")
        print(f"  Rabin-Miller: {rabin:.6f}")
        print()

    # 4. Mathematical functions
    print("4. Mathematical Functions:")
    print("-" * 30)

    # GCD and LCM
    x = ln.Long("48")
    y = ln.Long("18")
    print(f"GCD({x}, {y}) = {ln.gcd(x, y)}")
    # Note: LCM function not available in current bindings

    # Factorial
    n = ln.Long("10")
    print(f"Factorial({n}) = {ln.factorial(n)}")

    # Power and modular exponentiation
    base = ln.Long("2")
    exponent = ln.Long("10")
    print(f"Power: {base}^{exponent} = {ln.pow_long(base, exponent)}")

    # Modular exponentiation
    modulus = ln.Long("1000000007")
    mod_exp = ln.pow_mod(base, exponent, modulus)
    print(f"Modular exponentiation: {base}^{exponent} mod {modulus} = " f"{mod_exp}")

    # 5. SIMD availability
    print("\n5. SIMD Support:")
    print("-" * 30)

    if ln.is_simd_available():
        print("SIMD optimizations are available!")
    else:
        print("SIMD optimizations are not available.")

    # 6. Random matrix generation
    print("\n6. Random Matrix Generation:")
    print("-" * 30)

    random_matrix = ln.create_random_matrix(4, 4, -1.0, 1.0)
    print("Random 4x4 matrix (values between -1 and 1):")
    for i in range(4):
        for j in range(4):
            print(f"{random_matrix[i, j]:.3f}", end=" ")
        print()

    # 7. Numpy integration
    print("\n7. Numpy Integration:")
    print("-" * 30)

    # Create numpy array
    np_array = np.array([[1.0, 2.0, 3.0], [4.0, 5.0, 6.0], [7.0, 8.0, 9.0]])
    print("Numpy array:")
    print(np_array)

    # Convert to LongNums matrix
    ln_matrix = ln.matrix_from_numpy(np_array)
    print("\nConverted to LongNums matrix:")
    for i in range(3):
        for j in range(3):
            print(f"{ln_matrix[i, j]:.1f}", end=" ")
        print()

    # Convert back to numpy
    np_result = ln.matrix_to_numpy(ln_matrix)
    print("\nConverted back to numpy:")
    print(np_result)

    print("\n=== Example completed successfully! ===")


if __name__ == "__main__":
    main()
