#include "ray.h"

Ray Ray::reflectionRay(const HitRecord& record) const
{
	Vec3 x  = pointAtParameter(record.t);
	Vec3 wo = origin() - x;
	Vec3 n  = record.normal;
	wo.makeUnitVector();

	Vec3 wr = -wo + 2 * n * (dot(n, wo));
	wr.makeUnitVector();
	Ray reflection_ray(x, wr);

	// Note: May need to add an epsilon
	
	return reflection_ray;
}
