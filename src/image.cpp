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

Image::Image(int w, int h) : raster(w * h, rgb()), nx(w), ny(h) {}

// Methods
bool Image::get(int x, int y, rgb& color) const
{
	x = (x >= 0) ? x : this->nx + x;
	y = (y >= 0) ? y : this->ny + y;

	color = raster[((ny-1) - y)*nx + x];
	return true;
}

bool Image::set(int x, int y, const rgb& color)
{
	if(0 > x || x > this->nx) return false;
	if(0 > y || y > this->ny) return false;

	raster[((ny-1) - y)*nx + x] = color;
	return true;
}

void Image::writePNG(std::string fname)
{
	std::vector<unsigned char> png;
	std::vector<unsigned char> data(nx * ny * 4, 0);
	rgb temp;
	unsigned error;

	for(int x = 0; x < nx; x++)
	{
		for(int y = 0; y < ny; y++)
		{
			temp = raster[(y*nx + x)];

			data[(y*nx + x)*4]     = temp._r;
			data[(y*nx + x)*4 + 1] = temp._g;
			data[(y*nx + x)*4 + 2] = temp._b;
			data[(y*nx + x)*4 + 3] = 255;
		}
	}

	// force a png write
	std::string img_name(fname);
	size_t dot = fname.find_last_of(".");
	if(dot != std::string::npos)
	{
		img_name = fname.substr(0, dot) + ".png";
	}

	error = lodepng::encode(png, data, nx, ny);
	if(!error) lodepng::save_file(png, img_name);

	//if there's an error, display it
	if(error)
	{
		std::cerr << "encoder error " << error << ": " 
			<< lodepng_error_text(error) << std::endl;
	}
}

int Image::imread(std::string fname, int channels)
{
	int n;
	unsigned char cr, cb, cg;
	unsigned char *data = stbi_load(fname.c_str(), &nx, &ny, &n, channels);
	raster = std::vector<rgb>(nx * ny, rgb());

	for(int x = 0; x < nx; x++)
	{
		for(int y = 0; y < ny; y++)
		{
			cr = data[(y*nx + x)*4];
			cg = data[(y*nx + x)*4 + 1];
			cb = data[(y*nx + x)*4 + 2];

			raster[(y*nx + x)] = rgb(cr, cg, cb);
		}
	}

	stbi_image_free(data);
	return channels;
}
