#ifndef _RTMATH_H_
#define _RTMATH_H_

// for gaussian calculation
#define INV_SQRT_2PI 0.3989422804014327
#define INV_2PI 0.159154943091895335768
#define STD_DEV 0.16666666666

#define INV_PI 0.3183098861837906715377
#define INV_8PI 0.039788735772973833942

#include <random>
#include <utility>
#include <float.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "vector3.h"

namespace rtmath
{
	float randf(float start=0.0f, float end=1.0f);
	float gaussianPDF(float x, float m=0.0f, float s=1.0f);
	float gaussian2D(float x, float y, float s=1.0f);

	// Transform helpers
	Vec3 transformLoc(const glm::mat4& left_op, const Vec3& right_op);
	Vec3 transformVec(const glm::mat4& left_op, const Vec3& right_op);
}

#endif
