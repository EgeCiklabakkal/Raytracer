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
				tmax = 100000.0f;
				hit = false;
				Ray r = cam.getRay(i, j);

				for(const Shape* shape : scene.shapes)
				{
					if(shape->hit(r, 0.0001f, tmax, 0.0f, record))
					{
						tmax = record.t;
						hit = true;
					}
				}

				if(hit)
				{
					img.set(i, j, rgb(0.8f, 0.1f, 0.1f));
				}
		
				else
				{
					img.set(i, j, rgb(0.1f, 0.1f, 0.1f));
				}
			}
		}

		img.writePNG("simple.png");
	}

	return 0;
}
