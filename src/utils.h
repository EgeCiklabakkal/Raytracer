#ifndef _UTILS_H_
#define _UTILS_H_

// Macros
#define DEFAULT_THREAD_COUNT 8
#define DEFAULT_AIR_REFRACTION_INDEX 1.0f

// glm
//#include <glm/vec4.hpp>				// vec4, bvec4, dvec4, ivec4 and uvec4
//#include <glm/mat4x4.hpp>			// mat4, dmat4
//#include <glm/ext/matrix_transform.hpp> 	// perspective, translate, rotate
//#include <glm/trigonometric.hpp>		// radians
// Include all GLM core / GLSL features
#include <glm/glm.hpp> // vec2, vec3, mat4, radians

// Include all GLM extensions
#include <glm/ext.hpp> // perspective, translate, rotate

#include <iostream>
#include <math.h>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

#include "util/happly.h"
#include "util/tinyxml2.h"
#include "BVH.h"
#include "camera.h"
#include "dynArray.h"
#include "image.h"
#include "light.h"
#include "macros.h"
#include "material.h"
#include "mesh.h"
#include "ONB.h"
#include "ray.h"
#include "rdmGen.h"
#include "rgb.h"
#include "rtmath.h"
#include "safeStack.h"
#include "scene.h"
#include "shape.h"
#include "sphere.h"
#include "triangle.h"
#include "vector3.h"
#include "vertex.h"

#endif
