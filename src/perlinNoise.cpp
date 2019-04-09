#include "perlinNoise.h"
#include "rtmath.h"

PerlinNoise::PerlinNoise()
{
	int i;

	G[0] = Vec3( 1, 1, 0);
	G[1] = Vec3(-1, 1, 0);
	G[2] = Vec3( 1,-1, 0);
	G[3] = Vec3(-1,-1, 0);

	G[4] = Vec3( 1, 0, 1);
	G[5] = Vec3(-1, 0, 1);
	G[6] = Vec3( 1, 0,-1);
	G[7] = Vec3(-1, 0,-1);

	G[8] = Vec3( 0, 1, 1);
	G[9] = Vec3( 0,-1, 1);
	G[10]= Vec3( 0, 1,-1);
	G[11]= Vec3( 0,-1,-1);

	G[12]= Vec3( 1, 1, 0);
	G[13]= Vec3(-1, 1, 0);
	G[14]= Vec3( 0,-1, 1);
	G[15]= Vec3( 0,-1,-1);

	// fill phi
	for(i = 0; i < 16; i++)
	{
		P[i] = i;
	}

	// shuffle phi
	for(i = 14; i >= 0; i--)
	{
		int target = int(rtmath::randf() * i);
		int temp   = P[i+1];
		P[i+1]     = P[target];
		P[target]  = temp;
	}
}

float PerlinNoise::turbulance(const Vec3& p, int depth) const
{
	float sum = 0.0f;
	float weight = 1.0f;
	Vec3 ptemp(p);

	sum = fabs(noise(ptemp));

	for(int i = 1; i < depth; i++)
	{
		weight = weight * 2;
		ptemp.setX(p.x() * weight);
		ptemp.setY(p.y() * weight);
		ptemp.setZ(p.z() * weight);

		sum += fabs(noise(ptemp)) / weight;
	}

	return sum;
}

float PerlinNoise::dturbulance(const Vec3& p, int depth, float d) const
{
	float sum = 0.0f;
	float weight = 1.0f;
	Vec3 ptemp(p);

	sum = fabs(noise(ptemp)) / d;

	for(int i = 1; i < depth; i++)
	{
		weight = weight * d;
		ptemp.setX(p.x() * weight);
		ptemp.setY(p.y() * weight);
		ptemp.setZ(p.z() * weight);

		sum += fabs(noise(ptemp)) / d;
	}

	return sum;
}

float PerlinNoise::noise(const Vec3& p) const
{
	int    i,  j,  k;
	float fu, fv, fw, sum;
	Vec3 v;

	// Lattice corners
	i = (int)floor(p.x());
	j = (int)floor(p.y());
	k = (int)floor(p.z());
	
	// vector to point
	fu = p.x() - float(i);
	fv = p.y() - float(j);
	fw = p.z() - float(k);
	sum = 0.0f;

	v    = Vec3(fu, fv, fw);
	sum += knot( i,  j,  k, v);

	v    = Vec3(fu - 1, fv, fw);
	sum += knot( i + 1,  j,  k, v);

	v    = Vec3(fu, fv - 1, fw);
	sum += knot( i,  j + 1,  k, v);

	v    = Vec3(fu, fv, fw - 1);
	sum += knot( i,  j,  k + 1, v);

	v    = Vec3(fu - 1, fv - 1, fw);
	sum += knot( i + 1,  j + 1,  k, v);

	v    = Vec3(fu - 1, fv, fw - 1);
	sum += knot( i + 1,  j,  k + 1, v);

	v    = Vec3(fu, fv - 1, fw - 1);
	sum += knot( i,  j + 1,  k + 1, v);

	v    = Vec3(fu - 1, fv - 1, fw - 1);
	sum += knot( i + 1,  j + 1,  k + 1, v);

	return sum;
}
