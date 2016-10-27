#include "Long.h"
#include <thread>

vector<int> rev;

struct fft {
	vector<ReIm> wlen_pw;
	void trans(vector<ReIm>& a, bool inv);
	static void calc_rev(int n);
};

Long Long::strassen_mul(const Long & b) const
{
	//if (size() < 10 || b.size() < 10)
		//return operator*(b);
	vector<ReIm> fa(a.begin(), a.end()), fb(b.a.begin(), b.a.end());
	uint n = 1;
	bool flag = (bool)(sign * b.sign);

	auto nmax = std::max((uint)a.size(), b.size());
	while (n < nmax)  n <<= 1;
	n <<= 1;
	fa.resize(n), fb.resize(n);
	fft da, db, dc;
	fft::calc_rev(n);

	std::thread da_tr(&fft::trans, da, std::ref(fa), false);
	std::thread db_tr(&fft::trans, db, std::ref(fb), false);
												
	if (da_tr.joinable())
		da_tr.join();
	if (db_tr.joinable())
		db_tr.join();

	for (uint i = 0; i < n; ++i)
		fa[i] *= fb[i];

	dc.trans(fa, true);

	Long c(vector<ull>(n, 0));
	for (uint i = 0; i < n; ++i)
		c.set(i, ull(fa[i].real() + 0.5));

	return c.normal().changeSign(flag);
}


void fft::trans(vector<ReIm>& a, bool inv)
{
	uint n = a.size();

	for (uint i = 0; i < n; ++i)
		if (i < rev[i])
			swap(a[i], a[rev[i]]);

	for (uint len = 2; len <= n; len <<= 1) {
		double ang = 2 * PI / len * (inv ? -1 : 1);
		uint len2 = len >> 1;

		ReIm wlen(cos(ang), sin(ang));
		wlen_pw = vector<ReIm>(len2);
		wlen_pw[0] = ReIm(1, 0);
		for (uint i = 1; i < len2; ++i)
			wlen_pw[i] = wlen_pw[i - 1] * wlen;

		for (uint i = 0; i < n; i += len) {
			ReIm t;
			auto
				it_u = a.begin() + i,
				it_v = a.begin() + i + len2,
				it_u_end = a.begin() + i + len2,
				it_w = wlen_pw.begin();

			for (; it_u != it_u_end; ++it_u, ++it_v, ++it_w) {
				t = *it_v * *it_w;
				*it_v = *it_u - t;
				*it_u += t;
			}
		}
	}
	if (inv)
		for (uint i = 0; i < n; ++i)
			a[i] /= n;
}

void fft::calc_rev(int n) {
	rev = vector<int>(n);
	uint log_n = 0;
	while ((1 << log_n) < n)  ++log_n;

	for (uint i = 0; i < n; ++i) {
		rev[i] = 0;
		for (uint j = 0; j < log_n; ++j)
			if (i & (1 << j))
				rev[i] |= 1 << (log_n - 1 - j);
	}
}