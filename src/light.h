#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "shape.h"
#include "ONB.h"
#include "rtmath.h"

class Ray;
class HitRecord;

class SampleLight
{
	public:

	Vec3 position;
	Vec3 intensity;
	Vec3 relativeTo;

	SampleLight() {}
	SampleLight(Vec3 _position, Vec3 _intensity, Vec3 _relativeTo=Vec3()) :
	position(_position), intensity(_intensity), relativeTo(_relativeTo) {}
};

class Light
{
	public:

	virtual ~Light() {};

	virtual SampleLight sampleLight(const Ray& r, const HitRecord& record) const=0;
};

class PointLight : public Light
{
	public:
	
	Vec3 position;
	Vec3 intensity;

	PointLight() {}
	PointLight(Vec3 _position, Vec3 _intensity) : position(_position), intensity(_intensity) {}
	virtual ~PointLight() {}
	PointLight(const PointLight& pl) { *this = pl; }

	virtual SampleLight sampleLight(const Ray& r, const HitRecord& record) const
	{ 
		return SampleLight(position, intensity);
	}
};

class AmbientLight : public Light
{
	public:

	Vec3 intensity;

	virtual SampleLight sampleLight(const Ray& r, const HitRecord& record) const
	{ return SampleLight(); } // No use
};

class AreaLight : public Light
{
	public:

	Vec3 position;
	Vec3 intensity;
	float size;
	Vec3 normal;
	
	AreaLight() {}
	virtual ~AreaLight() {}

	virtual SampleLight sampleLight(const Ray& r, const HitRecord& record) const;
};

#endif
