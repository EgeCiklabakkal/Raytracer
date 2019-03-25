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
	transformedRecord.normal = rtmath::transformVec(glm::transpose(N), record.uvw.w());
	ONB _uvw;
	_uvw.initFromW(transformedRecord.normal);
	transformedRecord.uvw = _uvw;
	
	return transformedRecord;
}
