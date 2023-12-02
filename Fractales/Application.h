#pragma once

#include <iostream>
#include <string>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Application
{
	int width, height;
	const char* title;

	double lastTime = 0.0f;
	double deltaTime = 0.0f;
	int frames = 0;
public:
	Application(int width, int height, const char* title);
	void Init(GLFWwindow* w, const char* version);
	void addFPSCounter(GLFWwindow* w, double crtTime);
	virtual void Update();
	void Render();
	void Destroy();
	void NewFrame();
};