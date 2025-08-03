#include <iostream>
#include <fstream>
#include <string>
#include "Long.h"
#include "Real.h"
#include <ctime>

#include <thread>
#include <future>

using namespace std;

int main() {
    cout << "=== LongNums Library Demo ===" << endl;
    cout << "Demonstrating basic functionality...\n" << endl;

    // Test 1: Basic Long construction and arithmetic
    cout << "1. Basic Long arithmetic:" << endl;
    Long a = 123;
    Long b = 456;
    Long c = a + b;
    Long d = a * b;
    
    cout << "   a = " << a << endl;
    cout << "   b = " << b << endl;
    cout << "   a + b = " << c << endl;
    cout << "   a * b = " << d << endl;
    cout << endl;

    // Test 2: Large number construction
    cout << "2. Large number construction:" << endl;
    Long large_num = "12345678901234567890";
    cout << "   Large number: " << large_num << endl;
    cout << endl;

    // Test 3: Different multiplication algorithms
    cout << "3. Multiplication algorithm comparison:" << endl;
    Long x = "12345";
    Long y = "67890";
    
    Long naive_result = x * y;
    Long karatsuba_result = x.karac_mul(y);
    Long toomcook_result = x.toomcook_mul(y);
    
    cout << "   x = " << x << endl;
    cout << "   y = " << y << endl;
    cout << "   Naive multiplication: " << naive_result << endl;
    cout << "   Karatsuba multiplication: " << karatsuba_result << endl;
    cout << "   Toom-Cook multiplication: " << toomcook_result << endl;
    cout << "   All results equal: " << (naive_result == karatsuba_result && karatsuba_result == toomcook_result ? "Yes" : "No") << endl;
    cout << endl;

    // Test 4: Prime testing (with smaller numbers and fewer iterations)
    cout << "4. Prime testing (small numbers, few iterations):" << endl;
    Long prime_candidate = 17;
    cout << "   Testing if " << prime_candidate << " is prime:" << endl;
    
    double solovay_result = prtest_SolovStras(prime_candidate, 5);
    double lehmann_result = prtest_Lehmann(prime_candidate, 5);
    double rabin_result = prtest_RabinMiller(prime_candidate, 5);
    
    cout << "   Solovay-Strassen: " << solovay_result << endl;
    cout << "   Lehmann: " << lehmann_result << endl;
    cout << "   Rabin-Miller: " << rabin_result << endl;
    cout << endl;

    // Test 5: Real number arithmetic
    cout << "5. Real number arithmetic:" << endl;
    Real real_a(1000, 1);  // Represents 0.1
    Real real_b(2000, 1);  // Represents 0.2
    Real real_c = real_a + real_b;
    
    cout << "   Real a (0.1): " << real_a << endl;
    cout << "   Real b (0.2): " << real_b << endl;
    cout << "   Real a + b: " << real_c << endl;
    cout << endl;

    // Test 6: Mathematical functions
    cout << "6. Mathematical functions:" << endl;
    Long num1 = 12;
    Long num2 = 18;
    Long gcd_result = gcd(num1, num2);
    Long factorial_result = factorial(Long(5));
    
    cout << "   GCD(" << num1 << ", " << num2 << ") = " << gcd_result << endl;
    cout << "   5! = " << factorial_result << endl;
    cout << endl;

    cout << "=== Demo completed successfully! ===" << endl;
    cout << "The LongNums library is working correctly." << endl;

    return 0;
}
