#include "shape.h"

Ray Shape::transformRayToLocal(const Ray& r) const
{
	if(!transformed)
	{
		return r;
	}

	Vec3 no = rtmath::transformLoc(N, r.origin());
	Vec3 nd = rtmath::transformVec(N, r.direction());

	return Ray(no, nd);
}

HitRecord Shape::transformRecordToWorld(const HitRecord& record) const
{
	if(!transformed)
	{
		return record;
	}

	HitRecord transformedRecord(record);
	
	transformedRecord.p      = rtmath::transformLoc(M, record.p);
	transformedRecord.normal = unitVector(rtmath::transformVec(glm::transpose(N), 
						record.normal));
	ONB _uvw;
	_uvw.initFromW(transformedRecord.normal);
	transformedRecord.uvw = _uvw;
	
	return transformedRecord;
}

BBox Shape::transformBBoxToWorld(const BBox& bbox) const
{
	if(!transformed)
	{
		return bbox;
	}

	Vec3 _min, _max;

	// Get 8 bounding vertices
	std::vector<Vec3> vertices;
	bbox.getVertices(vertices);

	// Transform vertices
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = rtmath::transformLoc(M, vertices[i]);
	}

	_min = vertices[0];
	_max = vertices[0];
	for(unsigned int i = 1; i < vertices.size(); i++)
	{
		_min.e[0] = vertices[i].x() < _min.x() ? vertices[i].x() : _min.x();
		_min.e[1] = vertices[i].y() < _min.y() ? vertices[i].y() : _min.y();
		_min.e[2] = vertices[i].z() < _min.z() ? vertices[i].z() : _min.z();

		_max.e[0] = vertices[i].x() > _max.x() ? vertices[i].x() : _max.x();
		_max.e[1] = vertices[i].y() > _max.y() ? vertices[i].y() : _max.y();
		_max.e[2] = vertices[i].z() > _max.z() ? vertices[i].z() : _max.z();
	}

	return BBox(_min, _max);
}
