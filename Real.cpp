#include "Long.h"
#include "Real.h"

const char* Real::delim_mant = ".";
Real::Real(const Long & p1, uint m) : Long(p1), mantisa_place(m) {}

Real Real::operator+(const Real & o) const
{
	bool mmant = mantisa_place > o.mantisa_place;
	Long p1 = (mmant) ? *this : o;
	Long p2 = (mmant) ? o : *this;

	auto m1 = mmant ? mantisa_place : o.mantisa_place;
	auto m2 = mmant ? o.mantisa_place : mantisa_place;
	p1 = p1 + p2.shift(m1 - m2);
	return Real(p1);
}
Real Real::operator-(const Real & o) const
{
	bool mmant = mantisa_place > o.mantisa_place;
	Long p1 = (mmant) ? *this : o;
	Long p2 = (mmant) ? o : *this;

	auto m1 = mmant ? mantisa_place : o.mantisa_place;
	auto m2 = mmant ? o.mantisa_place : mantisa_place;
	p1 = p1 + p2.shift(m1 - m2);
	return Real(p1);

	p1 = mmant ? (p1 - p2.shift(m1 - m2)) : (p2.shift(m1 - m2) - p1);
	return Real(p1);
}
Real Real::operator*(const Real & o) const{
	Long p1 = Long(*this) * Long(o);
	return Real(p1, mantisa_place + o.mantisa_place);
}

Real::Real(const Real & o) : Long(o)
{
	mantisa_place = o.mantisa_place;
}

Real::Real(Real && o)
{
}

Real & Real::operator=(const Real & o)
{
	// TODO: insert return statement here
}

Real & Real::operator=(Real && o)
{
	// TODO: insert return statement here
}
