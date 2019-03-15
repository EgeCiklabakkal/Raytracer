#include "light.h"

SampleLight AreaLight::sampleLight(const Ray& r, const HitRecord& record) const
{
	float e1, e2;
	ONB onb;
	onb.initFromW(normal);
	e1 = rtmath::randf() - 0.5f;
	e2 = rtmath::randf() - 0.5f;
	Vec3 from(r.pointAtParameter(record.t));

	Vec3 ls(position + size*(e1*onb.u() + e2*onb.v()));
	Vec3 l(unitVector(from - ls));
	float declination = dot(l, normal);
	declination = (declination >= 0.0f) ? declination : -declination;
	Vec3 _I(intensity * declination * size * size);

	return SampleLight(ls, _I, from);
}
