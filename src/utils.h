#ifndef _UTILS_H_
#define _UTILS_H_

// Macros
#define DEFAULT_THREAD_COUNT 8

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
#include <limits>
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
#include "blinnPhongBRDF.h"
#include "BRDF.h"
#include "BTree.h"
#include "bumpRecord.h"
#include "BVH.h"
#include "camera.h"
#include "cbTexture.h"
#include "dynArray.h"
#include "helperUtils.h"
#include "image.h"
#include "imageTexture.h"
#include "instance.h"
#include "integrator.h"
#include "light.h"
#include "lightMesh.h"
#include "lightSphere.h"
#include "macros.h"
#include "material.h"
#include "mesh.h"
#include "modifiedBlinnPhongBRDF.h"
#include "modifiedPhongBRDF.h"
#include "ONB.h"
#include "pathtracingIntegrator.h"
#include "perlinNoise.h"
#include "perlinTexture.h"
#include "phongBRDF.h"
#include "ray.h"
#include "raytracingIntegrator.h"
#include "rdmGen.h"
#include "rgb.h"
#include "rtmath.h"
#include "safeStack.h"
#include "scene.h"
#include "shape.h"
#include "simpleBRDF.h"
#include "simpleCDF.h"
#include "simpleTexture.h"
#include "sphere.h"
#include "sphericalDirectionalLight.h"
#include "texture.h"
#include "tonemap.h"
#include "torranceSparrowBRDF.h"
#include "triangle.h"
#include "vector2.h"
#include "vector3.h"
#include "vertex.h"

#endif
