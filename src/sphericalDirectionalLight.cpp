#include "sphericalDirectionalLight.h"

SphericalDirectionalLight::SphericalDirectionalLight(Image* image, float normalizer,
						InterpolationMode interpolation_mode,
						TextureMode texture_mode,
						bool flipVertical) :
envMap{image, normalizer, interpolation_mode, DecalMode::REPLACEKD,
	texture_mode, false, 1.0f, flipVertical, false}
{}

bool SphericalDirectionalLight::sampleLight(const Scene* scene, const Ray& r,
				const HitRecord& record, SampleLight& sampledLight,
				bool nonluminous) const
{
	static float _PI_2 = 2.0f * M_PI;
	float phi = _PI_2 * rtmath::randf();
	float theta = acos(rtmath::randf());

	ONB onb;
	onb.initFromV(record.normal);
	Vec3 l = cos(theta) * onb.V +
		 sin(theta) * cos(phi) * onb.U +
		 sin(theta) * sin(phi) * onb.W;

	l.makeUnitVector();
	Ray shadow_ray(record.p + record.normal * scene->shadow_ray_epsilon, l);
	if(scene->bvh->shadowHit(shadow_ray, 0.0f, FLT_MAX, r.time, nonluminous)) // In shadow
	{
		return false;
	}	

	Vec3 L = value(l).asVec3();

	sampledLight = SampleLight(L * _PI_2, l);
	return true;
}

rgb SphericalDirectionalLight::value(Vec3 direction) const
{
	Vec3 l = unitVector(direction);

	// Calculate global phi/theta and u, v
	float phi_g   = atan2(l.z(), l.x());
	float theta_g = acos(l.y());
	float u = (-phi_g + M_PI) * INV_2PI;
	float v = (theta_g) * INV_PI;

	return envMap.value(Vec2(u, v), Vec3());
}
