#include "triangle.h"

Triangle::Triangle(const Vec3& _p0, const Vec3& _p1, const Vec3& _p2, const rgb& _color)
{
	vertices[0] = _p0;
	vertices[1] = _p1;
	vertices[2] = _p2;
	color = _color;
}

Triangle::Triangle(const Vec3& _p0, const Vec3& _p1, const Vec3& _p2, 
			const Vec3& _normal, const Material& _material, Texture* _texture)
{
	vertices[0] = _p0;
	vertices[1] = _p1;
	vertices[2] = _p2;
	normal = _normal;
	material = _material;
	texture = _texture;
}

/*
	┌       ┐ ┌   ┐   ┌   ┐
	│a  d  g│ │ β │   │ j │
	│b  e  h│ │ γ │ = │ k │
	│c  f  i│ │ t │   │ l │
	└       ┘ └   ┘   └   ┘
*/
bool Triangle::hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const
{
	Ray tray = transformRayToLocal(r);

	float _a = vertices[0].x() - vertices[1].x();
	float _b = vertices[0].y() - vertices[1].y();
	float _c = vertices[0].z() - vertices[1].z();
	float _d = vertices[0].x() - vertices[2].x();
	float _e = vertices[0].y() - vertices[2].y();
	float _f = vertices[0].z() - vertices[2].z();
	float _g = tray.direction().x();
	float _h = tray.direction().y();
	float _i = tray.direction().z();
	float _j = vertices[0].x() - tray.origin().x();
	float _k = vertices[0].y() - tray.origin().y();
	float _l = vertices[0].z() - tray.origin().z();

	float eihf = (_e * _i) - (_h * _f);
	float gfdi = (_g * _f) - (_d * _i);
	float dheg = (_d * _h) - (_e * _g);

	float M = _a*(eihf) +_b*(gfdi) + _c*(dheg);

	float beta = (_j*(eihf) + _k*(gfdi) + _l*(dheg)) / M;

	if(beta < 0.0f || beta > 1.0f) return false;

	float akjb = (_a * _k) - (_j * _b);
	float jcal = (_j * _c) - (_a * _l);
	float blkc = (_b * _l) - (_k * _c);

	float gamma = (_i*(akjb) + _h*(jcal) + _g*(blkc)) / M;

	if(gamma < 0.0f || beta + gamma > 1.0f) return false;

	float _t = -(_f*(akjb) + _e*(jcal) + _d*(blkc)) / M;

	if(_t >= tmin && _t <= tmax)
	{
		record.t 	 = _t;
		record.p	 = tray.origin() + _t*tray.direction();
		record.normal 	 = normal;
		ONB _uvw;
		_uvw.initFromW(normal);
		record.uvw	 = _uvw;
		record.color 	 = color;
		record.material  = material;
		record.time	 = r.time;
		record.texture   = texture;

		record = transformRecordToWorld(record);
		return true;
	}

	return false;
}

bool Triangle::shadowHit(const Ray& r, float tmin, float tmax, float time) const
{
	Ray tray = transformRayToLocal(r);

	float _a = vertices[0].x() - vertices[1].x();
	float _b = vertices[0].y() - vertices[1].y();
	float _c = vertices[0].z() - vertices[1].z();
	float _d = vertices[0].x() - vertices[2].x();
	float _e = vertices[0].y() - vertices[2].y();
	float _f = vertices[0].z() - vertices[2].z();
	float _g = tray.direction().x();
	float _h = tray.direction().y();
	float _i = tray.direction().z();
	float _j = vertices[0].x() - tray.origin().x();
	float _k = vertices[0].y() - tray.origin().y();
	float _l = vertices[0].z() - tray.origin().z();

	float eihf = (_e * _i) - (_h * _f);
	float gfdi = (_g * _f) - (_d * _i);
	float dheg = (_d * _h) - (_e * _g);

	float M = _a*(eihf) +_b*(gfdi) + _c*(dheg);

	float beta = (_j*(eihf) + _k*(gfdi) + _l*(dheg)) / M;

	if(beta < 0.0f || beta > 1.0f) return false;

	float akjb = (_a * _k) - (_j * _b);
	float jcal = (_j * _c) - (_a * _l);
	float blkc = (_b * _l) - (_k * _c);

	float gamma = (_i*(akjb) + _h*(jcal) + _g*(blkc)) / M;

	if(gamma < 0.0f || beta + gamma > 1.0f) return false;

	float _t = -(_f*(akjb) + _e*(jcal) + _d*(blkc)) / M;

	return (_t >= tmin && _t <= tmax);
}

bool Triangle::boundingBox(float time0, float time1, BBox& _box) const
{
	Vec3 vx(vertices[0].x(), vertices[1].x(), vertices[2].x());
	Vec3 vy(vertices[0].y(), vertices[1].y(), vertices[2].y());
	Vec3 vz(vertices[0].z(), vertices[1].z(), vertices[2].z());

	float xmin = vx.minComponent();
	float ymin = vy.minComponent();
	float zmin = vz.minComponent();

	float xmax = vx.maxComponent();
	float ymax = vy.maxComponent();
	float zmax = vz.maxComponent();

	_box = BBox(Vec3(xmin, ymin, zmin), Vec3(xmax, ymax, zmax));
	_box = transformBBoxToWorld(_box);
	return true;
}
