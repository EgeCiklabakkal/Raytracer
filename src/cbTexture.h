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

	CBTexture(float _offset=0.0f, float _scale=1.0f, float _normalizer=1.0f,
			rgb _black=rgb(0.0f), rgb _white=rgb(1.0f),
			DecalMode _decal_mode=DecalMode::REPLACEKD, bool _dimension3=true) :
	offset(_offset), scale(_scale), normalizer(_normalizer),
	black(_black), white(_white), dimension3(_dimension3)
	{ decal_mode = _decal_mode; }

	virtual ~CBTexture() {}

	rgb value(const Vec2& uv, const Vec3& p) const
	{
		int check;

		if(dimension3)
		{
			bool x = (int) ((p.x() + offset) * scale) % 2;
			bool y = (int) ((p.y() + offset) * scale) % 2;
			bool z = (int) ((p.z() + offset) * scale) % 2;

			bool xorXY = (x != y);
			check = (xorXY == z);
		}

		else	// 2d
		{
			bool x = (int) ((p.x() + offset) * scale) % 2;
			bool y = (int) ((p.y() + offset) * scale) % 2;

			check = (x == y);
		}

		return ((check ? white : black) / normalizer);
	}
};

#endif
