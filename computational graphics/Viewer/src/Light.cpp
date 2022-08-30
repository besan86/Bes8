#include "Light.h"
Light::Shading Light::ShadingT = Light::FLAT;
Light::Light() :
	lightingType(Light::PARALLEL),
	ambientColor(1.0f, 1.0f, 1.0f),
	diffuseColor(1.0f, 1.0f, 1.0f),
	specularColor(1.0f, 1.0f, 1.0f),
	ambientIntensity(.1f),
	speculatIntensity(.05f),
	postion(0.0f,1.0f,0.0f),
	rotations(0.0f,0.0f,0.0f)
{
	
}
glm::mat4x4 Light::rotateLight() {
	glm::mat4x4 rotate[3];

	rotate[0] = glm::mat4x4(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, cos(rotations.x * DEG2RAD), sin(rotations.x * DEG2RAD), 0.0f),
		glm::vec4(0.0f, -1.0f * sin(rotations.x * DEG2RAD), cos(rotations.x * DEG2RAD), 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	// y rotation
	rotate[1] = glm::mat4x4(glm::vec4(cos(rotations.y * DEG2RAD), 0.0f, -1.0f * sin(rotations.y * DEG2RAD), 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
		glm::vec4(sin(rotations.y * DEG2RAD), 0.0f, cos(rotations.y * DEG2RAD), 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	// z rotation
	rotate[2] = glm::mat4x4(glm::vec4(cos(rotations.z * DEG2RAD), sin(rotations.z * DEG2RAD), 0.0f, 0.0f),
		glm::vec4(-1.0f * sin(rotations.z * DEG2RAD), cos(rotations.z * DEG2RAD), 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	return rotate[0] * rotate[1] * rotate[2];
}
glm::ivec3& Light::getRotationsVec() {
	return rotations;
}
Light::Lighting& Light::GetLightingType() {
	return lightingType;
}
Light::Shading& Light::GetShadingT() {
	return ShadingT;
}
glm::vec3& Light::GetAmbientColor() {
	return ambientColor;
}
glm::vec3& Light::GetDiffuseColor() {
	return diffuseColor;
}
glm::vec3& Light::GetSpecularColor() {
	return specularColor;
}
float& Light::GetAmbientIntensity() {
	return ambientIntensity;
}
float& Light::GetSpeculatIntensity() {
	return speculatIntensity;
}
glm::vec3& Light::GetPostion() {
	return postion;
}