#ifndef _RAYTRACINGINTEGRATOR_H_
#define _RAYTRACINGINTEGRATOR_H_

#include "integrator.h"
#include "camera.h"
#include "helperUtils.h"
#include "sphericalDirectionalLight.h"

class RaytracingIntegrator : public Integrator
{
	public:

	RaytracingIntegrator(const Scene* _scene) : Integrator{_scene} {}
	virtual ~RaytracingIntegrator() {}

	virtual void render(Image* img, const Camera* cam,
				int threadCount, bool showProgress) const;
	static void raytrace_routine(const RaytracingIntegrator* integrator,
					const Scene* scene, const Camera* cam, Image* img,
					SafeStack<std::pair<float, float>>* pixels, int num_samples);
	static void raytrace_singleSample(const RaytracingIntegrator* integrator,
					const Scene* scene, const Camera* cam, Image* img,
					SafeStack<std::pair<float, float>>* pixels);

	rgb rayColor(const Ray& r, int recursion_depth, const Tonemap& tonemap,
			bool nonluminous=false, const Vec2& ij=Vec2()) const;
	rgb backgroundColor(const Vec2& ij, const Vec3& direction) const;
	rgb ambientColor(const HitRecord& record) const;
	rgb diffuseColor(const Ray& r, const HitRecord& record, const SampleLight& slight) const;
	rgb specularColor(const Ray& r, const HitRecord& record, const SampleLight& slight) const;
	rgb reflectionColor(const Ray& r, const HitRecord& record,
				int recursion_depth, const Tonemap& tonemap) const;
	rgb refractionColor(const Ray& r, const HitRecord& record,
				int recursion_depth, const Tonemap& tonemap) const;
	bool handleTexture(HitRecord& record, DecalMode& decal_mode, rgb& color) const;
	bool handleTonemap(const Tonemap& tonemap, HitRecord& record, rgb& color) const;
};

#endif
