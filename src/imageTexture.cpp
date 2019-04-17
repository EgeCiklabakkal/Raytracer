#include "imageTexture.h"

ImageTexture::ImageTexture(Image* _image, float normalizer,
				InterpolationMode _interpolation_mode,
				DecalMode _decal_mode,
				TextureMode _texture_mode,
				bool _bumpmap,
				float _bumpmapMultiplier,
				bool _flipVertical) :
image(_image), normalizer(normalizer), interpolation_mode(_interpolation_mode), 
texture_mode(_texture_mode), flipVertical(_flipVertical)
{
	decal_mode = _decal_mode;
	bumpmap    = _bumpmap;
	bumpmapMultiplier = _bumpmapMultiplier;
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

Vec3 ImageTexture::bumpNormal(const Vec2& uv, const Vec3& p, const Vec3& n,
				const Vec3& dpdu, const Vec3& dpdv) const
{
	rgb xy, x1y, xy1;
	float grayXY, grayX1Y, grayXY1;

	int i = floor(uv[0] * image->nx);
	int j = floor(uv[1] * image->ny);

	xy  = fetch(i, j);
	x1y = fetch(i+1, j);
	xy1 = fetch(i, j+1);

	grayXY  = ( xy.r() +  xy.g() +  xy.b()) / 3.0f;
	grayX1Y = (x1y.r() + x1y.g() + x1y.b()) / 3.0f;
	grayXY1 = (xy1.r() + xy1.g() + xy1.b()) / 3.0f;

	float dddu = (grayX1Y - grayXY) * bumpmapMultiplier;
	float dddv = (grayXY1 - grayXY) * bumpmapMultiplier;

	Vec3 dqdu = (dpdu + dddu * n);
	Vec3 dqdv = (dpdv + dddv * n);

	Vec3 nprime = unitVector(cross(dqdv, dqdu));

	nprime = (dot(nprime, n) > 0) ? nprime : -nprime;

	return unitVector(nprime);
}
