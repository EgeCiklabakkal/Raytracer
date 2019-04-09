#ifndef _PERLINNOISE_H_
#define _PERLINNOISE_H_

#include <math.h>
#include "vector3.h"

class PerlinNoise
{
	public:

	Vec3 G[16];
	int P[16];

	PerlinNoise();
	float noise(const Vec3& p) const;
	float turbulance(const Vec3& p, int depth) const;
	float dturbulance(const Vec3& p, int depth, float d) const;
	float omega(float t) const;
	Vec3 gamma(int i, int j, int k) const;
	int iGamma(int i, int j) const;
	float knot(int i, int j, int k, const Vec3& v) const;
};

inline float PerlinNoise::omega(float t) const
{
	t = (t > 0.0f) ? t : -t;
	return (-6.0f*t*t*t*t*t + 15.0f*t*t*t*t - 10.0f*t*t*t + 1.0f);
}

inline Vec3 PerlinNoise::gamma(int i, int j, int k) const
{
	int idx;
	idx = P[abs(k) % 16];
	idx = P[abs(j + idx) % 16];
	idx = P[abs(i + idx) % 16];

	return G[idx];
}

inline float PerlinNoise::knot(int i, int j, int k, const Vec3& v) const
{
	return (dot(gamma(i, j, k), v) * omega(v.x()) * omega(v.y()) * omega(v.z()));
}

inline int PerlinNoise::iGamma(int i, int j) const
{
	int idx;
	idx = P[abs(j) % 16];
	idx = P[abs(i + idx) % 16];

	return idx;
}

#endif
