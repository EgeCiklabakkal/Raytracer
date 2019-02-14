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
	float phong_exponent;
};

#endif
