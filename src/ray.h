#ifndef _RAY_H_
#define _RAY_H_

#include <utility>
#include "vector3.h"
#include "shape.h"
#include "rtmath.h"
#include "ONB.h"

class HitRecord;
class SampleLight;

class Ray
{
	public:

	// Origin, Direction
	Vec3 data[2];
	float weight;
	bool primary;
	float time;	// Creation time for motion blur calculations

	// Constructors
	Ray() { time = rtmath::randf(); }
	Ray(const Vec3& origin, const Vec3& direction, float _weight=1.0f, bool _primary=false) 
	: weight(_weight), primary(_primary)
	{ data[0] = origin; data[1] = direction; time = rtmath::randf(); }
	Ray(const Ray& r) { *this = r; }
	
	// Getters
	Vec3 origin() const { return data[0]; }
	Vec3 direction() const { return data[1]; }

	// Setters
	void setOrigin(const Vec3& v) { data[0] = v; }
	void setDirection(const Vec3& v) { data[1] = v; }
	void setTime(float _time) { time = _time; }
	
	// Methods
	Vec3 pointAtParameter(float t) const
	{ return data[0] + t*data[1]; }

	Ray reflectionRay(const HitRecord& record, float epsilon) const;
	Ray shadowRay(const HitRecord& record, const Vec3& to, float epsilon) const;
	Ray transmissionRay(const HitRecord& record, const Vec3& d, float epsilon) const;
	float parameterAtPoint(const Vec3& point) const;
	bool refract(const HitRecord& record, std::pair<float, float> refractionIndexes,
			Vec3& transmissionDirection) const;

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
