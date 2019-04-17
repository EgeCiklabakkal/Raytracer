#include "sphere.h"

Sphere::Sphere(const Vec3& _center, float _radius, const rgb& _color) :
center(_center), radius(_radius), color(_color) 
{}

Sphere::Sphere(const Vec3& _center, float _radius, 
		const Material& _material, Texture* _texture) :
center(_center), radius(_radius), material(_material), texture(_texture) 
{}

/*
	At² + Bt + C = 0
*/
bool Sphere::hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const
{
	Ray tray = transformRayToLocal(r);

	Vec3 d  = tray.direction();
	Vec3 o  = tray.origin();
	Vec3 oc = o - center;

	double A = dot(d, d);
	double B = 2 * dot(d, oc);
	double C = dot(oc, oc) - radius*radius;

	double discriminant = B*B - 4*A*C;

	// first check to see if ray intercepts sphere
	if(discriminant > 0)
	{
		discriminant = sqrt(discriminant);
		double _t = (-B - discriminant) / (2*A);

		// now check for a valid interval
		if(_t < tmin)
			_t = (-B + discriminant) / (2*A);
		if(_t < tmin || _t > tmax)
			return false;

		// we have a valid hit
		record.t 	 = _t;
		record.p	 = o + _t*d;
		record.normal 	 = unitVector(record.p - center);
		ONB _uvw;
		_uvw.initFromW(record.normal);
		record.uvw	 = _uvw;
		record.color  	 = color;
		record.material  = material;
		record.time      = r.time;
		record.texture   = texture;
		record.uv	 = textureUV(record.p);
		record.bump	 = bump(record);

		record = transformRecordToWorld(record);
		return true;
	}

	return false;
}

bool Sphere::shadowHit(const Ray& r, float tmin, float tmax, float time) const
{
	Ray tray = transformRayToLocal(r);

	Vec3 d  = tray.direction();
	Vec3 o  = tray.origin();
	Vec3 oc = o - center;

	double A = dot(d, d);
	double B = 2 * dot(d, oc);
	double C = dot(oc, oc) - radius*radius;

	double discriminant = B*B - 4*A*C;

	// first check to see if ray intercepts sphere
	if(discriminant > 0)
	{
		discriminant = sqrt(discriminant);
		double _t = (-B - discriminant) / (2*A);

		// now check for a valid interval
		if(_t < tmin)
			_t = (-B + discriminant) / (2*A);
		if(_t < tmin || _t > tmax)
			return false;

		// we have a valid hit
		return true;
	}

	return false;
}

bool Sphere::boundingBox(float time0, float time1, BBox& _box) const
{
	Vec3 radius3(radius, radius, radius);

	_box = BBox(center - radius3, center + radius3);
	_box = transformBBoxToWorld(_box);
	return true;
}

Vec2 Sphere::textureUV(const Vec3& p) const
{
	if(!texture)
	{
		return Vec2();	// dummy
	}

	// sphere is always unit at origin, no need to translate or divide by r
	//float theta = acos (dot(alignmentBasis.V, p));
	//float phi   = atan2(dot(alignmentBasis.W, p), dot(alignmentBasis.U, p));
	float theta = acos (p.y());
	float phi   = atan2(p.z(), p.x());

	float u = (-phi + M_PI) * INV_2PI;
	float v = theta * INV_PI;
	//float v = (M_PI - theta) * INV_PI;	// this reverses Y

	return Vec2(u, v);
}

BumpRecord Sphere::bump(const HitRecord& record) const
{
	Texture *texture = record.texture;

	if(texture && texture->bumpmap)
	{
		Vec3 p = record.p;
		float theta = acos (p.y());
		float phi   = atan2(p.z(), p.x());

		float dxdu =  p.z() * 2 * M_PI;
		float dydu =  0.0f;
		float dzdu = -p.x() * 2 * M_PI;

		float dxdv =  p.y() * cos(phi) * M_PI;
		float dydv = -sin(theta) * M_PI;
		float dzdv =  p.y() * sin(phi) * M_PI;

		Vec3 dpdu = Vec3(dxdu, dydu, dzdu);
		Vec3 dpdv = Vec3(dxdv, dydv, dzdv);

		return BumpRecord(dpdu, dpdv, M);
	}

	return BumpRecord();
}
