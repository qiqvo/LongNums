#pragma once
#include "core/long.h"

class Real : public Long{
	static const char* delim_mant;
	uint mantissa_place;
	Real normalmant();

public:
	Real cut(uint t = 16);
	uint set_mant(uint mant) { mantissa_place = mant; return mantissa_place; }
	uint get_mant() const { return mantissa_place; }
	Real(const Long& value, uint m = 0); // if you want 0.1: value = 1000, m = 1
	                                          //     0.01: 2, 100
	                                          //  0.000001: 6, 100
	
	Real operator+(const Real& other) const;
	Real operator-(const Real& other) const;
	Real operator*(const Real& other) const;

	Real(const Real &other);
	Real(Real &&other);
	Real& operator=(const Real& other);
	Real& operator=(Real&& other);

	operator std::string() const;
};

std::ostream & operator<<(std::ostream & stream, Real b);