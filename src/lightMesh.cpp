#include "lightMesh.h"

LightMesh::LightMesh(const glm::mat4& trans, Shape* _prim, const Material& _material,
			const std::vector<Shape*>& meshTriangles, 
			const Vec3& _radiance, bool _transformed) :
ObjectInstance{trans, _prim, _material, _transformed}, cdf(meshTriangles, trans), radiance(_radiance)
{
	object = true;
	material.luminous = true;
}

bool LightMesh::sampleLight(const Scene* scene, const Ray& r, const HitRecord& record,
				SampleLight& sampledLight, bool nonluminous) const
{
	MeshTriangle *triangle_ptr = cdf.query(rtmath::randf());
	Vec3 pa = rtmath::transformLoc(M, triangle_ptr->a());
	Vec3 pb = rtmath::transformLoc(M, triangle_ptr->b());
	Vec3 pc = rtmath::transformLoc(M, triangle_ptr->c());

	// Compute emitting point
	float e1 = rtmath::randf();
	float sqrt_e1 = sqrt(e1);
	float e2 = rtmath::randf();
	Vec3 p = (1.0f - e2) * pb + e2 * pc;
	Vec3 q = sqrt_e1 * p + (1.0f - sqrt_e1) * pa;

	HitRecord lightRecord;
	Ray sampleRay(record.p, unitVector(Vec3(q - record.p)));
	sampleRay.primary = true; // make the ray primary so that it may get culled
	if(triangle_ptr->hit(sampleRay, 0.0f, FLT_MAX, 0.0f, lightRecord, true, false))
	{
		Vec3 wi(q - record.p);
		Ray shadow_ray(r.shadowRay(record, q, scene->shadow_ray_epsilon));

		// nonluminous = false, check if the luminous object is blocking the light
		float tlight = shadow_ray.parameterAtPoint(q - wi * scene->shadow_ray_epsilon);
		if(scene->bvh->shadowHit(shadow_ray, 0.0f, tlight, r.time, false, false))
		{
			return false;
		}

		float r2 = std::max(0.000001f, wi.squaredLength());
		float costheta = std::max(0.0f, dot(lightRecord.normal, -wi));
		wi.makeUnitVector();
		float pw_1 = (cdf.area * costheta) / r2; // 1 / p(w)
		sampledLight = SampleLight(radiance * pw_1, wi);
		return true;
	}

	return false;
}

bool LightMesh::hit(const Ray& r, float tmin, float tmax, float time, HitRecord& record,
			bool cullFace, bool nonluminous) const
{
	if(nonluminous) return false;

	bool isHit = ObjectInstance::hit(r, tmin, tmax, time, record, cullFace, nonluminous);

	record.color = radiance;
	return isHit;
}

bool LightMesh::shadowHit(const Ray& r, float tmin, float tmax, float time,
			bool cullFace, bool nonluminous) const
{
	if(nonluminous) return false;

	return ObjectInstance::shadowHit(r, tmin, tmax, time, cullFace, nonluminous);
}
