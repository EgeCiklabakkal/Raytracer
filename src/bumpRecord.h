#ifndef _BUMPRECORD_H_
#define _BUMPRECORD_H_

#include <glm/mat4x4.hpp>
#include "vector3.h"

class BumpRecord
{
	public:

	Vec3 dpdu;
	Vec3 dpdv;

	BumpRecord() {}
	BumpRecord(const Vec3& _dpdu, const Vec3& _dpdv) :
	dpdu(_dpdu), dpdv(_dpdv) {}

	BumpRecord& operator=(const BumpRecord& rhs)
	{
		dpdu 	= rhs.dpdu;
		dpdv 	= rhs.dpdv;

		return *this;
	}
};

#endif
