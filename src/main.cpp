#include "utils.h"

int main(int argc, char* argv[])
{
	int width, height;
	hitRecord record;
	bool hit;
	float tmax;
	Vec3 direction(0, 0, -1);

	width = 500; height = 500;
	Sphere sphere = Sphere(Vec3(250, 250, -1000), 150, rgb(0.2f, 0.1f, 0.8f));
	Triangle triangle = Triangle(Vec3(300, 600, -800), Vec3(0, 100, -1000), Vec3(450, 20, -1000), rgb(0.9f, 0.2f, 0.1f));
	std::vector<Shape*> shapes = {&sphere, &triangle};
	Image img(width, height);

	tmax = 100000.0f;
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			hit = false;
			Ray r(Vec3(i, j, 0), direction);

			for(const Shape* shape : shapes)
			{
				if(shape->hit(r, 0.0001f, tmax, 0.0f, record))
				{
					tmax = record.t;
					hit = true;
				}
			}

			if(hit)
			{
				img.set(i, j, record.color);
			}
	
			else
			{
				img.set(i, j, rgb(0.1f, 0.1f, 0.1f));
			}
		}
	}

	img.writePNG("simple.png");

	return 0;
}
