#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vector3.h"
#include "BRDF.h"

class Material
{
	public:

	// Member variables
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	Vec3 mirror;
	Vec3 transparency;
	float phong_exponent;
	float refraction_index;
	float roughness;
	bool degamma;
	BRDF *brdf;
};

#endif
