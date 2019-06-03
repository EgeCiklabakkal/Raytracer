#include "camera.h"

void Camera::render(const Scene *scene, Image *img, int threadCount, bool showProgress) const
{
	integrator->render(scene, img, this, threadCount, showProgress);
}

// Jittered multisampling
void Camera::sampleRays(float x, float y, std::vector<Ray>& rays, int num_samples) const
{
	float rx, ry, dx, dy;	

	int nboxes = sqrt(num_samples);
	for(int i = 0; i < nboxes; i++)
	{
		for(int j = 0; j < nboxes; j++)
		{
			rx = rtmath::randf();
			ry = rtmath::randf();

			dx = (float(i) + rx) / float(nboxes);
			dy = (float(j) + ry) / float(nboxes);

			Vec3 s = q + pw*(x+dx)*across + ph*(y+dy)*up;
			rays.push_back(Ray(position, unitVector(s - position), 1.0f, true));
		}
	}
}

void Camera::sampleDOFRays(float x, float y, std::vector<Ray>& rays, int num_samples) const
{
	float rx, ry, dx, dy, t;
	float e1, e2;

	int nboxes = sqrt(num_samples);
	for(int i = 0; i < nboxes; i++)
	{
		for(int j = 0; j < nboxes; j++)
		{
			rx = rtmath::randf();
			ry = rtmath::randf();

			dx = (float(i) + rx) / float(nboxes);
			dy = (float(j) + ry) / float(nboxes);

			Vec3 s = q + pw*(x+dx)*across + ph*(y+dy)*up;
			t = (-focus_distance) / dot(-gaze, s - position);
			Vec3 p = position + (s - position) * t;

			e1 = rtmath::randf() - 0.5f;
			e2 = rtmath::randf() - 0.5f;

			Vec3 ls = position + aperture_size*(e1*across + e2*up);
			rays.push_back(Ray(ls, unitVector(p - ls), 1.0f, true));
		}
	}
}
