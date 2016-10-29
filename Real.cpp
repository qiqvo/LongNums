#include "Long.h"
#include "Real.h"

const char* Real::delim_mant = ".";
std::ostream & operator<<(std::ostream & stream, Real b) {
	stream << b.operator std::string();
	return stream;
}
Real::operator std::string() const {
	std::string s = "";
	if (!sign) s += "-";

	int i = size() - 1, j = real_size();
	int q = mantisa_place;
	if (q >= j) {
		--q;
		s += "0" + std::string(delim_mant);
		while (q-- > j)
			s += "0";
		while (i >= 0) 
			s += my_to_string(a[i--], base);
	}
	else {
		s += std::to_string(a[i--]);
		while (i >= 0) {
			if (j > mantisa_place)
				for (auto tmp = 0; tmp < bs; ++tmp) {
					if (j - tmp == mantisa_place)
						s += std::string(delim_mant);
				}
			else 
				j -= bs;

			s += my_to_string(a[i--], base);
		}
	}
	return s;
}
Real Real::normalmant()
{
	if (mantisa_place == 0)
		return *this;
	int bs = std::log10(base);
	vector<ull> vec;
	int j = 0;
	if (a[0] == 0) {
		do {
			++j;
		} while (j < size() && a[j] == 0); 
		while (j < size())
			vec.push_back(a[j++]);
		a = vec;
		return normalmant();
	}

	return *this;
}

Real Real::cut()
{
	if (size() >= 16) {
		a.erase(a.begin(), a.end() - 16);
	}
}

Real::Real(const Long & p1, uint m) : Long(p1), mantisa_place(m) {
	normalmant();
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
	return p3.normalmant();
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
	p3.mantisa_place = m1;       // x(n+1) = xn(2 - b * xn)
	return p3.normalmant();
}
Real Real::operator*(const Real & o) const {
	Long p1 = Long(*this) * Long(o);
	Real p2 = static_cast<Real>(p1);
	p2.mantisa_place = mantisa_place + o.mantisa_place;
	return p2.normalmant();
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
Long to_Long(const Real & a)
{
	vector<ull> c;
	auto i = 0, j = 0, q = 1;
	while (j < a.size()) {
		auto cou = 0;
		ull tmp = 0;
		while (cou <= Long::bs && i != a.get_mant()) {
			tmp += a.get_char(i++) * q;
			q *= 10;
		}
		c.push_back(tmp);
		++j;
	}
	return Long(c);
}