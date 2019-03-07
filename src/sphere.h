#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "shape.h"
#include "vector3.h"
#include "ray.h"
#include "rgb.h"
#include "material.h"

class Sphere : public Shape
{
	public:

	Vec3 center;
	float radius;
	rgb color;
	Material material;

	Sphere(const Vec3& _center, float _radius, const rgb& _color=rgb());
	Sphere(const Vec3& _center, float _radius, const Material& _material);
	virtual ~Sphere() {}

	// BBox boundingBox() const;

	bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time) const;	
	bool boundingBox(float time0, float time1, BBox& _box, float epsilon) const;
};

#endif
