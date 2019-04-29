#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "util/stb_image.h"
#define TINYEXR_IMPLEMENTATION
#include "util/tinyexr.h"

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

void Image::imwrite(std::string fname, const Tonemap& tonemap) const
{
	size_t dot = fname.find_last_of(".");

	if(fname.substr(dot + 1) == "exr")
	{
		writeEXR(fname);
	}

	else
	{
		std::string img_name;

		// force a png write
		img_name = fname.substr(0, dot) + ".png";
		writePNG(img_name, tonemap);
	}
}

void Image::writeEXR(std::string fname) const
{
	EXRHeader header;
	EXRImage image;

	InitEXRHeader(&header);
	InitEXRImage(&image);

	image.num_channels = 3;

	std::vector<float> images[3];
	images[0].resize(nx * ny);
	images[1].resize(nx * ny);
	images[2].resize(nx * ny);

	for(int i = 0; i < nx * ny; i++)
	{
		rgb temp = raster[i];
		images[0][i] = temp._r;
		images[1][i] = temp._g;
		images[2][i] = temp._b;
	}

	float* image_ptr[3];
	image_ptr[0] = &(images[2].at(0)); // B
	image_ptr[1] = &(images[1].at(0)); // G
	image_ptr[2] = &(images[0].at(0)); // R

	image.images = (unsigned char**)image_ptr;
	image.width = nx;
	image.height = ny;

	header.num_channels = 3;
	header.channels = (EXRChannelInfo *)malloc(sizeof(EXRChannelInfo) * header.num_channels); 
	// Must be BGR(A) order, since most of EXR viewers expect this channel order.
	strncpy(header.channels[0].name, "B", 255); header.channels[0].name[strlen("B")] = '\0';
	strncpy(header.channels[1].name, "G", 255); header.channels[1].name[strlen("G")] = '\0';
	strncpy(header.channels[2].name, "R", 255); header.channels[2].name[strlen("R")] = '\0';

	header.pixel_types = (int *)malloc(sizeof(int) * header.num_channels); 
	header.requested_pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
	for(int i = 0; i < header.num_channels; i++)
	{
		// pixel type of input image
    		header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;
		// pixel type of output image to be stored in .EXR
    		header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; 
	}

	const char* err;
	int ret = SaveEXRImageToFile(&image, &header, fname.c_str(), &err);
	if(ret != TINYEXR_SUCCESS)
	{
		fprintf(stderr, "Save EXR err: %s\n", err);
	}
	//printf("Saved exr file. [ %s ] \n", fname.c_str());

	free(header.channels);
	free(header.pixel_types);
	free(header.requested_pixel_types);
}

void Image::writePNG(std::string fname, const Tonemap& tonemap) const
{
	std::vector<unsigned char> png;
	std::vector<unsigned char> data(nx * ny * 4, 0);
	unsigned error;

	handleTonemap(tonemap, data);	// tonemapping fills in display values
	
	error = lodepng::encode(png, data, nx, ny);
	if(!error) lodepng::save_file(png, fname);

	//if there's an error, display it
	if(error)
	{
		std::cerr << "encoder error " << error << ": " 
			<< lodepng_error_text(error) << std::endl;
	}
}

int Image::imread(std::string fname, int channels)
{
	size_t dot = fname.find_last_of(".");

	if(fname.substr(dot + 1) == "exr")
	{
		return readEXR(fname, channels);
	}

	else
	{
		return readCommon(fname, channels);
	}
}

int Image::readEXR(std::string fname, int channels)
{
	float *data; // width * height * RGBA
	const char *err = nullptr;
	float fr, fg, fb;

	int ret = LoadEXR(&data, &nx, &ny, fname.c_str(), &err);
	raster = std::vector<rgb>(nx * ny, rgb());
	if(ret != TINYEXR_SUCCESS)
	{
		if(err)
		{
			fprintf(stderr, "ERR : %s\n", err);
			FreeEXRErrorMessage(err); // release memory of error message.
		}
	}

	for(int i = 0; i < nx * ny; i++)
	{
		fr = data[4*i];
		fg = data[4*i + 1];
		fb = data[4*i + 2];

		raster[i] = rgb(fr, fg, fb);
	}

	free(data); // relase memory of image data

	return channels;
}

int Image::readCommon(std::string fname, int channels)
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

bool Image::handleTonemap(const Tonemap& tonemap, std::vector<unsigned char>& data) const
{
	rgb temp;

	if(tonemap.tonemap_mode == TonemapMode::PHOTOGRAPHIC)
	{
		std::vector<float> Ls(nx * ny, 0.0f);	// L values in an array
		std::vector<float> Lws(nx * ny, 0.0f);	// Lw values in an array
		float avglogLw, acclogLw, N, Lwhite, burnoutKeepRatio, Ld, a, s, g;
		acclogLw = 0.0f;	// accumulated log Lw
		N = nx * ny;		// Number of pixels
		a = tonemap.a;
		s = tonemap.saturation;
		g = 1.0f / tonemap.gamma;	// 1 / gamma

		for(int i = 0; i < N; i++)	// calculate Lws into the array
		{
			temp = raster[i];
			Lws[i]  = 0.2126f * temp._r + 0.7152f * temp._g + 0.0722 * temp._b;
			acclogLw += log(1e-5 + Lws[i]);
		}
		avglogLw = exp((1.0f / N) * acclogLw);

		for(int i = 0; i < N; i++)	// initial scaling, key value ∊ [0, 1]
		{
			Ls[i] = (a / avglogLw) * Lws[i];
		}

		// Blooming
		std::vector<float> Lscopy(Ls);
		std::sort(Lscopy.begin(), Lscopy.end());
		burnoutKeepRatio = (100.0f - tonemap.burnout) / 100.0f;
		Lwhite = Lscopy[int(N * burnoutKeepRatio)];
		Lwhite = Lwhite * Lwhite;	// squared

		// Calculate Lcs into Ls
		for(int i = 0; i < N; i++)
		{
			Ld = (Ls[i] * (1.0f + (Ls[i] / Lwhite))) / (1.0f + Ls[i]);
			Ls[i] = std::clamp(Ld, 0.0f, 1.0f);
		}

		// calculate display values in rgb
		for(int i = 0; i < N; i++)
		{
			temp = raster[i];
			float rprime = std::clamp(pow(temp._r / Lws[i], s) * Ls[i], 0.0f, 1.0f);
			float gprime = std::clamp(pow(temp._g / Lws[i], s) * Ls[i], 0.0f, 1.0f);
			float bprime = std::clamp(pow(temp._b / Lws[i], s) * Ls[i], 0.0f, 1.0f);

			unsigned char rd = (int)floor(pow(rprime,  g) * 255.0f);
			unsigned char gd = (int)floor(pow(gprime,  g) * 255.0f);
			unsigned char bd = (int)floor(pow(bprime,  g) * 255.0f);

			data[i*4]     = rd;
			data[i*4 + 1] = gd;
			data[i*4 + 2] = bd;
			data[i*4 + 3] = 255;
		}

		return true;
	}

	// else: NOTONEMAP, just clamp
	for(int x = 0; x < nx; x++)
	{
		for(int y = 0; y < ny; y++)
		{
			temp = raster[(y*nx + x)];
			temp.clamp256();

			data[(y*nx + x)*4]     = temp._r;
			data[(y*nx + x)*4 + 1] = temp._g;
			data[(y*nx + x)*4 + 2] = temp._b;
			data[(y*nx + x)*4 + 3] = 255;
		}
	}

	return false;
}
