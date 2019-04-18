#include "image.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "util/stb_image.h"

// Constructors
Image::Image() {}

Image::Image(std::string fname)
{
	imread(fname);
}

Image::Image(int w, int h) : raster(w * h * 4, 0), nx(w), ny(h) {}

// Methods
bool Image::get(int x, int y, rgb& color) const
{
	x = (x >= 0) ? x : this->nx + x;
	y = (y >= 0) ? y : this->ny + y;

	unsigned char cr, cg, cb;

	cr = raster[(((ny-1) - y)*nx + x)*4];
	cg = raster[(((ny-1) - y)*nx + x)*4 + 1];
	cb = raster[(((ny-1) - y)*nx + x)*4 + 2];

	color = rgb(cr, cg, cb);
	return true;
}

bool Image::set(int x, int y, const rgb& color)
{
	if(0 > x || x > this->nx) return false;
	if(0 > y || y > this->ny) return false;

	unsigned char cr, cg, cb;

	cr = (unsigned char)color._r;
	cg = (unsigned char)color._g;
	cb = (unsigned char)color._b;

	raster[(((ny-1) - y)*nx + x)*4]     = cr;
	raster[(((ny-1) - y)*nx + x)*4 + 1] = cg;
	raster[(((ny-1) - y)*nx + x)*4 + 2] = cb;
	raster[(((ny-1) - y)*nx + x)*4 + 3] = 255;
	return true;
}

void Image::writePNG(std::string fname)
{
	std::vector<unsigned char> png;
	unsigned error;

	std::string img_name(fname);
	size_t dot = fname.find_last_of(".");
	if(dot != std::string::npos)
	{
		img_name = fname.substr(0, dot) + ".png";
	}

	error = lodepng::encode(png, raster, nx, ny);
	if(!error) lodepng::save_file(png, img_name);

	//if there's an error, display it
	if(error)
	{
		std::cerr << "encoder error " << error << ": " 
			<< lodepng_error_text(error) << std::endl;
	}
}

void Image::readPNG(std::string fname)
{
	unsigned int w, h;
	unsigned error = lodepng::decode(raster, w, h, fname);
	nx = w;
	ny = h;

	//if there's an error, display it
  	if(error) 
	{
		std::cerr << "decoder error " << error << ": " 
			<< lodepng_error_text(error) << std::endl;
	}

	// https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_decode.cpp
}

int Image::imread(std::string fname, int channels)
{
	int n;
	unsigned char *data = stbi_load(fname.c_str(), &nx, &ny, &n, channels);

	// Convert pointer to vector
	raster = std::vector<unsigned char>(data, data + nx*ny*channels);

	stbi_image_free(data);
	return channels;
}
