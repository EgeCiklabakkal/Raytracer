#include "modifiedBlinnPhongBRDF.h"

ModifiedBlinnPhongBRDF::ModifiedBlinnPhongBRDF(float _exponent, bool _normalized) :
exponent(_exponent), normalized(_normalized) {}

ModifiedBlinnPhongBRDF::~ModifiedBlinnPhongBRDF() {}

rgb ModifiedBlinnPhongBRDF::brdf(const Ray& r, const HitRecord& record,
					const SampleLight& slight) const
{
	static float exp_plus_8 = exponent + 8.0f;
	float costheta_i = std::max(0.0f, dot(record.normal, slight.wi));
	if(costheta_i > 0.0f)	// theta_i < 90°
	{
		rgb kd(record.material.diffuse);
		rgb ks(record.material.specular);

		Vec3 x(record.p);
		Vec3 wo(unitVector(r.origin() - x));
		Vec3 h(slight.wi + wo);

		float cosalpha_h = std::max(0.0f, dot(record.normal, unitVector(h)));

		if(normalized)
			return kd * INV_PI + ks * exp_plus_8 * INV_8PI * pow(cosalpha_h, exponent);
		else
			return kd + ks * pow(cosalpha_h, exponent);
	}

	return rgb(0.0f);
}

rgb ModifiedBlinnPhongBRDF::value(const Ray& r, const HitRecord& record,
					const SampleLight& slight) const
{
	rgb Li(slight.radiance);
	float costheta_i = std::max(0.0f, dot(record.normal, slight.wi));
	rgb f = brdf(r, record, slight);

	return (Li * f) * costheta_i;
}
