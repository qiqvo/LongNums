#include "matrix.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace alphatensor;

class MatrixTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test matrices
        A = Matrix(3, 3);
        B = Matrix(3, 3);
        
        // Initialize with known values
        for (size_type i = 0; i < 3; ++i) {
            for (size_type j = 0; j < 3; ++j) {
                A(i, j) = i + j;
                B(i, j) = i * j;
            }
        }
    }
    
    Matrix A, B;
};

TEST_F(MatrixTest, Constructor) {
    Matrix m(2, 3);
    EXPECT_EQ(m.rows(), 2);
    EXPECT_EQ(m.cols(), 3);
    EXPECT_TRUE(m.is_empty() == false);
}

TEST_F(MatrixTest, AccessOperator) {
    Matrix m(2, 2);
    m(0, 0) = 1.0;
    m(0, 1) = 2.0;
    m(1, 0) = 3.0;
    m(1, 1) = 4.0;
    
    EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(m(1, 0), 3.0);
    EXPECT_DOUBLE_EQ(m(1, 1), 4.0);
}

TEST_F(MatrixTest, Addition) {
    Matrix C = A + B;
    
    for (size_type i = 0; i < 3; ++i) {
        for (size_type j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(C(i, j), A(i, j) + B(i, j));
        }
    }
}

TEST_F(MatrixTest, Subtraction) {
    Matrix C = A - B;
    
    for (size_type i = 0; i < 3; ++i) {
        for (size_type j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(C(i, j), A(i, j) - B(i, j));
        }
    }
}

TEST_F(MatrixTest, Multiplication) {
    Matrix C = A * B;
    
    // Expected result for 3x3 matrices
    EXPECT_DOUBLE_EQ(C(0, 0), 0.0);  // (0+0)*0 + (0+1)*0 + (0+2)*0
    EXPECT_DOUBLE_EQ(C(0, 1), 0.0);  // (0+0)*0 + (0+1)*1 + (0+2)*2
    EXPECT_DOUBLE_EQ(C(0, 2), 0.0);  // (0+0)*0 + (0+1)*2 + (0+2)*4
    EXPECT_DOUBLE_EQ(C(1, 0), 0.0);  // (1+0)*0 + (1+1)*0 + (1+2)*0
    EXPECT_DOUBLE_EQ(C(1, 1), 5.0);  // (1+0)*0 + (1+1)*1 + (1+2)*2
    EXPECT_DOUBLE_EQ(C(1, 2), 10.0); // (1+0)*0 + (1+1)*2 + (1+2)*4
    EXPECT_DOUBLE_EQ(C(2, 0), 0.0);  // (2+0)*0 + (2+1)*0 + (2+2)*0
    EXPECT_DOUBLE_EQ(C(2, 1), 10.0); // (2+0)*0 + (2+1)*1 + (2+2)*2
    EXPECT_DOUBLE_EQ(C(2, 2), 20.0); // (2+0)*0 + (2+1)*2 + (2+2)*4
}

TEST_F(MatrixTest, ScalarMultiplication) {
    Matrix C = A * 2.0;
    
    for (size_type i = 0; i < 3; ++i) {
        for (size_type j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(C(i, j), A(i, j) * 2.0);
        }
    }
}

TEST_F(MatrixTest, Transpose) {
    Matrix T = A.transpose();
    
    for (size_type i = 0; i < 3; ++i) {
        for (size_type j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(T(i, j), A(j, i));
        }
    }
}

TEST_F(MatrixTest, Identity) {
    Matrix I(3, 3);
    I.identity();
    
    for (size_type i = 0; i < 3; ++i) {
        for (size_type j = 0; j < 3; ++j) {
            if (i == j) {
                EXPECT_DOUBLE_EQ(I(i, j), 1.0);
            } else {
                EXPECT_DOUBLE_EQ(I(i, j), 0.0);
            }
        }
    }
}

TEST_F(MatrixTest, Randomize) {
    Matrix m(3, 3);
    m.randomize(-1.0, 1.0, 42);
    
    // Check that all elements are within bounds
    for (size_type i = 0; i < 3; ++i) {
        for (size_type j = 0; j < 3; ++j) {
            EXPECT_GE(m(i, j), -1.0);
            EXPECT_LE(m(i, j), 1.0);
        }
    }
}

TEST_F(MatrixTest, Equality) {
    Matrix C = A;
    EXPECT_TRUE(A.is_equal(C));
    
    C(0, 0) += 1e-9; // Small change
    EXPECT_TRUE(A.is_equal(C, 1e-8)); // Should still be equal with tolerance
    
    C(0, 0) += 1e-7; // Larger change
    EXPECT_FALSE(A.is_equal(C, 1e-8)); // Should not be equal
}

TEST_F(MatrixTest, Submatrix) {
    Matrix sub = A.submatrix(0, 0, 2, 2);
    EXPECT_EQ(sub.rows(), 2);
    EXPECT_EQ(sub.cols(), 2);
    
    for (size_type i = 0; i < 2; ++i) {
        for (size_type j = 0; j < 2; ++j) {
            EXPECT_DOUBLE_EQ(sub(i, j), A(i, j));
        }
    }
}

TEST_F(MatrixTest, UtilityFunctions) {
    // Test create_identity
    Matrix I = create_identity(3);
    EXPECT_EQ(I.rows(), 3);
    EXPECT_EQ(I.cols(), 3);
    EXPECT_TRUE(I.is_square());
    
    for (size_type i = 0; i < 3; ++i) {
        for (size_type j = 0; j < 3; ++j) {
            if (i == j) {
                EXPECT_DOUBLE_EQ(I(i, j), 1.0);
            } else {
                EXPECT_DOUBLE_EQ(I(i, j), 0.0);
            }
        }
    }
    
    // Test create_random
    Matrix R = create_random(3, 3, -1.0, 1.0, 42);
    EXPECT_EQ(R.rows(), 3);
    EXPECT_EQ(R.cols(), 3);
    
    for (size_type i = 0; i < 3; ++i) {
        for (size_type j = 0; j < 3; ++j) {
            EXPECT_GE(R(i, j), -1.0);
            EXPECT_LE(R(i, j), 1.0);
        }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 