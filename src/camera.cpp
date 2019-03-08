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
