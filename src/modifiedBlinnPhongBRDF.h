#ifndef _MODIFIEDBLINNPHONGBRDF_H_
#define _MODIFIEDBLINNPHONGBRDF_H_

#include "BRDF.h"
#include "rgb.h"
#include "ray.h"
#include "shape.h"
#include "light.h"
#include "rtmath.h"

class ModifiedBlinnPhongBRDF : public BRDF
{
	public:

	float exponent;
	bool normalized;

	ModifiedBlinnPhongBRDF(float _exponent, bool _normalized=false);

	virtual ~ModifiedBlinnPhongBRDF();

	// helpers
	rgb brdf(const Ray& r, const HitRecord& record, const SampleLight& slight) const;

	virtual rgb value(const Ray& r, const HitRecord& record, const SampleLight& slight) const;
};

#endif
