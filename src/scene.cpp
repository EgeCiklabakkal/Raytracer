#include "utils.h"

void Scene::raytraceImages(int threadCount)
{
	int width, height;
	HitRecord record;

	for(const Camera& cam: cameras)
        {
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
			threads.push_back(std::thread(raytrace_routine, 
						this, &cam, &img, &pixels));
		}

		// Wait for them to complete
		for(int i = 0; i < threadCount; i++)
		{
			threads[i].join();
		}

                img.writePNG(cam.image_name);
        }
}

void Scene::raytrace_routine(Scene* scene, const Camera* cam, Image* img, 
		SafeStack<std::pair<float, float>>* pixels)
{
	std::pair<float, float> currPixel;
	while(pixels->pop(currPixel))
	{
		int i, j;
		i = currPixel.first;
		j = currPixel.second;

		Ray r = cam->getRay(i, j);

		rgb pixel_color = scene->rayColor(r, scene->max_recursion_depth);
		pixel_color /= 256.0f;
		pixel_color.clamp();

		img->set(i, j, pixel_color);
	}
}

rgb Scene::rayColor(const Ray& r, int recursion_depth) const
{
	rgb rcolor;
	float tmax = 100000.0f;		// Note: tmax, tmin, time can be made an argument
	float tmin = 0.0001f;		
	float time = 0.0f;
	bool hit = false;
	HitRecord record;

	for(const Shape* shape : this->shapes)
	{
		if(shape->hit(r, tmin, tmax, time, record))
		{
			tmax = record.t;
			hit = true;
		}
	}
	
	if(hit)
	{
		rcolor = ambientColor(record);

		// Loop over lights
		for(const Light* light_ptr : this->lights)
		{
			bool continue_light_loop = false;
			Ray shadow_ray(r.shadowRay(record, light_ptr, shadow_ray_epsilon));
			float tlight = shadow_ray.parameterAtPoint(light_ptr->position);
			for(const Shape* shape : this->shapes)
			{
				if(shape->shadowHit(shadow_ray, 0.0f, tlight, time))
				{
					continue_light_loop = true;
					break;	
				}		
			}

			if(!continue_light_loop)
			{
				rcolor += diffuseColor(r, record, light_ptr) +
						specularColor(r, record, light_ptr);
			}
		}
		// Add color from reflections
		rcolor += reflectionColor(r, record, recursion_depth);

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

rgb Scene::diffuseColor(const Ray& r, const HitRecord& record, const Light* light_ptr) const
{
	rgb I(light_ptr->intensity);
	rgb kd(record.material.diffuse);

	Vec3 x = r.pointAtParameter(record.t);
	Vec3 wi(light_ptr->position - x);
	float r2 = wi.squaredLength();
	wi.makeUnitVector();

	float costheta = std::max(0.0f, dot(wi, record.normal));

	return (kd * costheta * I) / r2;
}

rgb Scene::specularColor(const Ray& r, const HitRecord& record, const Light* light_ptr) const
{
	rgb I(light_ptr->intensity);
	rgb ks(record.material.specular);
	float phong_exp = record.material.phong_exponent;

	Vec3 x = r.pointAtParameter(record.t);
	Vec3 wi(light_ptr->position - x);
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

	return km * rayColor(r.reflectionRay(record, shadow_ray_epsilon), recursion_depth - 1);
}

Scene::~Scene()
{
	// Free lights
	for(Light* light : lights)
	{
		delete light;
	}
	lights.clear();

	// Free shapes
	for(Shape* shape : shapes)
	{
		delete shape;
	}
	shapes.clear();

	for(Mesh* mesh : meshes)
	{
		delete mesh;
	}
	meshes.clear();
}
