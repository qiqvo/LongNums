#include "Long.h"

struct PrimeTest {
	static double not_prime;
	static double SolovStras_met(Long & a, const Long & p);
	static double Lehmann_met(Long & a, const Long & p);
};
struct Rab_MilTest : PrimeTest {
	static Long b;
	static Long m;
	static double RabinMiller_met(Long & a, const Long & p);
};

Long Rab_MilTest::b = null;
Long Rab_MilTest::m = null;
double PrimeTest::not_prime = 1.;

//calculates Jacobian(a/n) n>0 and n is odd
Long Jacobi(Long& a, Long& n) {
	if (a == null)
		return null; // (0/n) = 0

	Long ans = pone;
	if (a < null) {
		a = a * -1;    // (a/n) = (-a/n)*(-1/n)
		if (n % 4 == 3)
			ans = ans * -1; // (-1/n) = -1 if n = 3 ( mod 4 ) 
	}
	if (a == pone)
		return ans; // (1/n) = 1

	while (a > null) {
		if (a < null) {
			a = a * -1;    // (a/n) = (-a/n)*(-1/n)
			if (n % 4 == 3)
				ans = ans * -1;    // (-1/n) = -1 if n = 3 ( mod 4 )
		}
		while (iseven(a)) {
			a = a / 2;    // Property (iii)
			if (n % 8 == 3 || n % 8 == 5)
				ans = ans * -1;
		}
		swap(a, n);    // Property (iv)
		if (a % 4 == 3 && n % 4 == 3)
			ans = ans * -1; // Property (iv)
		a = a%n; // because (a/p) = (a%p / p ) and 
				 // a%pi = (a%n)%pi if n % pi = 0
		if (a > n / 2)
			a = a - n;
	}
	if (n == pone)
		return ans;
	return null;
}
double PrimeTest::SolovStras_met(Long & a, const Long & p) {
	if (gcd(a, p) > pone)
		not_prime *= -1;

	if (not_prime < 0)
		return -1;

	Long j = pow_mod(a, (p - 1) / 2, p);
	auto _p = p;
	Long Jac = (p + Jacobi(a, _p)) % p;
	if (Jac == null || j != Jac)
		not_prime = -1;
	else
		not_prime *= 0.5;

	return not_prime;
}
double PrimeTest::Lehmann_met(Long & a, const Long & p) {
	if (not_prime < 0)
		return not_prime = -1;

	a = pow_mod(a, (p - 1) / 2, p);
	return not_prime = (a == pone || a == p - 1) ? not_prime * 0.5 : -1;
}
double Rab_MilTest::RabinMiller_met(Long & a, const Long & p) {
	if (not_prime < 0)
		return 0.;

	Long j = null;
	Long z = pow_mod(a, m, p);
	if (z == pone || z == p - 1)
		return not_prime *= 0.5;
	do {
		if (j > null && z == pone)
			return not_prime = -1;
		j = j + 1;
		if (j < b) {
			if (z != p - 1)
				z = pow_mod(z, Long(2), p);
			else
				return not_prime *= 0.5;
		}
		else if (z != p - 1)
			return not_prime = -1;
	} while (j != b);

	return not_prime *= 0.5;
}

double prtest_general(const Long & p, ull iter, double (*_met)(Long& a, const Long& p)) {
	ull l = 0;
	//Long a[] = { pone , pone }; // , pone, pone};
	Long a = pone;
	PrimeTest::not_prime = 1.;
	iter = iter;
	//std::thread thr1, thr2; //, thr3, thr4;
	while (PrimeTest::not_prime > 0. && l++ < iter) {
		//for (auto& i : a)
		//	i = rand(p.size(), p,
		//		[](const auto& a, const auto& p)
		//{return a < p && a != null;}
		//);
		a = rand(p.size(), p,
			[](const auto& a, const auto& p)
		{return a < p && a != null;}
		);

		_met(a, (p));
		//thr1 = std::thread(_met, a[0], (p));
		//thr2 = std::thread(_met, a[1], (p));
		//thr3 = std::thread(_met, a[2], (p));
		//thr4 = std::thread(_met, a[3], (p));      
		//
		//if (thr1.joinable())
		//	thr1.join();
		//if (thr2.joinable())
			//thr2.join();
		//if (thr3.joinable())
		//	thr3.join();
		//if (thr4.joinable())
		//	thr4.join();
	}

	return PrimeTest::not_prime;
}

double prtest_SolovStras(const Long & p, ull iter)
{
	return prtest_general(p, iter, &(PrimeTest::SolovStras_met));
}
double prtest_Lehmann(const Long & p, ull iter)
{
	return prtest_general(p, iter, &(PrimeTest::Lehmann_met));
}
double prtest_RabinMiller(const Long & p, ull iter)
{
	// b == how many times 2 divides p - 1 
	Rab_MilTest::b = pow(Long(2), [](const Long& p) -> Long {
		Long counter = null;
		for (uint i = 0; i < p.size(); ++i) {
			auto t = p[i];
			while (t && t & 1 == 0) {
				t >>= 1; counter = counter + 1;
			}
			if (t != 0) break;
		}
		return counter;
	}(p - 1));
	// m : p = 1 + 2^b  * m
	Rab_MilTest::m = (p - 1) / Rab_MilTest::b;
	return prtest_general(p, iter, &(Rab_MilTest::RabinMiller_met));
}