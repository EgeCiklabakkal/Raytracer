#ifndef _SIMPLECDF_H_
#define _SIMPLECDF_H_

#include <vector>

#include "shape.h"
#include "mesh.h"

class SimpleCDF
{
	public:

	float area;	// Sum of the areas of triangles
	unsigned int n;	// area_data.size()
	std::vector<float> area_data;
	std::vector<MeshTriangle*> ptr_data;

	SimpleCDF(const std::vector<Shape*>& meshTriangles, const glm::mat4& trans=glm::mat4(1.0f));
	~SimpleCDF() {}

	MeshTriangle* query(float key) const;
};

#endif
