#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vector3.h"

class Light
{
	public:

	Vec3 position;
	Vec3 intensity;
};

class PointLight : public Light
{
	public:

};

class AmbientLight : public Light
{
	public:

};

#endif
