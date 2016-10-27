#pragma once
#include "Long.h"

class Real : public Long{
	Real();
	const char* delim_mant = ".";
	uint mantisa_place;
public:
	Real(const Long& r);

	static const uint eps;

	Real operator+(const Real& o) const;
	Real operator-(const Real& o) const;
	Real operator*(const Real& o) const;
	Real operator/(const Real& o) const;
};