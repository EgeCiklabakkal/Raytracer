#include "vector3.h"

std::istream& operator>>(std::istream& is, Vec3& v)
{
	float temp;
	is >> temp;
	v.e[0] = temp;
	is >> temp;
	v.e[1] = temp;
	is >> temp;
	v.e[2] = temp;

	return is;
}

std::ostream& operator<<(std::ostream &os, const Vec3& v)
{
	os << '(' << v.e[0] << " " << v.e[1] << " " << v.e[2] << ')';
	return os;
}

float tripleProduct(const Vec3& v1, const Vec3& v2, const Vec3& v3)
{
	return dot((cross(v1, v2)), v3);
}
