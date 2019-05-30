#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "ray.h"
#include "ONB.h"
#include "vector2.h"
#include "vector3.h"
#include "rgb.h"
#include "material.h"
#include "texture.h"
#include "bumpRecord.h"
#include "BBox.h"
#include "rtmath.h"
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>         // perspective, translate, rotate

class Ray;
class rgb;
class BBox;

class HitRecord
{
	public:

	float t;	// Ray hits at p = Ray.origin() t*Ray.direction
	Vec3 p;		// point of intersection
	Vec2 uv;	// uv coordinates of texture
	Vec3 normal;
	ONB uvw;	// w is the outward normal
	rgb color;
	Material material;
	Texture *texture = nullptr;
	BumpRecord bump;
	float time;

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
		uv	 = rhs.uv;
		normal 	 = rhs.normal;
		color 	 = rhs.color;
		uvw 	 = rhs.uvw;
		material = rhs.material;
		time     = rhs.time;
		texture  = rhs.texture;
		bump     = rhs.bump;

		return *this;
	}
};

class Shape
{
	public:

	bool transformed   = false;	// Does the object have any transforms applied to it
	bool motionBlurred = false;	// Does the object have motion blur
	Vec3 velocity;			// Velocity vector causing the motion blur
	glm::mat4 M = glm::mat4(1.0f);	// Transformation matrix
	glm::mat4 N = glm::mat4(1.0f);	// Inv. Transformation matrix

	virtual ~Shape() = 0;

	virtual bool hit(const Ray& r, float tmin, float tmax,
				float time, HitRecord& record, bool nonluminous=false) const=0;
	virtual bool shadowHit(const Ray& r, float tmin, float tmax,
				float time, bool nonluminous=false) const=0;
	virtual bool boundingBox(float time0, float time1, BBox& _box) const=0;

	// Transform Methods
	Ray transformRayToLocal(const Ray& r) const;
	HitRecord transformRecordToWorld(const HitRecord& record) const;
	BBox transformBBoxToWorld(const BBox& bbox) const;
	bool setMotionBlur(const Vec3& _velocity);
	bool setTransform(const glm::mat4& transformMatrix, bool resetTransform=true);
};

inline Shape::~Shape() {}

#endif
