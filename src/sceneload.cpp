#include "utils.h"

// Helper Declerations
bool getChildTextWithDefault(tinyxml2::XMLElement* element, std::stringstream& ss, 
				std::string name, std::string _default);
int getCameraType(tinyxml2::XMLElement* element);
int getMeshType(tinyxml2::XMLElement* element, std::string& ply_path);
int getMeshShadingMode(tinyxml2::XMLElement* element);
void pushCameraLookAt(tinyxml2::XMLElement* element, std::stringstream& ss,
			std::vector<Camera>& cameras);
void pushCameraSimple(tinyxml2::XMLElement* element, std::stringstream& ss,
			std::vector<Camera>& cameras);
void pushFacesOfPlyMesh(std::vector<Shape*>& shapes, Mesh* mesh, std::vector<Vertex>& vertex_data, 
			int shadingMode, const std::string& fname, 
			const std::string& plyname);
void pushFacesOfMesh(std::vector<Shape*>& shapes, Mesh* mesh, std::vector<Vertex>& vertex_data,
			int shadingMode, std::stringstream& ss);

// Parse XML
void Scene::loadFromXML(const std::string& fname)
{
	tinyxml2::XMLDocument doc;
	std::stringstream ss;
	tinyxml2::XMLElement *child;

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
		int cam_type = getCameraType(element);

		if(cam_type == CAM_SIMPLE)
		{
			pushCameraSimple(element, ss, cameras);
		}

		else if(cam_type == CAM_LOOKAT)
		{
			pushCameraLookAt(element, ss, cameras);
		}

		element = element->NextSiblingElement("Camera");
	}

	// Lights
	element = scene_element->FirstChildElement("Lights");
	child = element->FirstChildElement("AmbientLight");
	ss << child->GetText() << std::endl;
	ss >> ambient_light.intensity[0] >> ambient_light.intensity[1] >> ambient_light.intensity[2];

	// PointLights
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

	// AreaLights
	element = scene_element->FirstChildElement("Lights");
	element = element->FirstChildElement("AreaLight");
	while(element)
	{
		AreaLight *area_light = new AreaLight();
		child = element->FirstChildElement("Position");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("Normal");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("Size");
		ss << child->GetText() << std::endl;
		child = element->FirstChildElement("Radiance");
		ss << child->GetText() << std::endl;

		ss >> area_light->position[0] >> area_light->position[1]
			>> area_light->position[2];
		ss >> area_light->normal[0] >> area_light->normal[1]
			>> area_light->normal[2];
		ss >> area_light->size;
		ss >> area_light->intensity[0] >> area_light->intensity[1]
			>> area_light->intensity[2];

		area_light->normal.makeUnitVector();
		lights.push_back(area_light);
		element = element->NextSiblingElement("AreaLight");
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
		getChildTextWithDefault(element, ss, "Roughness", "0");

		ss >> material.ambient[0] >> material.ambient[1] >> material.ambient[2];
		ss >> material.diffuse[0] >> material.diffuse[1] >> material.diffuse[2];
		ss >> material.specular[0] >> material.specular[1] >> material.specular[2];
		ss >> material.mirror[0] >> material.mirror[1] >> material.mirror[2];
		ss >> material.phong_exponent;
		ss >> material.transparency[0] >> material.transparency[1] 
			>> material.transparency[2];
		ss >> material.refraction_index;
		ss >> material.roughness;

		materials.push_back(material);
		element = element->NextSiblingElement("Material");
	}
	ss.clear();

	// VertexData
	element = scene_element->FirstChildElement("VertexData");
	if(element)
	{
		ss << element->GetText() << std::endl;
		Vertex vertex;
		while(!(ss >> vertex.position[0]).eof())
		{
			ss >> vertex.position[1] >> vertex.position[2];
			vertex_data.push_back(vertex);
		}
		ss.clear();
	}

	// Objects
	element = scene_element->FirstChildElement("Objects");
	element = element->FirstChildElement("Mesh");
	while(element)
	{
		std::string plyname;
		int mesh_type = getMeshType(element, plyname);
		std::vector<Shape*> meshTriangles;

		if(mesh_type == MESH_SIMPLE)
		{
			int itemp;
			Material meshMaterial;
			child = element->FirstChildElement("Material");
			ss << child->GetText() << std::endl;
			ss >> itemp;
			meshMaterial = materials[itemp - 1];
			Mesh *mesh = new Mesh(&vertex_data, meshMaterial);
			meshes.push_back(mesh);

			child = element->FirstChildElement("Faces");
			ss << child->GetText() << std::endl;	
			int shadingMode = getMeshShadingMode(element);
			pushFacesOfMesh(meshTriangles, mesh, vertex_data, shadingMode, ss);
		}

		else if(mesh_type == MESH_PLY)
		{
			int itemp;
			Material meshMaterial;
			child = element->FirstChildElement("Material");
			ss << child->GetText() << std::endl;
			ss >> itemp;
			meshMaterial = materials[itemp - 1];
			Mesh *mesh = new Mesh(&vertex_data, meshMaterial);
			meshes.push_back(mesh);
			int shadingMode = getMeshShadingMode(element);

			pushFacesOfPlyMesh(meshTriangles, mesh, vertex_data, shadingMode, fname, plyname);
		}

		ss.clear();
		BVH *meshBVH = new BVH(meshTriangles.data(), (int)meshTriangles.size(), 
					0, 0.0f, 0.0f);
		shapes.push_back(meshBVH);
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
		Vec3 a(vertex_data[vitemp.e[0] - 1].position);
		Vec3 b(vertex_data[vitemp.e[1] - 1].position);
		Vec3 c(vertex_data[vitemp.e[2] - 1].position);
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
		center = vertex_data[itemp - 1].position;

		child = element->FirstChildElement("Radius");
		ss << child->GetText() << std::endl;
		ss >> radius;

		Shape *sphere_ptr = new Sphere(center, radius, sphereMaterial);
		shapes.push_back(sphere_ptr);

		element = element->NextSiblingElement("Sphere");
	}

	// Pre-compute vertex normals if Smooth Shading
	for(Vertex& v : vertex_data)
	{
		if(v.hasSmoothShading())
		{
			v.averageNormal();
			v.makeUnitNormal();
		}
	}

	// BVH
	bvh = new BVH(shapes.data(), (int)shapes.size(), 0, 0.0f, 0.0f);
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

int getCameraType(tinyxml2::XMLElement* element)
{
	const char *type = element->Attribute("type");
	if(type)
	{
		std::string stype(type);
		if(stype == "lookAt")
		{
			return CAM_LOOKAT;
		}
	}

	return CAM_SIMPLE;
}

void pushCameraLookAt(tinyxml2::XMLElement* element, std::stringstream& ss,
			std::vector<Camera>& cameras)
{
	tinyxml2::XMLElement *child = element->FirstChildElement("Position");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("GazePoint");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("Up");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("FovY");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("NearDistance");
	ss << child->GetText() << std::endl;
	getChildTextWithDefault(element, ss, "FocusDistance", "0");
	getChildTextWithDefault(element, ss, "ApertureSize", "0");
	child = element->FirstChildElement("ImageResolution");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("ImageName");
	ss << child->GetText() << std::endl;
	getChildTextWithDefault(element, ss, "NumSamples", "1");

	// LookAt vars.
	Vec3 gazepoint;
	float fovy;

	Vec3 pos, up;
	float near_distance;
	float focus_distance;
	float aperture_size;
	int w, h;
	std::string img_name;
	int num_samples;
	ss >> pos[0] >> pos[1] >> pos[2];
	ss >> gazepoint[0] >> gazepoint[1] >> gazepoint[2];
	ss >> up[0] >> up[1] >> up[2];
	ss >> fovy;
	ss >> near_distance;
	ss >> focus_distance;
	ss >> aperture_size;
	ss >> w >> h;
	ss >> img_name;
	ss >> num_samples; 

	// LookAt to Simple calculations
	float _t = tan((fovy * M_PI) / (2 * 180.0f)) * near_distance;
	float _r = (float(w) / float(h)) * _t;
	float _l = -_r;
	float _b = -_t;
	std::array<float, 4> near_plane = {_l, _r, _b, _t};
	Vec3 gaze(gazepoint - pos);

	cameras.push_back(Camera(pos, gaze, up, near_plane, near_distance,
		 focus_distance, aperture_size, w, h, img_name, num_samples));
}

void pushCameraSimple(tinyxml2::XMLElement* element, std::stringstream& ss,
			std::vector<Camera>& cameras)
{
	tinyxml2::XMLElement *child = element->FirstChildElement("Position");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("Gaze");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("Up");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("NearPlane");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("NearDistance");
	ss << child->GetText() << std::endl;
	getChildTextWithDefault(element, ss, "FocusDistance", "0");
	getChildTextWithDefault(element, ss, "ApertureSize", "0");
	child = element->FirstChildElement("ImageResolution");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("ImageName");
	ss << child->GetText() << std::endl;
	getChildTextWithDefault(element, ss, "NumSamples", "1");

	std::array<float, 4> near_plane;
	Vec3 pos, gaze, up;
	float near_distance;
	float focus_distance;
	float aperture_size;
	int w, h;
	std::string img_name;
	int num_samples;
	ss >> pos[0] >> pos[1] >> pos[2];
	ss >> gaze[0] >> gaze[1] >> gaze[2];
	ss >> up[0] >> up[1] >> up[2];
	ss >> near_plane[0] >> near_plane[1] 
		>> near_plane[2] >> near_plane[3];
	ss >> near_distance;
	ss >> focus_distance;
	ss >> aperture_size;
	ss >> w >> h;
	ss >> img_name;
	ss >> num_samples; 

	cameras.push_back(Camera(pos, gaze, up, near_plane, near_distance,
		 focus_distance, aperture_size, w, h, img_name, num_samples));
}

int getMeshType(tinyxml2::XMLElement* element, std::string& ply_path)
{
	tinyxml2::XMLElement *child = element->FirstChildElement("Faces");
	const char *plyFile = child->Attribute("plyFile");
	if(plyFile)
	{
		ply_path = plyFile;
		return MESH_PLY;
	}

	return MESH_SIMPLE;
}

int getMeshShadingMode(tinyxml2::XMLElement* element)
{
	const char *shadingMode = element->Attribute("shadingMode");
	if(shadingMode)
	{
		std::string sshadingMode(shadingMode);
		if(sshadingMode == "smooth")
		{
			return MESH_SHADING_SMOOTH;
		}
	}

	return MESH_SHADING_FLAT;
}

void pushFacesOfPlyMesh(std::vector<Shape*>& shapes, Mesh* mesh, std::vector<Vertex>& vertex_data, 
			int shadingMode, const std::string& fname, 
			const std::string& plyname)
{
	std::size_t pos = fname.find_last_of("/");
	std::string plydir(fname.substr(0, pos+1));
	std::string ply_path = plydir + plyname;

	happly::PLYData plyIn(ply_path);

	std::vector<std::array<double, 3>> vPos = plyIn.getVertexPositions();
	std::vector<std::vector<size_t>> fInd = plyIn.getFaceIndices<size_t>();

	int offset = vertex_data.size();
	// push vertex_data
	for(auto& v : vPos)
	{
		vertex_data.push_back(Vertex(Vec3(v[0], v[1], v[2])));
	}
	
	for(auto& f : fInd)
	{
		if(f.size() == 3)	// Triangle
		{
			std::array<int, 3> facedata = {offset+(int)f[0], 
							offset+(int)f[1], 
							offset+(int)f[2]};
			Vec3 a(vertex_data[facedata[0]].position);
			Vec3 b(vertex_data[facedata[1]].position);
			Vec3 c(vertex_data[facedata[2]].position);
			Vec3 n(unitVector(cross(b - a, c - a)));
			Shape *mt = new MeshTriangle(facedata, mesh, n, shadingMode);
			shapes.push_back(mt);

			if(shadingMode == MESH_SHADING_SMOOTH)
			{
				// Add the found normal to each vertex
				for(int i = 0; i < 3; i++)
				{
					vertex_data[facedata[i]].addToNormal(n);
				}
			}
		}

		else if(f.size() == 4)	// Quad
		{
			std::array<int, 3> facedata0 = {offset+(int)f[0], 
							offset+(int)f[1], 
							offset+(int)f[2]};
			std::array<int, 3> facedata1 = {offset+(int)f[0], 
							offset+(int)f[2], 
							offset+(int)f[3]};
			Vec3 a(vertex_data[facedata0[0]].position);
			Vec3 b(vertex_data[facedata0[1]].position);
			Vec3 c(vertex_data[facedata0[2]].position);
			Vec3 d(vertex_data[facedata1[2]].position);

			Vec3 n0(unitVector(cross(b - a, c - a)));
			Vec3 n1(unitVector(cross(c - a, d - a)));
			Shape *mt0 = new MeshTriangle(facedata0, mesh, n0, shadingMode);
			Shape *mt1 = new MeshTriangle(facedata1, mesh, n1, shadingMode);
			shapes.push_back(mt0);
			shapes.push_back(mt1);

			if(shadingMode == MESH_SHADING_SMOOTH)
			{
				// Add the found normal to each vertex
				for(int i = 0; i < 3; i++)
				{
					vertex_data[facedata0[i]].addToNormal(n0);
					vertex_data[facedata1[i]].addToNormal(n1);
				}
			}
		}
	}
}

void pushFacesOfMesh(std::vector<Shape*>& shapes, Mesh* mesh, std::vector<Vertex>& vertex_data,
			int shadingMode, std::stringstream& ss)
{
	std::array<int, 3> ptemp;

	while(!(ss >> ptemp[0]).eof())
	{
		ss >> ptemp[1] >> ptemp[2];
		
		// 1 based to 0 based
		ptemp[0] = ptemp[0] - 1;
		ptemp[1] = ptemp[1] - 1;
		ptemp[2] = ptemp[2] - 1;

		// Precompute normal
		Vec3 a(vertex_data[ptemp[0]].position);
		Vec3 b(vertex_data[ptemp[1]].position);
		Vec3 c(vertex_data[ptemp[2]].position);
		Vec3 n(unitVector(cross(b - a, c - a)));
		Shape *mt = new MeshTriangle(ptemp, mesh, n, shadingMode);
		shapes.push_back(mt);

		if(shadingMode == MESH_SHADING_SMOOTH)
		{
			// Add the found normal to each vertex
			for(int i = 0; i < 3; i++)
			{
				vertex_data[ptemp[i]].addToNormal(n);
			}
		}
	}
}

