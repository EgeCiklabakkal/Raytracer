#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "shape.h"

class Sphere : public Shape
{
	public:

	Vec3 center;
	float radius;
	rgb color;
	Material material;
	Texture *texture;
	ONB alignmentBasis;

	Sphere(const Vec3& _center, float _radius, const rgb& _color=rgb());
	Sphere(const Vec3& _center, float _radius, 
		const Material& _material, Texture* _texture=nullptr);
	virtual ~Sphere() {}

	bool hit(const Ray& r, float tmin, float tmax,
			float time, HitRecord& record, bool nonluminous) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time, bool nonluminous) const;
	bool boundingBox(float time0, float time1, BBox& _box) const;

	Vec2 textureUV(const Vec3& p) const;
	BumpRecord bump(const HitRecord& record) const;
};

#endif
