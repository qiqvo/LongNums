#include "random/quad_con_gen.h"

long long QuadConGen::a = 67867966;
long long QuadConGen::c = 104395301;
long long QuadConGen::d = 92526;

QuadConGen::QuadConGen(long seed) : EvenlyGen(seed)
{
    _random = _seed;
    randomL();
}

long long QuadConGen::randomL()
{
    _random = (d * _random * _random +
        a*_random + c) % m;
    if (_random < 0) _random += m;
    return _random;
} 