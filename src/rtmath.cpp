#include "rtmath.h"

// You should keep using the same range throughout the program
// Because of the static variables
float rtmath::randf(float start, float end)
{
        //thread_local static std::default_random_engine e;
        thread_local static std::mt19937 e;
        thread_local static std::uniform_real_distribution<> dis(start, end);
        return dis(e);
}

float rtmath::gaussianPDF(float x, float m, float s)
{
        static const float inv_sqrt_2pi = INV_SQRT_2PI;
        float a = (x - m) / s;

        return (inv_sqrt_2pi / s) * exp(-0.5f * a * a);
}

float rtmath::gaussian2D(float x, float y, float s)
{
	// x,y ∊ [-0.5, 0.5]
	static const float inv_2pi = INV_2PI;
	float a = (x*x + y*y) / (2*s*s);

	return (inv_2pi) * exp(-a) / s*s;
}

Vec3 rtmath::randSampleOverHemisphere(const ONB& uvw, bool importanceSampling,
					float& pdf, const glm::mat4& trans)
{
	// w is along the surface normal
	static const float _2pi = 2.0f * M_PI;
	float e1 = randf();
	float e2 = randf();

	float phi = e1 * _2pi;
	float theta = (importanceSampling) ? asin(sqrt(e2)) : acos(e2);

	Vec3 sample = cos(theta) * uvw.W +
		      sin(theta) * cos(phi) * uvw.U +
		      sin(theta) * sin(phi) * uvw.V;
	sample = transformVec(trans, unitVector(sample));

	pdf = (importanceSampling) ? cos(theta) * INV_PI : INV_2PI;
	return unitVector(sample);
}

Vec3 rtmath::transformLoc(const glm::mat4& right_op, const Vec3& left_op)
{
	glm::vec4 v(left_op.x(), left_op.y(), left_op.z(), 1.0f);
	glm::vec4 res4 = right_op * v;

	return Vec3(res4[0], res4[1], res4[2]);	// No need for perspective divide
}

Vec3 rtmath::transformVec(const glm::mat4& right_op, const Vec3& left_op)
{
	glm::vec4 v(left_op.x(), left_op.y(), left_op.z(), 0.0f);
	glm::vec4 res4 = right_op * v;

	return Vec3(res4[0], res4[1], res4[2]);	// No need for perspective divide
}
