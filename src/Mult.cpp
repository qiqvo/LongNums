#include "long.h"
#include "real.h"
 
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
		throw "Division by zero!";

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
		throw "Division by zero!";
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

	Long r(vector<ull>(size(), 0)); Long c;
	// ull tmp = 0;  // Unused variable removed
	for (int i = size() - 1; i >= 0; i--) {
		c.shift(); c.set(0, a[i]);
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
		throw "Division by zero!";
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
	// ull tmp = 0;  // Unused variable removed
	r = binsearch(*this, b, c, null, *this);
	c = *this - r;
	if (c == null)
		c.clear();

	rem = c.normal();
	return r.normal().changeSign(fl);
}

Long Long::mult_inv(const Long& b) const
{
	// int fl = (sign * b.sign);  // Unused variable removed
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