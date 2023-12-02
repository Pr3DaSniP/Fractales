#include "Application.h"

Application::Application(int width, int height, const char* title)
{
	this->width = width;
	this->height = height;
	this->title = title;
}

void Application::Init(GLFWwindow* w, const char* version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(w, true);
	ImGui_ImplOpenGL3_Init(version);
	ImGui::StyleColorsDark();
}

void Application::addFPSCounter(GLFWwindow* w, double crtTime)
{
	std::string FPS;
	deltaTime = crtTime - lastTime;
	frames++;
	if (deltaTime >= 1.0 / 30.0)
	{
		FPS = std::to_string((1.0 / deltaTime) * frames);
		std::string newTitle = "Fractal - FPS: " + FPS;
		glfwSetWindowTitle(w, newTitle.c_str());
		lastTime = crtTime;
		frames = 0;
	}
}

void Application::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin(title);
}

void Application::Update()
{	
	ImGui::Separator();

	// Commandes
	ImGui::Text("Commands:");
	ImGui::Text("Left clic : Move the fractal");
	ImGui::Text("Scroll : Zoom in/out");
	ImGui::Text("Keep \"Z\" pressed : Zoom on a specific point");
	ImGui::Text("\"R\": Reset the fractal");
}

void Application::Render()
{
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}