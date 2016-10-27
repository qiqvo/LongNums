#include "Long.h"
#include <fstream>
#include <time.h>    // time
//#include <thread>
//#include <future>
//#include <stdexcept>   // for logic error

std::ostream & operator<<(std::ostream & stream, Long b)
{
	stream << b.operator std::string();
	return stream;
}

inline std::string my_to_string(const ull& a, const int base)
{
	std::string s = "";
	auto b = base; auto sz = 0;
	while (b != 1) {
		b = b / 10;
		++sz;
	}

	std::string q = std::to_string(a);
	auto ssz = q.size();
	while (ssz != sz){
		s += "0"; ++ssz;
	}
    s += std::to_string(a);

	return s;
}

const ull Long::base = 10000;
const ull Long::karacnaive = 5;
const ull Long::toom3naive = 5;

Long::Long() : sign(true) {}

Long::Long(const Long & o)
{
	operator=(o);
}
Long::Long(Long && o)
{
	operator=(o);
}
Long & Long::operator=(const Long & o)
{
	clear();
	for (auto i : o.a) {
		(*this).a.push_back(i);
	}
	sign = o.sign;
	return *this;
}
Long & Long::operator=(Long && o)
{
	clear();
	for (auto i : o.a) {
		a.push_back(i);
	}
	sign = o.sign;
	o.clear();
	return *this;
}

void Long::clear()
{
	sign = 0;
	a.clear();
}

uint Long::size() const
{
	return (uint)a.size();
}
ull Long::operator[](int i) const
{
	if (i >= size()) return 0;
	return a[i];
}
void Long::print(std::ostream & stream) const
{
	for (int i = size() - 1; i >= 0; --i) {
		stream << a[i];
	}
	stream << "\n";
}

Long::operator std::string() const
{
	if (size() == 1 && a[0] == 0){
		return std::string("0");
	}

	std::string s = "";
	if (!sign) s += "-";

	int i = size() - 1;
	s += std::to_string(a[i--]);
	while (i >= 0) {
		s += my_to_string(a[i--], base);
	}
	return s;
}

ull Long::set(uint i, ull v) {
	if (i >= size()) {
		insert(vector<ull>(i + 1 - size(), 0));
	}
	a[i] = v;
	return v;
}

void Long::insert(ull v)
{
	a.push_back(v);
}
Long Long::insert(vector<ull> x)
{
	a.insert(a.cbegin(), x.cbegin(), x.cend());
	return *this;
}

Long Long::shift(uint n)
{
	while (n-- > 0) {
		a.emplace(a.begin(), (ull)0);
	}
	return *this;
}
Long Long::changeSign(int b)
{
	if (b == (int)false) sign = false;
	else if (b == (int)true) sign = true;
	else sign = !sign;

	return *this;
}

Long Long::normal()
{
	ull tmp = 0;
	for (uint i = 0; i < size(); ++i) {
		a[i] += tmp;
		if (a[i] >= base) {
			tmp = a[i] / base;
			a[i] = a[i] % base;
		}
		else
			tmp = 0;
	}
	if (tmp != 0)
	{
		auto tmp2 = tmp;
		while (tmp > 0) {
			tmp2 = tmp % base;
			tmp = tmp / base;
			insert(tmp2);
		}
	}

	while (a.size() > 1 && a.back() == 0) {
		a.pop_back();
	}
	return *this;
}

Long::Long(uint v)
{
	operator=(v);
}

Long::Long(int v)
{
	operator=(v);
}

Long::Long(long v)
{
	operator=(v);
}

Long::Long(long long v)
{
	operator=(v);
}

Long::Long(unsigned long v)
{
	operator=(v);
}

Long::Long(ull v)
{
	operator=(v);
}

Long::Long(const char * v)
{
	operator=(v);
}

Long::Long(vector<ull> x)
{
	operator=(x);
}

Long Long::operator=(uint v)
{
	return operator=((ull)v);
}

Long Long::operator=(unsigned long v)
{
	return set((ull)(v));
}

Long Long::operator=(int v)
{
	return set((ull)(::abs(v)), v >= 0);
}

Long Long::operator=(long v)
{
	return set((ull)(::abs(v)), v >= 0);
}

Long Long::operator=(long long v)
{
	return set((ull)(::abs(v)) , v >= 0);
}

Long Long::operator=(ull v)
{
	return set(v);
}

Long Long::set(ull v, bool s)
{
	clear();
	sign = s;
	if (v == 0)
	{
		a.push_back(v);
		return *this;
	}

	auto tmp = v;
	while (v > 0) {
		tmp = v % base;
		v = v / base;
		insert(tmp);
	}
	return *this;
}

Long Long::operator=(const char* v)
{
	clear();
	ull e = 0; uint size = 0;
	while (*(v++)) ++size;
	if (size == 0)
		return null;

	auto b = 1;

	v = v - size - 1;
	
	auto lb = 0;
	if (v[0] == '-') {
		sign = false;
		lb = 1;
	}
	else if (v[0] == '+') {
		sign = true;
		lb = 1;
	}
	else {
		sign = true;
		lb = 0;
	}
	v += lb;
	int i = size - 1 - lb;

	while(i >= 0 && v[i]) {
		if (b < base) {
			e += (v[i--] - '0') * b;
			b = b * 10;
		}
		else {
			insert(e);
			e = 0; b = 1;
		}
	}
	if(e != 0)
		insert(e);

	return *this;
}

Long Long::operator=(vector<ull> x)
{
	clear();
	insert(x);
	sign = true;
	return *this;
}