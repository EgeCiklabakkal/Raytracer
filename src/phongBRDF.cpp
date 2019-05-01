#include "phongBRDF.h"

PhongBRDF::PhongBRDF(float _exponent) : exponent(_exponent) {}

PhongBRDF::~PhongBRDF() {}

rgb PhongBRDF::brdf(const Ray& r, const HitRecord& record, const SampleLight& slight) const
{
	float costheta_i = dot(record.normal, record.normal);
	if(costheta_i > 0.0f)	// theta_i < 90°
	{
		rgb kd(record.material.diffuse);
		rgb ks(record.material.specular);

		Vec3 x(record.p);
		Vec3 wo(unitVector(r.origin() - x));
		Vec3 wi(slight.wi);
		Vec3 n(record.normal);

		Vec3 wr(unitVector(-wi + 2 * n * (dot(n, wi)))); // reflection of wi
		float cosalpha_r = std::max(0.0f, dot(wr, wo));

		return kd + ks * (pow(cosalpha_r, exponent) / costheta_i);
	}

	return rgb(0.0f);
}

rgb PhongBRDF::value(const Ray& r, const HitRecord& record, const SampleLight& slight) const
{
	rgb Li(slight.intensity);
	float costheta_i = dot(record.normal, record.normal);
	rgb f = brdf(r, record, slight);

	return (Li * f) * costheta_i;
}
