#include "core/long.h"
#include <ctime>
#include <thread>

#include "random/random.h"

 
typedef QuadConGen RandGen;
RandGen rg((ull)time(NULL));

Long rand(const uint s)
{
    uint count = 0, sr = rg.randomL() % s;
    vector<ull> c(sr + 1);
    do {
        c[count++] = rg.randomL() % Long::base;
    } while (count < sr + 1);
    return Long(c);
}
Long rand(const uint s, const Long &c, bool(*condition)(const Long &a, const Long &b))
{
    Long a = null;
    do {
        a = rand(s);
    } while (!condition(a, c));
    return a;
}
Long abs(const Long & a)
{
    auto t = a;
    return t.changeSign();
}
Long factorial(const Long & N)
{
    auto n = N;
    auto ns = n - 1;
    	for (; ns > 0; ns = ns - 1) {
        n = n * ns;
    }
    return n;
}

Long pow_mod(const Long& x, const Long& y, const Long& N) {
    Long s = pone, t = x, u = y;
    while (u != null) {
        if (isodd(u))
            s = (s * (t)) % N;
        u = u / 2;
        t = (t * (t)) % N;
    }
    return s;
}
Long pow(const Long & a, const Long & n) {
    Long res = pone;
    auto _n = n, _a = a;
    while (_n != 0) {
        if (isodd(_n)) {
            res = res * (_a);
        }
        _a = _a * (_a);
        _n = _n / 2;
    }
    return res;
}
Long gcd(const Long& u, const Long& v)
{
    /* GCD(0,v) == v; GCD(u,0) == u, GCD(0,0) == 0 */
    if (u == v) return u;
    if (u == null) return v;
    if (v == null) return u;

    auto u_isodd = isodd(u),
        v_isodd = isodd(v);

    if (!u_isodd) // u is even
    {
        if (v_isodd) // v is odd
            return gcd(u / 2, v);
        else // both u and v are even
            return gcd(u / 2, v / 2) * 2;
    }
    else if (!v_isodd) // u is odd, v is even
        return gcd(u, v / 2);
    // reduce larger argument
    else if (u >= v)
        return gcd((u - v) / 2, v);
    else
        return gcd((v - u) / 2, u);
}
