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

	sampledLight = SampleLight(position, intensity, record.p);
	return true;
}
