#ifndef PRIME_TEST_H
#define PRIME_TEST_H

#include "core/long.h"
#include <functional>

// Prime testing base class
struct PrimeTest {
    static double not_prime;
    static double SolovStras_met(Long & a, const Long & p);
    static double Lehmann_met(Long & a, const Long & p);
};

// Rabin-Miller test class
struct Rab_MilTest : PrimeTest {
    static Long b;
    static Long m;
    static double RabinMiller_met(Long & a, const Long & p);
};

// Jacobi symbol calculation
Long Jacobi(Long& a, Long& n);

// General prime testing function
double prtest_general(const Long & p, ull iter, double (*_met)(Long& a, const Long& p));

// Specific prime testing methods
double prtest_SolovStras(const Long & p, ull iter);
double prtest_Lehmann(const Long & p, ull iter);
double prtest_RabinMiller(const Long & p, ull iter);

#endif // PRIME_TEST_H 