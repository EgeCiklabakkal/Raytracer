#include "mesh.h"

bool MeshTriangle::hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record) const
{
	Ray tray = transformRayToLocal(r);

	float _a = a().x() - b().x();
        float _b = a().y() - b().y();
        float _c = a().z() - b().z();
        float _d = a().x() - c().x();
        float _e = a().y() - c().y();
        float _f = a().z() - c().z();
        float _g = tray.direction().x();
        float _h = tray.direction().y();
        float _i = tray.direction().z();
        float _j = a().x() - tray.origin().x();
        float _k = a().y() - tray.origin().y();
        float _l = a().z() - tray.origin().z();

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
                record.t = _t;
		record.p = tray.origin() + _t*tray.direction();

		if(shadingMode == MESH_SHADING_SMOOTH)
		{
			Vec3 na = va().normal;
			Vec3 nb = vb().normal;
			Vec3 nc = vc().normal;

			record.normal = na + beta*(nb - na) + gamma*(nc - na);
		}
		
		else	// MESH_SHADING_FLAT
		{
			record.normal = normal;
		}

		ONB _uvw;
		_uvw.initFromW(record.normal);
		record.uvw	= _uvw;
                record.color 	= color;
                record.material = parent_mesh->material;
		record.time 	= r.time;		

		record = transformRecordToWorld(record);
                return true;
        }

        return false;
}

bool MeshTriangle::shadowHit(const Ray& r, float tmin, float tmax, float time) const
{
	Ray tray = transformRayToLocal(r);

        float _a = a().x() - b().x();
        float _b = a().y() - b().y();
        float _c = a().z() - b().z();
        float _d = a().x() - c().x();
        float _e = a().y() - c().y();
        float _f = a().z() - c().z();
        float _g = tray.direction().x();
        float _h = tray.direction().y();
        float _i = tray.direction().z();
        float _j = a().x() - tray.origin().x();
        float _k = a().y() - tray.origin().y();
        float _l = a().z() - tray.origin().z();

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

bool MeshTriangle::boundingBox(float time0, float time1, BBox& _box) const
{
        Vec3 vx(a().x(), b().x(), c().x());
        Vec3 vy(a().y(), b().y(), c().y());
        Vec3 vz(a().z(), b().z(), c().z());

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
