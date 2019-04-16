#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "vector2.h"
#include "vector3.h"
#include "rgb.h"

enum class DecalMode { REPLACEKD, BLENDKD, REPLACEALL };

class Texture
{
	public:

	bool bumpmap;
	float bumpmapMultiplier;
	DecalMode decal_mode;

	virtual ~Texture() = 0;

	virtual rgb value(const Vec2& uv , const Vec3& p) const=0;
	virtual Vec3 bumpNormal(const Vec2& uv, const Vec3& p, const Vec3& n, 
					const Vec3& dpdu, const Vec3& dpdv) const=0;
	DecalMode decalmode() const { return decal_mode; }
};

inline Texture::~Texture() {}

#endif
