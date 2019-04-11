#include "utils.h"

void Scene::raytraceImages(int threadCount, bool showProgress)
{
	int width, height;
	HitRecord record;

	for(const Camera& cam: cameras)
        {
		auto const prior = std::chrono::system_clock::now();

                width  = cam.image_width;
                height = cam.image_height;

                Image img(width, height);

                // Determine the pixels;
                SafeStack<std::pair<float, float>> pixels(width * height);
                for(int i = 0; i < width; i++)
                {
                        for(int j = 0; j < height; j++)
                        {
                                pixels.sstack.push_back(std::make_pair((float)i, (float)j));
                        }
                } 

		// Create threads
		std::vector<std::thread> threads;

		for(int i = 0; i < threadCount; i++)
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
		for(int i = 0; i < threadCount; i++)
		{
			threads[i].join();
		}

                img.writePNG(cam.image_name);

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
			rgb raycolor = scene->rayColor(r, scene->max_recursion_depth);

			weightsum += r.weight;
			pixel_color += raycolor * r.weight;
		}
		pixel_color /= weightsum;
		pixel_color.clamp256();

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

		Ray r = cam->getRay(i, j);
		rgb raycolor = scene->rayColor(r, scene->max_recursion_depth);
		raycolor.clamp256();
		img->set(i, j, raycolor);
	}
}

rgb Scene::rayColor(const Ray& r, int recursion_depth, bool cullFace) const
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
		if(!r.primary || (cullFace && (dot(r.direction(), record.normal) < 0)))
		{
			// Handle texture
			textured = handleTexture(record, decal_mode, rcolor);
			if(textured && decal_mode == DecalMode::REPLACEALL)
			{
				// no shading
				return rcolor;

			}// else, kd modified, continue shading

			rcolor = ambientColor(record);

			// Loop over lights
			for(const Light* light_ptr : this->lights)
			{
				SampleLight sampledLight = light_ptr->sampleLight(r, record);
				Ray shadow_ray(r.shadowRay(record, sampledLight, 
						shadow_ray_epsilon));
				float tlight = shadow_ray.parameterAtPoint
							(sampledLight.position);

				if(!bvh->shadowHit(shadow_ray, 0.0f, tlight, time))
				{
					rcolor += diffuseColor(r, record, sampledLight) +
						specularColor(r, record, sampledLight);
				}
			}

			// Add color from reflections
			rcolor += reflectionColor(r, record, recursion_depth);
		}

		// Add color from refractions
		rcolor += refractionColor(r, record, recursion_depth);

		// color is in bytes
		return rcolor;
	}

	return background_color;
}

rgb Scene::ambientColor(const HitRecord& record) const
{
	rgb Ia(ambient_light.intensity);
	rgb ka(record.material.ambient);

	return ka * Ia;
}

rgb Scene::diffuseColor(const Ray& r, const HitRecord& record, const SampleLight& slight) const
{
	rgb I(slight.intensity);
	rgb kd(record.material.diffuse);

	Vec3 x = r.pointAtParameter(record.t);
	Vec3 wi(slight.position - x);
	float r2 = wi.squaredLength();
	wi.makeUnitVector();

	float costheta = std::max(0.0f, dot(wi, record.normal));

	return (kd * costheta * I) / r2;
}

rgb Scene::specularColor(const Ray& r, const HitRecord& record, const SampleLight& slight) const
{
	rgb I(slight.intensity);
	rgb ks(record.material.specular);
	float phong_exp = record.material.phong_exponent;

	Vec3 x = r.pointAtParameter(record.t);
	Vec3 wi(slight.position - x);
	Vec3 wo(r.origin() - x);
	float r2 = wi.squaredLength();
	wi.makeUnitVector();
	wo.makeUnitVector();

	Vec3 h(wi + wo);
	h.makeUnitVector();

	float costheta = std::max(0.0f, dot(record.normal, h));

	return (ks * (pow(costheta, phong_exp)) * I) / r2;
}

rgb Scene::reflectionColor(const Ray& r, const HitRecord& record, int recursion_depth) const
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
				recursion_depth - 1);
}

rgb Scene::refractionColor(const Ray& r, const HitRecord& record, int recursion_depth) const
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
				return rgb(k) * rayColor(reflectionRay, recursion_depth - 1);
			}
		}
		
		float R0  = ((n1 - n2)*(n1 - n2)) / ((n1 + n2)*(n1 + n2));
		float omc = 1 - c;
		float R   = R0 + (1 - R0) * omc * omc * omc * omc * omc;
		Ray transmissionRay(r.transmissionRay(nrecord, transmissionDirection, 
					intersection_test_epsilon));

		return rgb(k) * (R * rayColor(reflectionRay, recursion_depth - 1)
					+ (1 - R) * rayColor(transmissionRay, recursion_depth - 1));
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
	delete bvh;
}
