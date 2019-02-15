#include "mesh.h"

bool TriangleMesh::hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const
{
	float mesh_tmin, mesh_tmax, mesh_time;
	HitRecord mesh_record;
	bool hit;

	mesh_tmin = tmin;
	mesh_tmax = tmax;
	mesh_time = time;
	hit  = false;
	for(const Triangle& triangle : this->triangles)
	{
		if(triangle.hit(r, mesh_tmin, mesh_tmax, mesh_time, mesh_record))
		{
			mesh_tmax = mesh_record.t;
			record = mesh_record;
			hit = true;
		}
	}

	if(hit)
	{
		return true;
	}

	return false;
}

bool TriangleMesh::shadowHit(const Ray& r, float tmin, float tmax, float time) const
{
	for(const Triangle& triangle : this->triangles)
	{
		if(triangle.shadowHit(r, tmin, tmax, time))
		{
			return true;
		}
	}

	return false;
}
