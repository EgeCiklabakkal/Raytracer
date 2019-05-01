#include "torranceSparrowBRDF.h"

TorranceSparrowBRDF::TorranceSparrowBRDF(float _exponent, float _refractive_index) :
exponent(_exponent), refractive_index(_refractive_index) {}

TorranceSparrowBRDF::~TorranceSparrowBRDF() {}

float TorranceSparrowBRDF::blinnDistribution(float cosalpha) const
{
	static float exp_plus_2 = exponent + 2.0f;

	return exp_plus_2 * INV_2PI * pow(cosalpha, exponent);
}

float TorranceSparrowBRDF::geometryTerm(const Vec3& wi, const Vec3& wo,
					const Vec3& n, const Vec3& h) const
{
	float nh      = dot(n , h);
	float nwo     = dot(n , wo);
	float nwi     = dot(n , wi);
	float woh     = dot(wo, h);
	float inv_woh = woh ? (1.0f / woh) : 1.0f;

	float term0 = 2.0f * nh * nwo * inv_woh;
	float term1 = 2.0f * nh * nwi * inv_woh;

	return std::min(1.0f, std::min(term0, term1));
}

float TorranceSparrowBRDF::fresnelReflection(float cosbeta) const
{
	static float n1 = DEFAULT_AIR_REFRACTION_INDEX;
	float n2 = refractive_index;
	float omc = 1.0f - cosbeta;
	float R0  = ((n1 - n2)*(n1 - n2)) / ((n1 + n2)*(n1 + n2));

	return R0 + (1.0f - R0) * omc * omc * omc * omc * omc;
}

rgb TorranceSparrowBRDF::brdf(const Ray& r, const HitRecord& record,
				const SampleLight& slight) const
{
	static float one_over_four = 1.0f / 4.0f;
	float costheta_i = std::max(0.0f, dot(record.normal, slight.wi));
	if(costheta_i > 0.0f)	// theta_i < 90°
	{
		rgb kd(record.material.diffuse);
		rgb ks(record.material.specular);

		Vec3 x(record.p);
		Vec3 wo(unitVector(r.origin() - x));
		Vec3 wi(slight.wi);
		Vec3 n(record.normal);
		Vec3 h(wi + wo);
		h.makeUnitVector();

		float cosalpha_h = std::max(0.0f, dot(n, h));
		float D = blinnDistribution(cosalpha_h);
		float G = geometryTerm(wi, wo, n, h);
		float F = fresnelReflection(std::max(0.0f, dot(wo, h)));
		float cosphi = std::max(0.0f, dot(wo, n));

		return kd * INV_PI + ks * ((D * F * G * one_over_four) / (costheta_i * cosphi));
	}

	return rgb(0.0f);
}

rgb TorranceSparrowBRDF::value(const Ray& r, const HitRecord& record,
				const SampleLight& slight) const
{
	rgb Li(slight.intensity);
	float costheta_i = std::max(0.0f, dot(record.normal, slight.wi));
	rgb f = brdf(r, record, slight);

	return (Li * f) * costheta_i;
}
