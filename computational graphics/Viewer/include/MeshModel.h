#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"
#include <glad/glad.h>
#include <vector>
struct ModelData
{
	glm::vec3 position_;
	glm::vec3 normal_;
	glm::vec2 textureCoords_;
};

class MeshModel
{
public:


	typedef enum  transformation{
		WORLD,
		LOCAL
	}TRANSFORMATION;


	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, std::vector<glm::vec2> textureCoords);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	const std::vector<glm::vec3>& getModelVertices() const;
	//material getters.
	glm::vec3& GetAmbientColor();
	glm::vec3& GetDiffuseColor();
	glm::vec3& GetSpecularColor();
	int& GetSpecularshine();

	//color getters.
	glm::vec3& getModelColor();
	glm::vec3& getFnormColor();
	glm::vec3& getVnormColor();
	glm::vec3& getBoxColor();
	//Getters for the transformation data
	glm::vec4& getLocScalingVec();
	glm::vec4& getWorScalingVec();
	glm::vec3& getWorTranslationVec();
	glm::vec3& getWorRotationVec();
	glm::vec3& getLocRotationVec();
	glm::vec3& getLocTranslationVec();
	glm::mat4x4 getModelTransformation();
	// bounding box & normals data getters.
	std::vector<glm::vec3>& getFacesOpoints();
	std::vector<glm::vec3>& getFacesNormals();
	std::vector<glm::vec3>& getVerticesNorms();
	std::vector<glm::vec3>& getBoxPoints();
	bool& getDrawBoxFlag();
	bool& getDrawFaceNormals();
	bool& getDrawVerticesNorms();

	GLuint GetVAO();
	const 	std::vector<ModelData>& getModelData();

private:

	GLuint vao;
	GLuint vbo;
	std::vector<ModelData> modelData_;

	//material data.
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	int specularshine_;

	//data loaded from obj file
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::string model_name_;

	// max, min, origin points of the mesh.
	glm::vec3 maxPoints;
	glm::vec3 minPoints;
	glm::vec3 originPoints;

	glm::vec3 meshModelColor_;
	glm::vec3 boxColor_;
	glm::vec3 faceNormsColor_;
	glm::vec3 vertexNormsColor_;
	// method to calculate max, min, origin points (called once in the constructor). 
	void calculatePoints();
	void normalizeVertices();
	//Transformations data:
	const float DEG2RAD = 3.14159265359f / 180;
	TRANSFORMATION type_;
	//local
	glm::vec4 locScalingVec;
	glm::vec3 locTranslationVec;
	glm::vec3 locRotationVec;
	//world
	glm::vec4 worScalingVec;
	glm::vec3 worTranslationVec;
	glm::vec3 worRotationVec;
	//world & local world transformation matrix calculations
	glm::mat4x4 calcTransformation(const glm::vec3& rotate_, const glm::vec3& translate_, const glm::vec4& scale_, MeshModel::TRANSFORMATION type);
	//normals per face data.
	bool drawFaceNormals;
	std::vector<glm::vec3> faceNormals;
	std::vector<glm::vec3> faceNormalsOpoints;
	//calculate normals per face (called once in the constructor).
	void calFacesNormals();
	// normal per vertex data.
	bool drawVerticesNormals;
	std::vector<glm::vec3> verticesNormMean;
	//calculates the average of the given normal vertices.
	void calVerticesNorms();
	//bounding box points.
	std::vector<glm::vec3> boxPoints;
	//prepares the bounding box points (called once in the constructor).
	bool drawBoundingBox;
	void caluculateBoxPoints();

};
