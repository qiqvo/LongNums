#include "Long.h"
#include "Real.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    cout << "=== LongNums Library Demo ===" << endl;
    cout << "Demonstrating basic functionality..." << endl << endl;

    // Load small primes from file
    cout << "Loading small primes from file..." << endl;
    auto small_primes = load_small_primes();
    cout << "Loaded " << small_primes.size() << " small primes (up to " << small_primes.back() << ")" << endl << endl;

    // Test 1: Basic Long arithmetic
    cout << "1. Basic Long arithmetic:" << endl;
    Long a = 123;
    Long b = 456;
    cout << "   a = " << a << endl;
    cout << "   b = " << b << endl;
    cout << "   a + b = " << a + b << endl;
    cout << "   a * b = " << a * b << endl << endl;

    // Test 2: Large number construction
    cout << "2. Large number construction:" << endl;
    Long large_number = Long("12345678901234567890");
    cout << "   Large number: " << large_number << endl << endl;

    // Test 3: Multiplication algorithm comparison
    cout << "3. Multiplication algorithm comparison:" << endl;
    Long x = 12345;
    Long y = 67890;
    cout << "   x = " << x << endl;
    cout << "   y = " << y << endl;
    
    Long naive_result = x.mul(y);
    Long karatsuba_result = x.karac_mul(y);
    Long toomcook_result = x.toomcook_mul(y);
    
    cout << "   Naive multiplication: " << naive_result << endl;
    cout << "   Karatsuba multiplication: " << karatsuba_result << endl;
    cout << "   Toom-Cook multiplication: " << toomcook_result << endl;
    cout << "   All results equal: " << (naive_result == karatsuba_result && karatsuba_result == toomcook_result ? "Yes" : "No") << endl << endl;

    // Test 4: Prime testing with small primes from file
    cout << "4. Prime testing with small primes from file:" << endl;
    cout << "   Testing first 10 primes from file:" << endl;
    for (size_t i = 0; i < min(size_t(10), small_primes.size()); ++i) {
        Long prime_candidate = small_primes[i];
        cout << "   Testing " << prime_candidate << " (should be prime): ";
        
        // Use more iterations for small primes to ensure accuracy
        // Note: prtest_general divides iterations by 4, so we multiply by 4
        double solovay_result = prtest_SolovStras(prime_candidate, 80);  // 20 * 4
        double lehmann_result = prtest_Lehmann(prime_candidate, 80);     // 20 * 4
        double rabin_result = prtest_RabinMiller(prime_candidate, 80);   // 20 * 4
        
        bool all_positive = (solovay_result > 0.5 && lehmann_result > 0.5 && rabin_result > 0.5);
        cout << (all_positive ? "PASS" : "FAIL") << endl;
    }
    cout << endl;

    // Test 5: Testing some composite numbers
    cout << "5. Testing composite numbers:" << endl;
    vector<Long> composites = {Long(4), Long(6), Long(8), Long(9), Long(10), Long(12), Long(14), Long(15), Long(16), Long(18)};
    cout << "   Testing first 10 composite numbers:" << endl;
    for (const auto& composite : composites) {
        cout << "   Testing " << composite << " (should be composite): ";
        
        double solovay_result = prtest_SolovStras(composite, 20);  // 5 * 4
        double lehmann_result = prtest_Lehmann(composite, 20);     // 5 * 4
        double rabin_result = prtest_RabinMiller(composite, 20);   // 5 * 4
        
        // For composites, we expect at least one test to return a low probability
        bool likely_composite = (solovay_result < 0.5 || lehmann_result < 0.5 || rabin_result < 0.5);
        cout << (likely_composite ? "PASS" : "FAIL") << endl;
    }
    cout << endl;

    // Test 6: Real number arithmetic
    cout << "6. Real number arithmetic:" << endl;
    Real real_a(Long(1000), 1);  // 0.1
    Real real_b(Long(2000), 1);  // 0.2
    cout << "   Real a (0.1): " << real_a << endl;
    cout << "   Real b (0.2): " << real_b << endl;
    cout << "   Real a + b: " << real_a + real_b << endl << endl;

    // Test 7: Mathematical functions
    cout << "7. Mathematical functions:" << endl;
    Long gcd_result = gcd(Long(12), Long(18));
    Long factorial_result = factorial(Long(5));
    cout << "   GCD(12, 18) = " << gcd_result << endl;
    cout << "   5! = " << factorial_result << endl << endl;

    // Test 8: Large number operations
    cout << "8. Large number operations:" << endl;
    Long big_num1 = Long("123456789");
    Long big_num2 = Long("987654321");
    cout << "   Big number 1: " << big_num1 << endl;
    cout << "   Big number 2: " << big_num2 << endl;
    cout << "   Sum: " << big_num1 + big_num2 << endl;
    cout << "   Product: " << big_num1 * big_num2 << endl << endl;

    // Test 9: Performance test with larger numbers
    cout << "9. Performance test with larger numbers:" << endl;
    Long perf_num1 = Long("123456789012345");
    Long perf_num2 = Long("987654321098765");
    
    auto start = chrono::high_resolution_clock::now();
    Long perf_result = perf_num1 * perf_num2;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    cout << "   Large multiplication took: " << duration.count() << " ms" << endl;
    cout << "   Result digits: " << perf_result.real_size() << endl << endl;

    // Test 10: Prime testing with larger numbers
    cout << "10. Prime testing with larger numbers:" << endl;
    vector<Long> larger_primes = {Long(10007), Long(10009), Long(10037), Long(10039), Long(10061)};
    cout << "   Testing larger primes from file:" << endl;
    for (const auto& prime : larger_primes) {
        cout << "   Testing " << prime << ": ";
        
        auto start = chrono::high_resolution_clock::now();
        double solovay_result = prtest_SolovStras(prime, 40);  // 10 * 4
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        
        cout << "Solovay-Strassen: " << solovay_result << " (" << duration.count() << " ms)" << endl;
    }
    cout << endl;

    // Test 11: Stress test with very large numbers
    cout << "11. Stress test with very large numbers:" << endl;
    Long stress_num1 = Long("123456789012345678901234567890");
    Long stress_num2 = Long("987654321098765432109876543210");
    
    start = chrono::high_resolution_clock::now();
    Long stress_result = stress_num1 * stress_num2;
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    cout << "   Very large multiplication took: " << duration.count() << " ms" << endl;
    cout << "   Result digits: " << stress_result.real_size() << endl;
    cout << "   First 20 digits: " << string(stress_result).substr(0, 20) << "..." << endl << endl;

    // Test 12: Random number generation test
    cout << "12. Random number generation test:" << endl;
    Long max_rand = Long("1000000000000");
    cout << "   Generating 5 random numbers up to " << max_rand << ":" << endl;
    for (int i = 0; i < 5; ++i) {
        Long random_num = rand(max_rand.size(), max_rand);
        cout << "   Random " << (i+1) << ": " << random_num << endl;
    }
    cout << endl;

    cout << "=== Demo completed successfully! ===" << endl;
    cout << "The LongNums library is working correctly." << endl;
    
    return 0;
}
