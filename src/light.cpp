#include "light.h"

void AreaLight::sampleLight(const Ray& r, const HitRecord& record, 
				std::vector<SampleLight>& samples) const
{
	float e1, e2;
	float rx, ry;
	ONB onb;
	onb.initFromW(normal);
	int isize = int(size);
	int sample_count = isize * isize;

	// Jittered sampling on light
	for(int i = 0; i < isize; i++)
	{
		for(int j = 0; j < isize; j++)
		{
			rx = rtmath::randf();
			ry = rtmath::randf();

			e1 = (float(i) + rx) / size - 0.5f;
			e2 = (float(j) + ry) / size - 0.5f;
			Vec3 from(r.pointAtParameter(record.t));

			Vec3 ls(position + size*(e1*onb.u() + e2*onb.v()));
			Vec3 l(unitVector(from - ls));
			float declination = dot(l, normal);
			declination = (declination >= 0.0f) ? declination : -declination;
			Vec3 _I(intensity * declination * size * size / float(sample_count));

			samples.push_back(SampleLight(ls, _I, from));
		}
	}
}
