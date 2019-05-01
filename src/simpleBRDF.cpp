#include "simpleBRDF.h"

SimpleBRDF::SimpleBRDF() {}

SimpleBRDF::~SimpleBRDF() {}

rgb SimpleBRDF::diffuseColor(const HitRecord& record, const SampleLight& slight) const
{
	rgb I(slight.intensity);
	rgb kd(record.material.diffuse);

	float costheta = std::max(0.0f, dot(slight.wi, record.normal));

	return kd * costheta * I;
}

rgb SimpleBRDF::specularColor(const Ray& r, const HitRecord& record,
				const SampleLight& slight) const
{
	rgb I(slight.intensity);
	rgb ks(record.material.specular);
	float phong_exp = record.material.phong_exponent;

	Vec3 x = record.p;
	Vec3 wo(unitVector(r.origin() - x));
	Vec3 h(slight.wi + wo);

	float costheta = std::max(0.0f, dot(record.normal, unitVector(h)));

	return ks * (pow(costheta, phong_exp)) * I;
}

rgb SimpleBRDF::value(const Ray& r, const HitRecord& record, const SampleLight& slight) const
{
	return diffuseColor(record, slight) + specularColor(r, record, slight);
}
