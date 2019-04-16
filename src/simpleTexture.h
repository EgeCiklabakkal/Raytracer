#ifndef _SIMPLETEXTURE_H_
#define _SIMPLETEXTURE_H_

#include "texture.h"

class SimpleTexture : public Texture
{
	// Single color solid texture
	public:
	
	rgb color;

	SimpleTexture(rgb _color, DecalMode _decal_mode=DecalMode::REPLACEKD,
			bool _bumpmap=false, float _bumpmapMultiplier=1.0f) :
	color(_color)
	{
		decal_mode = _decal_mode;
		bumpmap = _bumpmap;
		bumpmapMultiplier = _bumpmapMultiplier;
	}

	virtual ~SimpleTexture() {}

	rgb value(const Vec2& uv, const Vec3& p) const
	{
		return color;
	}

	Vec3 bumpNormal(const Vec2& uv, const Vec3& p, const Vec3& n,
				const Vec3& dpdu, const Vec3& dpdv) const
	{
		return n;
	}
};

#endif
