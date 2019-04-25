#include "light.h"

DirectionalLight::DirectionalLight(const Vec3& _direction, const Vec3& _radiance) :
direction(_direction),  radiance(_radiance) {}

bool DirectionalLight::sampleLight(const Scene* scene, const Ray& r,
					const HitRecord& record, SampleLight& sampledLight) const
{
	Ray shadow_ray(record.p + record.normal * scene->shadow_ray_epsilon, unitVector(-direction));
	shadow_ray.setTime(record.time);

	if(scene->bvh->shadowHit(shadow_ray, 0.0f, std::numeric_limits<float>::max(), r.time))
	{
		return false;
	}

	sampledLight = SampleLight(radiance, unitVector(-direction));
	return true;
}
