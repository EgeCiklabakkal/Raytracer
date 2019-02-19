#include "ray.h"

Ray Ray::reflectionRay(const HitRecord& record, float epsilon) const
{
	Vec3 x  = pointAtParameter(record.t);
	Vec3 wo = origin() - x;
	Vec3 n  = record.normal;
	wo.makeUnitVector();

	Vec3 wr = -wo + 2 * n * (dot(n, wo));
	wr.makeUnitVector();
	Ray reflection_ray(x + wr * epsilon, wr);
	
	return reflection_ray;
}

Ray Ray::shadowRay(const HitRecord& record, const Light* light_ptr, float epsilon) const
{
	Vec3 x  = pointAtParameter(record.t);
	Vec3 wi = light_ptr->position - x;
	wi.makeUnitVector();

	return Ray(x + wi * epsilon, wi);
}

// When using this method, make sure point is on the ray
float Ray::parameterAtPoint(const Vec3& point) const
{
	Vec3 wi(point - origin());
	Vec3 d(direction());
	
	return wi.length() / d.length();
}
