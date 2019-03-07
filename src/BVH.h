#ifndef _BVH_H_
#define _BVH_H_

#include "shape.h"

class BVH : public Shape
{
	public:

	Shape *left;
	Shape *right;
	BBox box;
	bool lastBranch;	// children are leaves
	// axis is 0:x, 1:y, 2:z

	BVH() {}
	BVH(Shape* s0, Shape* s1, float epsilon);
	BVH(Shape** shapes, int n, int axis, float time0, float time1, float epsilon);
	~BVH() 
	{
		if(!lastBranch)
		{
			delete left; delete right; 
		}
	}

	bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time) const;
	bool boundingBox(float time0, float time1, BBox& _box, float epsilon) const;
	int partitionBySpace(Shape** shapes, int n, int axis, float epsilon=1e-6);
};

#endif
