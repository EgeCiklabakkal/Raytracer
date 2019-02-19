#include "utils.h"

int main(int argc, char* argv[])
{
	int width, height;
	HitRecord record;

	Scene scene;
	scene.loadFromXML(argv[1]);

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

				rgb pixel_color = scene.rayColor(r, scene.max_recursion_depth);
				pixel_color /= 256.0f;
				pixel_color.clamp();

				img.set(i, j, pixel_color);
			}
		}

		img.writePNG(cam.image_name);
	}

	return 0;
}
