#ifndef _TORRANCESPARROWBRDF_H_
#define _TORRANCESPARROWBRDF_H_

#include "BRDF.h"
#include "rgb.h"
#include "ray.h"
#include "shape.h"
#include "light.h"
#include "macros.h"

class TorranceSparrowBRDF : public BRDF
{
	public:

	float exponent;
	float refractive_index;

	TorranceSparrowBRDF(float _exponent, float _refractive_index);

	virtual ~TorranceSparrowBRDF();

	// helpers
	float blinnDistribution(float cosalpha) const;
	float geometryTerm(const Vec3& wi, const Vec3& wo, const Vec3& n, const Vec3& h) const;
	float fresnelReflection(float cosbeta) const;
	rgb brdf(const Ray& r, const HitRecord& record, const SampleLight& slight) const;

	virtual rgb value(const Ray& r, const HitRecord& record, const SampleLight& slight) const;
};

#endif
