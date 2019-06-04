#include "pathtracingIntegrator.h"

void PathtracingIntegrator::render(Image* img, const Camera* cam,
					int threadCount, bool showProgress) const
{
	int i, j;
	int width, height;

	width  = cam->image_width;
	height = cam->image_height;

	// Determine the pixels;
	SafeStack<std::pair<float, float>> pixels(width * height);
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			pixels.sstack.push_back(std::make_pair((float)i, (float)j));
		}
	}

	// Create threads
	std::vector<std::thread> threads;

	for(i = 0; i < threadCount; i++)
	{
		if(cam->num_samples == 1)        // Single Sample
		{
			threads.push_back(std::thread(pathtrace_singleSample, this, scene, cam,
							img, &pixels));
		}

		else
		{
			threads.push_back(std::thread(pathtrace_routine, this, scene, cam,
							img, &pixels, cam->num_samples));
		}
	}

	if(showProgress)
	{
		utils::displayProgressBar(pixels);
	}

	// Wait for them to complete
	for(i = 0; i < threadCount; i++)
	{
		threads[i].join();
	}
}

void PathtracingIntegrator::pathtrace_routine(const PathtracingIntegrator* integrator,
						const Scene* scene, const Camera* cam, Image* img,
						SafeStack<std::pair<float, float>>* pixels,
						int num_samples)
{
	std::pair<float, float> currPixel;
	while(pixels->pop(currPixel))
	{
		int i, j;
		i = currPixel.first;
		j = currPixel.second;

		std::vector<Ray> sampledRays;
		if(cam->isDOF())
		{
			cam->sampleDOFRays(i, j, sampledRays, num_samples);
		}
		else
		{
			cam->sampleRays(i, j, sampledRays, num_samples);
		}

		rgb pixel_color(0.0f, 0.0f, 0.0f);
		float weightsum = 0.0f;
		for(const Ray& r : sampledRays)
		{
			rgb raycolor = integrator->rayColor(r, scene->max_recursion_depth,
								cam->tonemap, false, Vec2(i, j));

			weightsum += r.weight;
			pixel_color += raycolor * r.weight;
		}
		pixel_color /= weightsum;

		img->set(i, j, pixel_color);
	}
}

void PathtracingIntegrator::pathtrace_singleSample(const PathtracingIntegrator* integrator,
						const Scene* scene, const Camera* cam, Image* img,
						SafeStack<std::pair<float, float>>* pixels)
{
	std::pair<float, float> currPixel;
	while(pixels->pop(currPixel))
	{
		int i, j;
		i = currPixel.first;
		j = currPixel.second;

		Ray r = cam->getRay(i, j, true);
		rgb raycolor = integrator->rayColor(r, scene->max_recursion_depth,
							cam->tonemap, false, Vec2(i, j));
		img->set(i, j, raycolor);
	}
}

rgb PathtracingIntegrator::rayColor(const Ray& r, int recursion_depth,
					const Tonemap& tonemap, bool nonluminous,
					const Vec2& ij, bool indirect) const
{
	rgb rcolor(0.0f, 0.0f, 0.0f);
	float tmax = FLT_MAX;		// Note: tmax, tmin, time can be made an argument
	float tmin = scene->shadow_ray_epsilon;
	float time = 0.0f;
	HitRecord record;
	DecalMode decal_mode;
	bool textured;

	if(scene->bvh->hit(r, tmin, tmax, time, record, nonluminous))
	{
		if(!scene->cullFace || !r.primary || (dot(r.direction(), record.normal) < 0))
		{
			if(record.material.luminous)	// ray hit a luminous object
			{
				// if nextEventEstimation is applied,
				// return 0 radiance for indirect rays that hit a luminaire
				return (indirect && nextEventEstimation) ? rgb() : record.color;
			}

			// Handle texture
			textured = handleTexture(record, decal_mode, rcolor);
			handleTonemap(tonemap, record, rcolor);
			if(textured && decal_mode == DecalMode::REPLACEALL)
			{
				// no shading
				return rcolor;

			}// else, kd modified, continue shading

			rcolor = ambientColor(record);

			// if nextEventEstimation is applied, add color from direct lighting
			if(nextEventEstimation)
			{
				rcolor += directLightingColor(scene, r, record, nonluminous);
			}

			// Add color from indirect lighting
			rcolor += indirectLightingColor(scene, r, record, recursion_depth,
							tonemap, nonluminous, ij);

			// Add color from reflections
			rcolor += reflectionColor(r, record, recursion_depth, tonemap);
		}

		// Add color from refractions
		rcolor += refractionColor(r, record, recursion_depth, tonemap);

		return rcolor;
	}

	else if(r.primary)
	{
		return backgroundColor(ij, r.direction());
	}

	return rgb();
}

rgb PathtracingIntegrator::directLightingColor(const Scene* scene, const Ray& r,
						const HitRecord& record, bool nonluminous) const
{
	rgb color(0.0f);

	// Loop over lights
	for(const Light* light_ptr : scene->lights)
	{
		SampleLight sampledLight;
		bool notShadow = light_ptr->sampleLight(scene, r, record,
							sampledLight, nonluminous);

		if(notShadow)
		{
			// Diffuse and Specular shading
			color += record.material.brdf->value(r, record, sampledLight);
		}
	}

	return color;
}

rgb PathtracingIntegrator::indirectLightingColor(const Scene* scene, const Ray& r,
				const HitRecord& record, int recursion_depth,
				const Tonemap& tonemap, bool nonluminous, const Vec2& ij) const
{
	if(!recursion_depth) { return rgb(); }

	rgb color(0.0f);
	float pdf;
	Vec3 wi(rtmath::randSampleOverHemisphere(record.uvw_local,
							importanceSampling, pdf, record.M));
	Ray sample(record.p + record.normal * scene->shadow_ray_epsilon, wi);

	// Trace indirect ray
	rgb indirectRadiance = rayColor(sample, recursion_depth - 1, tonemap, nonluminous, ij, true);

	if(!indirectRadiance.isBlack())
	{
		SampleLight indirectLight(indirectRadiance.asVec3(), wi);
		color = record.material.brdf->value(r, record, indirectLight);
	}

	return color / pdf;
}

rgb PathtracingIntegrator::backgroundColor(const Vec2& ij, const Vec3& direction) const
{
	if(scene->hasBackgroundTexture)
	{
		int i = int(ij[0]) % scene->background_texture.nx;
		int j = int(ij[1]) % scene->background_texture.ny;

		rgb color;
		scene->background_texture.get(i, j, color);

		return color;
	}

	if(scene->hasSphericalDirectionalLight)
	{
		return scene->sd_light->value(direction);
	}

	return scene->background_color;
}

rgb PathtracingIntegrator::ambientColor(const HitRecord& record) const
{
	rgb Ia(scene->ambient_light);
	rgb ka(record.material.ambient);

	return ka * Ia;
}

rgb PathtracingIntegrator::diffuseColor(const Ray& r, const HitRecord& record,
					const SampleLight& slight) const
{
	rgb I(slight.radiance);
	rgb kd(record.material.diffuse);

	float costheta = std::max(0.0f, dot(slight.wi, record.normal));

	return kd * costheta * I;
}

rgb PathtracingIntegrator::specularColor(const Ray& r, const HitRecord& record,
					const SampleLight& slight) const
{
	rgb I(slight.radiance);
	rgb ks(record.material.specular);
	float phong_exp = record.material.phong_exponent;

	Vec3 x = record.p;
	Vec3 wo(unitVector(r.origin() - x));
	Vec3 h(slight.wi + wo);

	float costheta = std::max(0.0f, dot(record.normal, unitVector(h)));

	return ks * (pow(costheta, phong_exp)) * I;
}

rgb PathtracingIntegrator::reflectionColor(const Ray& r, const HitRecord& record,
						int recursion_depth, const Tonemap& tonemap) const
{
	rgb km(record.material.mirror);

	if(!recursion_depth)
	{
		return rgb();
	}

	if(!record.material.mirror.length())
	{
		return rgb();
	}

	return km * rayColor(r.reflectionRay(record, scene->shadow_ray_epsilon),
				recursion_depth - 1, tonemap);
}

rgb PathtracingIntegrator::refractionColor(const Ray& r, const HitRecord& record,
						int recursion_depth, const Tonemap& tonemap) const
{
	Vec3 transparency = record.material.transparency;

	if(!recursion_depth)
	{
		return rgb();
	}

	if(transparency.length())	// if p is on a dielectric
	{
		Vec3 transmissionDirection;
		Vec3 k;
		float c, n1, n2;
		Ray reflectionRay = r.reflectionRay(record, scene->intersection_test_epsilon);
		n1 = DEFAULT_AIR_REFRACTION_INDEX;
		n2 = record.material.refraction_index;
		HitRecord nrecord(record);

		if(dot(r.direction(), record.normal) < 0)
		{
			r.refract(record, std::pair<float, float>(n1, n2), transmissionDirection);
			c = -dot(r.direction(), record.normal);
			k = Vec3(1.0f, 1.0f, 1.0f);
		}

		else
		{
			Vec3 a(transparency);
			k = Vec3(pow(a.x(), record.t), pow(a.y(), record.t), pow(a.z(), record.t));

			nrecord.normal = -nrecord.normal;
			if(r.refract(nrecord, std::pair<float, float>(n2, n1),
				transmissionDirection))
			{
				c = dot(transmissionDirection, record.normal);
			}

			else
			{
				return rgb(k) * rayColor(reflectionRay,
							 recursion_depth - 1,
							 tonemap);
			}
		}

		float R0  = ((n1 - n2)*(n1 - n2)) / ((n1 + n2)*(n1 + n2));
		float omc = 1.0f - c;
		float R   = R0 + (1.0f - R0) * omc * omc * omc * omc * omc;
		Ray transmissionRay(r.transmissionRay(nrecord, transmissionDirection,
					scene->intersection_test_epsilon));

		return rgb(k) * (R * rayColor(reflectionRay, recursion_depth - 1, tonemap)
					+ (1.0f - R) * rayColor(transmissionRay,
								recursion_depth - 1,
								tonemap));
	}

	return rgb();
}

bool PathtracingIntegrator::handleTexture(HitRecord& record, DecalMode& decal_mode, rgb& color) const
{
	if(!record.texture)
	{
		// has no texture
		return false;
	}

	Texture *texture = record.texture;
	decal_mode = texture->decalmode();

	if(decal_mode == DecalMode::REPLACEKD)
	{
		record.material.diffuse = texture->value(record.uv, record.p).asVec3();
	}

	else if(decal_mode == DecalMode::BLENDKD)
	{
		record.material.diffuse += texture->value(record.uv, record.p).asVec3();
		record.material.diffuse /= 2.0f;
	}

	else
	{
		color = texture->value(record.uv, record.p);
	}

	if(texture->bumpmap)
	{
		Vec3 dpdu = rtmath::transformVec(record.M, record.bump.dpdu);
		Vec3 dpdv = rtmath::transformVec(record.M, record.bump.dpdv);
		record.normal = texture->bumpNormal(record.uv, record.p, record.normal, dpdu, dpdv);
	}

	return true;
}

bool PathtracingIntegrator::handleTonemap(const Tonemap& tonemap,
						HitRecord& record, rgb& color) const
{
	if(tonemap.tonemap_mode == TonemapMode::NOTONEMAP)
	{
		return false;
	}

	if(record.material.degamma)
	{
		record.material.ambient = record.material.ambient.comppow(tonemap.gamma);
		record.material.diffuse = record.material.diffuse.comppow(tonemap.gamma);
		record.material.specular = record.material.specular.comppow(tonemap.gamma);
		record.material.mirror = record.material.mirror.comppow(tonemap.gamma);
	}

	if(record.texture && record.texture->degamma)
	{
		color = rgb(color.asVec3().comppow(tonemap.gamma));
	}

	return true;
}
