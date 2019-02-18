#include "sphere.h"

Sphere::Sphere(const Vec3& _center, float _radius, const rgb& _color) :
center(_center), radius(_radius), color(_color) {}

Sphere::Sphere(const Vec3& _center, float _radius, const Material& _material) :
center(_center), radius(_radius), material(_material) {}

/*
	At² + Bt + C = 0
*/
bool Sphere::hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const
{
	Vec3 d  = r.direction();
	Vec3 o  = r.origin();
	Vec3 oc = o - center;

	double A = dot(d, d);
	double B = 2 * dot(d, oc);
	double C = dot(oc, oc) - radius*radius;

	double discriminant = B*B - 4*A*C;

	// first check to see if ray intercepts sphere
	if(discriminant > 0)
	{
		discriminant = sqrt(discriminant);
		double _t = (-B - discriminant) / (2*A);

		// now check for a valid interval
		if(_t < tmin)
			_t = (-B + discriminant) / (2*A);
		if(_t < tmin || _t > tmax)
			return false;

		// we have a valid hit
		record.t      = _t;
		record.normal = unitVector(o + _t*d - center);
		record.color  = color;
		record.material  = material;
		return true;
	}

	return false;
}

bool Sphere::shadowHit(const Ray& r, float tmin, float tmax, float time) const
{
	Vec3 d  = r.direction();
	Vec3 o  = r.origin();
	Vec3 oc = o - center;

	double A = dot(d, d);
	double B = 2 * dot(d, oc);
	double C = dot(oc, oc) - radius*radius;

	double discriminant = B*B - 4*A*C;

	// first check to see if ray intercepts sphere
	if(discriminant > 0)
	{
		discriminant = sqrt(discriminant);
		double _t = (-B - discriminant) / (2*A);

		// now check for a valid interval
		if(_t < tmin)
			_t = (-B + discriminant) / (2*A);
		if(_t < tmin || _t > tmax)
			return false;

		// we have a valid hit
		return true;
	}

	return false;
}

void Sphere::print() const
{
	std::cout << "center: " << center << " radius: " << radius << std::endl;
}
