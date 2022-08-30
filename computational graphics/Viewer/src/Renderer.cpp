#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"


#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height)
{
	InitOpenGLRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer_;
}

void Renderer::PutPixel(int i, int j,float z, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;
	if (z <= Zbuffer_[Z_INDEX(viewport_width_, i, j)]) {
		Zbuffer_[Z_INDEX(viewport_width_, i, j)] = z;
		color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
		color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
		color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
	}
}

void Renderer::plotLineHigh(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color) {
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	int xi = 1;
	if (dx < 0)
	{
		xi = -1;
		dx = -1 * dx;
	}
	int error = (2*dx) - dy;
	int start_x = p1.x;
	for (int i = p1.y; i < p2.y; i++) {
		PutPixel(start_x,i,2.0f,color);
		if (error > 0) {
			start_x += xi;
			error += (2*(dx-dy));
		}
		else {
			error += 2 * dx;
		}
	}
}
void Renderer::plotLineLow(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color) {
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	int yi = 1;
	if (dy < 0)
	{
		yi = -1;
		dy = -1 * dy;
	}
	int error = (2 * dy) - dx;
	int start_y = p1.y;
	for (int i = p1.x; i < p2.x; i++) {
		PutPixel(i, start_y,2.0f, color);
		if (error > 0) {
			start_y += yi;
			error += (2 * (dy - dx));
		}
		else {
			error += 2 * dy;
		}
	}
}


void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	if (abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
		if (p1.x > p2.x)
			plotLineLow(p2, p1, color);
		else
			plotLineLow(p1, p2, color);
	}
	else {
		if (p1.y > p2.y)
			plotLineHigh(p2, p1, color);
		else
			plotLineHigh(p1, p2, color);
	}
}
void Renderer::reallocateBuffer() {
	if (color_buffer_) {
		delete(color_buffer_);
	}
	if (Zbuffer_) delete[] Zbuffer_;
	CreateOpenGLBuffer(); //Do not remove this line.
	color_buffer_ = new float[3 * viewport_width_ * viewport_height_];
	Zbuffer_ = new float[viewport_width_ * viewport_height_];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}
void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenGLBuffer(); //Do not remove this line.
	color_buffer_ = new float[3 * w * h];
	Zbuffer_ = new float[w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex_);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc_);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc_);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::CreateOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width_, viewport_height_, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width_, viewport_height_);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width_, viewport_height_, GL_RGB, GL_FLOAT, color_buffer_);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc_);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			Zbuffer_[Z_INDEX(viewport_width_ ,i,j)] = INFINITY;
			PutPixel(i, j,INFINITY, color);
		}
	}
}

glm::vec3 hcTo3d(const glm::vec4& hc) {
	if (hc.w == 0) return glm::vec3(hc.x, hc.y, hc.z);
	return glm::vec3(hc.x / hc.w, hc.y / hc.w, hc.z / hc.w);
}





float MAX(float a, float b) {
	return (a > b) ? a : b;
}
float MIN(float a, float b) {
	return (a < b) ? a : b;
}




void Renderer::LoadTextures() {
	if (!textures.loadTexture("bin\\Debug\\crate.jpg", true))
	{
		textures.loadTexture("bin\\Release\\crate.jpg", true);
	}
}
void Renderer::LoadShaders() {
	Shader.loadShaders("vshader.glsl", "fshader.glsl");
}


void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width_ / 2;
	int half_height = viewport_height_ / 2;
	
	
	for (int model_index = 0; model_index < scene.GetModelCount(); model_index++) {
		MeshModel& currModel = scene.GetModel(model_index);
		const std::vector<glm::vec3>& modelVer = currModel.getModelVertices();
		glm::mat4x4 modelTransformation = currModel.getModelTransformation();
		glm::mat4x4 viewMat = scene.GetActiveCamera().GetViewTransformation();

		
		Shader.use();
		Shader.setUniform("model", modelTransformation);
		Shader.setUniform("view", viewMat);
		Shader.setUniform("modelColor", currModel.getModelColor());
		Shader.setUniform("projection", scene.GetActiveCamera().GetProjectionTransformation());
		Shader.setUniform("textureMap", 0);
		Shader.setUniform("activeTextures", scene.GetEnableTextures());


		if (scene.GetLightCount()) {
			glm::vec3 light_pos= hcTo3d(scene.GetActiveLight().rotateLight() * glm::vec4(scene.GetActiveLight().GetPostion(), 1.0f));
			Shader.setUniform("activelight", scene.GetEnableLighting());
		
			Shader.setUniform("toon", scene.GetEnableToons());
		
			Shader.setUniform("MatAmbientColor", currModel.GetAmbientColor());
			Shader.setUniform("MatdiffuseColor", currModel.GetDiffuseColor());
			Shader.setUniform("MatspecularColor", currModel.GetSpecularColor());
			Shader.setUniform("Matshinness", currModel.GetSpecularshine());

			Shader.setUniform("lightPos", light_pos);
			Shader.setUniform("ambientColor", scene.GetActiveLight().GetAmbientColor());
			Shader.setUniform("diffuseColor", scene.GetActiveLight().GetDiffuseColor());
			Shader.setUniform("specularColor", scene.GetActiveLight().GetSpecularColor());
			Shader.setUniform("cameraPos", scene.GetActiveCamera().getCamPos());
			Shader.setUniform("specularStr", scene.GetActiveLight().GetSpeculatIntensity());
			Shader.setUniform("ambientStr", scene.GetActiveLight().GetAmbientIntensity());
		
		
		}
		// Set 'textures' as the active texture at slot #0
		textures.bind(0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(currModel.GetVAO());
		glDrawArrays(GL_TRIANGLES, 0, currModel.getModelData().size());
		glBindVertexArray(0);

		// Unset 'textures' as the active texture at slot #0
		textures.unbind(0);

		// Drag our model's faces (triangles) in line mode (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(currModel.GetVAO());
		glDrawArrays(GL_TRIANGLES, 0, currModel.getModelData().size());
		glBindVertexArray(0);
	}
}

int Renderer::GetViewportWidth() const
{
	return viewport_width_;
}
void Renderer::setWidth(int w){
	viewport_width_ = w;
}
void Renderer::setHeight(int h) {
	viewport_height_ = h;
}
int Renderer::GetViewportHeight() const
{
	return viewport_height_;
}