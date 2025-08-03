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

class EvenlyGen : public GeneratorGeneral {
public:
	EvenlyGen(long seed = 0);

	virtual double random();

protected:
	static long long m; // 3 ** 20

};

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