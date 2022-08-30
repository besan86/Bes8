#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"

/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;
int local_world_transformation = 0;
int active_model_index = 0;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

/**
 * Function declarations
 */
void LightGUI(Scene& scene);
void renderCameraGui(Scene& scene);
void renderModelGui(Scene& scene);
static void GlfwErrorCallback(int error, const char* description);
static void GlfwResizecallback(GLFWwindow* window, int width, int height);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{
	int windowWidth = 1280, windowHeight = 720;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glEnable(GL_DEPTH_TEST);

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	glfwSetWindowSizeCallback(window,GlfwResizecallback);
	Renderer renderer(frameBufferWidth, frameBufferHeight);
	renderer.LoadShaders();
	renderer.LoadTextures();
	Scene scene = Scene();
	scene.AddCamera(std::make_shared<Camera>(windowWidth, windowHeight));
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		RenderFrame(window, scene, renderer, io);
    }

	Cleanup(window);
    return 0;
}
static void GlfwResizecallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		renderer.setWidth(frameBufferWidth);
		renderer.setHeight(frameBufferHeight);
		scene.GetActiveCamera().setAspectRatio((float)frameBufferWidth/(float)frameBufferHeight);
		renderer.reallocateBuffer();
	}

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown[65])
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
		}
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");
	
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed
	
	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
	renderCameraGui(scene);
	renderModelGui(scene);
	LightGUI(scene);
}

void renderCameraGui(Scene& scene) {
	ImGui::Begin("active camera");
	ImGui::InputFloat3("cam pos",(float*)&scene.GetActiveCamera().getCamPos());
	ImGui::RadioButton("prespective projection", (int*)&scene.GetActiveCamera().getProjType(), 1); ImGui::SameLine();
	ImGui::RadioButton("orthographic projection", (int*)&scene.GetActiveCamera().getProjType(), 0);
	ImGui::SliderInt("x cam rotate", &scene.GetActiveCamera().getXrotation(), 0, 360);
	ImGui::SliderInt("y cam rotate", &scene.GetActiveCamera().getYrotation(), 0, 360);
	ImGui::SliderInt("x world cam rotate", &scene.GetActiveCamera().getYwrotation(), -85, 85);
	ImGui::SliderInt("y world cam rotate", &scene.GetActiveCamera().getXwrotation(), -85, 85);
	ImGui::SliderInt("camera zoom",&scene.GetActiveCamera().getCamZoom(),1,10);
	if (scene.GetActiveCamera().getProjType() == Camera::ORTHOGRAPHIC) {
		ImGui::SliderInt("orthographic distance" , &scene.GetActiveCamera().getOrthoDist(),1,10);
	}
	else {
		ImGui::SliderInt("field of view",&scene.GetActiveCamera().getFOV(),45,120);
	}
	ImGui::End();
}
void renderModelGui(Scene& scene) {

	if (scene.GetModelCount() > 0) {
		ImGui::Begin("active mode");
		if (ImGui::Button("change active model")) {
			active_model_index = (active_model_index + 1) % scene.GetModelCount();
			scene.SetActiveModelIndex(active_model_index);
		}
		ImGui::Text("Material Specifcation:");
		ImGui::Separator();
		ImGui::ColorEdit3("Ambient Color", (float*)&scene.GetActiveModel().GetAmbientColor());
		ImGui::ColorEdit3("Diffuse Color", (float*)&scene.GetActiveModel().GetDiffuseColor());
		ImGui::ColorEdit3("Specular Color", (float*)&scene.GetActiveModel().GetSpecularColor());
		ImGui::SliderInt("shine",(int*)&scene.GetActiveModel().GetSpecularshine(),1,30);
		ImGui::Separator();
		ImGui::ColorEdit3("MeshModel Color", (float*)&scene.GetActiveModel().getModelColor());
		ImGui::ColorEdit3("Bounding Box Color", (float*)&scene.GetActiveModel().getBoxColor());
		ImGui::ColorEdit3("Face Normals Color", (float*)&scene.GetActiveModel().getFnormColor());
		ImGui::ColorEdit3("Vertix Normals Color", (float*)&scene.GetActiveModel().getVnormColor());
		
		ImGui::RadioButton("local transformation", &local_world_transformation,0); ImGui::SameLine();
		ImGui::RadioButton("world transformation", &local_world_transformation,1);
		ImGui::Checkbox("draw bounding box", &scene.GetActiveModel().getDrawBoxFlag());
		ImGui::Checkbox("draw faces normals", &scene.GetActiveModel().getDrawFaceNormals());
		ImGui::Checkbox("draw vertices normals", &scene.GetActiveModel().getDrawVerticesNorms());
		if (!local_world_transformation) {
			ImGui::SliderFloat3("translation", (float*)&scene.GetActiveModel().getLocTranslationVec(), -10, 10, "%.1f");
			ImGui::SliderFloat3("rotations", (float*)&scene.GetActiveModel().getLocRotationVec(), 0, 360, "%.1f");
			ImGui::InputFloat4("scaling", (float*)&scene.GetActiveModel().getLocScalingVec());
			
		}
		else {
			ImGui::SliderFloat3("world translation", (float*)&scene.GetActiveModel().getWorTranslationVec(), -10, 10, "%.1f");
			ImGui::SliderFloat3("world rotations", (float*)&scene.GetActiveModel().getWorRotationVec(), 0, 360, "%.1f");
			ImGui::InputFloat4("world scaling", (float*)&scene.GetActiveModel().getWorScalingVec());

		}
		ImGui::End();
	}
}
void LightGUI(Scene& scene)
{
	ImGui::Begin("lighting gui");
	ImGui::Checkbox("gussian blur",(bool*)&scene.getBlurFlag());
	ImGui::Checkbox("activate textures",(bool*)&scene.GetEnableTextures());
	if (scene.GetLightCount()) {
		ImGui::Checkbox("active light", (bool*)&scene.GetEnableLighting());
		ImGui::Checkbox("Toon Shading", (bool*)&scene.GetEnableToons());
		Light& active_light = scene.GetActiveLight();
		ImGui::Text("lighting type"); ImGui::SameLine();
		ImGui::RadioButton("Parallel", (int*)&active_light.GetLightingType(), 1); ImGui::SameLine();
		ImGui::RadioButton("Point", (int*)&active_light.GetLightingType(), 0);
		ImGui::Separator();
		ImGui::Text("shading type"); ImGui::SameLine();
		ImGui::RadioButton("FLAT", (int*)&active_light.GetShadingT(), 0); ImGui::SameLine();
		ImGui::RadioButton("GOURUAD", (int*)&active_light.GetShadingT(), 1); ImGui::SameLine();
		ImGui::RadioButton("PHONG", (int*)&active_light.GetShadingT(), 2);
		ImGui::Separator();
		ImGui::InputFloat3("Light pos", (float*)&active_light.GetPostion());
		ImGui::SliderInt3("rotate xyz", (int*)&active_light.getRotationsVec(), -360, 360);
		ImGui::SliderFloat("ambient strength", (float*)&active_light.GetAmbientIntensity(),0.01f,1.0f,"%.01f");
		ImGui::SliderFloat("specular strength", (float*)&active_light.GetSpeculatIntensity(),0.01f,1.0f,"%.01f");
		ImGui::ColorEdit3("ambient color", (float*)&active_light.GetAmbientColor());
		ImGui::ColorEdit3("diffuse color", (float*)&active_light.GetDiffuseColor());
		ImGui::ColorEdit3("specular color", (float*)&active_light.GetSpecularColor());


	}
	else {
		if (ImGui::Button("New Light")) {
			scene.AddLight(std::make_shared<Light>());
			scene.GetEnableLighting() = true;
		}
	}
	ImGui::End();
}