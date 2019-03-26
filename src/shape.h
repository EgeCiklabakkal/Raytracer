#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "ray.h"
#include "ONB.h"
#include "vector3.h"
#include "rgb.h"
#include "material.h"
#include "BBox.h"
#include "rtmath.h"
#include <glm/mat4x4.hpp>

class Ray;
class rgb;
class BBox;

class HitRecord
{
	public:

	float t;	// Ray hits at p = Ray.origin() t*Ray.direction
	Vec3 p;		// point of intersection
	Vec3 normal;
	ONB uvw;	// w is the outward normal
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
		t 	 = rhs.t;
		p	 = rhs.p;
		normal 	 = rhs.normal;
		color 	 = rhs.color;
		uvw 	 = rhs.uvw;
		material = rhs.material;

		return *this;
	}
};

class Shape
{
	public:

	bool transformed;	// Does the object have any transforms applied to it
	Vec3 motionBlur;	// 0 vector has no motion blur
	glm::mat4 M;		// Transformation matrix
	glm::mat4 N;		// Inv. Transformation matrix

	virtual ~Shape() = 0;

	virtual bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const=0;
	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const=0;
	virtual bool boundingBox(float time0, float time1, BBox& _box) const=0;

	// Transform Methods
	Ray transformRayToLocal(const Ray& r) const;
	HitRecord transformRecordToWorld(const HitRecord& record) const;
	BBox transformBBoxToWorld(const BBox& bbox) const;
};

inline Shape::~Shape() {}

#endif
