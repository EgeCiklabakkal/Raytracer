#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "vector2.h"
#include "vector3.h"
#include "rgb.h"

class Texture
{
	public:

	virtual rgb value(const Vec2& uv , const Vec3& p) const=0;
};

#endif
