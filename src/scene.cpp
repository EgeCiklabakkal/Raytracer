#include "utils.h"

void Scene::renderImages(int threadCount, bool showProgress)
{
	int width, height;
	HitRecord record;

	for(const Camera& cam: cameras)
        {
		int i, j;
		auto const prior = std::chrono::system_clock::now();

                width  = cam.image_width;
                height = cam.image_height;

                Image img(width, height);	

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
			if(cam.num_samples == 1)	// Single Sample
			{
				threads.push_back(std::thread(raytrace_singleSample, 
						this, &cam, &img, &pixels));
			}

			else
			{
				threads.push_back(std::thread(raytrace_routine, 
						this, &cam, &img, &pixels, cam.num_samples));
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

                img.imwrite(cam.image_name, cam.tonemap);

		// Time Duration Print
		auto const latter = std::chrono::system_clock::now();
		utils::printTimingInfo(cam.image_name, prior, latter);
        }
}

void Scene::raytrace_routine(Scene* scene, const Camera* cam, Image* img, 
		SafeStack<std::pair<float, float>>* pixels, int num_samples)
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
			rgb raycolor = scene->rayColor(r, scene->max_recursion_depth,
							cam->tonemap, Vec2(i, j));

			weightsum += r.weight;
			pixel_color += raycolor * r.weight;
		}
		pixel_color /= weightsum;

		img->set(i, j, pixel_color);
	}
}

void Scene::raytrace_singleSample(Scene* scene, const Camera* cam, Image* img, 
		SafeStack<std::pair<float, float>>* pixels)
{
	std::pair<float, float> currPixel;
	while(pixels->pop(currPixel))
	{
		int i, j;
		i = currPixel.first;
		j = currPixel.second;

		Ray r = cam->getRay(i, j, true);
		rgb raycolor = scene->rayColor(r, scene->max_recursion_depth,
						cam->tonemap, Vec2(i, j));
		img->set(i, j, raycolor);
	}
}

rgb Scene::rayColor(const Ray& r, int recursion_depth, const Tonemap& tonemap, const Vec2& ij) const
{
	rgb rcolor(0.0f, 0.0f, 0.0f);
	float tmax = 100000.0f;		// Note: tmax, tmin, time can be made an argument
	float tmin = shadow_ray_epsilon;		
	float time = 0.0f;
	HitRecord record;
	DecalMode decal_mode;
	bool textured;

	if(bvh->hit(r, tmin, tmax, time, record))
	{
		if(!cullFace || !r.primary || (dot(r.direction(), record.normal) < 0))
		{
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
			for(const Light* light_ptr : this->lights)
			{
				SampleLight sampledLight;
				bool notShadow = light_ptr->sampleLight(this, r,
									record, sampledLight);

				if(notShadow)
				{
					// Diffuse and Specular shading
					rcolor += record.material.brdf->value(r, record,
										sampledLight);
				}
			}

			// Add color from reflections
			rcolor += reflectionColor(r, record, recursion_depth, tonemap);
		}

		// Add color from refractions
		rcolor += refractionColor(r, record, recursion_depth, tonemap);

		// color is in bytes
		return rcolor;
	}

	else if(r.primary)
	{
		return backgroundColor(ij);
	}

	return rgb();
}

rgb Scene::backgroundColor(const Vec2 ij) const
{
	if(!hasBackgroundTexture)
	{
		return background_color;
	}

	int i = int(ij[0]) % background_texture.nx;
	int j = int(ij[1]) % background_texture.ny;

	rgb color;
	background_texture.get(i, j, color);

	return color;	
}

rgb Scene::ambientColor(const HitRecord& record) const
{
	rgb Ia(ambient_light);
	rgb ka(record.material.ambient);

	return ka * Ia;
}

rgb Scene::diffuseColor(const Ray& r, const HitRecord& record, const SampleLight& slight) const
{
	rgb I(slight.intensity);
	rgb kd(record.material.diffuse);

	float costheta = std::max(0.0f, dot(slight.wi, record.normal));

	return kd * costheta * I;
}

rgb Scene::specularColor(const Ray& r, const HitRecord& record, const SampleLight& slight) const
{
	rgb I(slight.intensity);
	rgb ks(record.material.specular);
	float phong_exp = record.material.phong_exponent;

	Vec3 x = record.p;
	Vec3 wo(unitVector(r.origin() - x));
	Vec3 h(slight.wi + wo);

	float costheta = std::max(0.0f, dot(record.normal, unitVector(h)));

	return ks * (pow(costheta, phong_exp)) * I;
}

rgb Scene::reflectionColor(const Ray& r, const HitRecord& record,
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

	return km * rayColor(r.reflectionRay(record, shadow_ray_epsilon), 	
				recursion_depth - 1, tonemap);
}

rgb Scene::refractionColor(const Ray& r, const HitRecord& record,
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
		Ray reflectionRay = r.reflectionRay(record, intersection_test_epsilon);
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
		float omc = 1 - c;
		float R   = R0 + (1 - R0) * omc * omc * omc * omc * omc;
		Ray transmissionRay(r.transmissionRay(nrecord, transmissionDirection, 
					intersection_test_epsilon));

		return rgb(k) * (R * rayColor(reflectionRay, recursion_depth - 1, tonemap)
					+ (1 - R) * rayColor(	transmissionRay,
								recursion_depth - 1,
								tonemap));
	}

	return rgb();
}

bool Scene::handleTexture(HitRecord& record, DecalMode& decal_mode, rgb& color) const
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
		Vec3 dpdu = rtmath::transformVec(record.bump.M, record.bump.dpdu);
		Vec3 dpdv = rtmath::transformVec(record.bump.M, record.bump.dpdv);
		record.normal = texture->bumpNormal(record.uv, record.p, record.normal, dpdu, dpdv);
	}

	return true;
}

bool Scene::handleTonemap(const Tonemap& tonemap, HitRecord& record, rgb& color) const
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

Scene::~Scene()
{
	// Free lights
	for(Light* light : lights)
	{
		delete light;
	}
	lights.clear();

	// Free BRDFs
	for(BRDF* brdf : brdfs)
	{
		delete brdf;
	}
	brdfs.clear();

	// Free meshes
	for(Mesh* mesh : meshes)
	{
		delete mesh;
	}
	meshes.clear();

	// Free textures
	for(Texture* texture : textures)
	{
		delete texture;
	}
	textures.clear();

	// Free texture images
	for(Image* textureImage : textureImages)
	{
		delete textureImage;
	}
	textureImages.clear();

	// Free primary mesh BVHs
	for(Shape* primMeshBVH : primMeshBVHs)
	{
		delete primMeshBVH;
	}
	primMeshBVHs.clear();

	// Free scene BVH
	if(bvh)
	{
		delete bvh;
	}
}
