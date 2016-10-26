#include "Long.h"
using namespace LONGNUM;

Long Long::toomcook_mul(const Long & b) const
{
	if (size() < toom3naive || b.size() < toom3naive)
		return karac_mul(b);

	const int k = 3;
	const int k2 = 5;

	auto ko = size();
	auto k_big = 0;
	Long big_part = null;

	if (size() > b.size()) {
		k_big = b.size();
		big_part = Long(vector<ull>({ a.begin() + k_big, a.end() }));
		int t = big_part.size();
		if (t <= k_big) {
			big_part = (big_part.insert(vector<ull>(k_big - t, 0)).toomcook_mul(b)).insert(vector<ull>(t, 0));
		}
		else {
			big_part = (big_part.toomcook_mul(b)).insert(vector<ull>(k_big, 0));
		}
		ko = k_big / 3;
	}
	else if (size() == b.size())
		ko = size() / 3;
	else
		return b.toomcook_mul(*this);

	if (ko == 0)
		return mul(b);

	bool fl = (bool)(sign * b.sign);

	Long U[k], V[k];

	U[0] = Long(vector<ull>({ a.begin(), a.begin() + ko }));
	U[1] = Long(vector<ull>({ a.begin() + ko, a.begin() + ko + ko }));
	if (k_big == 0)
		U[2] = Long(vector<ull>({ a.begin() + ko + ko, a.end() }));
	else
		U[2] = Long(vector<ull>({ a.begin() + ko + ko, a.begin() + ko + ko + ko }));

	V[0] = Long(vector<ull>({ b.a.begin() , b.a.begin() + ko }));
	V[1] = Long(vector<ull>({ b.a.begin() + ko , b.a.begin() + ko + ko }));
	V[2] = Long(vector<ull>({ b.a.begin() + ko + ko , b.a.end() }));

	/*
	p0  =   m0 + m2
	p(0)   =  m0
	p(1)   =  p0 + m1
	p(−1)  =  p0 − m1
	p(−2)  =  (p(−1) + m2) × 2 − m0
	p(∞)   =  m2
	*/
	Long P[k2 + 1], Q[k2 + 1]; // values of two polynoms in 5 points

	auto f = [](Long a[], Long b[]) {
		a[0] = b[0] + b[2];
		a[1] = b[0];					 // 0
		a[2] = a[0] + b[1];				 // 1 
		a[3] = a[0] - b[1];				 // -1 
		a[4] = (a[3] + b[2]) * 2 - b[0]; // -2
		a[5] = b[2];                     // inf
	};

	 //f(P, U);
	f(Q, V);

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

	return ((Res[4].insert(vector<ull>(ko + ko, 0)) + Res[2]).insert(vector<ull>(ko + ko, 0)) + Res[0]
		+ Res[1].insert(vector<ull>(ko, 0)) + Res[3].insert(vector<ull>(ko + ko + ko, 0)) + big_part).changeSign(fl);
}

