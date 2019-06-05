#ifndef _LIGHTSPHERE_H_
#define _LIGHTSPHERE_H_

#include "light.h"
#include "sphere.h"
#include "material.h"
#include "rtmath.h"

class LightSphere : public Light, public Sphere
{
	public:

	Vec3 radiance;

	LightSphere(const Vec3& _center, float _radius,
			const Material& _material, const Vec3& _radiance);
	virtual ~LightSphere() {}

	virtual bool sampleLight(const Scene* scene, const Ray& r, const HitRecord& record,
					SampleLight& sampledLight, bool nonluminous) const;
	bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record,
			bool cullFace, bool nonluminous) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time,
			bool cullFace, bool nonluminous) const;
};

#endif
