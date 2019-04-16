#ifndef _BUMPRECORD_H_
#define _BUMPRECORD_H_

#include <glm/mat4x4.hpp>
#include "vector3.h"

class BumpRecord
{
	public:

	Vec3 p;		// p (intersection point) in local coordinates
	Vec3 normal;
	Vec3 dpdu;
	Vec3 dpdv;
	glm::mat4 N;	// invTransform

	BumpRecord() {}
	BumpRecord(const Vec3& _p, const Vec3& _normal, const Vec3& _dpdu, const Vec3& _dpdv,
			const glm::mat4& _N=glm::mat4(1.0f)) :
	p(_p), normal(_normal), dpdu(_dpdu), dpdv(_dpdv), N(_N) {}

	BumpRecord& operator=(const BumpRecord& rhs)
	{
		p	= rhs.p;
		normal 	= rhs.normal;
		dpdu 	= rhs.dpdu;
		dpdv 	= rhs.dpdv;
		N	= rhs.N;

		return *this;
	}
};

#endif
