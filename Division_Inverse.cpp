#include "Long.h"
#include "Real.h"

const uint eps = 8;

Real Long::inverse() const {
	int bs = std::log10(base);
	auto v = vector<ull>(size(), (ull)0);
	v.emplace(v.begin(), (ull)1);
	Real approx(Long(v), v.size());
	const Real two = Real(Long(2), 0);
	const Real th = Real(*this, 0);
	for (uint i = 0; i < eps; ++i) { 
		approx = approx * (two - approx * th);
		std::cout << i << "   " << approx << std::endl;
	}
	return approx;
}