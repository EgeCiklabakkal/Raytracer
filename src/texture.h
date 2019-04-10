#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "vector2.h"
#include "vector3.h"
#include "rgb.h"

enum class DecalMode { REPLACEKD, BLENDKD, REPLACEALL };

class Texture
{
	public:

	DecalMode decal_mode;

	virtual rgb value(const Vec2& uv , const Vec3& p) const=0;
	DecalMode decalmode() const { return decal_mode; }
};

#endif
