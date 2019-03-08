#include "rtmath.h"

float rtmath::randf(float start, float end)
{
        static std::default_random_engine e;
        static std::uniform_real_distribution<> dis(start, end);
        return dis(e);
}

float rtmath::gaussianPDF(float x, float m, float s)
{
        static const float inv_sqrt_2pi = INV_SQRT_2PI;
        float a = (x - m) / s;

        return (inv_sqrt_2pi / s) * exp(-0.5f * a * a);
}

float rtmath::gaussian2D(float x, float y, float m, float s)
{
	// x,y ∊ [-0.5, 0.5]
	static const float inv_2pi = INV_2PI;
	float a = (x*x + y*y) / (2*s*s);

	return (inv_2pi * s * s) * exp(-0.5f *a);
}
