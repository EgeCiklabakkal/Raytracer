#include "instance.h"

ObjectInstance::ObjectInstance(const glm::mat4& trans, const glm::mat4& trans_inverse, 
				Shape* _prim, const Material& _material, 
				bool _transformed, bool _resetTransform) : 
prim(_prim), material(_material), resetTransform(_resetTransform)
{
	M = trans;
	N = trans_inverse;
	transformed = _transformed;
}

ObjectInstance::ObjectInstance(const glm::mat4& trans, Shape* _prim, const Material& _material,
			bool _transformed, bool _resetTransform) : 
prim(_prim), material(_material), resetTransform(_resetTransform)
{
	M = trans;
	N = glm::inverse(M);
	transformed = _transformed;
}

bool ObjectInstance::hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const
{
	Ray tray = transformRayToLocal(r);

	if(prim->hit(tray, tmin, tmax, time, record))
	{
		record = transformRecordToWorld(record);
		// Also update material
		record.material = material;
		return true;
	}

	return false;
}

bool ObjectInstance::shadowHit(const Ray& r, float tmin, float tmax, float time) const
{
	Ray tray = transformRayToLocal(r);

	return (prim->shadowHit(tray, tmin, tmax, time));
}

bool ObjectInstance::boundingBox(float time0, float time1, BBox& _box) const
{
	prim->boundingBox(time0, time1, _box);
	_box = transformBBoxToWorld(_box);

	return true;
}
