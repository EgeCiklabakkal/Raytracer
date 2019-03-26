#include "sphere.h"

Sphere::Sphere(const Vec3& _center, float _radius, const rgb& _color) :
center(_center), radius(_radius), color(_color) 
{
	transformed = false;
}

Sphere::Sphere(const Vec3& _center, float _radius, const Material& _material) :
center(_center), radius(_radius), material(_material) 
{
	transformed = false;
}

/*
	At² + Bt + C = 0
*/
bool Sphere::hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const
{
	Ray tray = transformRayToLocal(r);

	Vec3 d  = tray.direction();
	Vec3 o  = tray.origin();
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
		record.t 	 = _t;
		record.p	 = o + _t*d;
		record.normal 	 = unitVector(record.p - center);
		ONB _uvw;
		_uvw.initFromW(record.normal);
		record.uvw	 = _uvw;
		record.color  	 = color;
		record.material  = material;

		record = transformRecordToWorld(record);
		return true;
	}

	return false;
}

bool Sphere::shadowHit(const Ray& r, float tmin, float tmax, float time) const
{
	Ray tray = transformRayToLocal(r);

	Vec3 d  = tray.direction();
	Vec3 o  = tray.origin();
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

bool Sphere::boundingBox(float time0, float time1, BBox& _box) const
{
	Vec3 radius3(radius, radius, radius);

	_box = BBox(center - radius3, center + radius3);
	_box = transformBBoxToWorld(_box);
	return true;
}
