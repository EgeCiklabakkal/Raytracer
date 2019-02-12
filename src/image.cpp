#include "image.h"
#include <iostream>

// Class: Image

// Constructors
Image::Image() {}

Image::Image(int w, int h)
{
	this->nx = w;
	this->ny = h;

	raster = new rgb*[nx];
	for(int i = 0; i < nx; i++)
	{
		raster[i] = new rgb[ny];
	}
}

Image::Image(int w, int h, rgb background)
{
	this->nx = w;
	this->ny = h;

	raster = new rgb*[nx];
	for(int i = 0; i < nx; i++)
	{
		raster[i] = new rgb[ny];

		for(int j = 0; j < ny; j++)
		{
			raster[i][j] = background;
		}
	}
}

// Methods
bool Image::set(int x, int y, const rgb& color)
{
	if(0 > x || x > this->nx) return false;
	if(0 > y || y > this->ny) return false;

	raster[x][y] = color;
	return true;
}

void Image::gammaCorrect(float gamma)
{
	rgb temp;
	float exponent = 1.0f / gamma;
	
	for(int i = 0; i < nx; i++)
	{
		for(int j = 0; j < ny; j++)
		{
			temp = raster[i][j];

			raster[i][j] =      rgb(pow(temp.r(), exponent),
						pow(temp.g(), exponent),
						pow(temp.b(), exponent));
		}
	}
}

void Image::writePPM(std::ostream& out)
{
	// PPM header
	out << "P6\n";
	out << nx << " " << ny << "\n";
	out << "255\n";

	int i, j;
	unsigned int ir, ig , ib;
	unsigned char cr, cg, cb;

	for(j = ny-1; j >= 0; j--)
	{
		for(i = 0; i < nx; i++)
		{
			ir = (unsigned int)(256*raster[i][j].r());
			ig = (unsigned int)(256*raster[i][j].g());
			ib = (unsigned int)(256*raster[i][j].b());
			
			// Clamp to 255
			if (ir > 255) ir = 255;
			if (ig > 255) ig = 255;
			if (ib > 255) ib = 255;

			cr = (unsigned char)ir;
			cg = (unsigned char)ig;
			cb = (unsigned char)ib;

			out.put(cr);
			out.put(cg);
			out.put(cb);
		}
	}
}

void Image::writePPM3(std::ostream& out)
{
	// PPM header
	out << "P3\n";
	out << nx << " " << ny << "\n";
	out << "255\n";

	int i, j;
	unsigned int ir, ig , ib;

	for(j = ny-1; j >= 0; j--)
	{
		for(i = 0; i < nx; i++)
		{
			ir = (unsigned int)(256*raster[i][j].r());
			ig = (unsigned int)(256*raster[i][j].g());
			ib = (unsigned int)(256*raster[i][j].b());
			
			// Clamp to 255
			if (ir > 255) ir = 255;
			if (ig > 255) ig = 255;
			if (ib > 255) ib = 255;

			out << ir << " " << ig << " " << ib << " ";
		}
	}
}

void Image::readPPM(std::string fname)
{	
	char ch, type;
	char cr, cg, cb;
	int i, j, cols, rows;
	int num;

	std::ifstream infile;
	infile.open(fname.c_str());
	if(!infile.is_open())
	{
		std::cerr << " ERROR -- Couldn't open file \'" << fname << "\'.\n";
		exit(-1);
	}

	infile.get(ch);
	infile.get(type);
	infile >> cols >> rows >> num;

	this->nx = cols;
	this->ny = rows;

	raster = new rgb*[nx];
	for(i = 0; i < nx; i++)
	{
		for(j = 0; j < ny; j++)
		{
			raster[i] = new rgb[ny];
		}
	}

	// clean up newline
	infile.get(ch);

	for(j = ny-1; j >= 0; j--)
	{
		for(i = 0; i < nx; i++)
		{
			infile.get(cr);
			infile.get(cg);
			infile.get(cb);

			raster[i][j] =      rgb((float)((unsigned char)cr)/255.0f,
						(float)((unsigned char)cg)/255.0f, 
						(float)((unsigned char)cb)/255.0f);
		}
	}
}

void Image::writePNG(std::string fname)
{
	int i, j, idx;
	std::vector<unsigned char> img(ny * nx * 4);
	std::vector<unsigned char> png;

	unsigned error;
	unsigned int ir, ig, ib;
	unsigned char cr, cg, cb;

	idx = 0;
	for(j = ny-1; j >= 0; j--)
	{
		for(i = 0; i < nx; i++)
		{
			ir = (unsigned int)(256*raster[i][j].r());
			ig = (unsigned int)(256*raster[i][j].g());
			ib = (unsigned int)(256*raster[i][j].b());

			// Clamp to 255
			if (ir > 255) ir = 255;
			if (ig > 255) ig = 255;
			if (ib > 255) ib = 255;

			cr = (unsigned char)ir;
			cg = (unsigned char)ig;
			cb = (unsigned char)ib;

			img[idx]   = cr;	// R
			img[idx+1] = cg;	// G
			img[idx+2] = cb;	// B
			img[idx+3] = 255;	// A (fully opaque)
	
			idx += 4;
		}
	}

	error = lodepng::encode(png, img, nx, ny);
	if(!error) lodepng::save_file(png, fname);

	//if there's an error, display it
	if(error)
	{
		std::cerr << "encoder error " << error << ": " 
			<< lodepng_error_text(error) << std::endl;
	}
}

// Class: flatImage

// Constructors
flatImage::flatImage() {}
