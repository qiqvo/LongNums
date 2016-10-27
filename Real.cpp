#include "Long.h"
#include "Real.h"

int real_size() {
	
}

Real Real::normall() {
	auto real_size = [](vector<ull> v) -> int{
		uint s = v.size();
		uint b = base;
		ull last_n = v.back();
		return (s - 1)*std::log10(b) + std::log10(last_n);
	};
	int s = real_size(a) + real_size(part);
	vector<ull> p1(mantisa_place, (ull)0);
	vector<ull> p2(s - mantisa_place, (ull)0);
	int i = 0;
	auto ps = real_size(part);
	int bs = std::log10(base);
	while (i < s){
		if (i > mantisa_place) {
			auto t = (i > ps) ? a[i - mantisa_place] : part[i];
			p2[i - mantisa_place] = t;
		}
		else if (i < ps)
			p1[i] = part[i];
		else if (i >= ps)
			p1[i] = a[ps - i];
		i = i + bs;

	}

	return *this;
}

Real::Real(const Long & p1, const vector<ull>& p2) : Long(p1), part(p2), 
                            mantisa_place(p2.size() * std::log10(base)) {}

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
