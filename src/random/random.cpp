#include "random/random.h"

long long EvenlyGen::m = 3486784401;
long long QuadConGen::a = 67867966;
long long QuadConGen::c = 104395301;
long long QuadConGen::d = 92526;

GeneratorGeneral::GeneratorGeneral(long seed) :
	_seed(seed) { }
EvenlyGen::EvenlyGen(long seed) : GeneratorGeneral(seed)
{}
double EvenlyGen::random()
{
	return (randomL() + 0.0) / m;
}
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
