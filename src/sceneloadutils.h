#ifndef _SCENELOADUTILS_H
#define _SCENELOADUTILS_H

#include "utils.h"

// Helper Declerations
bool getChildTextWithDefault(tinyxml2::XMLElement* element, std::stringstream& ss,
                                std::string name, std::string _default);
bool getChildTextWithDefaultFromNode(tinyxml2::XMLNode* node, std::stringstream& ss,
                                std::string name, std::string _default);
int getCameraType(tinyxml2::XMLElement* element);
int getMeshType(tinyxml2::XMLElement* element, std::string& ply_path);
int getMeshShadingMode(tinyxml2::XMLElement* element);
int getIntAttributeWithDefault(tinyxml2::XMLElement* element, std::string name, int _default);
bool getBoolAttributeWithDefault(tinyxml2::XMLElement* element, std::string name, bool _default);

// Read All transformations into appropriate vectors as Matrices
int getTransformations(tinyxml2::XMLElement* element, std::stringstream& ss,
                        std::vector<glm::mat4>& translations,
                        std::vector<glm::mat4>& scalings,
                        std::vector<glm::mat4>& rotations);

// Apply specified transformations
bool applyTransforms(tinyxml2::XMLElement* element, std::stringstream& ss, glm::mat4& transMat,
                        const std::vector<glm::mat4>& translations,
                        const std::vector<glm::mat4>& scalings,
                        const std::vector<glm::mat4>& rotations);

// set Transformation of a shape (to set/update it after its creation)
bool setTransformOfShape(Shape* shape_ptr, tinyxml2::XMLElement* element, std::stringstream& ss,
                        glm::mat4& transMat,
                        const std::vector<glm::mat4>& translations,
                        const std::vector<glm::mat4>& scalings,
                        const std::vector<glm::mat4>& rotations,
                        bool resetTransform=true);

bool setMotionBlurOfShape(Shape* shape_ptr, tinyxml2::XMLElement* element, std::stringstream& ss);

void pushCameraLookAt(tinyxml2::XMLElement* element, std::stringstream& ss,
                        std::vector<Camera>& cameras);
void pushCameraSimple(tinyxml2::XMLElement* element, std::stringstream& ss,
                        std::vector<Camera>& cameras);
void pushFacesOfPlyMesh(std::vector<Shape*>& shapes, Mesh* mesh, std::vector<Vertex>& vertex_data,
                        int shadingMode, const std::string& fname,
                        const std::string& plyname);
void pushFacesOfMesh(std::vector<Shape*>& shapes, Mesh* mesh, std::vector<Vertex>& vertex_data,
                        int shadingMode, int vertexOffset, std::stringstream& ss);

// Specific to spheres 
// make the original sphere unit and apply transformations on it
bool makeUnitSphere(Sphere* sphere_ptr);
// calculate alignment of the sphere, used in texture mapping
bool setAlignmentOfSphere(Sphere* sphere_ptr);

// Parse methods
void parseAmbientLight(AmbientLight& light, tinyxml2::XMLElement* element, std::stringstream& ss);
void parsePointLight(PointLight* point_light, tinyxml2::XMLElement* element, std::stringstream& ss);
void parseAreaLight(AreaLight* area_light, tinyxml2::XMLElement* element, std::stringstream& ss);
void parseMaterial(Material& material, tinyxml2::XMLElement* element, std::stringstream& ss);

// SS reads
void readVec3FromSS(Vec3& out, std::stringstream& ss);

#endif
