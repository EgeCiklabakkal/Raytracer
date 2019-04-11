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
			PerlinPattern _perlin_pattern=PerlinPattern::PATCHY)
	: scale(_scale), normalizer(_normalizer), perlin_pattern(_perlin_pattern)
	{
		c0 = rgb(1.0f);
		c1 = rgb(0.0f);
		decal_mode = _decal_mode;
	}

	PerlinTexture(const rgb& _c0, const rgb& _c1, float _scale, float _normalizer=1.0f,
			DecalMode _decal_mode=DecalMode::REPLACEKD,
			PerlinPattern _perlin_pattern=PerlinPattern::PATCHY) :
	c0(_c0), c1(_c1), scale(_scale), normalizer(_normalizer), perlin_pattern(_perlin_pattern)
	{ decal_mode = _decal_mode; }

	virtual ~PerlinTexture() {}

	rgb value(const Vec2& uv, const Vec3& p, bool reverseY) const
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
};

#endif
