#include "Long.h"
 
 

Long Long::toomcook_mul(const Long & b) const
{
	if (size() < toom3naive || b.size() < toom3naive)
		return karac_mul(b);

	const int k = 3;
	const int k2 = 5;

	auto km = std::max(size(), b.size());

	bool fl = (bool)(sign * b.sign);

	Long U[k];
    Long V[k];

	auto f = [km](Long& wh, const Long& what, int mode) {
		if (mode == 1)
		    mode = km;
		if( mode == 2)
		    mode = km + km;
		for (uint i = mode; i < km + mode; ++i) {
			wh.a[i] = what[i];
		}
	};
	for (auto i = 0; i < k; ++i){
		U[i] = Long(vector<ull>(km, (ull)0));
		f(U[i], *this, i);
	}
	for (auto i = 0; i < k; ++i){
		V[i] = Long(vector<ull>(km, (ull)0));
		f(V[i], b, i);
	}
	
	/*
	p0  =   m0 + m2
	p(0)   =  m0
	p(1)   =  p0 + m1
	p(−1)  =  p0 − m1
	p(−2)  =  (p(−1) + m2) × 2 − m0
	p(∞)   =  m2
	*/
	Long P[k2 + 1], Q[k2 + 1]; // values of two polynoms in 5 points

	auto g = [](Long a[], Long b[]) {
		a[0] = b[0] + b[2];
		a[1] = b[0];					 // 0
		a[2] = a[0] + b[1];				 // 1 
		a[3] = a[0] - b[1];				 // -1 
		a[4] = (a[3] + b[2]) * 2 - b[0]; // -2
		a[5] = b[2];                     // inf
	};

    g(P, U);
	g(Q, V);

	Long R[k2]; // values of resulting polynom in 5 points

	R[0] = P[1].toomcook_mul(Q[1]); //   0
	R[1] = P[2].toomcook_mul(Q[2]); //   1 
	R[2] = P[3].toomcook_mul(Q[3]); //  -1 
	R[3] = P[4].toomcook_mul(Q[4]); //  -2
	R[4] = P[5].toomcook_mul(Q[5]); // inf

	Long Res[k2];

	Res[0] = R[0];
	Res[4] = R[4];
	Res[3] = (R[3] - R[1]) / 3;
	Res[1] = (R[1] - R[2]) / 2;
	Res[2] = R[2] - R[0];
	Res[3] = (Res[2] - Res[3]) / 2 + R[4] * 2;
	Res[2] = Res[2] + Res[1] - Res[4];
	Res[1] = Res[1] - Res[3];

	return ((Res[4].insert(vector<ull>(km + km, 0)) + Res[2]).insert(vector<ull>(km + km, 0)) + Res[0]
		+ Res[1].insert(vector<ull>(km, 0)) + Res[3].insert(vector<ull>(km + km + km, 0))).changeSign(fl);
}

