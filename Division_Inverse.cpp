#include "Long.h"
#include "Real.h"

Real inverse(const Long& a, uint iterations) {
	auto rs = a.real_size() + 1;
	Real approx(Long((ull)std::pow(10, (rs > Long::bs) ? Long::bs - rs % Long::bs : rs)), rs);
	const Real two = Real(Long(2), 0);
	const Real th = Real(a, 0);
	//std::cout << -1 << "   " << approx << std::endl;
	for (uint i = 0, count = 0; i < iterations; ++i, ++count) {
		approx = approx * (two - approx * th);
		if (count == 6) {
			count = 0;
			approx.cut(rs);
		}
	//	std::cout << i << "   " << approx << std::endl;
	}
	return approx.cut();
}
