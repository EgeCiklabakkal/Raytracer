#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <array>
#include "vector3.h"
#include "material.h"
#include "shape.h"
#include "triangle.h"

class TriangleMesh : public Shape
{
	public:

	// Member variables
	std::vector<Vec3> *vertex_data_ptr;
	std::vector<Material> *material_data_ptr;

	std::vector<Triangle> triangles;	// each face stores id of 3 vertices
	int material_id;

	// Constructors
	TriangleMesh() {}
	TriangleMesh(std::vector<Vec3> *vertex_ptr, std::vector<Material> *material_ptr,
		std::vector<Triangle>& _triangles, int _material_id) :
	vertex_data_ptr(vertex_ptr), material_data_ptr(material_ptr), triangles(_triangles),
	material_id(_material_id) {}

	bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time) const;
};

#endif
