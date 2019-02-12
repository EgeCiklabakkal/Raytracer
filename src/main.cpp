#include "rgb.h"
#include "image.h"
#include <fstream>

int main(int argc, char* argv[])
{
	Image img(10, 10, rgb(0.1f, 0.1f, 0.7f));

	std::ofstream ofs;
	ofs.open("try.ppm", std::ofstream::out);

	img.writePPM(ofs);

	ofs.close();
	return 0;
}
