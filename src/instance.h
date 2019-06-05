#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "shape.h"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>

class ObjectInstance : public Shape
{
	public:

	Shape *prim;		// Primary object pointer (primMeshBVH)
	Material material;	// Instances may change material
	Texture *texture;

	ObjectInstance() {}
	ObjectInstance(const glm::mat4& trans, const glm::mat4& trans_inverse, Shape* _prim, 
				const Material& _material, bool _transformed=false, 
				Texture* _texture=nullptr);
	ObjectInstance(const glm::mat4& trans, Shape* _prim, const Material& _material,
			bool _transformed=false, Texture* _texture=nullptr);
	bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record,
			bool cullFace, bool nonluminous) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time,
			bool cullFace, bool nonluminous) const;
	bool boundingBox(float time0, float time1, BBox& _box) const;

	~ObjectInstance() {}
};

#endif
