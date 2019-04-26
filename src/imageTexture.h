#ifndef _IMAGETEXTURE_H_
#define _IMAGETEXTURE_H_

#include <string>
#include <math.h>
#include "texture.h"
#include "image.h"

enum class InterpolationMode { NEAREST, BILINEAR };
enum class TextureMode { CLAMP, REPEAT };

class ImageTexture : public Texture
{
	public:

	Image *image;
	float normalizer;
	InterpolationMode interpolation_mode;
	TextureMode texture_mode;
	bool flipVertical;

	ImageTexture(Image* _image, float normalizer=255.0f,
			InterpolationMode _interpolation_mode=InterpolationMode::NEAREST,
			DecalMode _decal_mode=DecalMode::REPLACEKD,
			TextureMode _texture_mode=TextureMode::CLAMP,
			bool _bumpmap=false,
			float _bumpmapMultiplier=1.0f,
			bool flipVertical=true,
			bool _degamma=false);

	virtual ~ImageTexture();

	rgb value(const Vec2& uv, const Vec3& p) const;
	rgb fetch(int i, int j) const;
        Vec3 bumpNormal(const Vec2& uv, const Vec3& p, const Vec3& n,
				const Vec3& dpdu, const Vec3& dpdv) const;
};

#endif
