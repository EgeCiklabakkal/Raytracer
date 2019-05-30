#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "shape.h"
#include "vector3.h"
#include "rgb.h"
#include "ray.h"
#include "material.h"

class Triangle : public Shape
{
	public:
	
	// Member variables
	Vec3 vertices[3];	// are in ccw order
	Vec2 texCoords[3];
	Vec3 normal;
	Material material;
	Texture *texture;
	rgb color;

	// Constructors
	Triangle(const Vec3& _p0, const Vec3& _p1, const Vec3& _p2, const rgb& _color=rgb());
	Triangle(const Vec3& _p0, const Vec3& _p1, const Vec3& _p2,
			const Vec3& _normal, const Material& _material, Texture* _texture=nullptr);
	Triangle(const Vec3& _p0, const Vec3& _p1, const Vec3& _p2,
			const Vec2& _tc0, const Vec2& _tc1, const Vec2& _tc2,
			const Vec3& _normal, const Material& _material, Texture* _texture=nullptr);

	// Destructor
	~Triangle() {}
	
	// Virtual Methods
	bool hit(const Ray& r, float tmin, float tmax,
			float time, HitRecord& record, bool nonluminous) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time, bool nonluminous) const;
	bool boundingBox(float time0, float time1, BBox& _box) const;

	// Getters
	Vec3 p0() const { return vertices[0]; }
	Vec3 p1() const { return vertices[1]; }
	Vec3 p2() const { return vertices[2]; }
	Vec2 texCoord0() const { return texCoords[0]; }
	Vec2 texCoord1() const { return texCoords[1]; }
	Vec2 texCoord2() const { return texCoords[2]; }
	Vec2 textureUV(float beta, float gamma) const;

	// Methods
};

#endif
