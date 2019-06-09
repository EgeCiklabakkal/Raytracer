#ifndef _PHOTONMAPPINGINTEGRATOR_H_
#define _PHOTONMAPPINGINTEGRATOR_H_

#include "integrator.h"
#include "camera.h"
#include "helperUtils.h"
#include "sphericalDirectionalLight.h"
#include "KDTree.h"
#include "photon.h"
#include "photonHitRecord.h"

class PhotonmappingIntegrator : public Integrator
{
	public:

	float alpha;
	int times;			// how many progressive photon mapping passes
	long long int num_photons;	// how many photons should be emitted
	float r_initial;		// Initial radius for photon mapping step

	PhotonmappingIntegrator(const Scene* _scene, float _alpha=0.7f,
				int _times=16, long long int _num_photons=100000,
				float _r_initial=0.5f) :
	Integrator{_scene}, alpha(_alpha), times(_times),
	num_photons(_num_photons), r_initial(_r_initial) {}
	virtual ~PhotonmappingIntegrator() {}

	virtual void render(Image* img, const Camera* cam,
				int threadCount, bool showProgress) const;
	static void photonmap_routine(const PhotonmappingIntegrator* integrator,
					const Scene* scene, const Camera* cam, Image* img,
					SafeStack<std::pair<float, float>>* pixels,
					SafeStack<HitPoint>* hitpoints, int num_samples);
	static void photonmap_singleSample(const PhotonmappingIntegrator* integrator,
					const Scene* scene, const Camera* cam, Image* img,
					SafeStack<std::pair<float, float>>* pixels,
					SafeStack<HitPoint>* hitpoints);
	void photonMapping(Image* img, SafeStack<Photon>& photons, int time,
				KDTree& kdtree, const Tonemap& tonemap) const;
	int samplePhotons(SafeStack<Photon>& photons) const;

	rgb rayColor(const Ray& r, int recursion_depth, const Tonemap& tonemap,
			SafeStack<HitPoint>* hitpoints,
			bool nonluminous=false, const Vec2& ij=Vec2()) const;
	void tracePhoton(const Photon& photon, KDTree& kdtree,
				int recursion_depth, const Tonemap& tonemap) const;
	void photonReflection(const Photon& photon, const HitRecord& record, 
				KDTree& kdtree, int recursion_depth,
				const Tonemap& tonemap) const;
	void photonRefraction(const Photon& photon, const HitRecord& record,
					KDTree& kdtree, int recursion_depth,
					const Tonemap& tonemap) const;
	void photonPathTracing(const Photon& photon, const HitRecord& record,
					KDTree& kdtree, int recursion_depth,
					const Tonemap& tonemap) const;
	void accumulatePhoton(KDTreeNode* node, const HitRecord& record,
					const PhotonHitRecord& photonRecord) const;
	rgb backgroundColor(const Vec2& ij, const Vec3& direction) const;
	rgb ambientColor(const HitRecord& record) const;
	rgb diffuseColor(const Ray& r, const HitRecord& record, const SampleLight& slight) const;
	rgb specularColor(const Ray& r, const HitRecord& record, const SampleLight& slight) const;
	rgb reflectionColor(const Ray& r, const HitRecord& record, int recursion_depth,
				const Tonemap& tonemap, SafeStack<HitPoint>* hitpoints) const;
	rgb refractionColor(const Ray& r, const HitRecord& record, int recursion_depth,
				const Tonemap& tonemap, SafeStack<HitPoint>* hitpoints) const;
	bool handleTexture(HitRecord& record, DecalMode& decal_mode, rgb& color) const;
	bool handleTonemap(const Tonemap& tonemap, HitRecord& record, rgb& color) const;
	bool handleHitPoints(const Ray& r, const HitRecord& record,
				SafeStack<HitPoint>* hitpoints, const Vec2& ij) const;
	void radianceEstimate(KDTreeNode* node, Image* img, int time) const;
};

#endif
