#ifndef _LIGHTMESH_H_
#define _LIGHTMESH_H_

#include "light.h"
#include "instance.h"
#include "material.h"
#include "rtmath.h"
#include "simpleCDF.h"

class LightMesh : public Light, public ObjectInstance
{
	public:

	SimpleCDF cdf;
	Vec3 radiance;

	LightMesh(const glm::mat4& trans, Shape* _prim, const Material& _material,
			const std::vector<Shape*>& meshTriangles,
			const Vec3& _radiance, bool _transformed=false);
	virtual ~LightMesh() {}

	virtual bool sampleLight(const Scene* scene, const Ray& r, const HitRecord& record,
					SampleLight& sampledLight, bool nonluminous) const;
	bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record,
			bool cullFace, bool nonluminous) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time,
			bool cullFace, bool nonluminous) const;
};

#endif
