#ifndef _RDMGEN_H_
#define _RDMGEN_H_

class rdmGen
{
	public:

	unsigned long long seed;
	unsigned long long mult;
	unsigned long long llong_max;
	float float_max;

	rdmGen(unsigned long long _seed=7564231ULL) : 
	seed(_seed), mult(62089911ULL), llong_max(4294967295ULL), float_max(4294967295ULL) {}

	float operator()();
};

inline float rdmGen::operator()()
{
	seed = mult * seed;
	return float(seed % llong_max) / float_max;
}

#endif
