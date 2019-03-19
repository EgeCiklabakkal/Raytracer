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
			rx = rtmath::randf();
			ry = rtmath::randf();

			dx = (float(i) + rx) / float(nboxes);
			dy = (float(j) + ry) / float(nboxes);

			Vec3 s = q + pw*(x+dx)*across + ph*(y+dy)*up;
			rays.push_back(Ray(position, unitVector(s - position), 
			//		rtmath::gaussian2D(dx-0.5f, dy-0.5f, STD_DEV), true));
			1.0f, true));
		}
	}
}

void Camera::sampleDOFRays(float x, float y, std::vector<Ray>& rays, int num_samples) const
{
	float rx, ry, dx, dy, rx_cam, ry_cam;
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
			float t = (-focus_distance) / dot(-gaze, s - position);
			Vec3 p = position + (s - position) * t;

			// Jittered multisampling on camera
			int size = (int)std::ceil(aperture_size);
			for(int k = 0; k < size; k++)
			{
				for(int m = 0; m < size; m++)
				{
					rx_cam = rtmath::randf();
					ry_cam = rtmath::randf();

					e1 = (float(k) + rx_cam) / size - 0.5f;
					e2 = (float(m) + ry_cam) / size - 0.5f;

					Vec3 ls = position + aperture_size*(e1*across + e2*up);
					rays.push_back(Ray(ls, unitVector(p - ls), 
				//		rtmath::gaussian2D(dx-0.5f, dy-0.5f, STD_DEV),true));
				1.0f, true));
				}
			}
		}
	}
}
