#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture2D.h"

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(const Scene& scene);
	void SwapBuffers();
	void GussianBlur();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void reallocateBuffer();
	void setWidth(int w);
	void setHeight(int h);
	void LoadTextures();
	void LoadShaders();
	

private:
	void PutPixel(const int i, const int j, float z, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);


	/*---Bresenham's Algorithm---*/
	void plotLineHigh(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
	void plotLineLow(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
	/*---Bresenham's Algorithm---*/

	// draws mesh model bounding box.
	void drawBoundingBox(glm::mat4x4& trans_, std::vector<glm::vec3>& points, glm::vec3& color_);
	// draws faces / vertices normals (according to input).
	void drawNormals(const std::vector<glm::vec3>& nVec, const std::vector<glm::vec3>& oPoints, const glm::mat4x4& trans_, const glm::vec3& color_);
	// draw X,Y,Z axises and replaces them in the middle of the screen.
	void drawAxis(int width_,int height_, const glm::mat4x4& cameraTransformation);

	void CreateBuffers(int w, int h);
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();
	

	ShaderProgram Shader;
	Texture2D textures;

	
	float* Zbuffer_;
	float* color_buffer_;
	int viewport_width_;
	int viewport_height_;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;
};
