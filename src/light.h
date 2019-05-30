#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "shape.h"
#include "scene.h"
#include "ONB.h"
#include "rtmath.h"
#include <vector>
#include <limits>
#include <math.h>

class Scene;
class Ray;
class HitRecord;

class SampleLight
{
	public:

	Vec3 radiance;
	Vec3 wi;	// direction of incoming light, ie. light.position - p

	SampleLight() {}
	SampleLight(const Vec3& _radiance, const Vec3& _wi) :
	radiance(_radiance), wi(_wi) {}

	SampleLight& operator=(const SampleLight& rhs)
	{
		radiance   = rhs.radiance;
		wi 	   = rhs.wi;

		return *this;
	}
};

class Light
{
	public:

	bool object = false;	// Is light an object?

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

class SpotLight : public Light
{
	public:

	Vec3 position;
	Vec3 direction;
	Vec3 intensity;
	float alpha;	// CoverageAngle
	float beta;	// FalloffAngle

	SpotLight() {}
	SpotLight(const Vec3& _position, const Vec3& _direction, const Vec3& _intensity,
			float _alpha, float _beta);
	virtual ~SpotLight() {}

	virtual bool sampleLight(const Scene* scene, const Ray& r,
					const HitRecord& record, SampleLight& sampledLight) const;
	float falloff(const Vec3& wi) const;
};

#endif
