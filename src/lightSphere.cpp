#include "lightSphere.h"

LightSphere::LightSphere(const Vec3& _center, float _radius,
				const Material& _material, const Vec3& _radiance) :
Sphere{_center, _radius, _material}, radiance(_radiance)
{
	material.light = true;
}

bool LightSphere::sampleLight(const Scene* scene, const Ray& r,
				const HitRecord& record, SampleLight& sampledLight) const
{
	// These events take place in world coordinates
	// Center of the sphere should be transformed
	// Radius can't be transformed properly
	Vec3 tcenter = rtmath::transformLoc(M, center);
	Vec3 d(tcenter - record.p);
	float sintheta_max = radius / d.length();
	float costheta_max = sqrt(1.0f - sintheta_max * sintheta_max);

	// Construct the ONB along the d vector
	ONB onb;
	onb.initFromW(unitVector(d));

	float pw = INV_2PI * (1.0f / (1.0f - costheta_max));

	float e1    = rtmath::randf();
	float e2    = rtmath::randf();
	float phi   = 2.0f * M_PI * e1;
	float theta = acos(1.0f - e2 + e2 * costheta_max);

	Vec3 l = cos(theta) * onb.W +
		 sin(theta) * cos(phi) * onb.V +
		 sin(theta) * sin(phi) * onb.U;

	// Compute emitting point
	HitRecord lightRecord;
	Ray sampleRay(record.p, l);
	float isHit = hit(sampleRay, 0.0f, 100000.0f, 0.0f, lightRecord); // Change tmin/tmax

	if(!isHit)	// This shouldn't be the case as we are sampling the sphere already
	{
		return false;	// just in case
	}

	PointLight plight(lightRecord.p, radiance / pw);
	return plight.sampleLight(scene, r, record, sampledLight);
}

bool LightSphere::hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const
{
	bool isHit = Sphere::hit(r, tmin, tmax, time, record);

	record.color = radiance;
	return isHit;
}
