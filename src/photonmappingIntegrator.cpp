#include "photonmappingIntegrator.h"

// static variable definitions
std::mutex PhotonmappingIntegrator::pmLock;	// lock

void PhotonmappingIntegrator::render(Image* img, const Camera* cam,
					int threadCount, bool showProgress) const
{
	int i, j;
	int width, height;
	SafeStack<HitPoint> hitpoints;

	width  = cam->image_width;
	height = cam->image_height;

	// Determine the pixels
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
			threads.push_back(std::thread(raytrace_singleSample, this, scene, cam,
							img, &pixels, &hitpoints));
		}

		else
		{
			threads.push_back(std::thread(raytrace_routine, this, scene, cam,
							img, &pixels, &hitpoints, cam->num_samples));
		}
	}

	if(showProgress) { utils::displayProgressBar(pixels, "Ray Tracing Pass", 60); }

	// Wait for them to complete
	for(i = 0; i < threadCount; i++) { threads[i].join(); }

	// Construct kdtree from hitpoints
	KDTree kdtree(hitpoints.sstack);

	// Free up memory from hitpoints
	hitpoints.sstack.clear();
	hitpoints.sstack.shrink_to_fit();

	Image photonmapImg(width, height);
	for(i = 0; i < times; i++)
	{
		int time = i + 1;

		// Determine photons
		SafeStack<Photon> photons(num_photons);
		samplePhotons(photons);
		
		// Photon Tracing
		photonmapImg = Image(width, height); // reset image on each pass

		// Create threads
		std::vector<std::thread> photonPassThreads;

		for(j = 0; j < threadCount; j++)
		{
			photonPassThreads.push_back(std::thread(photonmap_routine, this, scene,
							cam, &photonmapImg, &photons, time, &kdtree));
		}

		if(showProgress)
		{
			utils::displayProgressBar(photons, "Photon Mapping Pass "
								+ std::to_string(time), 60);
		}

		// Wait for them to complete
		for(j = 0; j < threadCount; j++) { photonPassThreads[j].join(); }

		// Radius Reduction, Flux Correction and Radiance Estimation
		radianceEstimate(kdtree.root, &photonmapImg, time);
	}

	// Add color from photon map
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			rgb photonmapColor;
			photonmapImg.get(i, j, photonmapColor);
			img->add(i, j, photonmapColor / cam->num_samples);
		}
	}
}

void PhotonmappingIntegrator::raytrace_routine(const PhotonmappingIntegrator* integrator,
						const Scene* scene, const Camera* cam, Image* img,
						SafeStack<std::pair<float, float>>* pixels,
						SafeStack<HitPoint>* hitpoints,
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
								cam->tonemap, hitpoints,
								false, Vec2(i, j));

			weightsum += r.weight;
			pixel_color += raycolor * r.weight;
		}
		pixel_color /= weightsum;

		img->set(i, j, pixel_color);
	}
}

void PhotonmappingIntegrator::raytrace_singleSample(const PhotonmappingIntegrator* integrator,
						const Scene* scene, const Camera* cam, Image* img,
						SafeStack<std::pair<float, float>>* pixels,
						SafeStack<HitPoint>* hitpoints)
{
	std::pair<float, float> currPixel;
	while(pixels->pop(currPixel))
	{
		int i, j;
		i = currPixel.first;
		j = currPixel.second;

		Ray r = cam->getRay(i, j, true);
		rgb raycolor = integrator->rayColor(r, scene->max_recursion_depth,
							cam->tonemap, hitpoints,
							false, Vec2(i, j));
		img->set(i, j, raycolor);
	}
}

void PhotonmappingIntegrator::photonmap_routine(const PhotonmappingIntegrator* integrator,
						const Scene* scene, const Camera* cam,
						Image* img, SafeStack<Photon>* photons, int time,
						KDTree* kdtree)
{
	Photon currPhoton;
	while(photons->pop(currPhoton))
	{
		integrator->tracePhoton(currPhoton, kdtree,
					scene->max_recursion_depth, cam->tonemap);
	}
}

int PhotonmappingIntegrator::samplePhotons(SafeStack<Photon>& photons) const
{
	// Loop over lights
	// For simplicity, consider lights send same amount of photons
	unsigned int n = scene->lights.size();
	int i = 0;

	for(const Light* light_ptr : scene->lights)
	{
		int sampleCount = (1.0f / float(n)) * num_photons;
		for(int c = 0; c < sampleCount; c++)
		{
			if(i == num_photons) { break; }

			Photon photon;
			if(light_ptr->samplePhoton(photon))
			{
				photons.sstack.push_back(photon);
				i++;
			}
		}
	}

	return i;
}

rgb PhotonmappingIntegrator::rayColor(const Ray& r, int recursion_depth,
			const Tonemap& tonemap, SafeStack<HitPoint>* hitpoints,
			bool nonluminous, const Vec2& ij) const
{
	rgb rcolor(0.0f, 0.0f, 0.0f);
	float tmax = FLT_MAX;		// Note: tmax, tmin, time can be made an argument
	float tmin = scene->shadow_ray_epsilon;
	float time = 0.0f;
	HitRecord record;
	DecalMode decal_mode;
	bool textured;

	if(scene->bvh->hit(r, tmin, tmax, time, record, scene->cullFace, nonluminous))
	{
		if(record.material.luminous)	// ray hit a luminous object
		{
			return record.color;
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

		// Loop over lights
		for(const Light* light_ptr : scene->lights)
		{
			SampleLight sampledLight;
			bool notShadow = light_ptr->sampleLight(scene, r, record,
								sampledLight, nonluminous);

			if(notShadow)
			{
				// Diffuse and Specular shading
				rcolor += record.material.brdf->value(r, record,
									sampledLight);
			}
		}

		// Add color from reflections
		rcolor += reflectionColor(r, record, recursion_depth, tonemap, hitpoints, ij);

		// Add color from refractions
		rcolor += refractionColor(r, record, recursion_depth, tonemap, hitpoints, ij);

		// Store hit points on diffuse surfaces
		handleHitPoints(r, record, hitpoints, ij);

		return rcolor;
	}

	else if(r.primary)
	{
		return backgroundColor(ij, r.direction());
	}

	return rgb();
}

void PhotonmappingIntegrator::tracePhoton(const Photon& photon, KDTree* kdtree,
					int recursion_depth, const Tonemap& tonemap) const
{
	if(recursion_depth <= 0) { return; }

	rgb rcolor(0.0f, 0.0f, 0.0f);
	float tmax = FLT_MAX;		// Note: tmax, tmin, time can be made an argument
	float tmin = scene->shadow_ray_epsilon;
	float time = 0.0f;
	HitRecord record;
	DecalMode decal_mode;
	bool textured;

	if(scene->bvh->hit(photon.path, tmin, tmax, time, record, scene->cullFace))
	{
		// Handle texture
		textured = handleTexture(record, decal_mode, rcolor);
		handleTonemap(tonemap, record, rcolor);
		if(textured && decal_mode == DecalMode::REPLACEALL)
		{
			photonPathTracing(photon, record, kdtree, recursion_depth, tonemap);
			return;

		}

		if(record.material.diffuse.length())	// material has diffuse component
		{
			PhotonHitRecord photonRecord(record.p, record.normal,
							photon.path, photon.power);
			accumulatePhoton(kdtree->root, record, photonRecord);

			// Path trace photon
			photonPathTracing(photon, record, kdtree, recursion_depth, tonemap);
		}

		// Photon reflection
		photonReflection(photon, record, kdtree, recursion_depth, tonemap);

		// Photon refraction
		photonRefraction(photon, record, kdtree, recursion_depth, tonemap);
	}
}

void PhotonmappingIntegrator::accumulatePhoton(KDTreeNode* node, const HitRecord& record,
						const PhotonHitRecord& photonRecord) const
{
	if(node == nullptr) { return; }

	if(Vec3(node->hitpoint.p - photonRecord.p).length() < node->hitpoint.Rx &&
		dot(node->hitpoint.normal, photonRecord.normal) > 0.0f)
	{

		float costheta_i = std::max(0.000001f, dot(record.normal,
						-photonRecord.path.direction()));
		SampleLight photonLight((photonRecord.power / costheta_i).asVec3(),
					-photonRecord.path.direction());

		rgb flux(record.material.brdf->value(photonRecord.path, record, photonLight));

		{
			std::lock_guard<std::mutex> lock(pmLock);
			node->hitpoint.tau += flux;
			node->hitpoint.Mx++;
		}
	}

	float eps(scene->shadow_ray_epsilon);
	Vec3 normal(0.0f);

	if(node->splitAxis == Axis::X)      { normal.setX(1.0f); }
	else if(node->splitAxis == Axis::Y) { normal.setY(1.0f); }
	else if(node->splitAxis == Axis::Z) { normal.setZ(1.0f); }
	float l(dot(Vec3(photonRecord.p - node->hitpoint.p), normal));

	if(l < eps || std::abs(l) < node->hitpoint.Rx + eps)
		accumulatePhoton(node->left, record, photonRecord);
	if(l > -eps || std::abs(l) < node->hitpoint.Rx + eps)
		accumulatePhoton(node->right, record, photonRecord);
}

rgb PhotonmappingIntegrator::backgroundColor(const Vec2& ij, const Vec3& direction) const
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

rgb PhotonmappingIntegrator::ambientColor(const HitRecord& record) const
{
	rgb Ia(scene->ambient_light);
	rgb ka(record.material.ambient);

	return ka * Ia;
}

rgb PhotonmappingIntegrator::diffuseColor(const Ray& r, const HitRecord& record,
					const SampleLight& slight) const
{
	rgb I(slight.radiance);
	rgb kd(record.material.diffuse);

	float costheta = std::max(0.0f, dot(slight.wi, record.normal));

	return kd * costheta * I;
}

rgb PhotonmappingIntegrator::specularColor(const Ray& r, const HitRecord& record,
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

rgb PhotonmappingIntegrator::reflectionColor(const Ray& r, const HitRecord& record,
						int recursion_depth,
						const Tonemap& tonemap,
						SafeStack<HitPoint>* hitpoints,
						const Vec2& ij) const
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
				recursion_depth - 1, tonemap, hitpoints, false, ij);
}

void PhotonmappingIntegrator::photonReflection(const Photon& photon, const HitRecord& record,
						KDTree* kdtree, int recursion_depth,
						const Tonemap& tonemap) const
{
	rgb km(record.material.mirror);
	Ray r(photon.path);

	if(!recursion_depth) { return; }
	if(!record.material.mirror.length()) { return; }

	Photon reflectedPhoton(r.reflectionRay(record, scene->shadow_ray_epsilon),
				km * photon.power);
	tracePhoton(reflectedPhoton, kdtree, recursion_depth - 1, tonemap);
}

rgb PhotonmappingIntegrator::refractionColor(const Ray& r, const HitRecord& record,
						int recursion_depth,
						const Tonemap& tonemap,
						SafeStack<HitPoint>* hitpoints,
						const Vec2& ij) const
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
							 tonemap, hitpoints,
							 false, ij);
			}
		}

		float R0  = ((n1 - n2)*(n1 - n2)) / ((n1 + n2)*(n1 + n2));
		float omc = 1.0f - c;
		float R   = R0 + (1.0f - R0) * omc * omc * omc * omc * omc;
		Ray transmissionRay(r.transmissionRay(nrecord, transmissionDirection,
					scene->intersection_test_epsilon));

		return rgb(k) * (R * rayColor(reflectionRay, recursion_depth - 1, tonemap,
						hitpoints, false, ij)
					+ (1.0f - R) * rayColor(transmissionRay,
								recursion_depth - 1,
								tonemap,
								hitpoints,
								false, ij));
	}

	return rgb();
}

void PhotonmappingIntegrator::photonRefraction(const Photon& photon, const HitRecord& record,
						KDTree* kdtree, int recursion_depth,
						const Tonemap& tonemap) const
{
	Vec3 transparency = record.material.transparency;
	Ray r(photon.path);

	if(!recursion_depth) { return; }

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
				tracePhoton(Photon(reflectionRay, rgb(k) * photon.power),
						kdtree, recursion_depth - 1, tonemap);
				return;
			}
		}

		float R0  = ((n1 - n2)*(n1 - n2)) / ((n1 + n2)*(n1 + n2));
		float omc = 1.0f - c;
		float R   = R0 + (1.0f - R0) * omc * omc * omc * omc * omc;
		Ray transmissionRay(r.transmissionRay(nrecord, transmissionDirection,
					scene->intersection_test_epsilon));

		if(rtmath::randf() > 0.5f)
		{
			tracePhoton(Photon(reflectionRay, R * photon.power),
					kdtree, recursion_depth - 1, tonemap);
		}

		else
		{
			tracePhoton(Photon(transmissionRay, (1.0f - R) * photon.power),
					kdtree, recursion_depth - 1, tonemap);
		}
	}
}

void PhotonmappingIntegrator::photonPathTracing(const Photon& photon, const HitRecord& record,
						KDTree* kdtree, int recursion_depth,
						const Tonemap& tonemap) const
{
	if(!recursion_depth) { return; }

	float pdf;
	rgb kd(record.material.diffuse);

	Vec3 dir(rtmath::randSampleOverHemisphere(record.uvw, false, pdf));
	Ray path(record.p + record.normal * scene->shadow_ray_epsilon, dir);
	Photon reflectedPhoton(path, kd * photon.power);

	tracePhoton(reflectedPhoton, kdtree, recursion_depth - 1, tonemap);
}


bool PhotonmappingIntegrator::handleTexture(HitRecord& record,
						DecalMode& decal_mode, rgb& color) const
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

bool PhotonmappingIntegrator::handleTonemap(const Tonemap& tonemap,
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
		// divide by 255, apply an exponent of gamma, multiply back with 255
		color = rgb((color / 255.0f).asVec3().comppow(tonemap.gamma) * 255.0f);
	}

	return true;
}

bool PhotonmappingIntegrator::handleHitPoints(const Ray& r, const HitRecord& record,
					SafeStack<HitPoint>* hitpoints, const Vec2& ij) const
{
	if(record.material.diffuse.length())	// material has diffuse component
	{
		std::lock_guard<std::mutex> lock(pmLock);
		hitpoints->push(HitPoint(record.p, record.normal, r, record.material.brdf,
						ij[0], ij[1], r.weight, r_initial));
		return true;
	}

	return false;
}
	
void PhotonmappingIntegrator::radianceEstimate(KDTreeNode* node, Image* img, int time) const
{
	if(!node) { return; }

	float numerator(node->hitpoint.Nx + alpha * node->hitpoint.Mx);
	float denominator(node->hitpoint.Nx + node->hitpoint.Mx);
	if(denominator)
	{
		float factor(numerator / denominator);

		// Radius Reduction
		node->hitpoint.Rx *= sqrt(factor);

		// Flux Correction
		node->hitpoint.tau *= factor;
		node->hitpoint.Nx += alpha * node->hitpoint.Mx;
		node->hitpoint.Mx = 0;

		// Radiance Evaluation
		float N_emitted_1 = 1.0f / (num_photons * time);
		float r2_1 = 1.0f / (node->hitpoint.Rx * node->hitpoint.Rx);
		rgb L = INV_PI * r2_1 * node->hitpoint.tau * N_emitted_1;

		img->add(node->hitpoint.i, node->hitpoint.j, L);
	}

	radianceEstimate(node->left, img, time);
	radianceEstimate(node->right, img, time);
}
