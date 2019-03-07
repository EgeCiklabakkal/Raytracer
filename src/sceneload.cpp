#include "utils.h"

bool getChildTextWithDefault(tinyxml2::XMLElement* element, std::stringstream& ss, 
				std::string name, std::string _default);

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

	element = scene_element->FirstChildElement("IntersectionTestEpsilon");
	if(element)
	{
		ss << element->GetText() << std::endl;
	}

	else
	{
		ss << "1e-6" << std::endl;
	}
	ss >> intersection_test_epsilon;

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
		getChildTextWithDefault(element, ss, "MirrorReflectance", "0 0 0");
		getChildTextWithDefault(element, ss, "PhongExponent", "1");
		getChildTextWithDefault(element, ss, "Transparency", "0 0 0");
		getChildTextWithDefault(element, ss, "RefractionIndex", "1");

		ss >> material.ambient[0] >> material.ambient[1] >> material.ambient[2];
		ss >> material.diffuse[0] >> material.diffuse[1] >> material.diffuse[2];
		ss >> material.specular[0] >> material.specular[1] >> material.specular[2];
		ss >> material.mirror[0] >> material.mirror[1] >> material.mirror[2];
		ss >> material.phong_exponent;
		ss >> material.transparency[0] >> material.transparency[1] 
			>> material.transparency[2];
		ss >> material.refraction_index;

		materials.push_back(material);
		element = element->NextSiblingElement("Material");
	}
	ss.clear();

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
	while(element)
	{
		int itemp;
		std::array<int, 3> ptemp;
		Material meshMaterial;
		child = element->FirstChildElement("Material");
		ss << child->GetText() << std::endl;
		ss >> itemp;
		meshMaterial = materials[itemp - 1];
		Mesh *mesh = new Mesh(&vertex_data, meshMaterial);
		meshes.push_back(mesh);

		child = element->FirstChildElement("Faces");
		ss << child->GetText() << std::endl;
		while(!(ss >> ptemp[0]).eof())
		{
			ss >> ptemp[1] >> ptemp[2];
			
			// 1 based to 0 based
			ptemp[0] = ptemp[0] - 1;
			ptemp[1] = ptemp[1] - 1;
			ptemp[2] = ptemp[2] - 1;

			// Precompute normal
			Vec3 a(vertex_data[ptemp[0]]);
			Vec3 b(vertex_data[ptemp[1]]);
			Vec3 c(vertex_data[ptemp[2]]);
			Vec3 n(unitVector(cross(b - a, c - a)));
			Shape *mt = new MeshTriangle(ptemp, mesh, n);
			shapes.push_back(mt);
		}
		ss.clear();

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

		// Precompute normal
		Vec3 a(vertex_data[vitemp.e[0] - 1]);
		Vec3 b(vertex_data[vitemp.e[1] - 1]);
		Vec3 c(vertex_data[vitemp.e[2] - 1]);
		Vec3 n(unitVector(cross(b - a, c - a)));
		Shape *triangle_ptr = new Triangle(a, b, c, n, triangleMaterial);

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

	// BVH
	bvh = new BVH(shapes.data(), (int)shapes.size(), 0, 0.0f, 0.0f, intersection_test_epsilon);
}

// Helpers
bool getChildTextWithDefault(tinyxml2::XMLElement* element, std::stringstream& ss, 
				std::string name, std::string _default)
{
	auto child = element->FirstChildElement(name.data());
	if(child)
	{
		ss << child->GetText() << std::endl;
		return true;
	}

	else
	{
		ss << _default << std::endl;
		return false;
	}
}
