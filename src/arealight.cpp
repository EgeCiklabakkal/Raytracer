#include "light.h"

bool AreaLight::sampleLight(const Scene* scene, const Ray& r, const HitRecord& record,
				SampleLight& sampledLight, bool nonluminous) const
{
	float tlight, e1, e2, declination;
	ONB onb;
	onb.initFromW(normal);

	e1 = rtmath::randf() - 0.5f;
	e2 = rtmath::randf() - 0.5f;

	Vec3 from(record.p);
	Vec3 ls(position + size*(e1*onb.u() + e2*onb.v()));
	Ray shadow_ray(r.shadowRay(record, ls, scene->shadow_ray_epsilon));
	tlight = shadow_ray.parameterAtPoint(ls);

	if(scene->bvh->shadowHit(shadow_ray, 0.0f, tlight, r.time, false, nonluminous)) // In shadow
	{
		return false;
	}

	Vec3 wi(ls - from);
	float d2 = wi.squaredLength();
	wi.makeUnitVector();
	
	Vec3 l(-wi);
	declination = dot(l, normal);
	declination = (declination >= 0.0f) ? declination : -declination;
	Vec3 _I((radiance * declination * size * size) / d2);

	sampledLight = SampleLight(_I, wi);
	return true;
}
