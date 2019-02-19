#ifndef _RAY_H_
#define _RAY_H_

#include "vector3.h"
#include "shape.h"
#include "light.h"

class HitRecord;

class Ray
{
	public:

	// Origin, Direction
	Vec3 data[2];

	// Constructors
	Ray() {}
	Ray(const Vec3& origin, const Vec3& direction)
	{ data[0] = origin; data[1] = direction; }
	Ray(const Ray& r) { *this = r; }
	
	// Getters
	Vec3 origin() const { return data[0]; }
	Vec3 direction() const { return data[1]; }

	// Setters
	void setOrigin(const Vec3& v) { data[0] = v; }
	void setDirection(const Vec3& v) { data[1] = v; }
	
	// Methods
	Vec3 pointAtParameter(float t) const
	{ return data[0] + t*data[1]; }

	Ray reflectionRay(const HitRecord& record, float epsilon) const;
	Ray shadowRay(const HitRecord& record, const Light* light_ptr, float epsilon) const;
	float parameterAtPoint(const Vec3& point) const;

	// Function aliases
	Vec3 o() const { return origin(); }
	Vec3 d() const { return direction(); }

	friend std::ostream& operator<<(std::ostream& os, const Ray& r);
};

inline std::ostream& operator<<(std::ostream& os, const Ray& r)
{
	os << r.origin() << " + t" << r.direction();
	return os; 
}

#endif
