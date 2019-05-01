#ifndef _MODIFIEDPHONGBRDF_H_
#define _MODIFIEDPHONGBRDF_H_

#include "BRDF.h"
#include "rgb.h"
#include "ray.h"
#include "shape.h"
#include "light.h"

class ModifiedPhongBRDF : public BRDF
{
	public:

	float exponent;

	ModifiedPhongBRDF(float _exponent);

	virtual ~ModifiedPhongBRDF();

	// helpers
	rgb brdf(const Ray& r, const HitRecord& record, const SampleLight& slight) const;

	virtual rgb value(const Ray& r, const HitRecord& record, const SampleLight& slight) const;
};

#endif
