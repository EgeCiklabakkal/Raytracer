#include "shape.h"

Ray Shape::transformRayToLocal(const Ray& r) const
{
	Vec3 no, nd;

	if(!transformed && !motionBlurred)
	{
		return r;
	}	

	else if(!transformed && motionBlurred)
	{
		Vec3 lastCol = velocity * r.time;
		glm::mat4 currBlur = motionBlur;
		currBlur[3] = glm::vec4(lastCol[0], lastCol[1], lastCol[2], 1.0f); 	// Col major
		glm::mat4 invCurrBlur = glm::inverse(currBlur);

		no = rtmath::transformLoc(invCurrBlur, r.origin());
		nd = rtmath::transformVec(invCurrBlur, r.direction());
	}

	else if(transformed && !motionBlurred)
	{
		no = rtmath::transformLoc(N, r.origin());
		nd = rtmath::transformVec(N, r.direction());
	}

	else
	{
		Vec3 lastCol = velocity * r.time;
		glm::mat4 currBlur = motionBlur;
		currBlur[3] = glm::vec4(lastCol[0], lastCol[1], lastCol[2], 1.0f); 	// Col major
		glm::mat4 invCurrBlur = glm::inverse(currBlur);

		no = rtmath::transformLoc(N * invCurrBlur, r.origin());
		nd = rtmath::transformVec(N * invCurrBlur, r.direction());
	}
	
	return Ray(no, nd);
}

HitRecord Shape::transformRecordToWorld(const HitRecord& record) const
{
	HitRecord transformedRecord(record);

	if(!transformed && !motionBlurred)
	{
		return record;
	}

	else if(!transformed && motionBlurred)
	{
		Vec3 lastCol = velocity * record.time;
		glm::mat4 currBlur = motionBlur;
		currBlur[3] = glm::vec4(lastCol[0], lastCol[1], lastCol[2], 1.0f); 	// Col major
		glm::mat4 invCurrBlur = glm::inverse(currBlur);

		transformedRecord.p      = rtmath::transformLoc(currBlur, record.p);
		transformedRecord.normal = unitVector(rtmath::transformVec(
							glm::transpose(invCurrBlur), record.normal));
	}

	else if(transformed && !motionBlurred)
	{
		transformedRecord.p      = rtmath::transformLoc(M, record.p);
		transformedRecord.normal = unitVector(rtmath::transformVec(glm::transpose(N), 
										record.normal));
	}

	else
	{
		Vec3 lastCol = velocity * record.time;
		glm::mat4 currBlur = motionBlur;
		currBlur[3] = glm::vec4(lastCol[0], lastCol[1], lastCol[2], 1.0f); 	// Col major
		glm::mat4 invCurrBlur = glm::inverse(currBlur);

		transformedRecord.p      = rtmath::transformLoc(currBlur * M, record.p);
		transformedRecord.normal = unitVector(rtmath::transformVec(
								glm::transpose(N * invCurrBlur), 
								record.normal));
	}
	
	ONB _uvw;
	_uvw.initFromW(transformedRecord.normal);
	transformedRecord.uvw = _uvw;
	
	return transformedRecord;
}

BBox Shape::transformBBoxToWorld(const BBox& bbox) const
{
	if(!transformed && !motionBlurred)
	{
		return bbox;
	}

	else if(!transformed && motionBlurred)
	{
		BBox otherBox(bbox._min + velocity, bbox._max + velocity);
		return surrounding_box(bbox, otherBox);
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
	
	if(!motionBlurred)
	{
		return BBox(_min, _max);
	}

	else
	{
		BBox otherBox(_min + velocity, _max + velocity);
		return surrounding_box(BBox(_min, _max), otherBox);
	}
}

bool Shape::setMotionBlur(const Vec3& _velocity)
{
	if(_velocity.length())
	{
		motionBlurred = true;
		velocity   = _velocity;
		motionBlur = glm::translate(glm::mat4(1.0f), glm::vec3( velocity[0], 
									velocity[1], 
									velocity[2]));
		return true;
	}

	return false;
}
