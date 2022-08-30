#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>


class Light {
public:
	enum Lighting
	{
		POINT,
		PARALLEL
	};
	enum Shading
	{
		FLAT,
		GOURAUD,
		PHONG
	};
	Light();

	Lighting& GetLightingType();
	static Shading& GetShadingT();
	glm::vec3& GetAmbientColor();
	glm::vec3& GetDiffuseColor();
	glm::vec3& GetSpecularColor();
	float& GetAmbientIntensity();
	float& GetSpeculatIntensity();
	glm::vec3& GetPostion();
	glm::ivec3& getRotationsVec();
	glm::mat4x4 rotateLight();

private:
	const float DEG2RAD = 3.14159265359f / 180;
	glm::ivec3 rotations;
	Lighting lightingType;
	static Shading ShadingT;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float ambientIntensity;
	float speculatIntensity;
	glm::vec3 postion;

};