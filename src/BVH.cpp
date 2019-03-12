#include "BVH.h"

BVH::BVH(Shape* s0, Shape* s1)
{
	BBox b0, b1;
	left  = s0;
	right = s1;
	lastBranch = true;

	s0->boundingBox(0.0f, 0.0f, b0);
	s1->boundingBox(0.0f, 0.0f, b1);
	box = BBox(surrounding_box(b0, b1));
}

BVH::BVH(Shape** shapes, int n, int axis, float time0, float time1)
{
	if(n == 1)
	{
		*this = BVH(shapes[0], shapes[0]);
	}

	else if(n == 2)
	{
		*this = BVH(shapes[0], shapes[1]);
	}

	else
	{
		// Bounding Box calculation
		BBox boxAccumulator;
		shapes[0]->boundingBox(time0, time1, boxAccumulator);
		for(int i = 1; i < n; i++)
		{
			BBox temp;
			shapes[i]->boundingBox(time0, time1, temp);
			boxAccumulator = surrounding_box(boxAccumulator, temp);
		}
		box = boxAccumulator;
		lastBranch = false;

		int midpoint = partitionBySpace(shapes, n, axis);

		left  = new BVH(shapes, midpoint, (axis+1)%3, time0, time1);
		right = new BVH(&shapes[midpoint], n - midpoint, (axis+1)%3, time0, time1);
	}
}

bool BVH::hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const
{
	if(box.hit(r, tmin, tmax))
	{
		HitRecord leftRecord, rightRecord;

		bool hitLeft  = left->hit(r, tmin, tmax, time, leftRecord);
		bool hitRight = right->hit(r, tmin, tmax, time, rightRecord);

		if(hitLeft && hitRight)		// Both hit, get closest
		{
			record = leftRecord.t < rightRecord.t ? leftRecord : rightRecord;
			return true;
		}
		
		else if(hitLeft)
		{
			record = leftRecord;
			return true;
		}

		else if(hitRight)
		{
			record = rightRecord;
			return true;
		}

		else	// No hit
		{
			return false;
		}
	}

	return false;
}

bool BVH::shadowHit(const Ray& r, float tmin, float tmax, float time) const
{
	if(box.hit(r, tmin, tmax))
	{
		if(left->shadowHit(r, tmin, tmax, time))
		{
			return true;
		}

		return right->shadowHit(r, tmin, tmax, time);	
	}

	return false;
}

int BVH::partitionBySpace(Shape** shapes, int n, int axis)
{
	BBox _box;
	float shape_center;
	int partition_idx = 0;
	float midpoint = (box.min()[axis] + box.max()[axis]) / 2.0f;

	for(int i = 0; i < n; i++)
	{
		shapes[i]->boundingBox(0.0f, 0.0f, _box);
		shape_center = (_box.min()[axis] + _box.max()[axis]) / 2.0f;

		if(shape_center < midpoint)
		{
			std::swap(shapes[partition_idx], shapes[i]);
			partition_idx++;
		}
	}

	// if space division fails to place items in both left/right, just partition by object count
	if(partition_idx == 0 || partition_idx == n)
	{
		partition_idx = n / 2;
	}

	return partition_idx;
}

inline bool BVH::boundingBox(float time0, float time1, BBox& _box) const
{
	_box = box;
	return true;
}
