#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "shape.h"
#include "vector3.h"
#include "rgb.h"
#include "ray.h"

class Triangle : public Shape
{
	public:
	
	// Member variables
	Vec3 vertices[3];	// are in ccw order

	// Constructors
	Triangle(const Vec3& _p0, const Vec3& _p1, const Vec3& _p2, const rgb& _color=rgb());
	
	// Virtual Methods
	bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time) const;

	// Getters
	Vec3 p0() { return vertices[0]; }
	Vec3 p1() { return vertices[1]; }
	Vec3 p2() { return vertices[2]; }

	rgb color;
};

#endif
