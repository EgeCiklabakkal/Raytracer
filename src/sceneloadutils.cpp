#include "sceneloadutils.h"

// Helpers
bool getBackgroundTexture(tinyxml2::XMLNode* node, std::string fname, Image& image)
{
	auto child = node->FirstChildElement("BackgroundTexture");
	if(child)
	{
		std::string textureName(child->GetText());
		std::size_t pos = fname.find_last_of("/");
	        std::string fpath(fname.substr(0, pos+1));

		image.imread(fpath + textureName);
		return true;
	}

	return false;
}

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

bool getChildTextWithDefaultFromNode(tinyxml2::XMLNode* node, std::stringstream& ss, 
				std::string name, std::string _default)
{
	auto child = node->FirstChildElement(name.data());
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

bool getBoolChildWithDefaultFromNode(tinyxml2::XMLNode* node, std::string name,
					bool _default, bool& val)
{
	tinyxml2::XMLElement *child = node->FirstChildElement(name.data());
	if(child)
	{
		std::string sbool(child->GetText());
		val = (sbool == "true") ? true : false;
		return true;
	}

	else
	{
		val = _default;
		return false;
	}
}

bool getIntChildWithDefault(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::string name, int _default, int& val)
{
	tinyxml2::XMLElement *child = element->FirstChildElement(name.data());

	if(child)
	{
		ss << child->GetText() << std::endl;
		ss >> val;
		return true;
	}

	else
	{
		val = _default;
		return false;
	}
}

bool getFloatChildWithDefault(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::string name, float _default, float& val)
{
	tinyxml2::XMLElement *child = element->FirstChildElement(name.data());
	if(child)
	{
		ss << child->GetText() << std::endl;
		ss >> val;
		return true;
	}

	else
	{
		val = _default;
		return false;
	}
}

bool getBoolChildWithDefault(tinyxml2::XMLElement* element, std::string name,
				bool _default, bool& val)
{
	tinyxml2::XMLElement *child = element->FirstChildElement(name.data());
	if(child)
	{
		std::string sbool(child->GetText());
		val = (sbool == "true") ? true : false;
		return true;
	}

	else
	{
		val = _default;
		return false;
	}
}

bool getVec3ChildWithDefault(tinyxml2::XMLElement* element, std::stringstream& ss,
                                std::string name, const Vec3& _default, Vec3& val)
{
	float x, y, z;
	tinyxml2::XMLElement *child = element->FirstChildElement(name.data());
	if(child)
	{
		ss << child->GetText() << std::endl;
		ss >> x >> y >> z;
		val = Vec3(x, y, z);
		return true;
	}

	else
	{
		val = _default;
		return false;
	}
}

bool getrgbChildWithDefault(tinyxml2::XMLElement* element, std::stringstream& ss,
                                std::string name, const rgb& _default, rgb& val)
{
	float x, y, z;
	tinyxml2::XMLElement *child = element->FirstChildElement(name.data());
	if(child)
	{
		ss << child->GetText() << std::endl;
		ss >> x >> y >> z;
		val = rgb(x, y, z);
		return true;
	}

	else
	{
		val = _default;
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

int getIntAttributeWithDefault(tinyxml2::XMLElement* element, std::string name, int _default)
{
	const char *sattr = element->Attribute(name.data());
	if(sattr)
	{
		return atoi(sattr);
	}

	return _default;
}

bool getBoolAttributeWithDefault(tinyxml2::XMLElement* element, std::string name, bool _default)
{
	const char *attr = element->Attribute(name.data());
	if(attr)
	{
		std::string sattr(attr);
		if(sattr == "true")
		{
			return true;
		}

		return false;
	}

	return _default;
}

float getFloatAttributeWithDefault(tinyxml2::XMLElement* element, std::string name, float _default)
{
	const char *sattr = element->Attribute(name.data());
	if(sattr)
	{
		return atof(sattr);
	}

	return _default;
}

int getCameras(tinyxml2::XMLElement* element, std::stringstream& ss, std::vector<Camera>& cameras)
{
	int cameraCount = 0;	

	element = element->FirstChildElement("Camera");
	while(element)
	{
		cameraCount++;
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

	return cameraCount;
}

int getLights(tinyxml2::XMLNode* node, tinyxml2::XMLElement* element,
		std::stringstream& ss, std::vector<Light*>& lights)
{
	int lightcount = 0;

	// PointLights
	element = node->FirstChildElement("Lights");
	element = element->FirstChildElement("PointLight");
	while(element)
	{
		lightcount++;
		PointLight *point_light = new PointLight();
		parsePointLight(point_light, element, ss);

		lights.push_back(point_light);
		element = element->NextSiblingElement("PointLight");
	}

	// AreaLights
	element = node->FirstChildElement("Lights");
	element = element->FirstChildElement("AreaLight");
	while(element)
	{
		lightcount++;
		AreaLight *area_light = new AreaLight();
		parseAreaLight(area_light, element, ss);

		lights.push_back(area_light);
		element = element->NextSiblingElement("AreaLight");
        }

	// DirectionalLight
	element = node->FirstChildElement("Lights");
	element = element->FirstChildElement("DirectionalLight");
	while(element)
	{
		lightcount++;
		DirectionalLight *directional_light = new DirectionalLight();
		parseDirectionalLight(directional_light, element, ss);

		lights.push_back(directional_light);
		element = element->NextSiblingElement("DirectionalLight");
	}

	// SpotLights
	element = node->FirstChildElement("Lights");
	element = element->FirstChildElement("SpotLight");
	while(element)
	{
		lightcount++;
		SpotLight *spot_light = new SpotLight();
		parseSpotLight(spot_light, element, ss);

		lights.push_back(spot_light);
		element = element->NextSiblingElement("SpotLight");
	}

	return lightcount;
}

int getTransformations(tinyxml2::XMLElement* element, std::stringstream& ss, 
			std::vector<glm::mat4>& translations, 
			std::vector<glm::mat4>& scalings, 
			std::vector<glm::mat4>& rotations,
			std::vector<glm::mat4>& composites)
{
	if(!element)
	{
		return 0;
	}

	glm::mat4 transformation;	// Temp transformation variable
	int count_trans = 0;

	// Get Translations
	tinyxml2::XMLElement *element_trans = element->FirstChildElement("Translation");
	while(element_trans)
	{
		float t_x, t_y, t_z;
		ss << element_trans->GetText() << std::endl;
		ss >> t_x >> t_y >> t_z;
		transformation = glm::translate(glm::mat4(1.0f), glm::vec3(t_x, t_y, t_z));
		translations.push_back(transformation);
		count_trans++;
		element_trans = element_trans->NextSiblingElement("Translation");
	}

	// Get Scalings
	element_trans = element->FirstChildElement("Scaling");
	while(element_trans)
	{
		float s_x, s_y, s_z;
		ss << element_trans->GetText() << std::endl;
		ss >> s_x >> s_y >> s_z;
		transformation = glm::scale(glm::mat4(1.0f), glm::vec3(s_x, s_y, s_z));
		scalings.push_back(transformation);
		count_trans++;
		element_trans = element_trans->NextSiblingElement("Scaling");
	}

	// Get Rotations
	element_trans = element->FirstChildElement("Rotation");
	while(element_trans)
	{
		float r_deg, r_x, r_y, r_z;
		ss << element_trans->GetText() << std::endl;
		ss >> r_deg >> r_x >> r_y >> r_z;
		transformation = glm::rotate(glm::mat4(1.0f), 
						glm::radians(r_deg), 
						glm::vec3(r_x, r_y, r_z));
		rotations.push_back(transformation);
		count_trans++;
		element_trans = element_trans->NextSiblingElement("Rotation");
	}

	element_trans = element->FirstChildElement("Composite");
	while(element_trans)
	{
		float arrMat[16];
		ss << element_trans->GetText() << std::endl;
		for(int i = 0; i < 16; i++)
		{
			ss >> arrMat[i];
		}

		// row major to column major
		transformation = glm::transpose(glm::make_mat4(arrMat));
		composites.push_back(transformation);
		count_trans++;
		element_trans = element_trans->NextSiblingElement("Composite");
	}

	ss.clear();
	return count_trans;
}

int getTextures(tinyxml2::XMLElement* element, std::stringstream& ss, const std::string& fname,
			std::vector<Image*>& textureImages, std::vector<Texture*>& textures)
{
	if(!element)
	{
		return 0;
	}

	std::size_t pos = fname.find_last_of("/");
	std::string fpath(fname.substr(0, pos+1));

	tinyxml2::XMLElement *child;
	std::set<std::string> uniqueImageNames;
	std::map<std::string, int> mapImageNameToIndex;
	int idx = 0;

	tinyxml2::XMLElement *element_texture = element->FirstChildElement("Texture");
	while(element_texture)
	{
		bool bumpmap = getBoolAttributeWithDefault(element_texture, "bumpmap", false);
		float bumpmapMultiplier = getFloatAttributeWithDefault(element_texture,
									"bumpmapMultiplier", 1.0f);
		bool degamma = getBoolAttributeWithDefault(element_texture, "degamma", false);
		child = element_texture->FirstChildElement("ImageName");
		std::string textureName(child->GetText());
		DecalMode decal_mode;
		float normalizer;

		if(textureName == "perlin")	// Perlin Texture
		{
			float scale;
			PerlinPattern perlin_pattern;

			getFloatChildWithDefault(element_texture, ss, "ScalingFactor",
							1.0f, scale);
			getFloatChildWithDefault(element_texture, ss, "Normalizer",
							1.0f, normalizer);
			decal_mode = getDecalMode(element_texture);
			perlin_pattern = getPerlinPattern(element_texture);

			PerlinTexture *perlinTexture = new PerlinTexture(scale, normalizer,
										decal_mode,
										perlin_pattern,
										bumpmap,
										bumpmapMultiplier,
										degamma);
			textures.push_back(perlinTexture);
		}

		else if(textureName == "checkerboard")	// Checkerboard Texture
		{
			float offset, scale;
			rgb black, white;

			getrgbChildWithDefault(element_texture, ss, "BlackColor", rgb(), black);
			getrgbChildWithDefault(element_texture, ss, "WhiteColor", rgb(1.0f), white);
			getFloatChildWithDefault(element_texture, ss, "ScalingFactor", 
							1.0f, scale);
			getFloatChildWithDefault(element_texture, ss, "Offset", 
							0.01f, offset);
			getFloatChildWithDefault(element_texture, ss, "Normalizer",
							1.0f, normalizer);
			decal_mode = getDecalMode(element_texture);

			CBTexture *cbTexture = new CBTexture(offset, scale, normalizer,
								black, white, decal_mode,
								bumpmap, bumpmapMultiplier,
								degamma);

			textures.push_back(cbTexture);
		}

		else	// Image Texture
		{
			Image* textureImage;
			InterpolationMode interpolation_mode;
			TextureMode texture_mode;
			bool flipVertical;

			// Image instancing
			if(uniqueImageNames.find(textureName) != uniqueImageNames.end())
			{
				// This image has previously been read
				textureImage = textureImages[mapImageNameToIndex[textureName]];
			}

			else
			{
				textureImage = new Image(fpath + textureName);
				textureImages.push_back(textureImage);	// to idx
				mapImageNameToIndex[textureName] = idx;
				idx++;
			}

			interpolation_mode = getInterpolationMode(element_texture);
			decal_mode = getDecalMode(element_texture);
			texture_mode = getTextureModeWithDefault
					(element_texture, TextureMode::CLAMP);
			getFloatChildWithDefault(element_texture, ss, "Normalizer",
							255.0f, normalizer);
			getBoolChildWithDefault(element_texture, "FlipVertical",
							true, flipVertical);

			ImageTexture *imageTexture = new ImageTexture(textureImage,
									normalizer,
									interpolation_mode,
									decal_mode,
									texture_mode,
									bumpmap,
									bumpmapMultiplier,
									flipVertical,
									degamma);
			textures.push_back(imageTexture);
		}

		element_texture = element_texture->NextSiblingElement("Texture");
	}

	ss.clear();
	return textures.size();
}

int getBRDFs(tinyxml2::XMLElement* element, std::stringstream& ss, std::vector<BRDF*>& brdfs)
{
	// brdfs[0] is always simpleBRDF, serves as a default for non-specified BRDFs for mats.
	BRDF *simple = new SimpleBRDF();
	brdfs.push_back(simple);
	int count_brdf = 1;

	if(!element)
	{
		return count_brdf;
	}

	tinyxml2::XMLElement *child = element->FirstChildElement();
	while(child)
	{
		count_brdf++;
		getSingleBRDF(child, ss, brdfs);

		child = child->NextSiblingElement();
	}

	return count_brdf;
}

bool getSingleBRDF(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::vector<BRDF*>& brdfs)
{
	std::string tag(element->Name());

	if(tag == "OriginalPhong")
	{
		getOriginalPhongBRDF(element, ss, brdfs);
	}

	else if(tag == "ModifiedPhong")
	{
		getModifiedPhongBRDF(element, ss, brdfs);
	}

	return true;
}

bool getOriginalPhongBRDF(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::vector<BRDF*>& brdfs)
{
	float exponent;
	tinyxml2::XMLElement *child = element->FirstChildElement("Exponent");
	ss << child->GetText() << std::endl;
	ss >> exponent;

	BRDF *brdf = new PhongBRDF(exponent);
	brdfs.push_back(brdf);
	return true;
}

bool getModifiedPhongBRDF(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::vector<BRDF*>& brdfs)
{
	float exponent;
	tinyxml2::XMLElement *child = element->FirstChildElement("Exponent");
	ss << child->GetText() << std::endl;
	ss >> exponent;

	BRDF *brdf = new ModifiedPhongBRDF(exponent);
	brdfs.push_back(brdf);
	return true;
}

int getMaterials(tinyxml2::XMLElement* element, std::stringstream& ss,
                        std::vector<Material>& materials, const std::vector<BRDF*>& brdfs)
{
	if(!element)
	{
		return 0; // Then, you can't compute shading, so this shouldn't happen
	}

	int count_mat = 0;

	tinyxml2::XMLElement *child = element->FirstChildElement("Material");
	while(child)
	{
		count_mat++;
		Material material;
		parseMaterial(material, child, ss, brdfs);

		materials.push_back(material);
		child = child->NextSiblingElement("Material");
	}
	ss.clear();

	return count_mat;
}

InterpolationMode getInterpolationMode(tinyxml2::XMLElement* element)
{
	tinyxml2::XMLElement *child = element->FirstChildElement("Interpolation");

	std::string stext(child->GetText());

	if(stext == "nearest")
	{
		return InterpolationMode::NEAREST;
	}

	else	// "bilinear"
	{
		return InterpolationMode::BILINEAR;
	}
}

DecalMode getDecalMode(tinyxml2::XMLElement* element)
{
	tinyxml2::XMLElement *child = element->FirstChildElement("DecalMode");

	std::string stext(child->GetText());

	if(stext == "replace_kd")
	{
		return DecalMode::REPLACEKD;
	}

	else if(stext == "blend_kd")
	{
		return DecalMode::BLENDKD;
	}

	else	// "replace_all"
	{
		return DecalMode::REPLACEALL;
	}
}

TextureMode getTextureModeWithDefault(tinyxml2::XMLElement* element, TextureMode _default)
{
	tinyxml2::XMLElement *child = element->FirstChildElement("Appearance");

	if(child)
	{
		std::string stext(child->GetText());

		if(stext == "repeat")
		{
			return TextureMode::REPEAT;
		}

		else	// "clamp"
		{
			return TextureMode::CLAMP;
		}
	}

	else
	{
		return _default;
	}
}

PerlinPattern getPerlinPattern(tinyxml2::XMLElement* element)
{
	tinyxml2::XMLElement *child = element->FirstChildElement("Appearance");

	std::string stext(child->GetText());

	if(stext == "patch")
	{
		return PerlinPattern::PATCHY;
	}

	else	// "vein"
	{
		return PerlinPattern::VEINY;
	}
}

bool applyTransforms(tinyxml2::XMLElement* element, std::stringstream& ss, glm::mat4& transMat,
			const std::vector<glm::mat4>& translations, 
			const std::vector<glm::mat4>& scalings, 
			const std::vector<glm::mat4>& rotations,
			const std::vector<glm::mat4>& composites)
{
	if(!element)	// No transformations
	{
		return false;
	}

	char ttype;
	int  tid;

	ss << element->GetText() << std::endl;
	while(ss >> ttype)
	{
		ss >> tid;
		tid -= 1;	// 1 based to 0 conversion
		switch(ttype)
		{
			case 't':
				transMat = translations[tid] * transMat;
				break;
			case 's':
				transMat = scalings[tid] * transMat;
				break;
			case 'r':
				transMat = rotations[tid] * transMat;
				break;
			case 'c':
				transMat = composites[tid] * transMat;
		}
	}

	ss.clear();
	return true;
}

bool getTonemap(tinyxml2::XMLElement* element, std::stringstream& ss, Tonemap& tonemap)
{
	if(!element)
	{
		tonemap = Tonemap();	// NOTONEMAP
		return false;
	}

	TonemapMode tonemap_mode;
	float a, burnout, saturation, gamma;

	tinyxml2::XMLElement *child = element->FirstChildElement("TMO");
	std::string smode(child->GetText());
	if(smode == "Photographic") { tonemap_mode = TonemapMode::PHOTOGRAPHIC; }
	else { tonemap_mode = TonemapMode::NOTONEMAP; }
	child = element->FirstChildElement("TMOOptions");
	ss << child->GetText() << std::endl;
	ss >> a >> burnout;
	getFloatChildWithDefault(element, ss, "Saturation", 1.0f, saturation);
	getFloatChildWithDefault(element, ss, "Gamma", 2.2f, gamma);

	tonemap = Tonemap(tonemap_mode, a, burnout, saturation, gamma);
	return true;
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

	// Tonemap Info
	Tonemap tonemap;
	child = element->FirstChildElement("Tonemap");
	getTonemap(child, ss, tonemap);

	cameras.push_back(Camera(pos, gaze, up, near_plane, near_distance,
		 focus_distance, aperture_size, w, h, img_name, num_samples, tonemap));
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

	// Tonemap Info
	Tonemap tonemap;
	child = element->FirstChildElement("Tonemap");
	getTonemap(child, ss, tonemap);

	cameras.push_back(Camera(pos, gaze, up, near_plane, near_distance,
		 focus_distance, aperture_size, w, h, img_name, num_samples, tonemap));
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
			std::vector<Vec2>& texCoord_data,
			int shadingMode, const std::string& fname, const std::string& plyname)
{
	std::size_t pos = fname.find_last_of("/");
	std::string plydir(fname.substr(0, pos+1));
	std::string ply_path = plydir + plyname;

	happly::PLYData plyIn(ply_path);

	std::vector<std::array<double, 3>> vPos = plyIn.getVertexPositions();
	std::vector<std::vector<size_t>> fInd = plyIn.getFaceIndices<size_t>();
	std::vector<std::array<double, 2>> vUV = plyIn.getVertexUV();

	int texoffset = texCoord_data.size();
	if(vUV.size())
	{	
		// push uv data
		for(auto& uv : vUV)
		{
			texCoord_data.push_back(Vec2(uv[0], uv[1]));
		}
	}

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
			std::array<int, 3> facedata = { offset+(int)f[0], 
							offset+(int)f[1], 
							offset+(int)f[2]};
			std::array<int, 3> texcdata;
			if(vUV.size())
			{
				texcdata[0] = texoffset+(int)f[0];
				texcdata[1] = texoffset+(int)f[1];
				texcdata[2] = texoffset+(int)f[2];
			}

			else
			{
				texcdata[0] = 0;
				texcdata[1] = 0;
				texcdata[2] = 0;
			}

			Vec3 a(vertex_data[facedata[0]].position);
			Vec3 b(vertex_data[facedata[1]].position);
			Vec3 c(vertex_data[facedata[2]].position);
			Vec3 n(unitVector(cross(b - a, c - a)));
			Shape *mt = new MeshTriangle(facedata, texcdata, mesh, n, shadingMode);
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
			std::array<int, 3> texcdata0;
			std::array<int, 3> texcdata1;
			if(vUV.size())
			{
				texcdata0[0] = texoffset+(int)f[0];
				texcdata0[1] = texoffset+(int)f[1];
				texcdata0[2] = texoffset+(int)f[2];
				texcdata1[0] = texoffset+(int)f[0];
				texcdata1[1] = texoffset+(int)f[2];
				texcdata1[2] = texoffset+(int)f[3];
			}

			else
			{
				texcdata0[0] = 0;
				texcdata0[1] = 0;
				texcdata0[2] = 0;
				texcdata1[0] = 0;
				texcdata1[1] = 0;
				texcdata1[2] = 0;
			}

			Vec3 a(vertex_data[facedata0[0]].position);
			Vec3 b(vertex_data[facedata0[1]].position);
			Vec3 c(vertex_data[facedata0[2]].position);
			Vec3 d(vertex_data[facedata1[2]].position);

			Vec3 n0(unitVector(cross(b - a, c - a)));
			Vec3 n1(unitVector(cross(c - a, d - a)));
			Shape *mt0 = new MeshTriangle(facedata0, texcdata0, mesh, n0, shadingMode);
			Shape *mt1 = new MeshTriangle(facedata1, texcdata1, mesh, n1, shadingMode);
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
			int shadingMode, int vertexOffset,
			int textureOffset, std::stringstream& ss)
{
	std::array<int, 3> ptemp;
	std::array<int, 3> textemp;

	while(!(ss >> ptemp[0]).eof())
	{
		ss >> ptemp[1] >> ptemp[2];

		textemp = ptemp;
		
		// 1 based to 0 based
		ptemp[0] = ptemp[0] + vertexOffset - 1;
		ptemp[1] = ptemp[1] + vertexOffset - 1;
		ptemp[2] = ptemp[2] + vertexOffset - 1;

		textemp[0] = textemp[0] + textureOffset - 1;
		textemp[1] = textemp[1] + textureOffset - 1;
		textemp[2] = textemp[2] + textureOffset - 1;

		// Precompute normal
		Vec3 a(vertex_data[ptemp[0]].position);
		Vec3 b(vertex_data[ptemp[1]].position);
		Vec3 c(vertex_data[ptemp[2]].position);
		Vec3 n(unitVector(cross(b - a, c - a)));
		Shape *mt = new MeshTriangle(ptemp, textemp, mesh, n, shadingMode);
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

bool makeUnitSphere(Sphere* sphere_ptr)
{
	Vec3 origCenter = sphere_ptr->center;
	float origRad   = sphere_ptr->radius;

	glm::mat4 toOrigCenter = glm::translate(glm::mat4(1.0f), glm::vec3(origCenter[0], 
										origCenter[1], 
										origCenter[2]));
	glm::mat4 toOrigRad = glm::scale(glm::mat4(1.0f), glm::vec3(origRad));

	sphere_ptr->setTransform(toOrigCenter * toOrigRad);
	sphere_ptr->center = Vec3(0.0f);
	sphere_ptr->radius = 1.0f;

	return true;
}

bool setAlignmentOfSphere(Sphere* sphere_ptr)
{
	Vec3 xaxis = Vec3(1.0f, 0.0f, 0.0f);
	Vec3 yaxis = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 zaxis = Vec3(0.0f, 0.0f, 1.0f);

	if(!sphere_ptr->transformed)
	{
		sphere_ptr->alignmentBasis.set(xaxis, yaxis, zaxis);
		return false;
	}

	// else, calculate alignment
	glm::mat4 trans_mat = sphere_ptr->M;
	Vec3 abU = unitVector(rtmath::transformVec(trans_mat, xaxis));
	Vec3 abV = unitVector(rtmath::transformVec(trans_mat, yaxis));
	Vec3 abW = unitVector(rtmath::transformVec(trans_mat, zaxis));
	sphere_ptr->alignmentBasis.set(abU, abV, abW);

	return true;
}

bool setTransformOfShape(Shape* shape_ptr, tinyxml2::XMLElement* element, std::stringstream& ss, 
			glm::mat4& transMat,
			const std::vector<glm::mat4>& translations, 
			const std::vector<glm::mat4>& scalings, 
			const std::vector<glm::mat4>& rotations,
			const std::vector<glm::mat4>& composites,
			bool resetTransform)
{
	tinyxml2::XMLElement *trans_element = element->FirstChildElement("Transformations");
	bool _transformed = applyTransforms(trans_element, ss, transMat,
						translations, scalings, rotations, composites);
	if(_transformed)
	{
		shape_ptr->setTransform(transMat, resetTransform);
		return true;
	}

	return false;
}

bool setMotionBlurOfShape(Shape* shape_ptr, tinyxml2::XMLElement* element, std::stringstream& ss)
{
	Vec3 velocity;
	bool motionBlurred = getChildTextWithDefault(element, ss, "MotionBlur", "0 0 0");
	ss >> velocity[0] >> velocity[1] >> velocity[2];
	if(motionBlurred)
	{
		shape_ptr->setMotionBlur(velocity);
		return true;
	}

	return false;
}

void parseAmbientLight(Vec3& amblight, tinyxml2::XMLElement* element, std::stringstream& ss)
{
	tinyxml2::XMLElement *child;

        child = element->FirstChildElement("AmbientLight");
        ss << child->GetText() << std::endl;
        ss >> amblight[0] >> amblight[1] >> amblight[2];
}

void parsePointLight(PointLight* point_light, tinyxml2::XMLElement* element, std::stringstream& ss)
{
	tinyxml2::XMLElement *child;

	child = element->FirstChildElement("Position");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("Intensity");
	ss << child->GetText() << std::endl;

	ss >> point_light->position[0] >> point_light->position[1]
		>> point_light->position[2];
	ss >> point_light->intensity[0] >> point_light->intensity[1]
		>> point_light->intensity[2];
}

void parseAreaLight(AreaLight* area_light, tinyxml2::XMLElement* element, std::stringstream& ss)
{
	tinyxml2::XMLElement *child;

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
	ss >> area_light->radiance[0] >> area_light->radiance[1]
		>> area_light->radiance[2];

	// Make sure its normal is unit
	area_light->normal.makeUnitVector();	
}

void parseDirectionalLight(DirectionalLight* directional_light,
				tinyxml2::XMLElement* element, std::stringstream& ss)
{
	tinyxml2::XMLElement *child;

	child = element->FirstChildElement("Direction");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("Radiance");
	ss << child->GetText() << std::endl;

	ss >> directional_light->direction[0] >> directional_light->direction[1]
		>> directional_light->direction[2];
	ss >> directional_light->radiance[0] >> directional_light->radiance[1]
		>> directional_light->radiance[2];
}

void parseSpotLight(SpotLight* spot_light, tinyxml2::XMLElement* element, std::stringstream& ss)
{
	tinyxml2::XMLElement *child;

	child = element->FirstChildElement("Position");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("Direction");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("Intensity");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("CoverageAngle");
	ss << child->GetText() << std::endl;
	child = element->FirstChildElement("FalloffAngle");
	ss << child->GetText() << std::endl;

	ss >> spot_light->position[0] >> spot_light->position[1]
		>> spot_light->position[2];
	ss >> spot_light->direction[0] >> spot_light->direction[1]
		>> spot_light->direction[2];
	ss >> spot_light->intensity[0] >> spot_light->intensity[1]
		>> spot_light->intensity[2];
	ss >> spot_light->alpha;
	ss >> spot_light->beta;
}

void parseMaterial(Material& material, tinyxml2::XMLElement* element,
			std::stringstream& ss, const std::vector<BRDF*>& brdfs)
{
	tinyxml2::XMLElement *child;
	
	int brdf_id = getIntAttributeWithDefault(element, "BRDF", 0);	// 0 is simple default
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
	material.degamma = getBoolAttributeWithDefault(element, "degamma", false);
	material.brdf = brdfs[brdf_id];	// Already 1-based indexing (0 for simple)
}

void readVec3FromSS(Vec3& out, std::stringstream& ss)
{
	ss >> out[0] >> out[1] >> out[2];
}
