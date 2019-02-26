#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include <string>
#include <math.h>
#include "vector3.h"
#include "rgb.h"

class Scene
{
	public:

	rgb background_color;
	float shadow_ray_epsilon;
	float intersection_test_epsilon;
	int max_recursion_depth;
	std::vector<Camera> cameras;
	AmbientLight ambient_light;
	std::vector<Light*> lights;
	std::vector<Material> materials;
	std::vector<Vec3> vertex_data;
	std::vector<Shape*> shapes;
	std::vector<Mesh*> meshes;

	~Scene();
	// Methods
	void loadFromXML(const std::string& fname);
	void raytraceImages(int threadCount);
	static void raytrace_routine(Scene* scene, const Camera* cam, FlatImage* img, 
			SafeStack<std::pair<float, float>>* pixels);

	rgb rayColor(const Ray& r, int recursion_depth) const;
	rgb ambientColor(const HitRecord& record) const;
	rgb diffuseColor(const Ray& r, const HitRecord& record, const Light* light_ptr) const;
	rgb specularColor(const Ray& r, const HitRecord& record, const Light* light_ptr) const;
	rgb reflectionColor(const Ray& r, const HitRecord& record, int recursion_depth) const;
	rgb refractionColor(const Ray& r, const HitRecord& record, int recursion_depth) const;
};

#endif
