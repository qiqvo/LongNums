#include "random/evenly_gen.h"

long long EvenlyGen::m = 3486784401;

EvenlyGen::EvenlyGen(long seed) : GeneratorGeneral(seed)
{}

double EvenlyGen::random()
{
    return (randomL() + 0.0) / m;
} 