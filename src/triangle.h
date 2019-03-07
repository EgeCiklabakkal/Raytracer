#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "shape.h"
#include "vector3.h"
#include "rgb.h"
#include "ray.h"
#include "material.h"

class Triangle : public Shape
{
	public:
	
	// Member variables
	Vec3 vertices[3];	// are in ccw order
	Vec3 normal;
	Material material;
	rgb color;

	// Constructors
	Triangle(const Vec3& _p0, const Vec3& _p1, const Vec3& _p2, const rgb& _color=rgb());
	Triangle(const Vec3& _p0, const Vec3& _p1, const Vec3& _p2,
			 const Vec3& _normal, const Material& _material);

	// Destructor
	~Triangle() {}
	
	// Virtual Methods
	bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time) const;
	bool boundingBox(float time0, float time1, BBox& _box, float epsilon) const;

	// Getters
	Vec3 p0() { return vertices[0]; }
	Vec3 p1() { return vertices[1]; }
	Vec3 p2() { return vertices[2]; }

	// Methods
};

#endif
