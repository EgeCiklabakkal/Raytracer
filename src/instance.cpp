#include "instance.h"

ObjectInstance::ObjectInstance(const glm::mat4& trans, const glm::mat4& trans_inverse, 
				Shape* _prim, const Material& _material, 
				bool _transformed, Texture* _texture) : 
prim(_prim), material(_material), texture(_texture)
{
	M = trans;
	N = trans_inverse;
	transformed = _transformed;
}

ObjectInstance::ObjectInstance(const glm::mat4& trans, Shape* _prim, const Material& _material,
			bool _transformed, Texture* _texture) : 
prim(_prim), material(_material), texture(_texture)
{
	M = trans;
	N = glm::inverse(M);
	transformed = _transformed;
}

bool ObjectInstance::hit(const Ray& r, float tmin, float tmax,
				float time, HitRecord& record, bool nonluminous) const
{
	Ray tray = transformRayToLocal(r);

	if(prim->hit(tray, tmin, tmax, time, record, nonluminous))
	{
		record = transformRecordToWorld(record);
		// Also update material and texture
		record.material = material;
		record.texture  = texture;

		// Bump transform
		Vec3 blurDistance = (motionBlurred) ? velocity * time : Vec3(0.0f);
		glm::mat4 currBlur = glm::translate(glm::mat4(1.0f), glm::vec3( blurDistance[0],
										blurDistance[1],
										blurDistance[2]));
		record.bump.M = currBlur * M;
		return true;
	}

	return false;
}

bool ObjectInstance::shadowHit(const Ray& r, float tmin, float tmax,
				float time, bool nonluminous) const
{
	Ray tray = transformRayToLocal(r);

	return (prim->shadowHit(tray, tmin, tmax, time, nonluminous));
}

bool ObjectInstance::boundingBox(float time0, float time1, BBox& _box) const
{
	prim->boundingBox(time0, time1, _box);
	_box = transformBBoxToWorld(_box);

	return true;
}
