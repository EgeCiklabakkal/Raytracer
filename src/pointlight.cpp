#include "light.h"

bool PointLight::sampleLight(const Scene* scene, const Ray& r,
				const HitRecord& record, SampleLight& sampledLight) const
{
	Ray shadow_ray(r.shadowRay(record, position, scene->shadow_ray_epsilon));
	float tlight = shadow_ray.parameterAtPoint(position);

	if(scene->bvh->shadowHit(shadow_ray, 0.0f, tlight, r.time))	// In shadow
	{
		return false;
	}

	Vec3 wi(position - record.p);
	float d2 = wi.squaredLength();

	sampledLight = SampleLight(intensity / d2, unitVector(wi));
	return true;
}
