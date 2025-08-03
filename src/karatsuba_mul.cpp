#include "long.h"

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
	// Long xxyy = (xl + xr).karac_mul(yl + yr) - (xlyl + xryr);

	return (Long().insert(xlyl.a).insert(vector<ull>(2 * k, 0)) + xryr 
		+ ((xl + xr).karac_mul(yl + yr) - (xlyl + xryr)).insert(vector<ull>(k, 0))).changeSign(fl);
}
