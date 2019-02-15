#include "utils.h"

int main(int argc, char* argv[])
{
	int width, height;
	HitRecord record;
	bool hit;
	float tmax;
	Vec3 direction(0, 0, -1);

	width = 500; height = 500;
	Sphere sphere = Sphere(Vec3(250, 250, -1000), 150, rgb(0.2f, 0.1f, 0.8f));
	Triangle triangle = Triangle(Vec3(300, 600, -800), Vec3(0, 100, -1000), Vec3(450, 20, -1000), rgb(0.9f, 0.2f, 0.1f));
	std::vector<Shape*> shapes = {&sphere, &triangle};
	Image img(width, height);

	std::array<float, 4> lrbt = {-50.0f, 50.0f, -50.0f, 50.0f};
	Camera cam = Camera(Vec3(250.0f, 250.0f, -500.0f), direction, Vec3(0.0f, 1.0f, 0.0f), lrbt, 100.0f, width, height, "simple.png");

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			tmax = 100000.0f;
			hit = false;
			Ray r = cam.getRay(i, j);

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
