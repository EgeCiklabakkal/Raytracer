#ifndef _BBOX_H_
#define _BBOX_H_

#include "ray.h"

class Ray;

class BBox
{
	public:

	// Member variables
	Vec3 _min;
	Vec3 _max;

	BBox() {}
	BBox(const Vec3& v1, const Vec3& v2) { _min = v1; _max = v2; }
	Vec3 min() const { return _min; } 
	Vec3 max() const { return _max; }

	bool hit(const Ray& r, float tmin, float tmax) const;
};

BBox surrounding_box(const BBox& b0, const BBox& b1);

#endif
