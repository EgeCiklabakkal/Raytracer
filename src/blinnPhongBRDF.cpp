#include "blinnPhongBRDF.h"

BlinnPhongBRDF::BlinnPhongBRDF(float _exponent) : exponent(_exponent) {}

BlinnPhongBRDF::~BlinnPhongBRDF() {}

rgb BlinnPhongBRDF::brdf(const Ray& r, const HitRecord& record, const SampleLight& slight) const
{
	float costheta_i = std::max(0.0f, dot(record.normal, slight.wi));
	if(costheta_i > 0.0f)	// theta_i < 90°
	{
		rgb kd(record.material.diffuse);
		rgb ks(record.material.specular);

		Vec3 x(record.p);
		Vec3 wo(unitVector(r.origin() - x));
		Vec3 h(slight.wi + wo);

		float cosalpha_h = std::max(0.0f, dot(record.normal, unitVector(h)));

		return kd + ks * (pow(cosalpha_h, exponent) / costheta_i);
	}

	return rgb(0.0f);
}

rgb BlinnPhongBRDF::value(const Ray& r, const HitRecord& record, const SampleLight& slight) const
{
	rgb Li(slight.radiance);
	float costheta_i = std::max(0.0f, dot(record.normal, slight.wi));
	rgb f = brdf(r, record, slight);

	return (Li * f) * costheta_i;
}
