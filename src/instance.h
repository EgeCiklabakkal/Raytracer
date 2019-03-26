#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "shape.h"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>

class ObjectInstance : public Shape
{
	public:

	Shape *prim;		// Primary object pointer
	bool resetTransform;

	ObjectInstance() : resetTransform(false) {}
	ObjectInstance(const glm::mat4& trans, const glm::mat4& trans_inverse, 
			Shape* _prim, bool _resetTransform=false);
	ObjectInstance(const glm::mat4& trans, Shape* _prim, bool _resetTransform=false);
	bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time) const;
	bool boundingBox(float time0, float time1, BBox& _box) const;

	~ObjectInstance() {}
};

#endif