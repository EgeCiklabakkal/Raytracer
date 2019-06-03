#include "utils.h"

void Scene::renderImages(int threadCount, bool showProgress)
{
	int width, height;
	HitRecord record;

	for(const Camera* cam: cameras)
        {
		auto const prior = std::chrono::system_clock::now();

                width  = cam->image_width;
                height = cam->image_height;

		Image img(width, height);

                cam->render(&img, threadCount, showProgress);

                img.imwrite(cam->image_name, cam->tonemap);

		// Time Duration Print
		auto const latter = std::chrono::system_clock::now();
		utils::printTimingInfo(cam->image_name, prior, latter);
        }
}

Scene::~Scene()
{
	// Free lights
	for(Light* light : lights)
	{
		// Object lights will be freed in BVH
		if(!light->object) delete light;
	}
	lights.clear();

	// Free cameras
	for(Camera* cam : cameras)
	{
		delete cam;
	}
	cameras.clear();

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
