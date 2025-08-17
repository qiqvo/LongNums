import longnums as ln
import numpy as np

# Matrix operations with SIMD
a = np.random.uniform(-1.0, 1.0, (4, 4))
b = np.random.uniform(-1.0, 1.0, (4, 4))

A = ln.matrix_from_numpy(a)
B = ln.matrix_from_numpy(b)
C = A.multiply(B, ln.MatrixAlgorithm.SIMD_NAIVE)

c_1 = ln.matrix_to_numpy(C)
c_2 = np.matmul(a, b)
print(c_1)
print(c_2)

print(np.allclose(c_1, c_2, atol=1e-10))

print(c_1 == c_2)

# # Prime testing
# n = ln.Long("17")
# is_prime = ln.solovay_strassen_test(n, 10)
