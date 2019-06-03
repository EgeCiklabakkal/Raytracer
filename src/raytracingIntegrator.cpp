#include "raytracingIntegrator.h"

void RaytracingIntegrator::render(const Scene* scene, Image* img, const Camera* cam,
					int threadCount, bool showProgress) const
{
	int i, j;
	int width, height;

	width  = cam->image_width;
	height = cam->image_height;

	// Determine the pixels;
	SafeStack<std::pair<float, float>> pixels(width * height);
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			pixels.sstack.push_back(std::make_pair((float)i, (float)j));
		}
	}

	// Create threads
	std::vector<std::thread> threads;

	for(i = 0; i < threadCount; i++)
	{
		if(cam->num_samples == 1)        // Single Sample
		{
			threads.push_back(std::thread(Scene::raytrace_singleSample,
					scene, cam, img, &pixels));
		}

		else
		{
			threads.push_back(std::thread(Scene::raytrace_routine,
					scene, cam, img, &pixels, cam->num_samples));
		}
	}

	if(showProgress)
	{
		utils::displayProgressBar(pixels);
	}

	// Wait for them to complete
	for(i = 0; i < threadCount; i++)
	{
		threads[i].join();
	}
}
