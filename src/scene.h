#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include <string>
#include <math.h>
#include "vector3.h"
#include "vector2.h"
#include "vertex.h"
#include "rgb.h"
#include "light.h"
#include "mesh.h"
#include "BVH.h"
#include "image.h"
#include "tonemap.h"
#include "BRDF.h"

class Light;
class BRDF;
class SphericalDirectionalLight;
class Camera;

class Scene
{
	public:

	rgb background_color;
	Image background_texture;
	SphericalDirectionalLight *sd_light;
	bool hasBackgroundTexture;
	bool hasSphericalDirectionalLight;
	float shadow_ray_epsilon;
	float intersection_test_epsilon;
	int max_recursion_depth;
	bool cullFace;
	std::vector<Camera*> cameras;
	Vec3 ambient_light;
	std::vector<Light*> lights;
	std::vector<BRDF*> brdfs;	// brdfs[0] is always simpleBRDF
	std::vector<Material> materials;
	std::vector<Image*> textureImages;
	std::vector<Texture*> textures;
	std::vector<Vertex> vertex_data;
	std::vector<Vec2> texCoord_data;
	std::vector<Shape*> shapes;
	std::vector<Mesh*> meshes;
	std::vector<Shape*> primMeshBVHs;
	BVH *bvh = nullptr;

	~Scene();
	// Methods
	void loadFromXML(const std::string& fname);
	void renderImages(int threadCount, bool showProgress=true);
};

#endif
