#pragma once
#include "Long.h"

class Real : public Long{
	const char* delim_mant = ".";
	uint mantisa_place;
public:
	const uint get_mant() const { return mantisa_place; }
	Real(const Long& p1, uint m); // p1.p2
	Real(const Long& p1, const vector<ull>& p2, uint mant); // p1.p2

	Real operator+(const Real& o) const;
	Real operator-(const Real& o) const;
	Real operator*(const Real& o) const;
};