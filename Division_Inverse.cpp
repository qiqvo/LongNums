#include "Long.h"

struct Real {
	Long a;
	static const uint eps;
	uint mantisa_place;
};

const uint Real::eps = 90;
Real Long::inverse() const {
	Real aprrox{ 1, 1 };
	for (uint i = 0; i < Real::eps; ++i) {
		approx = aprrox * (Long(2).insert() - (*this) * aprrox);
	}
}