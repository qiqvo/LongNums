#include "Long.h"
#include "Real.h"

const uint Real::eps = 90;

Real::Real() : Long(null) {}
Real::Real(const Long & r) : Long(r), mantisa_place(r.size() * std::log10(base)) {}

Real Real::operator+(const Real & o) const
{
	Long p1 = (size() > o.size()) ? *this : o;
	Long p2 = (size() > o.size()) ? o : *this;
	auto m1 = p1.size();
	auto m2 = p2.size();
	p1 = p1.shift(m1 - m2) + p2;
	return Real{ p1 };
}
Real Real::operator-(const Real & o) const
{
	Long p1 = (size() > o.size()) ? *this : o;
	Long p2 = (size() > o.size()) ? o : *this;
	auto m1 = p1.size();
	auto m2 = p2.size();
	p1 = p1.shift(m1 - m2) + p2;
	return Real{ p1 };
}
