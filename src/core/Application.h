#pragma once

#include <string>

#include <GLFW/glfw3.h>

class Application
{
public:
    Application(GLFWwindow* window, const char* glslVersion, std::string title);
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    void beginFrame();

    void endFrame();

    void updateFPSCounter(double currentTime);

private:
    GLFWwindow* m_window;
    std::string m_title;

    double m_lastTime = 0.0;
    int m_frames = 0;
};
