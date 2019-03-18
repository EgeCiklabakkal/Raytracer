#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "shape.h"
#include "ONB.h"
#include "rtmath.h"
#include <vector>

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

	virtual void sampleLight(const Ray& r, const HitRecord& record, 
		std::vector<SampleLight>& samples) const=0;
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

	virtual void sampleLight(const Ray& r, const HitRecord& record, 
		std::vector<SampleLight>& samples) const
	{ 
		samples.push_back(SampleLight(position, intensity));
	}
};

class AmbientLight : public Light
{
	public:

	Vec3 intensity;

	virtual void sampleLight(const Ray& r, const HitRecord& record, 
		std::vector<SampleLight>& samples) const
	{ return; } // No use
};

class AreaLight : public Light
{
	public:

	Vec3 position;
	Vec3 intensity;	// Radiance
	float size;
	Vec3 normal;
	
	AreaLight() {}
	virtual ~AreaLight() {}

	virtual void sampleLight(const Ray& r, const HitRecord& record, 
		std::vector<SampleLight>& samples) const;
};

#endif
