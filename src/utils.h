#ifndef _UTILS_H_
#define _UTILS_H_

// Macros
#define DEFAULT_THREAD_COUNT 8
#define DEFAULT_AIR_REFRACTION_INDEX 1.0f

// Cameras
#define CAM_SIMPLE 0
#define CAM_LOOKAT 1
// Meshes
#define MESH_SIMPLE 0
#define MESH_PLY 1

#include <iostream>
#include <math.h>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

#include "util/tinyxml2.h"
#include "BVH.h"
#include "camera.h"
#include "dynArray.h"
#include "image.h"
#include "light.h"
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

#include "util/happly.h"

#endif
