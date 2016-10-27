#pragma once
#include "Long.h"

class Real : public Long{
	static const char* delim_mant;
	uint mantisa_place;
	Real normalmant();
	Real cutlastnulls();
public:
	const uint get_mant() const { return mantisa_place; }
	Real(const Long& p1, uint m = 0); // p1.p2
	
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