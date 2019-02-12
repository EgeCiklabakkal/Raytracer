#include "rgb.h"
#include "image.h"
#include "vector3.h"
#include <fstream>

int main(int argc, char* argv[])
{
	Image img(1024, 1024, rgb(0.1f, 0.1f, 0.7f));

	img.writePNG("try.png");

	Vec3 v;
	Vec3 v1(1, 3, 5);
	Vec3 v2(1, 3, 5);

	std::cout << v1 << "." << v2 << "=" << dot(v1, v2) << std::endl;

	return 0;
}
