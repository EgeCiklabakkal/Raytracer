#include "modifiedPhongBRDF.h"

ModifiedPhongBRDF::ModifiedPhongBRDF(float _exponent, bool _normalized) :
exponent(_exponent), normalized(_normalized) {}

ModifiedPhongBRDF::~ModifiedPhongBRDF() {}

rgb ModifiedPhongBRDF::brdf(const Ray& r, const HitRecord& record, const SampleLight& slight) const
{
	float costheta_i = std::max(0.0f, dot(record.normal, slight.wi));
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

		if(normalized)
			return kd * INV_PI + ks *(exponent + 2)*INV_2PI* pow(cosalpha_r, exponent);
		else
			return kd + ks * pow(cosalpha_r, exponent);
	}

	return rgb(0.0f);
}

rgb ModifiedPhongBRDF::value(const Ray& r, const HitRecord& record,
				const SampleLight& slight) const
{
	rgb Li(slight.intensity);
	float costheta_i = std::max(0.0f, dot(record.normal, slight.wi));
	rgb f = brdf(r, record, slight);

	return (Li * f) * costheta_i;
}
