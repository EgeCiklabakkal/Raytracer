#ifndef _RTMATH_H_
#define _RTMATH_H_

// for gaussian calculation
#define INV_SQRT_2PI 0.3989422804014327
#define INV_2PI 0.159154943091895335768
#define STD_DEV 0.16666666666

#include <random>
#include <utility>

namespace rtmath
{
	float randf(float start=0.0f, float end=1.0f);
	float gaussianPDF(float x, float m=0.0f, float s=1.0f);
	float gaussian2D(float x, float y, float s=1.0f);
}

#endif
