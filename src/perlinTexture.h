#ifndef _PERLINTEXTURE_H_
#define _PERLINTEXTURE_H_

#include <math.h>
#include "texture.h"
#include "perlinNoise.h"

enum class PerlinPattern { PATCHY, VEINY, TURBULANCE };

class PerlinTexture : public Texture
{
	public:

	rgb c0, c1;
	float scale;
	float normalizer;
	PerlinNoise perlin_noise;
	PerlinPattern perlin_pattern;

	PerlinTexture(float _scale=1.0f, float _normalizer=1.0f,
			DecalMode _decal_mode=DecalMode::REPLACEKD,
			PerlinPattern _perlin_pattern=PerlinPattern::PATCHY,
			bool _bumpmap=false, float _bumpmapMultiplier=1.0f)
	: scale(_scale), normalizer(_normalizer), perlin_pattern(_perlin_pattern)
	{
		c0 = rgb(1.0f);
		c1 = rgb(0.0f);
		decal_mode = _decal_mode;
		bumpmap = _bumpmap;
		bumpmapMultiplier = _bumpmapMultiplier;
	}

	PerlinTexture(const rgb& _c0, const rgb& _c1, float _scale, float _normalizer=1.0f,
			DecalMode _decal_mode=DecalMode::REPLACEKD,
			PerlinPattern _perlin_pattern=PerlinPattern::PATCHY,
			bool _bumpmap=false, float _bumpmapMultiplier=1.0f) :
	c0(_c0), c1(_c1), scale(_scale), normalizer(_normalizer), perlin_pattern(_perlin_pattern)
	{
		decal_mode = _decal_mode;
		bumpmap = _bumpmap;
		bumpmapMultiplier = _bumpmapMultiplier;
	}

	virtual ~PerlinTexture() {}

	rgb value(const Vec2& uv, const Vec3& p) const
	{
		float val;
		
		if(perlin_pattern == PerlinPattern::PATCHY)
		{
			val = (1.0f + perlin_noise.noise(p * scale)) / 2.0f;
		}

		else if(perlin_pattern == PerlinPattern::VEINY)
		{
			val = abs(perlin_noise.noise(p * scale));
		}

		else	// TURBULANCE
		{
			val = perlin_noise.turbulance(p * scale, 5.0f);
		}

		return (val*c0 + (1.0f - val)*c1) / normalizer;
	}

	Vec3 bumpNormal(const Vec2& uv, const Vec3& p, const Vec3& n,
				const Vec3& dpdu, const Vec3& dpdv) const
	{
		float gray, graydx, graydy, graydz;
		float dddx, dddy, dddz;
		static float epsilon = 1e-3;

		// Calculate gradient (partial derivatives of displacement function)
		Vec3 vecdx(p.x() + epsilon, p.y(), p.z());
		Vec3 vecdy(p.x(), p.y() + epsilon, p.z());
		Vec3 vecdz(p.x(), p.y(), p.z() + epsilon);

		gray   = value(uv, p).grayscale();
		graydx = value(uv, vecdx).grayscale();
		graydy = value(uv, vecdy).grayscale();
		graydz = value(uv, vecdz).grayscale();

		dddx = (graydx - gray) / epsilon;
		dddy = (graydy - gray) / epsilon;
		dddz = (graydz - gray) / epsilon;
		Vec3 g(dddx, dddy, dddz);	// gradient

		Vec3 gSurface(g - n * dot(n, g));

		return unitVector(n - gSurface);
	}
};

#endif
