#ifndef _SIMPLETEXTURE_H_
#define _SIMPLETEXTURE_H_

#include "texture.h"

class SimpleTexture : public Texture
{
	// Single color solid texture
	public:
	
	rgb color;

	SimpleTexture(rgb _color, DecalMode _decal_mode=DecalMode::REPLACEKD) :
	color(_color) { decal_mode = _decal_mode; }
	rgb value(const Vec2& uv, const Vec3& p) const
	{
		return color;
	}
};

#endif
