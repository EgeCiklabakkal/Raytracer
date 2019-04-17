#ifndef _BUMPRECORD_H_
#define _BUMPRECORD_H_

#include <glm/mat4x4.hpp>
#include "vector3.h"

class BumpRecord
{
	public:

	Vec3 dpdu;
	Vec3 dpdv;
	glm::mat4 M;	// Transform

	BumpRecord() {}
	BumpRecord(const Vec3& _dpdu, const Vec3& _dpdv, const glm::mat4& _M=glm::mat4(1.0f)) :
	dpdu(_dpdu), dpdv(_dpdv), M(_M) {}

	BumpRecord& operator=(const BumpRecord& rhs)
	{
		dpdu 	= rhs.dpdu;
		dpdv 	= rhs.dpdv;
		M	= rhs.M;

		return *this;
	}
};

#endif
