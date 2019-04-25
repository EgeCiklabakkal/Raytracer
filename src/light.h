#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "shape.h"
#include "scene.h"
#include "ONB.h"
#include "rtmath.h"
#include <vector>
#include <limits>

class Scene;
class Ray;
class HitRecord;

class SampleLight
{
	public:

	Vec3 intensity;
	Vec3 wi;	// direction of incoming light to the intersection point

	SampleLight() {}
	SampleLight(const Vec3& _intensity, const Vec3& _wi) :
	intensity(_intensity), wi(_wi) {}

	SampleLight& operator=(const SampleLight& rhs)
	{
		intensity  = rhs.intensity;
		wi 	   = rhs.wi;

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

class DirectionalLight : public Light
{
	public:

	Vec3 direction;
	Vec3 radiance;

	DirectionalLight() {}
	DirectionalLight(const Vec3& direction, const Vec3& radiance);
	virtual ~DirectionalLight() {}

	virtual bool sampleLight(const Scene* scene, const Ray& r,
					const HitRecord& record, SampleLight& sampledLight) const;
};

#endif
