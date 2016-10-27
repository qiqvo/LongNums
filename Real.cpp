#include "Long.h"
#include "Real.h"

int real_size(vector<ull> v){
	uint s = v.size();
	uint b = Long::base;
	ull last_n = v.back();
	return (s - 1)*std::log10(b) + std::log10(last_n);
};

Real::Real(const Long & p1, uint m = 0) : Long(p1), mantisa_place(m) {}

Real::Real(const Long & p1, const vector<ull>& p2, uint mant) : Real(p1, p2) {
	mantisa_place = (mant);
}

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
