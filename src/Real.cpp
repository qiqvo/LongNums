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
	int q = mantissa_place;
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
			if (j > static_cast<int>(mantissa_place))
				for (auto tmp = 0; tmp < static_cast<int>(bs); ++tmp) {
					if (j - tmp == static_cast<int>(mantissa_place))
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
	if (mantissa_place == 0)
		return *this;
	// int bs = std::log10(base);  // Unused variable removed
	vector<ull> vec;
	int j = 0;
	if (a[0] == 0) {
		do {
			++j;
		} while (j < static_cast<int>(size()) && a[j] == 0); 
		while (j < static_cast<int>(size()))
			vec.push_back(a[j++]);
		a = vec;
		return normalmant();
	}

	return *this;
}

Real Real::cut(uint t)
{
	if (size() >= t) {
		mantissa_place -= (size() - t) * bs;
		a.erase(a.begin(), a.end() - t);
	}
	return *this;
}

Real::Real(const Long & value, uint m) : Long(value), mantissa_place(m) {
	normalmant();
}

Real Real::operator+(const Real & other) const
{
	bool mmant = mantissa_place > other.mantissa_place;
	Long p1 = (mmant) ? *this : other;
	Long p2 = (mmant) ? other : *this;

	auto m1 = mmant ? mantissa_place : other.mantissa_place;
	auto m2 = mmant ? other.mantissa_place : mantissa_place;
	p1 = p1 + p2.shift(m1 - m2);
	Real p3 = static_cast<Real>(p1);
	p3.mantissa_place = m1;
	return p3.normalmant();
}
Real Real::operator-(const Real & other) const {
	bool mmant = mantissa_place > other.mantissa_place;
	Long p1 = (mmant) ? *this : other;
	Long p2 = (mmant) ? other : *this;

	auto m1 = mmant ? mantissa_place : other.mantissa_place;
	auto m2 = mmant ? other.mantissa_place : mantissa_place;
	// int bs = std::log10(base);  // Unused variable removed
	auto t10 = 0; auto t = p2[0];
	while (t % 10 == 0) {
		++t10; t = t / 10;
	}

	int shifting = (m1 - m2 + t10);
	p1 = mmant ? (p1 - p2.shiftback(shifting)) : (p2.shiftback(shifting) - p1);
	Real p3 = static_cast<Real>(p1);
	p3.mantissa_place = m1;       // x(n+1) = xn(2 - b * xn)
	return p3.normalmant();
}
Real Real::operator*(const Real & other) const {
	Long p1 = Long(*this) * Long(other);
	Real p2 = static_cast<Real>(p1);
	p2.mantissa_place = mantissa_place + other.mantissa_place;
	return p2.normalmant();
}

Real::Real(const Real & other) : Long(other)
{
	operator=(other);
}

Real::Real(Real && other)
{
	operator=(other);
}

Real & Real::operator=(const Real & other)
{
	mantissa_place = other.mantissa_place;
	clear();
	for (auto i : other.a) {
		a.push_back(i);
	}
	sign = other.sign;
	mantissa_place = other.mantissa_place;
	return *this;
}

Real & Real::operator=(Real && other) {
	clear();
	for (auto i : other.a) {
		a.push_back(i);
	}
	sign = other.sign;
	mantissa_place = other.mantissa_place;
	other.clear();
	return *this;
}
Long to_Long(const Real & a)
{
	vector<ull> c;
	int i = a.real_size(), j = a.get_mant(), k = a.size() - 1;
	// int tmp = 0;  // Unused variable removed
	while (k > 0 && i > j) {
		c.push_back(a[k]);
		i -= (a[k] == 0) ? std::log10(a[k]) + 1 : Long::bs;
		--k;
	}
	for (int d = 0; d < static_cast<int>(c.size()) / 2; ++d) 
		std::swap(c[d], c[c.size() - d - 1]);

	return Long(c);// / std::pow(10, j - i - tmp);
}