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
	rgb **raster;
	int nx;		// raster width
	int ny;		// raster height

	Image();
	Image(int w, int h);
	Image(int w, int h, rgb background);
	bool set(int x, int y, const rgb& color);
	void gammaCorrect(float gamma);
	void writePPM(std::ostream& out);
	void writePPM3(std::ostream& out);
	void readPPM(std::string fname);
	void writePNG(std::string fname);
};

class flatImage
{
	public:

	// Member variables
	std::vector<unsigned char> raster;	// Image is stored as 1D array
	int nx;
	int ny;

	flatImage();
	flatImage(int w, int h);
	flatImage(int w, int h, rgb background);
	bool set(int x, int y, const rgb& color);
	void gammaCorrect(float gamma);
	void writePPM(std::ostream& out);
	void writePPM3(std::ostream& out);
	void readPPM(std::string fname);
	void writePNG(std::string fname);
};

#endif
