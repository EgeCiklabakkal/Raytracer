#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <cmath>
#include <string>
#include <fstream>
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
};

#endif
