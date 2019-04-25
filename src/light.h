#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "shape.h"
#include "scene.h"
#include "ONB.h"
#include "rtmath.h"
#include <vector>

class Scene;
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

	SampleLight& operator=(const SampleLight& rhs)
	{
		position   = rhs.position;
		intensity  = rhs.intensity;
		relativeTo = rhs.relativeTo;

		return *this;
	}
};

class Light
{
	public:

	virtual ~Light() {};

	virtual bool sampleLight(const Scene* scene, const Ray& r,
					const HitRecord& record, SampleLight& sampledLight) const=0;
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

	virtual bool sampleLight(const Scene* scene, const Ray& r,
					const HitRecord& record, SampleLight& sampledLight) const;
};

class AreaLight : public Light
{
	public:

	Vec3 position;
	Vec3 radiance;
	float size;
	Vec3 normal;
	
	AreaLight() {}
	virtual ~AreaLight() {}

	virtual bool sampleLight(const Scene* scene, const Ray& r,
					const HitRecord& record, SampleLight& sampledLight) const;
};

#endif
