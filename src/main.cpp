#include "utils.h"

int main(int argc, char* argv[])
{
	int width, height;
	HitRecord record;
	bool hit;
	float tmax;

	Scene scene;
	scene.loadFromXML("simple.xml");

	for(Camera& cam: scene.cameras)
	{
		width  = cam.image_width;
		height = cam.image_height;

		Image img(width, height);

		for(int i = 0; i < width; i++)
		{
			for(int j = 0; j < height; j++)
			{
				Ray r = cam.getRay(i, j);

				img.set(i, j, scene.rayColor(r, 6));
			}
		}

		img.writePNG("simple.png");
	}

	return 0;
}
