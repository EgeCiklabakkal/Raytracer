#include "light.h"

SpotLight::SpotLight(const Vec3& _position, const Vec3& _direction, const Vec3& _intensity,
			float _alpha, float _beta) :
position(_position), direction(_direction), intensity(_intensity), alpha(_alpha), beta(_beta) {}

bool SpotLight::sampleLight(const Scene* scene, const Ray& r, const HitRecord& record,
				SampleLight& sampledLight, bool nonluminous) const
{
	Ray shadow_ray(r.shadowRay(record, position, scene->shadow_ray_epsilon));
	float tlight = shadow_ray.parameterAtPoint(position);

	if(scene->bvh->shadowHit(shadow_ray, 0.0f, tlight, r.time, false, nonluminous))
	{
		return false;
	}

	Vec3 wi(position - record.p);
	float d2 = wi.squaredLength();

	sampledLight = SampleLight((intensity * falloff(wi)) / d2, unitVector(wi));
	return true;
}

float SpotLight::falloff(const Vec3& wi) const
{
	float costheta = dot(unitVector(-wi), unitVector(direction));
	float cosalpha = cos(((alpha/2.0f) * M_PI) / 180.0f);
	float cosbeta  = cos(((beta/2.0f) * M_PI) / 180.0f);

	if(costheta < cosalpha) { return 0.0f; }
	if(costheta > cosbeta)  { return 1.0f; }

	float delta = (costheta - cosalpha) / (cosbeta - cosalpha);

	return (delta * delta) * (delta * delta);
}
