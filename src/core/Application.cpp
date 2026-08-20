#include "Application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Application::Application(GLFWwindow* window, const char* glslVersion, std::string title)
    : m_window(window)
    , m_title(std::move(title))
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
    ImGui::StyleColorsDark();
}

Application::~Application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::updateFPSCounter(double currentTime)
{
    double deltaTime = currentTime - m_lastTime;
    m_frames++;

    if (deltaTime >= 1.0 / 30.0)
    {
        double fps = (1.0 / deltaTime) * m_frames;
        std::string newTitle = m_title + " - FPS: " + std::to_string(fps);
        glfwSetWindowTitle(m_window, newTitle.c_str());

        m_lastTime = currentTime;
        m_frames = 0;
    }
}

void Application::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin(m_title.c_str());
}

void Application::endFrame()
{
    ImGui::Separator();
    ImGui::Text("Commands:");
    ImGui::Text("Left clic : Move the fractal");
    ImGui::Text("Scroll : Zoom in/out");
    ImGui::Text("Keep \"Z\" pressed : Zoom on a specific point");
    ImGui::Text("\"R\": Reset the fractal");

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
