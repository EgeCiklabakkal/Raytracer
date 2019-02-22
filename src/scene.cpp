#include <sstream>
#include <stdexcept>
#include <iostream>
#include "utils.h"

rgb Scene::rayColor(const Ray& r, int recursion_depth) const
{
	rgb rcolor;
	float tmax = 100000.0f;		// Note: tmax, tmin, time can be made an argument
	float tmin = 0.0001f;		
	float time = 0.0f;
	bool hit = false;
	HitRecord record;

	for(const Shape* shape : this->shapes)
	{
		if(shape->hit(r, tmin, tmax, time, record))
		{
			tmax = record.t;
			hit = true;
		}
	}
	
	if(hit)
	{
		rcolor = ambientColor(record);

		// Loop over lights
		for(const Light* light_ptr : this->lights)
		{
			bool continue_light_loop = false;
			Ray shadow_ray(r.shadowRay(record, light_ptr, shadow_ray_epsilon));
			float tlight = shadow_ray.parameterAtPoint(light_ptr->position);
			for(const Shape* shape : this->shapes)
			{
				if(shape->shadowHit(shadow_ray, 0.0f, tlight, time))
				{
					continue_light_loop = true;
					break;	
				}		
			}

			if(!continue_light_loop)
			{
				rcolor += diffuseColor(r, record, light_ptr) +
						specularColor(r, record, light_ptr);
			}
		}
		// Add color from reflections
		rcolor += reflectionColor(r, record, recursion_depth);

		// color is in bytes
		return rcolor;
	}

	return background_color;
}

rgb Scene::ambientColor(const HitRecord& record) const
{
	rgb Ia(ambient_light.intensity);
	rgb ka(record.material.ambient);

	return ka * Ia;
}

rgb Scene::diffuseColor(const Ray& r, const HitRecord& record, const Light* light_ptr) const
{
	rgb I(light_ptr->intensity);
	rgb kd(record.material.diffuse);

	Vec3 x = r.pointAtParameter(record.t);
	Vec3 wi(light_ptr->position - x);
	float r2 = wi.squaredLength();
	wi.makeUnitVector();

	float costheta = std::max(0.0f, dot(wi, record.normal));

	return (kd * costheta * I) / r2;
}

rgb Scene::specularColor(const Ray& r, const HitRecord& record, const Light* light_ptr) const
{
	rgb I(light_ptr->intensity);
	rgb ks(record.material.specular);
	float phong_exp = record.material.phong_exponent;

	Vec3 x = r.pointAtParameter(record.t);
	Vec3 wi(light_ptr->position - x);
	Vec3 wo(r.origin() - x);
	float r2 = wi.squaredLength();
	wi.makeUnitVector();
	wo.makeUnitVector();

	Vec3 h(wi + wo);
	h.makeUnitVector();

	float costheta = std::max(0.0f, dot(record.normal, h));

	return (ks * (pow(costheta, phong_exp)) * I) / r2;
}

rgb Scene::reflectionColor(const Ray& r, const HitRecord& record, int recursion_depth) const
{
	rgb km(record.material.mirror);

	if(!recursion_depth)
	{
		return rgb();
	}

	return km * rayColor(r.reflectionRay(record, shadow_ray_epsilon), recursion_depth - 1);
}

Scene::~Scene()
{
	// Free lights
	for(Light* light : lights)
	{
		delete light;
	}
	lights.clear();

	// Free shapes
	for(Shape* shape : shapes)
	{
		delete shape;
	}
	shapes.clear();
}

// Parse XML
void Scene::loadFromXML(const std::string& fname)
{
	tinyxml2::XMLDocument doc;
	std::stringstream ss;

	auto load_failed = doc.LoadFile(fname.c_str());
	if(load_failed)
	{
		throw std::runtime_error("Error: XML file failed to load!");
	}

	auto scene_element = doc.FirstChild();
	if(!scene_element)
	{
		throw std::runtime_error("Error: Scene element missing!");
	}

	// BackgroundColor
	auto element = scene_element->FirstChildElement("BackgroundColor");
	if(element)
	{
		ss << element->GetText() << std::endl;
	}

	else
	{
		ss << "0 0 0" << std::endl;
	}
	int icolor;
	ss >> icolor;
	background_color._r = float(icolor);
	ss >> icolor;
	background_color._g = float(icolor);
	ss >> icolor;
	background_color._b = float(icolor);

	// ShadowRayEpsilon
	element = scene_element->FirstChildElement("ShadowRayEpsilon");
	if(element)
	{
		ss << element->GetText() << std::endl;
	}

	else
	{
		ss << "0.001" << std::endl;
	}
	ss >> shadow_ray_epsilon;

	// MaxRecursionDepth
	element = scene_element->FirstChildElement("MaxRecursionDepth");
	if(element)
	{
		ss << element->GetText() << std::endl;
	}

	else
	{
		ss << "0" << std::endl;
	}
	ss >> max_recursion_depth;
	
	// Cameras
	element = scene_element->FirstChildElement("Cameras");
	element = element->FirstChildElement("Camera");
	while(element)
	{
		auto child = element->FirstChildElement("Position");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("Gaze");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("Up");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("NearPlane");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("NearDistance");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("ImageResolution");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("ImageName");
		ss << child->GetText() << std::endl;

		std::array<float, 4> near_plane;
		Vec3 pos, gaze, up;
		float near_distance;
		int w, h;
		std::string img_name;
		ss >> pos[0] >> pos[1] >> pos[2];
		ss >> gaze[0] >> gaze[1] >> gaze[2];
		ss >> up[0] >> up[1] >> up[2];
		ss >> near_plane[0] >> near_plane[1] 
			>> near_plane[2] >> near_plane[3];
		ss >> near_distance;
		ss >> w >> h;
		ss >> img_name;

		cameras.push_back(Camera(pos, gaze, up, near_plane, near_distance, w, h, img_name));
		element = element->NextSiblingElement("Camera");
	}

	// Lights
	element = scene_element->FirstChildElement("Lights");
	auto child = element->FirstChildElement("AmbientLight");
	ss << child->GetText() << std::endl;
	ss >> ambient_light.intensity[0] >> ambient_light.intensity[1] >> ambient_light.intensity[2];
	element = element->FirstChildElement("PointLight");
	while(element)
	{
		PointLight *point_light = new PointLight();
		child = element->FirstChildElement("Position");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("Intensity");
		ss << child->GetText() << std::endl;

		ss >> point_light->position[0] >> point_light->position[1] 
			>> point_light->position[2];
		ss >> point_light->intensity[0] >> point_light->intensity[1] 
			>> point_light->intensity[2];

		lights.push_back(point_light);
		element = element->NextSiblingElement("PointLight");
	}

	// Materials
	element = scene_element->FirstChildElement("Materials");
	element = element->FirstChildElement("Material");
	Material material;
	while(element)
	{
		child = element->FirstChildElement("AmbientReflectance");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("DiffuseReflectance");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("SpecularReflectance");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("MirrorReflectance");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("PhongExponent");
		ss << child->GetText() << std::endl;

		ss >> material.ambient[0] >> material.ambient[1] >> material.ambient[2];
		ss >> material.diffuse[0] >> material.diffuse[1] >> material.diffuse[2];
		ss >> material.specular[0] >> material.specular[1] >> material.specular[2];
		ss >> material.mirror[0] >> material.mirror[1] >> material.mirror[2];
		ss >> material.phong_exponent;

		materials.push_back(material);
		element = element->NextSiblingElement("Material");
	}

	// VertexData
	element = scene_element->FirstChildElement("VertexData");
	ss << element->GetText() << std::endl;
	Vec3 vertex;
	while(!(ss >> vertex[0]).eof())
	{
		ss >> vertex[1] >> vertex[2];
		vertex_data.push_back(vertex);
	}
	ss.clear();

	// Objects
	element = scene_element->FirstChildElement("Objects");
	element = element->FirstChildElement("Mesh");
	int itemp;
	Vec3i vitemp;
	while(element)
	{
		std::vector<Triangle> meshTriangles;
		Material meshMaterial;
		child = element->FirstChildElement("Material");
		ss << child->GetText() << std::endl;
		ss >> itemp;
		meshMaterial = materials[itemp - 1];

		child = element->FirstChildElement("Faces");
		ss << child->GetText() << std::endl;
		while(!(ss >> vitemp.e[0]).eof())
		{
			ss >> vitemp.e[1] >> vitemp.e[2];
			Triangle ttemp = Triangle(vertex_data[vitemp.e[0] - 1], 
				vertex_data[vitemp.e[1] - 1], vertex_data[vitemp.e[2] - 1], 
					meshMaterial);
			meshTriangles.push_back(ttemp);
		}
		ss.clear();

		Shape *shape_ptr = new TriangleMesh(meshTriangles, meshMaterial);
		shapes.push_back(shape_ptr);

		element = element->NextSiblingElement("Mesh");
	}
	ss.clear();

	element = scene_element->FirstChildElement("Objects");
	element = element->FirstChildElement("Triangle");
	while(element)
	{
		Material triangleMaterial;
		int itemp;
		Vec3i vitemp;
		child = element->FirstChildElement("Material");
		ss << child->GetText() << std::endl;
		ss >> itemp;
		triangleMaterial = materials[itemp - 1];

		child = element->FirstChildElement("Indices");
		ss << child->GetText() << std::endl;
		ss >> vitemp.e[0] >> vitemp.e[1] >> vitemp.e[2];
		
		Shape *triangle_ptr = new Triangle(vertex_data[vitemp.e[0] - 1],
                                vertex_data[vitemp.e[1] - 1], vertex_data[vitemp.e[2] - 1],
                                        triangleMaterial);

		shapes.push_back(triangle_ptr);
		element = element->NextSiblingElement("Triangle");
	}

	element = scene_element->FirstChildElement("Objects");
	element = element->FirstChildElement("Sphere");
	while(element)
	{
		Material sphereMaterial;
		Vec3 center;
		float radius;
		int itemp;
		child = element->FirstChildElement("Material");
		ss << child->GetText() << std::endl;
		ss >> itemp;
		sphereMaterial = materials[itemp - 1];
		
		child = element->FirstChildElement("Center");
		ss << child->GetText() << std::endl;
		ss >> itemp;
		center = vertex_data[itemp - 1];

		child = element->FirstChildElement("Radius");
		ss << child->GetText() << std::endl;
		ss >> radius;

		Shape *sphere_ptr = new Sphere(center, radius, sphereMaterial);
		shapes.push_back(sphere_ptr);

		element = element->NextSiblingElement("Sphere");
	}
}
