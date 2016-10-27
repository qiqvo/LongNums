#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <thread>
#include <math.h>   // pi 3.14159265359
#include <complex>  // std::complex
#include <utility>  // std::swap
#include <algorithm>  // std::max
#include "Random.h"

#define _USE_MATH_DEFINES
#define PI 3.14159265358979323846

	//#define KARALOG
#define DEBUG

	using std::vector;
	using std::swap;
	typedef unsigned int uint;
	typedef unsigned short ush;
	typedef std::complex<double> ReIm;
	typedef unsigned long long ull;


#ifndef LONGCLASS
    #define LONGCLASS
class Long
{
protected:
 bool sign;
    Long();
    void clear();
    Long set(ull v, bool s = true);
    ull set(uint i, ull v); // dangerous ! use normal after!

	void insert(ull v = 0);
	Long insert(vector<ull>);
	void shift(uint n = 1);
	Long normal();

	vector<ull> a;   // 2383597  --->   a == { 7 , 9 , 5 , 3 ... 2 }
	static const ull karacnaive;
	static const ull toom3naive;
public:
	static const ull base;

	Long changeSign(int b = 3);

	uint size() const;
	ull operator[](int i) const;
	void print(std::ostream & stream = std::cout) const;

	Long sum(const Long& b) const;
	Long neg(const Long& b) const;
	Long mul(const Long& b) const;
	Long divide(const int b, Long& rem) const;
	Long divide(const Long& b, Long& rem) const;
	Long karac_mul(const Long& b) const;
	Long toomcook_mul(const Long & b) const;
	Long strassen_mul(const Long & b) const; // using 2 threads! 
public:
	Long(uint);
	Long(int);
	Long(long);
	Long(long long);
	Long(unsigned long);
	Long(ull);
	Long(const char*);
	Long(vector<ull>); // dangerous! ((((

	Long operator=(uint);
	Long operator=(int);
	Long operator=(long);
	Long operator=(long long);
	Long operator=(unsigned long);
	Long operator=(ull);
	Long operator=(const char *);
	Long operator=(vector<ull>); // dangerous!

public:
	Long(const Long &o);
	Long(Long &&o);
	Long& operator=(const Long& o);
	Long& operator=(Long&& o);

	Long operator+(const Long& o) const;
	Long operator-(const Long& o) const;
	Long operator*(const Long& o) const;
	Long operator/(const int o)   const;
	Long operator/(const Long& o) const;
	Long operator%(const Long& o) const;

	bool operator<(const Long& o)  const;
	bool operator>(const Long& o)  const;
	bool operator<=(const Long& o) const;
	bool operator>=(const Long& o) const;
	bool operator==(const Long& o) const;
	bool operator!=(const Long& o) const;

	operator std::string() const;
};

const auto null = Long(0);
const auto pone = Long(1);
const auto mone = Long(-1);

// even == chetnoe
inline bool iseven(const Long & a) {
    return !(a[0] & 1);
}
// odd == nechetnoe
inline bool isodd(const Long & a) {
    return (a[0] & 1);
}

std::ostream & operator<<(std::ostream & stream, Long b);

Long rand(const uint s);
Long rand(const uint s, const Long &c,
    bool(*condition)(const Long& a, const Long& b) = [](auto& a, auto& b) { return true; });

Long abs(const Long& a);
Long factorial(const Long& n);
Long gcd(const Long& u, const Long& v);
Long pow(const Long& u, const Long& v);
Long pow_mod(const Long& a, const Long& pow, const Long& N);

/* using 4 threads */
double prtest_SolovStras(const Long & p, ull iter);
double prtest_Lehmann(const Long & p, ull iter);
double prtest_RabinMiller(const Long & p, ull iter);

Long Jacobi(Long& a, Long& b);

std::string my_to_string(const ull& a, const int base);
#endif
