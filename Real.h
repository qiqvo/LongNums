#pragma once
#include "Long.h"

class Real : public Long{
	vector<ull> part;
	const char* delim_mant = ".";
	uint mantisa_place;
	Real normall();
public:
	Real(const Long& p1, const vector<ull>& p2); // p1.p2

	Real operator+(const Real& o) const;
	Real operator-(const Real& o) const;
	Real operator*(const Real& o) const;
	Real operator/(const Real& o) const;
};