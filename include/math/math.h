#ifndef MATH_H
#define MATH_H

#include "core/long.h"
#include <vector>

// Random number generation
Long rand(const uint s);
Long rand(const uint s, const Long &c, bool(*condition)(const Long &a, const Long &b));

// Basic arithmetic functions
Long abs(const Long & a);
Long factorial(const Long & N);

// Power and modular arithmetic
Long pow_mod(const Long& x, const Long& y, const Long& N);
Long pow(const Long & a, const Long & n);

// Greatest common divisor
Long gcd(const Long& u, const Long& v);

#endif // MATH_H 