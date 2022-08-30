#include "Camera.h"

Camera::Camera(int w, int h):
projType_(PRESPECTIVE),
aspectRatio_((float)w/(float)h),
zNear_(1.f),
zFar_(10.f),
FOV_(45),
zoom(1),
xRotation(0),
yRotation(0),
camPos_(0.0f,0.0f,4.0f),
orthoDist(1),
xwRotation(0),
ywRotation(0)
{
	
}
void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up) {
	glm::vec3 n_eye(eye);
	glm::vec3 n_at(at);
	rotateCam(n_eye);

	n_at.x = tan(ywRotation * DEG2RAD) * sqrt(pow(eye.x, 2)+pow(eye.y, 2)+pow(eye.z, 2));
	n_at.y = tan(xwRotation * DEG2RAD) * sqrt(pow(eye.x, 2)+pow(eye.y, 2)+pow(eye.z, 2));
	glm::vec3 zaxis = glm::normalize(n_eye -n_at);
	glm::vec3 xaxis = glm::normalize(glm::cross(up,zaxis));
	glm::vec3 yaxis = glm::normalize(glm::cross(zaxis,xaxis));

	glm::vec3 TranslateBy(-1.0f * (xaxis.x * n_eye.x + xaxis.y * n_eye.y + xaxis.z * n_eye.z),
		-1.0f*(yaxis.x * n_eye.x + yaxis.y * n_eye.y + yaxis.z * n_eye.z),
		-1.0f*(zaxis.x * n_eye.x + zaxis.y * n_eye.y + zaxis.z * n_eye.z));
	view_transformation_ = glm::mat4x4(glm::vec4(xaxis,0.0f),
										glm::vec4(yaxis,0.0f),
										glm::vec4(zaxis,0.0f),
										glm::vec4(TranslateBy,1.0f));
}
Camera::~Camera()
{
	
}
int& Camera::getCamZoom() {
	return zoom;
}
glm::vec3& Camera::getCamPos() {
	return camPos_;
}
int& Camera::getXrotation(){
	return xRotation;
}
int& Camera::getYrotation(){
	return yRotation;
}
int& Camera::getXwrotation(){
	return xwRotation;
}
int& Camera::getYwrotation(){
	return ywRotation;
}
int& Camera::getOrthoDist() {
	return orthoDist;
}
void Camera::setAspectRatio(float aspect_) {
	aspectRatio_ = aspect_;
}
Camera::PROJECTION& Camera::getProjType() {
	return projType_;
}
int& Camera::getFOV() {
	return FOV_;
}
void Camera::rotateCam(glm::vec3& camPos ) {
	glm::mat4x4 rotations[2]; // x & y rotations.
	glm::vec4 camPosHC(camPos,1.0f);
	glm::vec3 c4(-1 * camPos.x, -1 * camPos.y, -1 * camPos.z);
	

	//camera x rotate.
	glm::vec4 x_c1(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec4 x_c2(0.0f, cos(xRotation * DEG2RAD), sin(xRotation * DEG2RAD), 0.0f);
	glm::vec4 x_c3(0.0f, -1 * sin(xRotation * DEG2RAD), cos(xRotation * DEG2RAD), 0.0f);
	glm::vec4 x_c4(0.0f, 0.0f, 0.0f, 1.0f);
	rotations[0] = glm::mat4x4(x_c1, x_c2, x_c3, x_c4);
	//camera x rotate.
	glm::vec4 y_c1(cos(yRotation * DEG2RAD), 0.0, -1 * sin(yRotation * DEG2RAD), 0.0f);
	glm::vec4 y_c2(0.0f, 1.0f, 0.0f, 0.0f);
	glm::vec4 y_c3(sin(yRotation * DEG2RAD), 0.0f, cos(yRotation * DEG2RAD), 0.0f);
	glm::vec4 y_c4(0.0f, 0.0f, 0.0f, 1.0f);
	rotations[1] = glm::mat4x4(y_c1, y_c2, y_c3, y_c4);

	camPosHC =  rotations[0] * rotations[1]   * camPosHC;

	camPos = glm::vec3(camPosHC.x/ camPosHC.w , camPosHC.y/ camPosHC.w , camPosHC.z/ camPosHC.w);


}
void Camera::prespectiveProjection() {
	float fov_tan = tan(FOV_ * DEG2RAD);
	float h_ = zNear_ * fov_tan;
	float w_ = h_ * aspectRatio_;
	float NearMulZoom = zNear_ * zoom;
	glm::vec4 col1(zoom/(aspectRatio_*tan(FOV_*DEG2RAD)),0.0f,0.0f,0.0f);
	glm::vec4 col2(0.0f, zoom / tan(FOV_  * DEG2RAD),0.0f,0.0f);
	glm::vec4 col3(0.0f,0.0f,(-1*(zFar_ + NearMulZoom))/(zFar_-zNear_),-1.0f);
	glm::vec4 col4(0.0f,0.0f,-2*zFar_*zNear_/(zFar_-zNear_),0.0f);
	projection_transformation_ = glm::mat4x4(col1,col2,col3,col4);
}
void Camera::orthographicProjection(){
	glm::vec4 orthoTranslate(0.0f,0.0f,-1*((zFar_+zNear_)/(zFar_-zNear_)),1.0f);
	glm::vec4 col1(zoom*1.0f/orthoDist,0.0f,0.0f,0.0f);
	glm::vec4 col2(0.0f,zoom*1.0f/orthoDist,0.0f,0.0f);
	glm::vec4 col3(0.0f,0.0f,zoom*-2.0f/(orthoDist*(zFar_-zNear_)),0.0f);
	projection_transformation_ = glm::mat4x4(col1,col2,col3,orthoTranslate);
}
const glm::mat4x4& Camera::GetProjectionTransformation() 
{
	if (projType_ == ORTHOGRAPHIC) {
		orthographicProjection();
	}
	else {
		prespectiveProjection();
	}
	return projection_transformation_;
}

const glm::mat4x4& Camera::GetViewTransformation() 
{
	SetCameraLookAt(camPos_, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return view_transformation_;
}