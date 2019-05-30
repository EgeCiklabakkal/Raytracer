#include "lightSphere.h"

LightSphere::LightSphere(const Vec3& _center, float _radius,
				const Material& _material, const Vec3& _radiance) :
Sphere{_center, _radius, _material}, radiance(_radiance)
{
	object = true;
	material.light = true;
}

bool LightSphere::sampleLight(const Scene* scene, const Ray& r,
				const HitRecord& record, SampleLight& sampledLight) const
{
	// Apply the sampling in local coordinates
	Vec3 localp = rtmath::transformLoc(N, record.p);
	Vec3 d(center - localp);
	float len_d = d.length();
	if(len_d < radius) return false;

	float sintheta_max = radius / len_d;
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
	Ray sampleRay(record.p, rtmath::transformVec(M, l));
	if(hit(sampleRay, 0.00001f, FLT_MAX, 0.0f, lightRecord)) // Change tmin/tmax
	{
		PointLight plight(lightRecord.p + lightRecord.normal * 0.001f, radiance / pw);
		return plight.sampleLight(scene, r, record, sampledLight);
	}

	return false;
}

bool LightSphere::hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const
{
	bool isHit = Sphere::hit(r, tmin, tmax, time, record);

	record.color = radiance;
	return isHit;
}
