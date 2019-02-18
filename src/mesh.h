#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <array>
#include "vector3.h"
#include "material.h"
#include "shape.h"
#include "triangle.h"

// Mesh class can be changed at a later time to store vertices, rather than triangles
// Then MeshTriangle class can inherit from class Mesh

class TriangleMesh : public Shape
{
	public:

	// Member variables
	std::vector<Triangle> triangles;	// each face stores id of 3 vertices
	Material material;

	// Constructors
	TriangleMesh() {}
	TriangleMesh(std::vector<Triangle>& _triangles, const Material& _material) :
	triangles(_triangles), material(_material) {}
	virtual ~TriangleMesh() {}

	bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time) const;

	void print() const;
};

#endif
