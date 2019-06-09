#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <math.h>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include "util/lodepng.h"
#include "rgb.h"
#include "tonemap.h"

class Image
{
	public:

	// Member variables
	std::vector<rgb> raster;	// Image is stored as 1D array
	int nx;
	int ny;

	Image();
	Image(std::string fname);
	Image(int w, int h);
	Image(int w, int h, rgb background);
	bool get(int x, int y, rgb& color) const;
	bool set(int x, int y, const rgb& color);
	bool add(int x, int y, const rgb& color);
	void gammaCorrect(float gamma);
	int imread(std::string fname, int channels=4);
	void imwrite(std::string fname, const Tonemap& tonemap=Tonemap()) const;
	int readCommon(std::string fname, int channels=4);
	int readEXR(std::string fname, int channels=4);
	void writePNG(std::string fname, const Tonemap& tonemap=Tonemap()) const;
	void writeEXR(std::string fname) const;
	bool handleTonemap(const Tonemap& tonemap, std::vector<unsigned char>& data) const;
};

#endif
