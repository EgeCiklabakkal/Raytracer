#include "light.h"

bool PointLight::sampleLight(const Scene* scene, const Ray& r, const HitRecord& record,
				SampleLight& sampledLight, bool nonluminous) const
{
	Ray shadow_ray(r.shadowRay(record, position, scene->shadow_ray_epsilon));
	float tlight = shadow_ray.parameterAtPoint(position);

	if(scene->bvh->shadowHit(shadow_ray, 0.0f, tlight, r.time, false, nonluminous)) // In shadow
	{
		return false;
	}

	Vec3 wi(position - record.p);
	float d2 = wi.squaredLength();

	sampledLight = SampleLight(intensity / d2, unitVector(wi));
	return true;
}

bool PointLight::samplePhoton(Photon& photon) const
{
	float u, v, sinv;
	static float _2pi = 2.0f * M_PI;

	// Generate random direction
	u = _2pi * rtmath::randf();
	v = 2.0f * acos(sqrt(rtmath::randf()));
	sinv = sin(v);
	Vec3 d(cos(u) * sinv, cos(v), sin(u) * sinv);

	Ray path(position, d);
	rgb power(intensity * 4.0f * M_PI);
	photon = Photon(path, power, 1.0f);

	return true;
}
