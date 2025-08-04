#pragma once

#include "evenly_gen.h"

class QuadConGen : public EvenlyGen
{
    // seed = 1034
    static long long a;  // b = a-1 ; b%3 == 0; 
    static long long c;
    static long long d; // d % 6 == 0 
                        // m == 3**20
public:
    long long randomL();
    QuadConGen(long seed = 0);
}; 