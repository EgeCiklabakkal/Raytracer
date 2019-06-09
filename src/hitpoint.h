#ifndef _HITPOINT_H_
#define _HITPOINT_H_

#include "shape.h"

// Hitpoints from ray tracing pass of Photon Mapping
class HitPoint
{
	public:

	Vec3 p;		// hit location
	Vec3 normal;	// surface normal at p
	Ray ray;	// ray that hit the point
	BRDF *brdf;	// brdf
	float i, j;	// pixel location
	float wgt;	// weight
	float Rx;	// current photon radius
	float Nx;	// accumulated photon count
	float Mx;	// added photon count
	rgb tau;	// accumulated reflected flux

	HitPoint() : Rx(1.0f), Nx(0.0f), Mx(0.0f), tau(0.0f) {}
	HitPoint(const Vec3& _p, const Vec3& _normal, const Ray& _ray, BRDF* _brdf,
		float _i, float _j, float _wgt, float _Rx=1.0f, float _Nx=0.0f,
		float _Mx=0.0f, const rgb& _tau=rgb()) :
	p(_p), normal(_normal), ray(_ray), brdf(_brdf), i(_i), j(_j), wgt(_wgt), Rx(_Rx),
	Nx(_Nx), Mx(_Mx), tau(_tau) {}
};

#endif
