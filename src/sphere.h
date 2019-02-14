#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "shape.h"
#include "vector3.h"
#include "ray.h"
#include "rgb.h"

class Sphere : public Shape
{
	public:

	Vec3 center;
	float radius;
	rgb color;

	Sphere(const Vec3& _center, float _radius, const rgb& _color=rgb());

	// BBox boundingBox() const;

	bool hit(const Ray& r, float tmin, float tmax, float time, hitRecord& record) const;	
	bool shadowHit(const Ray& r, float tmin, float tmax, float time) const;	
};

#endif
