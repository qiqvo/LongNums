#pragma once

class GeneratorGeneral
{
public:
    GeneratorGeneral(long seed = 0);

protected:
    virtual long long randomL() = 0;
    virtual double random() = 0;

    long _seed;
    long long _random;
}; 