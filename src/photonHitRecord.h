#ifndef _PHOTONHITRECORD_H_
#define _PHOTONHITRECORD_H_

#include "rgb.h"

class PhotonHitRecord
{
	public:

	Vec3 p;
	Vec3 normal;
	Ray path;
	rgb power;

	PhotonHitRecord(const Vec3& _p, const Vec3& _normal, const Ray& _path, const rgb& _power) :
	p(_p), normal(_normal), path(_path), power(_power) {}
};

#endif
