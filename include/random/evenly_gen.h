#pragma once

#include "generator_general.h"

class EvenlyGen : public GeneratorGeneral {
public:
    EvenlyGen(long seed = 0);

    virtual double random();

protected:
    static long long m; // 3 ** 20
}; 