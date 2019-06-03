#ifndef _PATHTRACINGINTEGRATOR_H_
#define _PATHTRACINGINTEGRATOR_H_

#include "integrator.h"
#include "camera.h"
#include "helperUtils.h"
#include "sphericalDirectionalLight.h"

class PathtracingIntegrator : public Integrator
{
	public:

	bool nextEventEstimation;
	bool importanceSampling;
	bool russianRoulette;

	PathtracingIntegrator(const Scene* _scene, bool _nextEventEstimation,
				bool _importanceSampling, bool _russianRoulette) :
	Integrator{_scene}, nextEventEstimation(_nextEventEstimation),
	importanceSampling(_importanceSampling), russianRoulette(_russianRoulette) {}

	virtual ~PathtracingIntegrator() {}

	virtual void render(Image* img, const Camera* cam,
				int threadCount, bool showProgress) const;
	static void pathtrace_routine(const PathtracingIntegrator* integrator,
					const Scene* scene, const Camera* cam, Image* img,
					SafeStack<std::pair<float, float>>* pixels, int num_samples);
	static void pathtrace_singleSample(const PathtracingIntegrator* integrator,
					const Scene* scene, const Camera* cam, Image* img,
					SafeStack<std::pair<float, float>>* pixels);

	rgb rayColor(const Ray& r, int recursion_depth, const Tonemap& tonemap,
			bool nonluminous=false, const Vec2& ij=Vec2(), bool indirect=false) const;
	rgb directLightingColor(const Scene* scene, const Ray& r,
				const HitRecord& record, bool nonluminous) const;
	rgb indirectLightingColor(const Scene* scene, const Ray& r, const HitRecord& record,
					int recursion_depth, const Tonemap& tonemap,
					bool nonluminous, const Vec2& ij) const;
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
