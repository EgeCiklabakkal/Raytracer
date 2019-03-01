#include "camera.h"

float randf(float start=0.0f, float end=1.0f);

void Camera::sampleRays(float x, float y, std::vector<Ray>& rays, int num_samples) const
{
	for(int i = 0; i < num_samples; i++)
	{
		float dx = randf(-0.5f, 0.5f);
		float dy = randf(-0.5f, 0.5f);

		Vec3 s = q + (pw*(x+dx) +pw/2)*across + (ph*(y+dy) + ph/2)*up;
		rays.push_back(Ray(position, unitVector(s - position)));
	}
}

// Helpers
float randf(float start, float end)
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(start, end);
	return dis(e);
}
