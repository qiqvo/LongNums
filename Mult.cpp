#include "Long.h"
using namespace LONGNUM;

ull binsearch(const Long& a, const Long& b, Long& pr, ull l, ull r);

Long Long::mul(const Long& b) const
{
	bool fl = (bool)(sign * b.sign);
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
	bool fl = (bool)(sign);
	if (t < 0) {
		fl = (bool)(fl * false);
		t.changeSign();
	}

	Long r(vector<ull>(size(), 0)); Long c;
	ull tmp = 0;
	for (int i = size() - 1; i >= 0; i--) {
		c.shift(); c.set(0, a[i]);
		tmp = 0;
		if (c.size() >= b.size() && (c >= b)) {
			if (c == b) {
				c.clear();
				r.set(i, (ull)1);
			}
			else {
				auto pr = null;
				auto mid = binsearch(c, b, pr, 0, base);
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

ull binsearch(const Long& a, const Long& b, Long& pr, ull l, ull r) {
	auto mid = (r + l) / 2;
	auto _l = l, _r = r, mid_pr = mid;
	/*
	bool *I_was = new bool[mid * 2];
	int ii = 0;
	while (ii < mid * 2) {
		I_was[ii++] = false;
	}
	*/
	do {
		pr = b.mul(mid);
		mid_pr = mid;
		/*
		if (I_was[mid])
			break;
		else
			I_was[mid] = true;
		*/
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