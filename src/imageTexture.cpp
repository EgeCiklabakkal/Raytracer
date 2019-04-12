#include "imageTexture.h"

ImageTexture::ImageTexture(Image* _image, float normalizer,
				InterpolationMode _interpolation_mode,
				DecalMode _decal_mode,
				TextureMode _texture_mode,
				bool _flipVertical) :
image(_image), normalizer(normalizer), interpolation_mode(_interpolation_mode), 
texture_mode(_texture_mode), flipVertical(_flipVertical)
{
	decal_mode = _decal_mode;
}

ImageTexture::~ImageTexture() {}

rgb ImageTexture::value(const Vec2& uv, const Vec3& p) const
{
	float i = uv[0] * image->nx;
	float j = uv[1] * image->ny;

	if(interpolation_mode == InterpolationMode::NEAREST)
	{
		return fetch((int)round(i), (int)round(j)) / normalizer;
	}

	// InterpolationMode::BILINIAR

	int ip = (int)floor(i);
	int iq = (int)floor(j);

	float dx = i - (float)ip;
	float dy = j - (float)iq;

	return  (fetch(ip  , iq  ) * (1 - dx) * (1 - dy) +
		 fetch(ip+1, iq  ) * (    dx) * (1 - dy) +
		 fetch(ip  , iq+1) * (1 - dx) * (    dy) +
		 fetch(ip+1, iq+1) * (    dx) * (    dy)) / normalizer;
}

rgb ImageTexture::fetch(int i, int j) const
{
	rgb color;
	
	if(texture_mode == TextureMode::CLAMP)
	{
		i = (i >= image->nx) ? (image->nx - 1) : i;
		j = (j >= image->ny) ? (image->ny - 1) : j;
		j = (flipVertical) ? (image->ny - 1) - j : j;
		image->get(i, j, color);
	}

	else	// TextureMode::REPEAT
	{
		i %= image->nx;
		j %= image->ny;
		j = (flipVertical) ? (image->ny - 1) - j : j;
		image->get(i, j, color);
	}

	return color;
}
