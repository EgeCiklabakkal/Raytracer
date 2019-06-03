#include "shape.h"

Ray Shape::transformRayToLocal(const Ray& r) const
{
	Vec3 no, nd;
	Ray tray;

	if(!transformed && !motionBlurred)
	{
		return r;
	}	

	else if(!transformed && motionBlurred)
	{
		Vec3 blurDistance = velocity * r.time;
		glm::mat4 currBlur = glm::translate(glm::mat4(1.0f), glm::vec3( blurDistance[0],
										blurDistance[1],
										blurDistance[2])); 
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
		Vec3 blurDistance = velocity * r.time;
		glm::mat4 currBlur = glm::translate(glm::mat4(1.0f), glm::vec3( blurDistance[0],
										blurDistance[1],
										blurDistance[2])); 
		glm::mat4 invCurrBlur = glm::inverse(currBlur);

		no = rtmath::transformLoc(N * invCurrBlur, r.origin());
		nd = rtmath::transformVec(N * invCurrBlur, r.direction());
	}
	
	tray = Ray(no, nd);
	tray.setTime(r.time);
	return tray;
}

HitRecord Shape::transformRecordToWorld(const HitRecord& record) const
{
	HitRecord transformedRecord(record);

	if(!transformed && !motionBlurred)
	{
		transformedRecord.M	 = glm::mat4(1.0f);
		return transformedRecord;
	}

	else if(!transformed && motionBlurred)
	{
		Vec3 blurDistance = velocity * record.time;
		glm::mat4 currBlur = glm::translate(glm::mat4(1.0f), glm::vec3( blurDistance[0],
										blurDistance[1],
										blurDistance[2]));
		glm::mat4 invCurrBlur = glm::inverse(currBlur);

		transformedRecord.p      = rtmath::transformLoc(currBlur, record.p);
		transformedRecord.normal = unitVector(rtmath::transformVec(
							glm::transpose(invCurrBlur), record.normal));
		transformedRecord.M	 = currBlur * M;
	}

	else if(transformed && !motionBlurred)
	{
		transformedRecord.p      = rtmath::transformLoc(M, record.p);
		transformedRecord.normal = unitVector(rtmath::transformVec(glm::transpose(N), 
										record.normal));
		transformedRecord.M	 = M;
	}

	else
	{
		Vec3 blurDistance = velocity * record.time;
		glm::mat4 currBlur = glm::translate(glm::mat4(1.0f), glm::vec3( blurDistance[0],
										blurDistance[1],
										blurDistance[2]));
		glm::mat4 invCurrBlur = glm::inverse(currBlur);

		transformedRecord.p      = rtmath::transformLoc(currBlur * M, record.p);
		transformedRecord.normal = unitVector(rtmath::transformVec(
								glm::transpose(N * invCurrBlur), 
								record.normal));
		transformedRecord.M	 = currBlur * M;
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
		return true;
	}

	return false;
}

bool Shape::setTransform(const glm::mat4& transformMatrix, bool resetTransform)
{
	if(resetTransform)
	{
		M = transformMatrix;
	}

	else
	{
		M = transformMatrix * M;
	}

	N = glm::inverse(M);
	transformed = true;

	return true;
}
