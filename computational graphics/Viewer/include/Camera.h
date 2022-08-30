#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	typedef enum proj
	{
		ORTHOGRAPHIC,
		PRESPECTIVE
	}PROJECTION;

	Camera(int w,int h);
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	glm::vec3& getCamPos();
	int& getCamZoom();
	int& getXrotation();
	int& getYrotation();
	int& getXwrotation();
	int& getYwrotation();
	int& getOrthoDist();
	int& getFOV();
	PROJECTION& getProjType();
	void setAspectRatio(float aspect_);

	void rotateCam(glm::vec3& camPos);
	
	const glm::mat4x4& GetProjectionTransformation() ;
	const glm::mat4x4& GetViewTransformation() ;

private:

	const float DEG2RAD = 3.14159265359f / 180; //  pi/180

	PROJECTION projType_;
	float aspectRatio_;
	float zNear_;
	float zFar_;
	int xRotation;
	int yRotation;
	int xwRotation;
	int ywRotation;
	int orthoDist;
	int FOV_;
	int zoom;
	glm::vec3 camPos_;

	void prespectiveProjection();
	void orthographicProjection();

	glm::mat4x4 view_transformation_;
	glm::mat4x4 projection_transformation_;
};
