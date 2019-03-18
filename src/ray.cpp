#include "ray.h"

Ray Ray::reflectionRay(const HitRecord& record, float epsilon) const
{
	Vec3 x  = pointAtParameter(record.t);
	Vec3 wo = origin() - x;
	Vec3 n  = record.normal;
	wo.makeUnitVector();

	Vec3 wr = -wo + 2 * n * (dot(n, wo));
	wr.makeUnitVector();
	Ray reflection_ray(x + n * epsilon, wr);

	if(record.material.roughness)	// Glossy
	{
		float e1 = rtmath::randf() - 0.5;
		float e2 = rtmath::randf() - 0.5;

		ONB onb;
		onb.initFromW(reflection_ray.direction());

		reflection_ray.data[1] += record.material.roughness*(e1*onb.u() + e2*onb.v());
	}
	
	return reflection_ray;
}

Ray Ray::shadowRay(const HitRecord& record, const SampleLight& slight, float epsilon) const
{
	Vec3 x  = pointAtParameter(record.t);
	Vec3 wi = slight.position - x;
	wi.makeUnitVector();

	return Ray(x + wi * epsilon, wi);
}

Ray Ray::transmissionRay(const HitRecord& record, const Vec3& d, float epsilon) const
{
	Vec3 x = pointAtParameter(record.t);

	return Ray(x + -record.normal * epsilon, unitVector(d));
}

// When using this method, make sure point is on the ray
float Ray::parameterAtPoint(const Vec3& point) const
{
	return dot(point - origin(), unitVector(direction()));
}

bool Ray::refract(const HitRecord& record, std::pair<float, float> refractionIndexes, 
			Vec3& transmissionDirection) const
{
	float n1, n2, dn, cosphi;
	Vec3 n, d;

	n1 = refractionIndexes.first;
	n2 = refractionIndexes.second;
	n  = record.normal;
	d  = direction();
	dn = dot(d, n);

	if((cosphi = 1 - ((n1*n1 * (1 - dn*dn)) / (n2*n2))) < 0)
	{
		return false;
	}

	cosphi = sqrt(cosphi);
	transmissionDirection = (n1 * (d - n * dn) / n2) - (n * cosphi);
	
	return true;
}
