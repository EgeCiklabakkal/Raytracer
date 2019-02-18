#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include <string>
#include "vector3.h"
#include "rgb.h"

class Scene
{
	public:

	rgb background_color;
	float shadow_ray_epsilon;
	int max_recursion_depth;
	std::vector<Camera> cameras;
	AmbientLight ambient_light;
	std::vector<Light*> lights;
	std::vector<Material> materials;
	std::vector<Vec3> vertex_data;
	std::vector<Shape*> shapes;

	~Scene();
	// Methods
	void loadFromXML(const std::string& fname);
	rgb rayColor(const Ray& r, int max_recursion_depth);
};

#endif
