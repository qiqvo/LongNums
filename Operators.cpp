#include "Long.h"
using namespace LONGNUM;

Long Long::operator+(const Long & o) const
{
	return sum(o);
}
Long Long::operator-(const Long & o) const
{
	return neg(o);
}
Long Long::operator*(const Long & o) const
{
	if (size() == 0 || o.size() == 0)
		return null;
	else if (o == mone) {
		auto t = *this;
		return t.changeSign();
	}
	else if (o == null) {
		return null;
	}
	else if (o == pone) {
		auto t = *this;
		return t;
	}
	else if (size() == 1 && o.size() == 1) {
		return Long(a[0] * o[0]).changeSign(sign * o.sign);
	}
	else if (size() < karacnaive || o.size() < karacnaive)
		return mul(o);
	else if (size() < toom3naive || o.size() < toom3naive)
		return karac_mul(o);
	else
		return toomcook_mul(o);
}
Long Long::operator/(const int o) const
{
	Long rem;
	return divide(o, rem);
}

Long Long::operator/(const Long & o) const {
	Long rem;
	return divide(o, rem);
}

Long Long::operator%(const Long & o) const {
	Long rem;
	divide(o, rem);
	return rem;
}

