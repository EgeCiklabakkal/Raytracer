#ifndef _CBTEXTURE_H_
#define _CBTEXTURE_H_

#include "texture.h"

class CBTexture : public Texture
{	
	// Checkerboard solid texture
	public:

	float offset;
	float scale;

	CBTexture(float _offset, float _scale) : offset(_offset), scale(_scale) {}
	rgb value(const Vec2& uv, const Vec3& p) const
	{
		int check = (	(int)std::floor((p.x() + offset) * scale) +
				(int)std::floor((p.y() + offset) * scale) +
				(int)std::floor((p.z() + offset) * scale)) % 2;
		return check ? rgb(1.0f) : rgb(0.0f);
	}
};

#endif
