#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "ray.h"
#include "vector3.h"
#include "rgb.h"
#include "material.h"
#include "BBox.h"

class Ray;
class rgb;
class BBox;

class HitRecord
{
	public:

	float t;
	Vec3 normal;
	rgb color;
	Material material;

	// Constructors
	HitRecord() {}
	HitRecord(float _t, const Vec3& _normal, const rgb& _color=rgb()) :
	t(_t), normal(_normal), color(_color) {}
	HitRecord(float _t, const Vec3& _normal, const Material& _material) :
	t(_t), normal(_normal), material(_material) {}
	HitRecord(const HitRecord& rhs) { *this = rhs; }


	HitRecord& operator=(const HitRecord& rhs)
	{
		t      = rhs.t;
		normal = rhs.normal;
		color  = rhs.color;
		material = rhs.material;

		return *this;
	}
};

class Shape
{
	public:

	virtual ~Shape() = 0;

	virtual bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const=0;
	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const=0;
	virtual bool boundingBox(float time0, float time1, BBox& _box) const=0;
};

inline Shape::~Shape() {}

#endif
