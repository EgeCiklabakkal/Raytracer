#ifndef _DEFAULTBRDF_H_
#define _DEFAULTBRDF_H_

#include "BRDF.h"
#include "rgb.h"
#include "ray.h"
#include "shape.h"
#include "light.h"

class SimpleBRDF : public BRDF	// This class isn't really a BRDF, it is blinn-phong shading model
{
	public:

	SimpleBRDF();

	virtual ~SimpleBRDF();

	// helpers
	rgb diffuseColor(const HitRecord& record, const SampleLight& slight) const;
	rgb specularColor(const Ray& r, const HitRecord& record, const SampleLight& slight) const;

	virtual rgb value(const Ray& r, const HitRecord& record, const SampleLight& slight) const;
};

#endif
