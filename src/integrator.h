#ifndef _INTEGRATOR_H_
#define _INTEGRATOR_H_

#include "scene.h"
#include "image.h"

class Camera;

class Integrator
{
	public:

	const Scene *scene;

	Integrator(const Scene* _scene) : scene(_scene) {}
	virtual ~Integrator() = 0;

	virtual void render(Image* img, const Camera* cam,
				int threadCount, bool showProgress) const=0;
};

inline Integrator::~Integrator() {}

#endif
