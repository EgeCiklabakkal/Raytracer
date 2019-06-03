#ifndef _RAYTRACINGINTEGRATOR_H_
#define _RAYTRACINGINTEGRATOR_H_

#include "integrator.h"
#include "camera.h"
#include "helperUtils.h"

class RaytracingIntegrator : public Integrator
{
	public:

	virtual ~RaytracingIntegrator() {}

	virtual void render(const Scene* scene, Image* img, const Camera* cam,
				int threadCount, bool showProgress) const;
};

#endif
