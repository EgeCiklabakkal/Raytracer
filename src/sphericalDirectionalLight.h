#ifndef _SPHERICALDIRECTIONALLIGHT_H_
#define _SPHERICALDIRECTIONALLIGHT_H_

#include "ONB.h"
#include "light.h"
#include "imageTexture.h"
#include "rtmath.h"

class Scene;

class SphericalDirectionalLight : public Light
{
	public:

	ImageTexture envMap;

	SphericalDirectionalLight(Image* image, float normalizer=1.0f,
				InterpolationMode interpolation_mode=InterpolationMode::NEAREST,
				TextureMode texture_mode=TextureMode::CLAMP,
				bool flipVertical=true);
	virtual ~SphericalDirectionalLight() { delete envMap.image; }

	virtual bool sampleLight(const Scene* scene, const Ray& r, const HitRecord& record,
					SampleLight& sampledLight, bool nonluminous) const;
	rgb value(Vec3 direction) const;
};

#endif
