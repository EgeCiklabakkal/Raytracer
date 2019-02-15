#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "ray.h"
#include "vector3.h"
#include "rgb.h"

class Ray;
class rgb;

class HitRecord
{
	public:

	float t;
	Vec3 normal;
	rgb color;

	HitRecord() {}
	HitRecord(float _t, const Vec3& _normal, const rgb& _color=rgb()) :
	t(_t), normal(_normal), color(_color) {}

	HitRecord& operator=(const HitRecord& rhs)
	{
		t      = rhs.t;
		normal = rhs.normal;
		color  = rhs.color;

		return *this;
	}
};

class Shape
{
	public:

	virtual bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const=0;
	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const=0;
};

#endif
