#ifndef _SCENE_H_
#define _SCENE_H_

#include "vector3.h"
#include <vector>
#include <string>

class Scene
{
	public:

	Vec3i background_color;
	float shadow_ray_epsilon;
	int max_recursion_depth;
	std::vector<Camera> cameras;
	Vec3 ambient_light;
	std::vector<Light> lights;
	std::vector<Material> materials;
	std::vector<Shape*> shapes;

	void loadFromXml(const std::string& fname);
};

#endif
