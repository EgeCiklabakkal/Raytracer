#include "camera.h"

// Jittered multisampling
void Camera::sampleRays(float x, float y, std::vector<Ray>& rays, int num_samples) const
{
	float rx, ry, dx, dy;	

	int nboxes = sqrt(num_samples);
	for(int i = 0; i < nboxes; i++)
	{
		for(int j = 0; j < nboxes; j++)
		{
			rx = rtmath::randf(0.0f, 1.0f);
			ry = rtmath::randf(0.0f, 1.0f);

			dx = (float(i) + rx) / float(nboxes);
			dy = (float(j) + ry) / float(nboxes);

			Vec3 s = q + pw*(x+dx)*across + ph*(y+dy)*up;
			rays.push_back(Ray(position, unitVector(s - position), 
					rtmath::gaussian2D(rx-0.5f, ry-0.5f)));
		}
	}
}

void Camera::sampleDOFRays(float x, float y, std::vector<Ray>& rays, int num_samples) const
{
	float rx, ry, dx, dy;
	float e1, e2;

	int nboxes = sqrt(num_samples);
	for(int i = 0; i < nboxes; i++)
	{
		for(int j = 0; j < nboxes; j++)
		{
			rx = rtmath::randf(0.0f, 1.0f);
			ry = rtmath::randf(0.0f, 1.0f);

			dx = (float(i) + rx) / float(nboxes);
			dy = (float(j) + ry) / float(nboxes);

			Vec3 s = q + pw*(x+dx)*across + ph*(y+dy)*up;
			float t = (-focus_distance) / dot(-gaze, s - position);
			Vec3 p = position + (s - position) * t;
			e1 = rtmath::randf(-0.5f, 0.5f);
			e2 = rtmath::randf(-0.5f, 0.5f);
			Vec3 ls = position + aperture_size*(e1*across + e2*up);

			rays.push_back(Ray(ls, unitVector(p - ls), 
					rtmath::gaussian2D(rx-0.5f, ry-0.5f)));
		}
	}
}
