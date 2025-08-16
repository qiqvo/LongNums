#include "core/long.h"
#include "core/real.h"
#include <fstream>
#include <time.h>
#include <stdexcept>

std::ostream & operator<<(std::ostream & stream, Long b) {
	stream << b.operator std::string();
	return stream;
}

inline std::string my_to_string(const ull& a, const int base)
{
	std::string s = "";
	auto b = base; 
	auto sz = 0;
	while (b != 1) {
		b = b / 10;
		++sz;
	}

	std::string q = std::to_string(a);
	auto ssz = q.size();
	while (static_cast<int>(ssz) != sz){
		s += "0"; 
		++ssz;
	}
    s += std::to_string(a);

	return s;
}

const ull Long::base = 10000;
const uint Long::bs = std::log10(Long::base);
const ull Long::karacnaive = 25;
const ull Long::toom3naive = 30;
const ull Long::strasnaive = 40;
const char* Long::delim = ",";

Long::Long() : sign(true) {}

Long::~Long()
{
	clear();
}

Long::Long(const Long & other)
{
	operator=(other);
}

Long::Long(Long && other)
{
	operator=(other);
}

Long & Long::operator=(const Long & other)
{
	clear();
	a = other.a;
	sign = other.sign;
	return *this;
}

Long & Long::operator=(Long && other)
{
	clear();
	a = other.a;
	sign = other.sign;
	other.clear();
	return *this;
}

void Long::clear()
{
	sign = 0;
	a.clear();
}

const vector<ull>& Long::container() const {
	return a;
}

uint Long::size() const
{
	return (uint)a.size();
}

uint Long::real_size() const{
	int t = std::log10(a.back());
	return (uint)(a.size())*bs - (bs - ((a.back() != 0) ? t + 1 : 0));
}

ull Long::operator[](int i) const
{
	if (i >= static_cast<int>(size())) return 0;
	return a[i];
}

ull Long::get_char(uint i) const{
	auto f = [](ull num, int i) -> int {
		auto j = 0;
		while(num != 0) {
			++j;
			if (j == i) {
				return num % 10;
			}
			num = num / 10;
		}
		return -1;
	};
	int temp_i = i % bs;
	return f(a[i / bs], temp_i);
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
	if (n > 3)
		return insert(vector<ull>(n, (ull)0));
	while (n-- > 0) {
		a.emplace(a.begin(), (ull)0);
	}
	return *this;
}

Long Long::shiftaccurate(uint n)
{
	while (n > bs) {
		shift(1); 
		n -= bs;
	}
	while (n != 0) {
		a[0] *= 10;
		--n;
	}
	return normal();
}

Long Long::shiftback(uint n)
{
	while (n > bs) {
		shift(1); 
		n -= bs;
	}
	a.emplace(a.begin(), (ull)0);

	return normal();
}

Long Long::changeSign(int b)
{
	if (b == 1) sign = 1;
	else if (b == -1) sign = -1;
	else {
		sign = sign == 1 ? -1 : 1;
	}
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

// Constructors
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

Long::Long(const vector<ull>& x)
{
	operator=(x);
}

// Assignment operators
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
	ull e = 0; 
	uint size = 0;
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
		if (b < static_cast<int>(base)) {
			e += (v[i--] - '0') * b;
			b = b * 10;
		}
		else {
			insert(e);
			e = 0; 
			b = 1;
		}
	}
	if(e != 0)
		insert(e);

	return *this;
}

Long Long::operator=(const vector<ull>& x)
{
	clear();
	insert(x);
	sign = true;
	return *this;
}

// Arithmetic operators
Long Long::operator+(const Long & other) const
{
	return sum(other);
}

Long Long::operator-(const Long & other) const
{
	return neg(other);
}

Long Long::operator*(const Long & other) const
{
	if (size() == 0 || other.size() == 0)
		return null;
	else if (other == mone) {
		auto t = *this;
		return t.changeSign();
	}
	else if (other == null) {
		return null;
	}
	else if (other == pone) {
		auto t = *this;
		return t;
	}
	else if (size() == 1 && other.size() == 1) {
		return Long(a[0] * other[0]).changeSign(sign * other.sign);
	}
	else if (size() < karacnaive || other.size() < karacnaive)
		return mul(other);
	else if (size() < toom3naive || other.size() < toom3naive)
		return karac_mul(other);
	else
		return strassen_mul(other);
}

Long Long::operator/(const int other) const
{
	Long rem;
	return divide(other, rem);
}

Long Long::operator/(const Long & other) const {
	Long rem;
	return divide(other, rem);
}

Long Long::operator%(const Long & other) const {
	Long rem;
	divide(other, rem);
	return rem;
}

// Comparison operators
bool Long::operator==(const Long & other) const
{
	if (size() == 1 && other.size() == 1 && a[0] == 0 && other[0] == 0)
		return true;
	if (sign != other.sign) return false;
	else if (size() != other.size()) return false;
	for (int i = 0; i < static_cast<int>(size()); ++i) {
		if (a[i] != other[i]) return false;
	}
	return true;
}

bool Long::operator!=(const Long & other) const
{
	return !operator==(other);
}

bool Long::operator<(const Long & other) const
{
	if (sign < other.sign) return true;
	else if (size() < other.size()) return true;
	else if (size() == other.size()) {
		for (int i = size() - 1; i >= 0; --i) {
			if (a[i] > other[i]) return false;
			else if (a[i] < other[i]) return true;
		}
		return false;
	}
	else return false;
}

bool Long::operator>(const Long & other) const
{
	if (sign > other.sign) return true;
	else if (size() > other.size())
		return true;
	else if (size() == other.size()) {
		for (int i = size() - 1; i >= 0; --i) {
			if (a[i] < other[i]) return false;
			else if (a[i] > other[i]) return true;
		}
		return false;
	}
	else return false;
}

bool Long::operator<=(const Long & other) const
{
	if (sign < other.sign) return true;
	else if (size() < other.size()) return true;
	else if (size() == other.size()) {
		for (int i = size() - 1; i >= 0; --i) {
			if (a[i] > other[i]) return false;
			else if (a[i] < other[i]) return true;
		}
		return true;
	}
	else return false;
}

bool Long::operator>=(const Long & other) const
{
	if (sign > other.sign) return true;
	else if (size() > other.size())
		return true;
	else if (size() == other.size()) {
		for (int i = size() - 1; i >= 0; --i) {
			if (a[i] < other[i]) return false;
		else if (a[i] > other[i]) return true;
		}
		return true;
	}
	else return false;
}

// Arithmetic operations
Long Long::sum(const Long& b) const
{
	int fl = b.sign;
	if (fl != sign) {  // -a xor -b
		if (fl == -1) {     // a + (-b) == a - b
			auto l = b;
			return neg(l.changeSign());
		}
		else {         // -a + b == b - a
			auto l = *this;
			return b.neg(l.changeSign());
		}
	}

	if (b.size() > size())  // this one is bigger or ==
		return b.sum(*this);

	Long c = *this;
	c.a.resize(c.size() + 1);
	for (uint i = 0; i < size(); ++i) {
		c.a[i] += b[i];
	}

	return c.normal().changeSign(fl);
}

Long Long::neg(const Long & b) const
{
	if (b.sign == -1 ||                       //  a - (-b) == a + b
		(sign == -1 && b.sign == 1)) {    //  -a - b == -a + (-b)
		auto l = b;
		return sum(l.changeSign());
	}

	if (b.operator>(*this)) {
		return (b.neg(*this)).changeSign(false);
	}

	Long c = *this;
	bool tmp = 0;

	for (uint i = 0; i < size(); ++i) {
		if (tmp) {
			if (c.a[i] > 0) {
				c.a[i] = c.a[i] - 1;
				tmp = false;
			}
			else {
				c.a[i] = base - 1;
				tmp = true;
			}
		}

		if (c.a[i] >= b[i])
			c.a[i] = (c.a[i] - b[i]);
		else if (i + 1 < size()) {
			c.a[i] = (base + c.a[i] - b[i]);
			tmp = true;
		}
		else {
#ifdef DEBUG
			std::cout << *this << " >= " << b << "\n";
			std::cout << c.a[i] << " >= " << b[i] << "\n";
#endif // DEBUG

			throw std::logic_error("bye from minus ver2");
		}
	}

	return c.normal();
}

// Binary search template for division
template <class T>
T binsearch(const Long& a, const Long& b, Long& pr, const T& _l, const T& _r);

Long Long::mul(const Long& b) const
{
	int fl = (sign * b.sign);
	Long tmp;
	int len = size() + b.size() - 1;
	if (len <= 0) return null;

	tmp.insert(vector<ull>(len, 0));
	for (uint i = 0; i < size(); ++i)
		for (uint j = 0; j < b.size(); ++j)
			tmp.a[i + j] += a[i] * b[j];

	return tmp.normal().changeSign(fl);
}

Long Long::divide(const int b, Long& rem) const
{
	if (b == 0 || size() == 0)
		throw std::runtime_error("Division by zero!");

	auto t = b;
	ull c = 0;
	int fl = (sign);
	if (b < 0) {
		fl = (fl * 0);
		t = ::abs(b);
	}

	Long r = *this;
	for (int i = size() - 1; i >= 0; i--) {
		c = a[i] + c * base;
		r.set(i, c / t);
		c = c % t;
	}
	rem = c;
	return r.normal().changeSign(fl);
}

Long Long::divide(const Long & b, Long& rem) const
{
	if (size() == 0 || b.size() == 0 || b == 0)
		throw std::runtime_error("Division by zero!");
	else if ((size() == 1 && a[0] == 0))
		return 0;
	else if (size() < b.size() || *this < b) {
		rem = *this;
		return null;
	}
	else if (*this == b)
		return pone;
	else if (b.size() == 1) 
		return divide((int)b[0], rem);

	auto t = b;
	int fl = (sign);
	if (t < 0) {
		fl = (fl * -1);
		t.changeSign();
	}

	Long r(vector<ull>(size(), 0)); 
	Long c;
	for (int i = size() - 1; i >= 0; i--) {
		c.shift(); 
		c.set(0, a[i]);
		ull tmp = 0;
		if (c.size() >= b.size() && (c >= b)) {
			if (c == b) {
				c.clear();
				r.set(i, (ull)1);
			}
			else {
				auto pr = null;
				auto mid = binsearch<ull>(c, b, pr, 0, base);
				c = c - pr;
				if (c == null) 
					c.clear();

				r.set(i, mid);
			}
		}
		else
			r.set(i, tmp);
	}
	rem = c.normal();
	return r.normal().changeSign(fl);
}

template <class T>
T binsearch(const Long& a, const Long& b, Long& pr, const T& _l, const T& _r) {
	auto l = _l, r = _r;
	auto mid = (r + l) / 2;
	auto mid_pr = mid;
	do {
		pr = b * (mid);
		mid_pr = mid;
		if (pr < a) 
			l = mid;
		else if (pr > a) 
			r = mid;
		else
			break;

		mid = (r + l) / 2;
	} while (mid != mid_pr);

	return mid;
}

Long Long::divide2(const Long & b, Long& rem) const
{
	if (size() == 0 || b.size() == 0 || b == 0)
		throw std::runtime_error("Division by zero!");
	else if ((size() == 1 && a[0] == 0))
		return 0;
	else if (size() < b.size() || *this < b) {
		rem = *this;
		return null;
	}
	else if (*this == b)
		return pone;
	else if (b.size() == 1)
		return divide((int)b[0], rem);

	auto t = b;
	int fl = (sign);
	if (t < 0) {
		fl = (fl * -1);
		t.changeSign();
	}

	Long r, c;
	r = binsearch(*this, b, c, null, *this);
	c = *this - r;
	if (c == null)
		c.clear();

	rem = c.normal();
	return r.normal().changeSign(fl);
}

Long Long::mult_inv(const Long& b) const
{
	Long tmp;
	int len = size() + b.size() - 1;
	if (len <= 0) return null;

	auto _b = inverse(b);
	int i = 0;
	while (size() * bs - i> 5 && b.size() * bs - i> 5) {
		i += bs;
	}
	return to_Long(_b * *this);
}

Long Long::karac_mul(const Long & b) const
{
	if ((size() < karacnaive && b.size() < karacnaive)) {
		return mul(b);
	}
	auto k = std::max(size(), b.size()) / 2;
	if (k * 2 < std::max(size(), b.size()))
		k += 1;

	if (k == 0)
		return mul(b);

	int fl = (sign * b.sign);

	Long xr(vector<ull>(k, (ull)0));
	Long xl(vector<ull>(k, (ull)0));
	Long yr(vector<ull>(k, (ull)0));
	Long yl(vector<ull>(k, (ull)0));

	static auto f = [](Long& wh, const Long& what, int k, int mode) {
		if (mode == 1)
			mode = k;
		for (uint i = mode; i < static_cast<uint>(k + mode) && i < static_cast<uint>(what.size()); ++i) {
			wh.a[i - mode] = what[i];
		}
		wh.normal();
	};
	
	f(xr, *this, k, 0);
	f(yr, b, k, 0);
	f(xl, *this, k, 1);
	f(yl, b, k, 1);

	Long xlyl = xl.karac_mul(yl);
	Long xryr = xr.karac_mul(yr);

	return (Long().insert(xlyl.a).insert(vector<ull>(2 * k, 0)) + xryr 
		+ ((xl + xr).karac_mul(yl + yr) - (xlyl + xryr)).insert(vector<ull>(k, 0))).changeSign(fl);
}

Real inverse(const Long& a, uint iterations) {
	auto rs = a.real_size() + 1;
	Real approx(Long((ull)std::pow(10, (rs > Long::bs) ? Long::bs - rs % Long::bs : rs)), rs);
	const Real two = Real(Long(2), 0);
	const Real th = Real(a, 0);
	
	for (uint i = 0, count = 0; i < iterations; ++i, ++count) {
		approx = approx * (two - approx * th);
		if (count == 6) {
			count = 0;
			approx.cut(rs + 5);
		}
	}
	return approx.cut();
}