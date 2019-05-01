#ifndef _BLINNPHONGBRDF_H_
#define _BLINNPHONGBRDF_H_

#include "BRDF.h"
#include "rgb.h"
#include "ray.h"
#include "shape.h"
#include "light.h"

class BlinnPhongBRDF : public BRDF
{
	public:

	float exponent;

	BlinnPhongBRDF(float _exponent);

	virtual ~BlinnPhongBRDF();

	// helpers
	rgb brdf(const Ray& r, const HitRecord& record, const SampleLight& slight) const;

	virtual rgb value(const Ray& r, const HitRecord& record, const SampleLight& slight) const;
};

#endif
