#include "camera.h"

float randf(float start=0.0f, float end=1.0f);
float gaussianPDF(float x, float m=0.0f, float s=1.0f);

// Jittered multisampling
void Camera::sampleRays(float x, float y, std::vector<Ray>& rays, int num_samples) const
{
	
	int nboxes = sqrt(num_samples);
	for(int i = 0; i < nboxes; i++)
	{
		for(int j = 0; j < nboxes; j++)
		{
			float dx = (float(i) + randf(0.0f, 1.0f)) / float(nboxes);
			float dy = (float(j) + randf(0.0f, 1.0f)) / float(nboxes);

			Vec3 s = q + pw*(x+dx)*across + ph*(y+dy)*up;
			float dist = sqrt((0.5f - dx) * (0.5f - dx)
					 + (0.5f - dy) * (0.5f - dy));
			rays.push_back(Ray(position, unitVector(s - position), gaussianPDF(dist)));
		}
	}
}

// Helpers
float randf(float start, float end)
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(start, end);
	return dis(e);
}

float gaussianPDF(float x, float m, float s)
{
	static const float inv_sqrt_2pi = INV_SQRT_2PI;
	float a = (x - m) / s;

	return (inv_sqrt_2pi / s) * exp(-0.5f * a * a);
}
