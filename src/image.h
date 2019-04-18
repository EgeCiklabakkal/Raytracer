#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include "util/lodepng.h"
#include "rgb.h"

class Image
{
	public:

	// Member variables
	std::vector<unsigned char> raster;	// Image is stored as 1D array
	int nx;
	int ny;

	Image();
	Image(std::string fname);
	Image(int w, int h);
	Image(int w, int h, rgb background);
	bool get(int x, int y, rgb& color) const;
	bool set(int x, int y, const rgb& color);
	void gammaCorrect(float gamma);
	int imread(std::string fname, int channels=4);
	void readPNG(std::string fname);
	void writePNG(std::string fname);
};

#endif
