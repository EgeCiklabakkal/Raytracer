#include "sceneloadutils.h"

// Parse XML
void Scene::loadFromXML(const std::string& fname)
{
	tinyxml2::XMLDocument doc;
	std::stringstream ss;
	tinyxml2::XMLElement *element;
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
	getChildTextWithDefaultFromNode(scene_element, ss, "BackgroundColor", "0 0 0");
	Vec3 background_color_vec3;
	readVec3FromSS(background_color_vec3, ss);
	background_color = rgb(background_color_vec3);

	// ShadowRayEpsilon
	getChildTextWithDefaultFromNode(scene_element, ss, "ShadowRayEpsilon", "0.001");
	ss >> shadow_ray_epsilon;

	// IntersectionTestEpsilon
	getChildTextWithDefaultFromNode(scene_element, ss, "IntersectionTestEpsilon", "1e-6");
	ss >> intersection_test_epsilon;

	// MaxRecursionDepth
	getChildTextWithDefaultFromNode(scene_element, ss, "MaxRecursionDepth", "0");
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
	parseAmbientLight(ambient_light, element, ss);

	// PointLights
	element = element->FirstChildElement("PointLight");
	while(element)
	{
		PointLight *point_light = new PointLight();
		parsePointLight(point_light, element, ss);

		lights.push_back(point_light);
		element = element->NextSiblingElement("PointLight");
	}

	// AreaLights
	element = scene_element->FirstChildElement("Lights");
	element = element->FirstChildElement("AreaLight");
	while(element)
	{
		AreaLight *area_light = new AreaLight();
		parseAreaLight(area_light, element, ss);

		lights.push_back(area_light);
		element = element->NextSiblingElement("AreaLight");
	}

	// Transformations
	std::vector<glm::mat4> translations;
	std::vector<glm::mat4> scalings;
	std::vector<glm::mat4> rotations;
	element = scene_element->FirstChildElement("Transformations");
	getTransformations(element, ss, translations, scalings, rotations);

	// Textures
	element = scene_element->FirstChildElement("Textures");
	getTextures(element, ss, fname, textureImages, textures);

	// Materials
	element = scene_element->FirstChildElement("Materials");
	element = element->FirstChildElement("Material");
	while(element)
	{
		Material material;
		parseMaterial(material, element, ss);		
	
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

	// TexCoordData
	element = scene_element->FirstChildElement("TexCoordData");
	if(element)
	{
		ss << element->GetText() << std::endl;
		Vec2 texCoord;
		while(!(ss >> texCoord[0]).eof())
		{
			ss >> texCoord[1];
			texCoord_data.push_back(texCoord);
		}
		ss.clear();
	}

	// Objects
	std::vector<ObjectInstance*> baseMeshInstances;
	element = scene_element->FirstChildElement("Objects");
	element = element->FirstChildElement("Mesh");
	while(element)
	{
		std::string plyname;
		int mesh_type = getMeshType(element, plyname);
		std::vector<Shape*> meshTriangles;
		Material meshMaterial;
		Texture *instanceTexture = nullptr;
		bool _transformed;
		tinyxml2::XMLElement *trans_element;
		glm::mat4 transMatInstance(1.0f);
		int itemp;

		if(mesh_type == MESH_SIMPLE)
		{
			child = element->FirstChildElement("Material");
			ss << child->GetText() << std::endl;
			ss >> itemp;
			meshMaterial = materials[itemp - 1];
			Mesh *mesh = new Mesh(&vertex_data, &texCoord_data, meshMaterial);
			meshes.push_back(mesh);

			child = element->FirstChildElement("Faces");
			ss << child->GetText() << std::endl;	
			int shadingMode  = getMeshShadingMode(element);
			int vertexOffset = getIntAttributeWithDefault(child, "vertexOffset", 0);
			pushFacesOfMesh(meshTriangles, mesh, vertex_data, 
						shadingMode, vertexOffset, ss);
		}

		else if(mesh_type == MESH_PLY)
		{
			child = element->FirstChildElement("Material");
			ss << child->GetText() << std::endl;
			ss >> itemp;
			meshMaterial = materials[itemp - 1];
			Mesh *mesh = new Mesh(&vertex_data, &texCoord_data, meshMaterial);
			meshes.push_back(mesh);
			int shadingMode = getMeshShadingMode(element);

			pushFacesOfPlyMesh(meshTriangles, mesh, vertex_data, 
						shadingMode, fname, plyname);
		}

		ss.clear();
		BVH *meshBVH = new BVH(meshTriangles.data(), (int)meshTriangles.size(), 
					0, 0.0f, 0.0f);
		primMeshBVHs.push_back(meshBVH);
		
		// Apply Mesh/Instance Transformations
		trans_element = element->FirstChildElement("Transformations");
		_transformed = applyTransforms(trans_element, ss, transMatInstance,
					translations, scalings, rotations);

		if(getIntChildWithDefault(element, ss, "Texture", 0, itemp))	// has texture
		{
			instanceTexture = textures[itemp - 1];
		}

		// Create instance
		ObjectInstance *instance_ptr = new ObjectInstance(transMatInstance, meshBVH, 
									meshMaterial,
									_transformed,
									instanceTexture);
		// Set Motion Blur
		setMotionBlurOfShape(instance_ptr, element, ss);

		shapes.push_back(instance_ptr);
		baseMeshInstances.push_back(instance_ptr);

		element = element->NextSiblingElement("Mesh");
	}
	ss.clear();

	element = scene_element->FirstChildElement("Objects");
	element = element->FirstChildElement("MeshInstance");
	while(element)
	{
		bool _transformed;
		tinyxml2::XMLElement *trans_element;
		glm::mat4 transMatInstance(1.0f);
		Shape *primBVH;

		int baseMeshId = getIntAttributeWithDefault(element, "baseMeshId", 1);
		bool resetTransform = getBoolAttributeWithDefault(element, "resetTransform", false); 
		
		Material instanceMaterial;
		Texture *instanceTexture = nullptr;
		int itemp;
		child = element->FirstChildElement("Material");
		ss << child->GetText() << std::endl;
		ss >> itemp;
		instanceMaterial = materials[itemp - 1];

		ObjectInstance *baseMeshInstance = baseMeshInstances[baseMeshId - 1];
		primBVH = baseMeshInstance->prim;
		trans_element = element->FirstChildElement("Transformations");
		if(resetTransform)
		{
			_transformed = applyTransforms(trans_element, ss, transMatInstance,
							translations, scalings, rotations);
		}

		else
		{
			transMatInstance = baseMeshInstance->M;
			_transformed = applyTransforms(trans_element, ss, transMatInstance,
							translations, scalings, rotations);
			_transformed = _transformed || baseMeshInstance->transformed;
		}

		if(getIntChildWithDefault(element, ss, "Texture", 0, itemp))	// has texture
		{
			instanceTexture = textures[itemp - 1];
		}

		// Create instance
		ObjectInstance *instance_ptr = new ObjectInstance(transMatInstance, primBVH, 
								instanceMaterial,
								_transformed,
								instanceTexture);
		// Set Motion Blur
		setMotionBlurOfShape(instance_ptr, element, ss);

		shapes.push_back(instance_ptr);

		element = element->NextSiblingElement("MeshInstance");
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

		// Set Transformation
		glm::mat4 transMatTriangle(1.0f);
		setTransformOfShape(triangle_ptr, element, ss, transMatTriangle,
					translations, scalings, rotations);

		// Set Motion Blur
		setMotionBlurOfShape(triangle_ptr, element, ss);

		shapes.push_back(triangle_ptr);
		element = element->NextSiblingElement("Triangle");
	}

	element = scene_element->FirstChildElement("Objects");
	element = element->FirstChildElement("Sphere");
	while(element)
	{
		Material sphereMaterial;
		Texture *sphereTexture = nullptr;
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

		if(getIntChildWithDefault(element, ss, "Texture", 0, itemp))	// has texture
		{
			sphereTexture = textures[itemp - 1];
		}

		Shape *sphere_ptr = new Sphere(center, radius, sphereMaterial, sphereTexture);

		makeUnitSphere((Sphere*) sphere_ptr);

		// Set Transformation (resetTransform is false, consider unitSphere transforms)
		glm::mat4 transMatSphere(1.0f);
		setTransformOfShape(sphere_ptr, element, ss, transMatSphere,
					translations, scalings, rotations, false);

		// Set Motion Blur
		setMotionBlurOfShape(sphere_ptr, element, ss);

		// Set alignment
		//setAlignmentOfSphere((Sphere*) sphere_ptr);

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
