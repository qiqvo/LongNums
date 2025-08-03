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

//#define DEBUG

using std::vector;
using std::swap;
typedef unsigned int uint;
typedef unsigned short ush;
typedef unsigned long long ull;

class Long;
class Real;

class Long
{
protected:
    int sign; // -1 for neg, 1 for pos 
	static const char* delim;

	Long();
    void clear();
    Long set(ull v, bool s = true);
    ull set(uint i, ull v); // dangerous ! use normal after!

	void insert(ull v = 0);
	Long insert(vector<ull>);
	virtual Long normal();

	vector<ull> a;   // 2383597  --->   a == { 7 , 9 , 5 , 3 ... 2 }
	static const ull karacnaive;
	static const ull toom3naive;
	static const ull strasnaive;
	
public:
	static const ull base;
	static const uint bs; 

	Long changeSign(int b = 3);

	const vector<ull>& container() const;
	uint size() const;
	uint real_size() const;

	ull operator[](int i) const;
	ull get_char(uint i) const;
	void print(std::ostream & stream = std::cout) const;

	Long shift(uint n = 1);
	Long shiftaccurate(uint n = 1);
	Long shiftback(uint n = 1);
	Long sum(const Long& b) const;
	Long neg(const Long& b) const;
	Long mul(const Long& b) const;
	Long divide(const int b, Long& rem) const;
	Long divide(const Long& b, Long& rem) const;
	Long divide2(const Long & b, Long& rem) const;
	Long mult_inv(const Long& b) const;
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
	Long(const vector<ull>& x); // dangerous!

	Long operator=(uint);
	Long operator=(int);
	Long operator=(long);
	Long operator=(long long);
	Long operator=(unsigned long);
	Long operator=(ull);
	Long operator=(const char *);
	Long operator=(const vector<ull>& x); // dangerous!

public:
	Long(const Long &o);
	Long(Long &&o);
	Long& operator=(const Long& o);
	Long& operator=(Long&& o);

	virtual Long operator+(const Long& o) const;
	virtual Long operator-(const Long& o) const;
	virtual Long operator*(const Long& o) const;
	Long operator/(const int o)   const;
	Long operator/(const Long& o) const;
	Long operator%(const Long& o) const;

	bool operator<(const Long& o)  const;
	bool operator>(const Long& o)  const;
	bool operator<=(const Long& o) const;
	bool operator>=(const Long& o) const;
	bool operator==(const Long& o) const;
	bool operator!=(const Long& o) const;

	virtual operator std::string() const;

	~Long();
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
    bool(*condition)(const Long& a, const Long& b) = [](auto& a, auto& b) { (void)a; (void)b; return true; });

Real inverse(const Long& a, uint iterations = 15);
Long abs(const Long& a);
Long factorial(const Long& n);
Long gcd(const Long& u, const Long& v);
Long pow(const Long& u, const Long& v);
Long pow_mod(const Long& a, const Long& pow, const Long& N);

double prtest_SolovStras(const Long & p, ull iter);
double prtest_Lehmann(const Long & p, ull iter);
double prtest_RabinMiller(const Long & p, ull iter);

Long Jacobi(Long& a, Long& b);

std::string my_to_string(const ull& a, const int base);
Long to_Long(const Real& a);