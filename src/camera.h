#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"
#include "ONB.h"
#include <string>
#include <array>

class Camera
{
	public:

	// Member variables
	Vec3 position;
	Vec3 gaze;
	Vec3 up;
	std::array<float, 4> near_plane;
	float near_distance;
	int image_width, image_height;
	std::string image_name;

	ONB uvw;
	Vec3 across;

	// Constructors
	Camera() {}
	Camera(Vec3 _position, Vec3 _gaze, Vec3 _up, std::array<float, 4> _near_plane, 
		float _near_distance, int _image_width, int _image_height, std::string _image_name) :
	position(_position), gaze(_gaze), up(_up), 
	near_plane(_near_plane), image_width(_image_width), 
	image_height(_image_height), image_name(_image_name)
	{
		near_distance = _near_distance;
		uvw.initFromWV(-gaze, up);
		across = uvw.u();
		up     = uvw.v();
	}

	Ray getRay(float x, float y);	// TODO: define here
};

#endif
