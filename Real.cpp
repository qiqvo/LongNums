#include "Long.h"
#include "Real.h"

const char* Real::delim_mant = ".";
Real::operator std::string() const {
	std::string t = Long(*this);
	int i = 0;
	while (i != std::string::npos && i < mantisa_place) { ++i; }
	t.insert(t.begin() + i, delim_mant[0]);
	return t;
}
Real Real::normalmant()
{
	if (mantisa_place == 0)
		return *this;
	int bs = std::log10(base);
	for (int i = 0, j = 0; i < mantisa_place && j < size(); ++j){	
		if (i > mantisa_place)
			break;
		if (uint(std::log10(a[j])) < base - 1) {
			auto tmp = 0;
			auto at = a[j];
			while (at < base && i + tmp < mantisa_place) {
				at *= 10;
				++tmp;
			}
			if (at > base)
				at /= 10;
			a[j] = at;
		}
		i += bs;
	}

	return *this;
}
Real Real::cutlastnulls()
{
	vector<ull> vec;
	int bs = std::log10(base);
	int i = 0, j = 0;
	while (i < mantisa_place) {
		if (a[j] != 0) {
			break;
		}
		++j;
		i += bs;
	}
	while(j < size())			
		vec.push_back(a[j++]);
	a = vec;
	return *this;
}
Real::Real(const Long & p1, uint m) : Long(p1), mantisa_place(m) {
	normalmant(); cutlastnulls();
}

Real Real::operator+(const Real & o) const
{
	bool mmant = mantisa_place > o.mantisa_place;
	Long p1 = (mmant) ? *this : o;
	Long p2 = (mmant) ? o : *this;

	auto m1 = mmant ? mantisa_place : o.mantisa_place;
	auto m2 = mmant ? o.mantisa_place : mantisa_place;
	p1 = p1 + p2.shift(m1 - m2);
	Real p3 = static_cast<Real>(p1);
	p3.mantisa_place = m1;
	return p3.cutlastnulls();
}
Real Real::operator-(const Real & o) const {
	bool mmant = mantisa_place > o.mantisa_place;
	Long p1 = (mmant) ? *this : o;
	Long p2 = (mmant) ? o : *this;

	auto m1 = mmant ? mantisa_place : o.mantisa_place;
	auto m2 = mmant ? o.mantisa_place : mantisa_place;
	int bs = std::log10(base);
	auto t10 = 0; auto t = p2[0];
	while (t % 10 == 0) {
		++t10; t = t / 10;
	}

	int shifting = (m1 - m2 + t10);
	p1 = mmant ? (p1 - p2.shiftback(shifting)) : (p2.shiftback(shifting) - p1);
	Real p3 = static_cast<Real>(p1);
	p3.mantisa_place = m1;
	return p3.cutlastnulls();
}
Real Real::operator*(const Real & o) const{
	Long p1 = Long(*this) * Long(o);
	Real p2 = static_cast<Real>(p1);
	p2.mantisa_place = mantisa_place + o.mantisa_place;
	return p2.cutlastnulls();
}

Real::Real(const Real & o) : Long(o)
{
	operator=(o);
}

Real::Real(Real && o)
{
	operator=(o);
}

Real & Real::operator=(const Real & o)
{
	mantisa_place = o.mantisa_place;
	clear();
	for (auto i : o.a) {
		a.push_back(i);
	}
	sign = o.sign;
	mantisa_place = o.mantisa_place;
	return *this;
}

Real & Real::operator=(Real && o) {
	clear();
	for (auto i : o.a) {
		a.push_back(i);
	}
	sign = o.sign;
	mantisa_place = o.mantisa_place;
	o.clear();
	return *this;
}
