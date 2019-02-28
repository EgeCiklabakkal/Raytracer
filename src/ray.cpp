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
	
	return reflection_ray;
}

Ray Ray::shadowRay(const HitRecord& record, const Light* light_ptr, float epsilon) const
{
	Vec3 x  = pointAtParameter(record.t);
	Vec3 wi = light_ptr->position - x;
	wi.makeUnitVector();

	return Ray(x + wi * epsilon, wi);
}

Ray Ray::transmissionRay(const HitRecord& record, const Vec3& d, float epsilon) const
{
	Vec3 x = pointAtParameter(record.t);

	return Ray(x + -record.normal * epsilon, d);
}

// When using this method, make sure point is on the ray
float Ray::parameterAtPoint(const Vec3& point) const
{
	Vec3 wi(point - origin());
	Vec3 d(direction());
	
	return wi.length() / d.length();
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
