#ifndef _MATERIAL_H_
#define _MATERIAL_H_

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
};

#endif
