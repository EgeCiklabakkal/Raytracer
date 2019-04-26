#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"
#include "ONB.h"
#include "rtmath.h"
#include "tonemap.h"
#include <array>
#include <string>
#include <vector>

class Camera
{
	public:

	// Member variables
	Vec3 position;
	Vec3 across;
	Vec3 gaze;
	Vec3 up;
	std::array<float, 4> near_plane;	// l, r , b, t
	float near_distance;
	float focus_distance;
	float aperture_size;
	int image_width, image_height;
	std::string image_name;
	int num_samples;
	Tonemap tonemap;

	ONB uvw;
	float pw, ph;				// pixel width/height
	Vec3 q;					// left-bottom edge coordinate

	// Constructors
	Camera() {}
	Camera(Vec3 _position, Vec3 _gaze, Vec3 _up, std::array<float, 4> _near_plane, 
		float _near_distance, float _focus_distance, float _aperture_size,
		int _image_width, int _image_height, std::string _image_name,
		int _num_samples, const Tonemap& _tonemap) :
	position(_position), gaze(_gaze), up(_up), near_plane(_near_plane), 
	near_distance(_near_distance), focus_distance(_focus_distance), 
	aperture_size(_aperture_size), image_width(_image_width), 
	image_height(_image_height), image_name(_image_name),
	num_samples(_num_samples), tonemap(_tonemap)
	{
		uvw.initFromWV(-gaze, up);
		across =  uvw.u();
		up     =  uvw.v();
		gaze   = -uvw.w();

		pw = (near_plane[1] - near_plane[0]) / image_width;
		ph = (near_plane[3] - near_plane[2]) / image_height;

		q  = position + near_distance*gaze + near_plane[0]*across + near_plane[2]*up;
	}

	Ray getRay(float x, float y, bool primary=true) const
	{
		Vec3 s = q + (pw*x + pw/2)*across + (ph*y + ph/2)*up;

		return Ray(position, unitVector(s - position), 1.0f, primary);
	}

	bool isDOF() const { return (int)aperture_size; }

	// num_samples should be perfect square(even if it isn't it is "cast" to lower ps)
	void sampleRays(float x, float y, std::vector<Ray>& rays, int num_samples=64) const;
	void sampleDOFRays(float x, float y, std::vector<Ray>& rays, int num_samples=64) const;
};

#endif
