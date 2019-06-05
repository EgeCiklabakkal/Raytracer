#ifndef _BVH_H_
#define _BVH_H_

#include "shape.h"

class BVH : public Shape
{
	public:

	Shape *left;
	Shape *right;
	BBox box;
	// axis is 0:x, 1:y, 2:z

	BVH() {}
	BVH(Shape* s0, Shape* s1);
	BVH(Shape* s0, Shape* s1, const BBox& _bbox);
	BVH(Shape** shapes, int n, int axis, float time0, float time1);
	~BVH() 
	{
		if(left)  delete left; 
		if(right) delete right; 
	}

	Shape* buildBranch(Shape** shapes, int n, int axis, float time0, float time1);
	bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record,
			bool cullFace=true, bool nonluminous=false) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time,
			bool cullFace=true, bool nonluminous=false) const;
	bool boundingBox(float time0, float time1, BBox& _box) const;
	int partitionBySpace(Shape** shapes, int n, int axis);
};

#endif
