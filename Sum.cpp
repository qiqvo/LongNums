#include "Long.h"
 
 

Long Long::sum(const Long& b) const
{
	bool fl = b.sign;
	if (fl != sign) {  // -a xor -b
		if (!fl) {     // a + (-b) == a - b
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
	if (b.sign == false ||                       //  a - (-b) == a + b
		(sign == false && b.sign == true)) {    //  -a - b == -a + (-b)
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
		//return null;
	}

	return c.normal();
}
