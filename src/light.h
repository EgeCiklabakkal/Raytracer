#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vector3.h"

class Light
{
	public:

};

class PointLight : public Light
{
	public:

	Vec3 position;
	Vec3 intensity;
};

class AmbientLight : public Light
{
	public:

	Vec3 intensity;	
};

#endif
