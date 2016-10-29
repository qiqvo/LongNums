#pragma once
#include "Long.h"

class Real : public Long{
	static const char* delim_mant;
	uint mantisa_place;
	Real normalmant();

public:
	// 0.000000000005151510000000021540000000058887 -> 0.00000000000515151
	Real cut(uint t = 16);
	uint set_mant(uint mant) { mantisa_place = mant; }
	const uint get_mant() const { return mantisa_place; }
	Real(const Long& p1, uint m = 0); // if you want 0.1: p1 = 1000, m = 1
	                                          //     0.01: 2, 100
	                                          //  0.000001: 6, 100
	
	Real operator+(const Real& o) const;
	Real operator-(const Real& o) const;
	Real operator*(const Real& o) const;

	Real(const Real &o);
	Real(Real &&o);
	Real& operator=(const Real& o);
	Real& operator=(Real&& o);

	 operator std::string() const;
};

std::ostream & operator<<(std::ostream & stream, Real b);