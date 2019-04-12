#ifndef _UTILS_H_
#define _UTILS_H_

// Macros
#define DEFAULT_THREAD_COUNT 8
#define DEFAULT_AIR_REFRACTION_INDEX 1.0f
#define PROGRESS_BAR_WIDTH 80
#define PROGRESS_BAR_UPDATE_WAIT 10 		// In miliseconds
#define SHOW_PROGRESS_BAR true

// glm
#include <glm/vec4.hpp>				// vec4, bvec4, dvec4, ivec4 and uvec4
#include <glm/mat4x4.hpp>			// mat4, dmat4
#include <glm/ext/matrix_transform.hpp> 	// perspective, translate, rotate
#include <glm/trigonometric.hpp>		// radians
#include <glm/matrix.hpp>			// matrix functions: transpose, inverse, etc.
#include <glm/gtx/string_cast.hpp>		// glm::to_string(mat)
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <iostream>
#include <map>
#include <math.h>
#include <set>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <vector>

#include "util/happly.h"
#include "util/tinyxml2.h"
#include "BVH.h"
#include "camera.h"
#include "cbTexture.h"
#include "dynArray.h"
#include "image.h"
#include "imageTexture.h"
#include "instance.h"
#include "light.h"
#include "macros.h"
#include "material.h"
#include "mesh.h"
#include "ONB.h"
#include "perlinNoise.h"
#include "perlinTexture.h"
#include "ray.h"
#include "rdmGen.h"
#include "rgb.h"
#include "rtmath.h"
#include "safeStack.h"
#include "scene.h"
#include "shape.h"
#include "simpleTexture.h"
#include "sphere.h"
#include "texture.h"
#include "triangle.h"
#include "vector2.h"
#include "vector3.h"
#include "vertex.h"

// util helper functions
namespace utils
{
	void displayProgressBar(SafeStack<std::pair<float, float>>& pixels);	

	template<typename T>
	void printTimeDiff(std::ostream& out, T prior, T latter)
	{
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>
								(latter - prior).count();
		auto const msecs = diff % 1000;
		diff /= 1000;
		auto const secs = diff % 60;
		diff /= 60;
		auto const mins = diff % 60;
		diff /= 60;
		auto const hours = diff % 24;
		diff /= 24;
		auto const days = diff;
 
		bool printed_earlier = false;
		if(days >= 1) 
		{
			printed_earlier = true;
			out << days << "d";
		}
		if(printed_earlier || hours >= 1)
		{
			printed_earlier = true;
			out << hours << "h";
		}
		if(printed_earlier || mins >= 1)
		{
			printed_earlier = true;
			out << mins << "m";
		}

		// always print seconds
		out << secs << ",";
		out << msecs << "s";

		// adapted from https://stackoverflow.com/a/22064449
	}

	template<typename T>
	void printFancyTimeDiff(std::ostream& out, T prior, T latter)
	{
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>
								(latter - prior).count();
		auto const msecs = diff % 1000;
		diff /= 1000;
		auto const secs = diff % 60;
		diff /= 60;
		auto const mins = diff % 60;
		diff /= 60;
		auto const hours = diff % 24;
		diff /= 24;
		auto const days = diff;
 
		bool printed_earlier = false;
		if(days >= 1) 
		{
			printed_earlier = true;
			out << days << (1 != days ? " days" : " day") << ' ';
		}
		if(printed_earlier || hours >= 1)
		{
			printed_earlier = true;
			out << hours << (1 != hours ? " hours" : " hour") << ' ';
		}
		if(printed_earlier || mins >= 1)
		{
			printed_earlier = true;
			out << mins << (1 != mins ? " minutes" : " minute") << ' ';
		}
		if(printed_earlier || secs >= 1)
		{
			printed_earlier = true;
			out << secs << (1 != secs ? " seconds" : " second") << ' ';
		}
		if (printed_earlier || msecs >= 1)
		{
			printed_earlier = true;
			out << msecs << (1 != msecs ? " milliseconds" : " millisecond");
		}

		// from https://stackoverflow.com/a/22064449
	}

	template<typename T>
	void printTimingInfo(const std::string& image_name, T prior, T latter, bool fancy=false)
	{
		std::cout << image_name << " saved in ";
		if(fancy)
			printFancyTimeDiff(std::cout, prior, latter);
		else
			printTimeDiff(std::cout, prior, latter);
		std::cout << std::endl;
	}
}

#endif
