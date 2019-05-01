#ifndef _SCENELOADUTILS_H
#define _SCENELOADUTILS_H

#include "utils.h"

// Helper Declerations
bool getBackgroundTexture(tinyxml2::XMLNode* node, std::string fname, Image& image);
bool getChildTextWithDefault(tinyxml2::XMLElement* element, std::stringstream& ss,
                                std::string name, std::string _default);
bool getChildTextWithDefaultFromNode(tinyxml2::XMLNode* node, std::stringstream& ss,
                                std::string name, std::string _default);
bool getBoolChildWithDefaultFromNode(tinyxml2::XMLNode* node, std::string name,
					bool _default, bool& val);
bool getIntChildWithDefault(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::string name, int _default, int& val);
bool getFloatChildWithDefault(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::string name, float _default, float& val);
bool getBoolChildWithDefault(tinyxml2::XMLElement* element, std::string name,
					bool _default, bool& val);
bool getVec3ChildWithDefault(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::string name, const Vec3& _default, Vec3& val);
bool getrgbChildWithDefault(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::string name, const rgb& _default, rgb& val);

int getCameraType(tinyxml2::XMLElement* element);
int getMeshType(tinyxml2::XMLElement* element, std::string& ply_path);
int getMeshShadingMode(tinyxml2::XMLElement* element);
int getIntAttributeWithDefault(tinyxml2::XMLElement* element, std::string name, int _default);
bool getBoolAttributeWithDefault(tinyxml2::XMLElement* element, std::string name, bool _default);
float getFloatAttributeWithDefault(tinyxml2::XMLElement* element, std::string name, float _default);

// Read cameras
int getCameras(tinyxml2::XMLElement* element, std::stringstream& ss, std::vector<Camera>& cameras);

// Read lights
int getLights(tinyxml2::XMLNode* node, tinyxml2::XMLElement* element,
		std::stringstream& ss, std::vector<Light*>& lights);

// Read All transformations into appropriate vectors as Matrices
int getTransformations(tinyxml2::XMLElement* element, std::stringstream& ss,
                        std::vector<glm::mat4>& translations,
                        std::vector<glm::mat4>& scalings,
                        std::vector<glm::mat4>& rotations,
			std::vector<glm::mat4>& composites);

// Read all texture images, create texture objects
int getTextures(tinyxml2::XMLElement* element, std::stringstream& ss, const std::string& fname,
			std::vector<Image*>& textureImages, std::vector<Texture*>& textures);

// Read BRDFs
int getBRDFs(tinyxml2::XMLElement* element, std::stringstream& ss, std::vector<BRDF*>& brdfs);
bool getSingleBRDF(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::vector<BRDF*>& brdfs);
bool getOriginalPhongBRDF(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::vector<BRDF*>& brdfs);
bool getModifiedPhongBRDF(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::vector<BRDF*>& brdfs);
bool getOriginalBlinnPhongBRDF(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::vector<BRDF*>& brdfs);
bool getModifiedBlinnPhongBRDF(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::vector<BRDF*>& brdfs);
bool getTorranceSparrowBRDF(tinyxml2::XMLElement* element, std::stringstream& ss,
				std::vector<BRDF*>& brdfs);

// Read Materials
int getMaterials(tinyxml2::XMLElement* element, std::stringstream& ss,
			std::vector<Material>& materials, const std::vector<BRDF*>& brdfs);

InterpolationMode getInterpolationMode(tinyxml2::XMLElement* element);
DecalMode getDecalMode(tinyxml2::XMLElement* element);
TextureMode getTextureModeWithDefault(tinyxml2::XMLElement* element, TextureMode _default);
PerlinPattern getPerlinPattern(tinyxml2::XMLElement* element);

// Apply specified transformations
bool applyTransforms(tinyxml2::XMLElement* element, std::stringstream& ss, glm::mat4& transMat,
                        const std::vector<glm::mat4>& translations,
                        const std::vector<glm::mat4>& scalings,
                        const std::vector<glm::mat4>& rotations,
			const std::vector<glm::mat4>& composites);

// set Transformation of a shape (to set/update it after its creation)
bool setTransformOfShape(Shape* shape_ptr, tinyxml2::XMLElement* element, std::stringstream& ss,
                        glm::mat4& transMat,
                        const std::vector<glm::mat4>& translations,
                        const std::vector<glm::mat4>& scalings,
                        const std::vector<glm::mat4>& rotations,
			const std::vector<glm::mat4>& composites,
                        bool resetTransform=true);

bool setMotionBlurOfShape(Shape* shape_ptr, tinyxml2::XMLElement* element, std::stringstream& ss);

bool getTonemap(tinyxml2::XMLElement* element, std::stringstream& ss, Tonemap& tonemap);
void pushCameraLookAt(tinyxml2::XMLElement* element, std::stringstream& ss,
                        std::vector<Camera>& cameras);
void pushCameraSimple(tinyxml2::XMLElement* element, std::stringstream& ss,
                        std::vector<Camera>& cameras);
void pushFacesOfPlyMesh(std::vector<Shape*>& shapes, Mesh* mesh, std::vector<Vertex>& vertex_data,
                        std::vector<Vec2>& texCoord_data,
			int shadingMode, const std::string& fname, const std::string& plyname);
void pushFacesOfMesh(std::vector<Shape*>& shapes, Mesh* mesh, std::vector<Vertex>& vertex_data,
                        int shadingMode, int vertexOffset,
			int textureOffset, std::stringstream& ss);

// Specific to spheres 
// make the original sphere unit and apply transformations on it
bool makeUnitSphere(Sphere* sphere_ptr);
// calculate alignment of the sphere, used in texture mapping
bool setAlignmentOfSphere(Sphere* sphere_ptr);

// Parse methods
void parseAmbientLight(Vec3& amblight, tinyxml2::XMLElement* element, std::stringstream& ss);
void parsePointLight(PointLight* point_light, tinyxml2::XMLElement* element, std::stringstream& ss);
void parseAreaLight(AreaLight* area_light, tinyxml2::XMLElement* element, std::stringstream& ss);
void parseDirectionalLight(DirectionalLight* directional_light,
				tinyxml2::XMLElement* element, std::stringstream& ss);
void parseSpotLight(SpotLight* spot_light, tinyxml2::XMLElement* element, std::stringstream& ss);
void parseMaterial(Material& material, tinyxml2::XMLElement* element,
			std::stringstream& ss, const std::vector<BRDF*>& brdfs);

// SS reads
void readVec3FromSS(Vec3& out, std::stringstream& ss);

#endif
