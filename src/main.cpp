#include "rgb.h"
#include "image.h"
#include <fstream>

int main(int argc, char* argv[])
{
	Image img(1024, 1024, rgb(0.1f, 0.1f, 0.7f));

	img.writePNG("try.png");

	return 0;
}
