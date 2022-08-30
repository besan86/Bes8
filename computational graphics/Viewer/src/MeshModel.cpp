#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, std::vector<glm::vec2> textureCoords) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	locScalingVec(1.0f, 1.0f, 1.0f, 0.0f),
	locRotationVec(0.0f, 0.0f, 0.0f),
	locTranslationVec(0.0f, 0.0f, 0.0f),
	worScalingVec(1.0f, 1.0f, 1.0f, 0.0f),
	worRotationVec(0.0f, 0.0f, 0.0f),
	worTranslationVec(0.0f, 0.0f, 0.0f),
	drawFaceNormals(false),
	drawVerticesNormals(false),
	drawBoundingBox(false),
	meshModelColor_(1.0f, 1.0f, 1.0f),
	boxColor_(1.0f, 0.0f, 0.0f),
	faceNormsColor_(1.0f, 1.0f, 0.0f),
	vertexNormsColor_(0.0f, 1.0f, 1.0f),
	ambientColor(1.0f, 1.0f, 1.0f),
	diffuseColor(1.0f, 1.0f, 1.0f),
	specularColor(1.0f, 1.0f, 1.0f),
	specularshine_(1)
{
	calculatePoints();  // calculate max,min,origin points
	normalizeVertices(); // from [A,B] to [0,1]
	calFacesNormals(); // calculate face normals.
	caluculateBoxPoints(); // calculates bounding box points.
	calVerticesNorms(); // calculates the vertices normals.

	modelData_.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;
			int normalIndex = currentFace.GetNormalIndex(j) - 1;
			ModelData data_;
			data_.position_ = vertices[vertexIndex];
			data_.normal_ = normals[normalIndex];

			if (textureCoords.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				data_.textureCoords_ = textureCoords[textureCoordsIndex];
			}
			modelData_.push_back(data_);
		}
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelData_.size() * sizeof(ModelData), &modelData_[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelData), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelData), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelData), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

}

void MeshModel::calculatePoints() {
	for (int i = 0; i < vertices_.size(); i++) {
		if (!i) {
			maxPoints = vertices_.at(i);
			minPoints = vertices_.at(i);
		}else{
			maxPoints.x = (maxPoints.x > vertices_.at(i).x) ? maxPoints.x : vertices_.at(i).x;
			maxPoints.y = (maxPoints.y > vertices_.at(i).y) ? maxPoints.y : vertices_.at(i).y;
			maxPoints.z = (maxPoints.z > vertices_.at(i).z) ? maxPoints.z : vertices_.at(i).z;
	
			minPoints.x = (minPoints.x < vertices_.at(i).x) ? minPoints.x : vertices_.at(i).x;
			minPoints.y = (minPoints.y < vertices_.at(i).y) ? minPoints.y : vertices_.at(i).y;
			minPoints.z = (minPoints.z < vertices_.at(i).z) ? minPoints.z : vertices_.at(i).z;
		}
	}
	originPoints = glm::vec3((maxPoints+minPoints)/2.0f);
}
void MeshModel::calVerticesNorms() {
	verticesNormMean = std::vector<glm::vec3>(vertices_.size());
	int i;
	for (i = 0; i < faces_.size(); i++) {
		verticesNormMean.at(faces_.at(i).GetVertexIndex(0) - 1) += normals_.at(faces_.at(i).GetNormalIndex(0) - 1);
		verticesNormMean.at(faces_.at(i).GetVertexIndex(1) - 1) += normals_.at(faces_.at(i).GetNormalIndex(1) - 1);
		verticesNormMean.at(faces_.at(i).GetVertexIndex(2) - 1) += normals_.at(faces_.at(i).GetNormalIndex(2) - 1);
	}
	for (i = 0; i < verticesNormMean.size(); i++) {
		verticesNormMean.at(i) = glm::normalize(verticesNormMean.at(i));
	}
}
void MeshModel::normalizeVertices() {
	glm::vec3 ABto01(abs(maxPoints.x - minPoints.x),abs(maxPoints.y - minPoints.y),abs(maxPoints.z - minPoints.z));
	for (int i = 0; i < vertices_.size(); i++) {
		vertices_.at(i) /= ABto01;
	}
	maxPoints /= ABto01;
	minPoints /= ABto01;
	originPoints /= ABto01;
}
void MeshModel::calFacesNormals() {
	faceNormals= std::vector<glm::vec3>(faces_.size());
	faceNormalsOpoints= std::vector<glm::vec3>(faces_.size());
	for (int i = 0; i < faces_.size(); i++) {
		glm::vec3 p1(vertices_.at(faces_.at(i).GetVertexIndex(0)-1));
		glm::vec3 p2(vertices_.at(faces_.at(i).GetVertexIndex(1)-1));
		glm::vec3 p3(vertices_.at(faces_.at(i).GetVertexIndex(2)-1));

		faceNormals.at(i) =  glm::cross((p2-p1),(p3-p1));
		faceNormalsOpoints.at(i) = (p1+p2+p3)/3.0f;
		
	}
}

void MeshModel::caluculateBoxPoints() {
	boxPoints.push_back(glm::vec3(minPoints));
	boxPoints.push_back(glm::vec3(minPoints.x,minPoints.y,maxPoints.z));
	boxPoints.push_back(glm::vec3(minPoints.x,maxPoints.y,minPoints.z));
	boxPoints.push_back(glm::vec3(minPoints.x,maxPoints.y,maxPoints.z));
	boxPoints.push_back(glm::vec3(maxPoints));
	boxPoints.push_back(glm::vec3(maxPoints.x,minPoints.y,minPoints.z));
	boxPoints.push_back(glm::vec3(maxPoints.x, minPoints.y,maxPoints.z));
	boxPoints.push_back(glm::vec3(maxPoints.x,maxPoints.y,minPoints.z));
}
const std::vector<glm::vec3>& MeshModel::getModelVertices() const {
	return vertices_;
}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces_[index];
}

int MeshModel::GetFacesCount() const
{
	return faces_.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name_;
}

//GETTERS.
glm::vec3& MeshModel::GetAmbientColor() {
	return ambientColor;
}
glm::vec3& MeshModel::GetDiffuseColor() {
	return diffuseColor;
}
glm::vec3& MeshModel::GetSpecularColor() {
	return specularColor;
}
int& MeshModel::GetSpecularshine() {
	return specularshine_;
}
glm::vec3& MeshModel::getModelColor(){
	return meshModelColor_;
}
glm::vec3& MeshModel::getFnormColor(){
	return faceNormsColor_;
}
glm::vec3& MeshModel::getVnormColor(){
	return vertexNormsColor_;
}
glm::vec3& MeshModel::getBoxColor() {
	return boxColor_;
}
glm::vec4& MeshModel::getLocScalingVec() {
	return locScalingVec;
}
glm::vec4& MeshModel::getWorScalingVec() {
	return worScalingVec;
}
glm::vec3& MeshModel::getWorTranslationVec() {
	return worTranslationVec;
}
glm::vec3& MeshModel::getWorRotationVec() {
	return worRotationVec;
}
glm::vec3& MeshModel::getLocRotationVec() {
	return locRotationVec;
}
glm::vec3& MeshModel::getLocTranslationVec() {
	return locTranslationVec;
}
std::vector<glm::vec3>& MeshModel::getFacesOpoints() {
	return faceNormalsOpoints;
}
std::vector<glm::vec3>& MeshModel::getFacesNormals() {
	return faceNormals;
}
std::vector<glm::vec3>& MeshModel::getVerticesNorms() {
	return verticesNormMean;
}
std::vector<glm::vec3>& MeshModel::getBoxPoints() {
	return boxPoints;
}
bool& MeshModel::getDrawBoxFlag() {
	return drawBoundingBox;
}
bool& MeshModel::getDrawFaceNormals() {
	return drawFaceNormals;
}
bool& MeshModel::getDrawVerticesNorms() {
	return drawVerticesNormals;
}

GLuint MeshModel::GetVAO() {
	return vao;
}
const 	std::vector<ModelData>& MeshModel::getModelData() {
	return modelData_;
}
//transformation getter
glm::mat4x4 MeshModel::getModelTransformation() {
	return calcTransformation(worRotationVec,worTranslationVec,worScalingVec,WORLD) 
		* calcTransformation(locRotationVec,locTranslationVec,locScalingVec,LOCAL);
}

//world & local world transformation matrix calculations

glm::mat4x4 MeshModel::calcTransformation(const glm::vec3& rotate_, const glm::vec3& translate_, const glm::vec4& scale_,MeshModel::TRANSFORMATION type) {
	glm::mat4x4 rotations[3];
	glm::mat4x4 scaling(1);
	glm::mat4x4 translation(1);

	scaling[0][0] = scale_.x + scale_.w;
	scaling[1][1] = scale_.y + scale_.w;
	scaling[2][2] = scale_.z + scale_.w;

	translation[3][0] = translate_.x;
	translation[3][1] = translate_.y;
	translation[3][2] = translate_.z;

	// x rotation
	rotations[0] = glm::mat4x4(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, cos(rotate_.x * DEG2RAD), sin(rotate_.x * DEG2RAD), 0.0f),
		glm::vec4(0.0f, -1.0f * sin(rotate_.x * DEG2RAD), cos(rotate_.x * DEG2RAD), 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	// y rotation
	rotations[1] = glm::mat4x4(glm::vec4(cos(rotate_.y * DEG2RAD), 0.0f, -1.0f * sin(rotate_.y * DEG2RAD), 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
		glm::vec4(sin(rotate_.y * DEG2RAD), 0.0f, cos(rotate_.y * DEG2RAD), 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	// z rotation
	rotations[2] = glm::mat4x4(glm::vec4(cos(rotate_.z * DEG2RAD), sin(rotate_.z * DEG2RAD), 0.0f, 0.0f),
		glm::vec4(-1.0f * sin(rotate_.z * DEG2RAD), cos(rotate_.z * DEG2RAD), 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	if(type == WORLD)
		return translation * rotations[2] * rotations[1] * rotations[0] * scaling;
	else
	{
		glm::vec4 c1(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec4 c2(0.0f, 1.0f, 0.0f, 0.0f);
		glm::vec4 c3(0.0f, 0.0f, 1.0f, 0.0f);
		glm::vec4 c4(-1.0f*originPoints.x,-1.0f*originPoints.y,-1.0f*originPoints.z,1.0f);
		glm::vec4 c5(originPoints.x,originPoints.y,originPoints.z, 1.0f);
		glm::mat4x4 toScreenOrigin(c1,c2,c3,c4);
		glm::mat4x4 toModelOrigin(c1,c2,c3,c5);
		return translation * toModelOrigin * rotations[2] * rotations[1] * rotations[0] * scaling * toScreenOrigin;
	}
}


