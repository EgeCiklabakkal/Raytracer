#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include <string>
#include <math.h>
#include "vector3.h"
#include "vector2.h"
#include "vertex.h"
#include "rgb.h"
#include "mesh.h"
#include "BVH.h"
#include "image.h"

class Scene
{
	public:

	rgb background_color;
	Image background_texture;
	bool hasBackgroundTexture;
	float shadow_ray_epsilon;
	float intersection_test_epsilon;
	int max_recursion_depth;
	bool cullFace;
	std::vector<Camera> cameras;
	AmbientLight ambient_light;
	std::vector<Light*> lights;
	std::vector<Material> materials;
	std::vector<Image*> textureImages;
	std::vector<Texture*> textures;
	std::vector<Vertex> vertex_data;
	std::vector<Vec2> texCoord_data;
	std::vector<Shape*> shapes;
	std::vector<Mesh*> meshes;
	std::vector<Shape*> primMeshBVHs;
	BVH *bvh;

	~Scene();
	// Methods
	void loadFromXML(const std::string& fname);
	void renderImages(int threadCount, bool showProgress=true);
	static void raytrace_routine(Scene* scene, const Camera* cam, Image* img, 
			SafeStack<std::pair<float, float>>* pixels, int num_samples);
	static void raytrace_singleSample(Scene* scene, const Camera* cam, Image* img,
			SafeStack<std::pair<float, float>>* pixels);

	rgb rayColor(const Ray& r, int recursion_depth, Vec2 ij=Vec2()) const;
	rgb backgroundColor(const Vec2 ij) const;
	rgb ambientColor(const HitRecord& record) const;
	rgb diffuseColor(const Ray& r, const HitRecord& record, const SampleLight& slight) const;
	rgb specularColor(const Ray& r, const HitRecord& record, const SampleLight& slight) const;
	rgb reflectionColor(const Ray& r, const HitRecord& record, int recursion_depth) const;
	rgb refractionColor(const Ray& r, const HitRecord& record, int recursion_depth) const;
	bool handleTexture(HitRecord& record, DecalMode& decal_mode, rgb& color) const;
};

#endif
