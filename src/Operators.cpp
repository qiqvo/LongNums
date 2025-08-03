#include "long.h"

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

