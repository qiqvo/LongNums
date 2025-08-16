#include <iostream>
#include <cassert>
#include "../include/long.h"
#include "../include/real.h"

// Simple test framework for LongNums
class TestFramework {
private:
    int passed = 0;
    int failed = 0;
    
public:
    template<typename T>
    void assert_equal(const T& actual, const T& expected, const std::string& test_name) {
        if (actual == expected) {
            std::cout << "✓ " << test_name << " PASSED" << std::endl;
            passed++;
        } else {
            std::cout << "✗ " << test_name << " FAILED" << std::endl;
            std::cout << "  Expected: " << expected << std::endl;
            std::cout << "  Actual: " << actual << std::endl;
            failed++;
        }
    }
    
    void print_summary() {
        std::cout << "\n=== Test Summary ===" << std::endl;
        std::cout << "Passed: " << passed << std::endl;
        std::cout << "Failed: " << failed << std::endl;
        std::cout << "Total: " << (passed + failed) << std::endl;
    }
};

int main() {
    TestFramework test;
    
    std::cout << "Running LongNums Basic Tests\n" << std::endl;
    
    // Test 1: Basic Long construction
    Long a = 123;
    Long b = 456;
    test.assert_equal(a, Long(123), "Long construction from int");
    test.assert_equal(b, Long(456), "Long construction from int");
    
    // Test 2: Basic addition
    Long c = a + b;
    test.assert_equal(c, Long(579), "Basic addition");
    
    // Test 3: Basic multiplication
    Long d = a * b;
    test.assert_equal(d, Long(56088), "Basic multiplication");
    
    // Test 4: String construction
    Long e = "123456789";
    test.assert_equal(e, Long(123456789), "Long construction from string");
    
    // Test 5: Real number construction
    Real f(1000, 1);  // Represents 0.1
    Real g(2000, 1);  // Represents 0.2
    Real h = f + g;
    // Note: Real comparison might need custom implementation
    
    // Test 6: Karatsuba multiplication
    Long large_a = "123456789";
    Long large_b = "987654321";
    Long karatsuba_result = large_a.karac_mul(large_b);
    Long naive_result = large_a * large_b;
    test.assert_equal(karatsuba_result, naive_result, "Karatsuba multiplication correctness");
    
    // Test 7: Prime testing
    Long prime_candidate = 17;
    double solovay_result = prtest_SolovStras(prime_candidate, 10);
    test.assert_equal(solovay_result > 0, true, "Solovay-Strassen test for prime number");
    
    // Test 8: Prime testing for 13
    Long thirteen = 13;
    double solovay_result_13 = prtest_SolovStras(thirteen, 10);
    test.assert_equal(solovay_result_13 > 0, true, "Solovay-Strassen test for 13 (should be prime)");
    
    test.print_summary();
    
    return 0;
} 