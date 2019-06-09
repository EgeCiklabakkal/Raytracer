#ifndef _PHOTON_H_
#define _PHOTON_H_

#include "ray.h"
#include "rgb.h"

class Photon
{
	public:

	Ray path;
	rgb power;
	float wgt;

	Photon() {}
	Photon(const Ray& _path, const rgb& _power, float _wgt=1.0f) :
	path(_path), power(_power), wgt(_wgt) {}
};

#endif
