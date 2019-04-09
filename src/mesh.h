#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <array>
#include "vector3.h"
#include "material.h"
#include "shape.h"
#include "triangle.h"
#include "vertex.h"
#include "macros.h"

// Triangle Mesh
class Mesh
{
	public:

	// Member variables
	std::vector<Vertex>* scene_vertex_data;
	Material material;
	Texture *texture;

	// Constructors
	Mesh() {}
	Mesh(std::vector<Vertex>* _scene_vertex_data, 
		const Material& _material, Texture* _texture=nullptr) :
	scene_vertex_data(_scene_vertex_data), material(_material), texture(_texture) {}
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
	int shadingMode;	// 0: Flat, 1: Smooth
	rgb color;

	// Constructors
	MeshTriangle(const std::array<int, 3>& _p, Mesh* _parent_mesh, const Vec3& _normal, 
			int _shadingMode=MESH_SHADING_FLAT) :
	p(_p), parent_mesh(_parent_mesh), normal(_normal), shadingMode(_shadingMode) {}

	// Destructor
	virtual ~MeshTriangle() {}

	// Virtual Methods
	bool hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time) const;
	bool boundingBox(float time0, float time1, BBox& _box) const;

	// Getters
	Vec3 a() const { return (*parent_mesh->scene_vertex_data)[p[0]].position; }
	Vec3 b() const { return (*parent_mesh->scene_vertex_data)[p[1]].position; }
	Vec3 c() const { return (*parent_mesh->scene_vertex_data)[p[2]].position; }

	Vertex va() const { return (*parent_mesh->scene_vertex_data)[p[0]]; }
	Vertex vb() const { return (*parent_mesh->scene_vertex_data)[p[1]]; }
	Vertex vc() const { return (*parent_mesh->scene_vertex_data)[p[2]]; }

	int ia() const { return p[0]; }
	int ib() const { return p[1]; }
	int ic() const { return p[2]; }
};

#endif
