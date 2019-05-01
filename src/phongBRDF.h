#ifndef _PHONGBRDF_H_
#define _PHONGBRDF_H_

#include "BRDF.h"
#include "rgb.h"
#include "ray.h"
#include "shape.h"
#include "light.h"

class PhongBRDF : public BRDF
{
	public:

	float exponent;

	PhongBRDF(float _exponent);

	virtual ~PhongBRDF();

	// helpers
	rgb brdf(const Ray& r, const HitRecord& record, const SampleLight& slight) const;

	virtual rgb value(const Ray& r, const HitRecord& record, const SampleLight& slight) const;
};

#endif
