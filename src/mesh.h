#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <array>
#include "vector3.h"
#include "material.h"
#include "shape.h"
#include "triangle.h"

// Triangle Mesh
class Mesh
{
	public:

	// Member variables
	std::vector<Vec3>* scene_vertex_data;
	Material material;

	// Constructors
	Mesh() {}
	Mesh(std::vector<Vec3>* _scene_vertex_data, const Material& _material) :
	scene_vertex_data(_scene_vertex_data), material(_material) {}
	virtual ~Mesh() {}
};

class MeshTriangle : public Shape
{
	public:

	// Member variables
	// Vertices are named as a, b, c in ccw order
	std::array<int, 3> p;	// Indices to vertex_data (0 based indexing)
	Mesh *parent_mesh;
	Vec3 normal;
	rgb color;

	// Constructors
	MeshTriangle(const std::array<int, 3>& _p, Mesh* _parent_mesh, const Vec3& _normal) :
	p(_p), parent_mesh(_parent_mesh), normal(_normal) {}

	// Destructor
	virtual ~MeshTriangle() {}

	// Virtual Methods
	bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time) const;

	// Getters
	Vec3 a() const { return (*parent_mesh->scene_vertex_data)[p[0]]; }
	Vec3 b() const { return (*parent_mesh->scene_vertex_data)[p[1]]; }
	Vec3 c() const { return (*parent_mesh->scene_vertex_data)[p[2]]; }

	int ia() const { return p[0]; }
	int ib() const { return p[1]; }
	int ic() const { return p[2]; }
};

#endif
