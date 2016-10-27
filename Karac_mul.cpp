#include "Long.h"

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

	auto f1 = [k](Long& wh, const Long& what) {
		for (uint i = 0; i < k; ++i) {
			wh.a[i] = what[i];
		}
	};
	auto f2 = [k](Long& wh, const Long& what) {
		for (uint i = k; i < k + k; ++i) {
			wh.a[i - k] = what[i];
		}
	};
	auto f = [k](Long& wh, const Long& what, int mode) {
		if (mode == 1)
			mode = k;
		for (uint i = mode; i < k + mode; ++i) {
			wh.a[i - mode] = what[i];
		}
		wh.normal();
	};
	f(xr, *this, 0);
	f(yr, b, 0);
	f(xl, *this, 1);
	f(yl, b, 1);

	Long xlyl = xl.karac_mul(yl);
	Long xryr = xr.karac_mul(yr);
	// Long xxyy = (xl + xr).karac_mul(yl + yr) - (xlyl + xryr);

	return (Long().insert(xlyl.a).insert(vector<ull>(2 * k, 0)) + xryr 
		+ ((xl + xr).karac_mul(yl + yr) - (xlyl + xryr)).insert(vector<ull>(k, 0))).changeSign(fl);
}
