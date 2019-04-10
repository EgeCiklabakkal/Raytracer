#ifndef _CBTEXTURE_H_
#define _CBTEXTURE_H_

#include "texture.h"

class CBTexture : public Texture
{	
	// Checkerboard solid texture
	public:

	float offset;
	float scale;
	float normalizer;
	rgb black;
	rgb white;
	bool dimension3;

	CBTexture(float _offset, float _scale, float _normalizer=1.0f,
			rgb _black=rgb(0.0f), rgb _white=rgb(1.0f),
			bool _dimension3=true, DecalMode _decal_mode=DecalMode::REPLACEKD) :
	offset(_offset), scale(_scale), normalizer(_normalizer),
	black(_black), white(_white), dimension3(_dimension3)
	{ decal_mode = _decal_mode; }
	rgb value(const Vec2& uv, const Vec3& p) const
	{
		int check;

		if(dimension3)
		{
			check = (	(int)std::floor((p.x() + offset) * scale)  +
					(int)std::floor((p.y() + offset) * scale)  +
					(int)std::floor((p.z() + offset) * scale)) % 2;
		}

		else	// 2d
		{
			check = (	(int)std::floor((p.x() + offset) * scale)  +
					(int)std::floor((p.y() + offset) * scale)) % 2;
		}

		return ((check ? white : black) / normalizer);
	}
};

#endif
