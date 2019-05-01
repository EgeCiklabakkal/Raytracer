#ifndef _BRDF_H_
#define _BRDF_H_

#include "rgb.h"

class Ray;
class HitRecord;
class SampleLight;

class BRDF
{
	public:

	virtual ~BRDF() = 0;

	virtual rgb value(const Ray& r, const HitRecord& record, const SampleLight& slight) const=0;
};

inline BRDF::~BRDF() {}

#endif
