#include "lightMesh.h"

LightMesh::LightMesh(const glm::mat4& trans, Shape* _prim, const Material& _material,
			const Vec3& _radiance, bool _transformed) :
ObjectInstance{trans, _prim, _material, _transformed}, radiance(_radiance)
{
	object = true;
	material.luminous = true;
}

bool LightMesh::sampleLight(const Scene* scene, const Ray& r, const HitRecord& record,
				SampleLight& sampledLight, bool nonluminous) const
{
	return false;
}

bool LightMesh::hit(const Ray& r, float tmin, float tmax,
			float time, HitRecord& record, bool nonluminous) const
{
	if(nonluminous) return false;

	bool isHit = ObjectInstance::hit(r, tmin, tmax, time, record, nonluminous);

	record.color = radiance;
	return isHit;
}

bool LightMesh::shadowHit(const Ray& r, float tmin, float tmax,
				float time, bool nonluminous) const
{
	if(nonluminous) return false;

	return ObjectInstance::shadowHit(r, tmin, tmax, time, nonluminous);
}
