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
};

class Shape
{
	public:

	virtual bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const=0;
	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const=0;
};

#endif
