#include "lightSphere.h"

LightSphere::LightSphere(const Vec3& _center, float _radius,
				const Material& _material, const Vec3& _radiance) :
Sphere{_center, _radius, _material}, radiance(_radiance)
{
	object = true;
	material.luminous = true;
}

bool LightSphere::sampleLight(const Scene* scene, const Ray& r, const HitRecord& record,
				SampleLight& sampledLight, bool nonluminous) const
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
		 sin(theta) * cos(phi) * onb.U +
		 sin(theta) * sin(phi) * onb.V;

	// Compute emitting point
	HitRecord lightRecord;
	Ray sampleRay(record.p, unitVector(rtmath::transformVec(M, l)));
	if(hit(sampleRay, 0.00001f, FLT_MAX, 0.0f, lightRecord, false)) // Change tmin/tmax
	{
		Ray shadow_ray(r.shadowRay(record, lightRecord.p, scene->shadow_ray_epsilon));
		float tlight = shadow_ray.parameterAtPoint(lightRecord.p);

		// nonluminous = true, so that light object will get ignored
		if(scene->bvh->shadowHit(shadow_ray, 0.0f, tlight, r.time, true))
		{
			return false;
		}

		Vec3 wi(lightRecord.p - record.p);

		sampledLight = SampleLight(radiance / pw, unitVector(wi));
		return true;
	}

	return false;
}

bool LightSphere::hit(const Ray& r, float tmin, float tmax,
			float time, HitRecord& record, bool nonluminous) const
{
	if(nonluminous)	return false;

	bool isHit = Sphere::hit(r, tmin, tmax, time, record, nonluminous);

	record.color = radiance;
	return isHit;
}

bool LightSphere::shadowHit(const Ray& r, float tmin, float tmax,
				float time, bool nonluminous) const
{
	if(nonluminous) return false;

	return Sphere::shadowHit(r, tmin, tmax, time, nonluminous);
}
