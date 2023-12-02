#include <imgui.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "linmath.h"

#include <iostream>
#include <string>

#include "Application.h"
#include "Fractale.h"

int selectedFractal = 0;

std::vector<Fractale*> fractales;

int screenWidth = 1600;
int screenHeight = 900;

float zoomFactor = 1.f;

struct Colors {
	int selectedPalette = 0;
	void Menu() {
		ImGui::NewLine();
		ImGui::Text("Colors:");
		ImGui::Combo("##color_pallet", &selectedPalette,
			"Original\0Fire\0Electric\0Gold\0Verdoyante\0Perle", 6);

		fractales[selectedFractal]->setActiveShader(selectedPalette);
	}
};

void resetRenderingShader() {
	int width, height;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	fractales[selectedFractal]->setActiveShader(0);
	fractales[selectedFractal]->getShader()->setInt("maxIter", 40);
	fractales[selectedFractal]->getShader()->setFloat("zoom", 0.f);
	fractales[selectedFractal]->getShader()->setFloat("mouseX", 0.f);
	fractales[selectedFractal]->getShader()->setFloat("mouseY", 0.f);
	fractales[selectedFractal]->getShader()->setFloat("centerX", 0.f);
	fractales[selectedFractal]->getShader()->setFloat("centerY", 0.f);
	fractales[selectedFractal]->getShader()->setFloat("infiniteZoom", false);
	fractales[selectedFractal]->getShader()->setFloat("zoomFactor", 1.f);
	fractales[selectedFractal]->getShader()->setInt("width", width);
	fractales[selectedFractal]->getShader()->setInt("height", height);
	zoomFactor = 1.f;
}

int main()
{
	if (!glfwInit())
		return -1;

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Fractales", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return -1;

	glViewport(0, 0, screenWidth, screenHeight);

	const char* glsl_version = "#version 330";

	float vertices[] =
	{
		//    x      y      z   
			-1.0f, -1.0f, -0.0f,	// Bottom Left 
			 1.0f,  1.0f, -0.0f,	// Top Right
			-1.0f,  1.0f, -0.0f,	// Top Left
			 1.0f, -1.0f, -0.0f		// Bottom Right
	};

	unsigned int indices[] =
	{
			0, 1, 2,
			0, 3, 1
	};
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	fractales.push_back(new Mandelbrot());
	fractales.push_back(new Julia());
	fractales.push_back(new BurningShip());
	fractales.push_back(new Tricorn());
	fractales.push_back(new Multibrot());

	Application app{ screenWidth, screenHeight, "Fractales" };
	app.Init(window, glsl_version);

	Colors colors;

	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
	{
		fractales[selectedFractal]->getShader()->setFloat(
			"zoom", 
			fractales[selectedFractal]->getShader()->getFloat("zoom") + yoffset * 0.1f
		);
	});

	// Si on appuie sur une touche du clavier
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (key == GLFW_KEY_R && action == GLFW_PRESS)
			resetRenderingShader();

		if (key == GLFW_KEY_W && action == GLFW_REPEAT)
		{
			// Get mouse position
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			float x = (xpos / screenWidth) * 4 - 2;
			float y = (ypos / screenHeight) * 2 - 1;

			double xd = (xpos / screenWidth) * 4 - 2;

			std::pair<float, float> coords = fractales[selectedFractal]->getCoordsForZoom();

			fractales[selectedFractal]->getShader()->setFloat("infiniteZoom", true);
			fractales[selectedFractal]->getShader()->setFloat("centerX", coords.first);
			fractales[selectedFractal]->getShader()->setFloat("centerY", coords.second);

			fractales[selectedFractal]->getShader()->setFloat("zoomFactor", zoomFactor);

			zoomFactor += 10.f;
		}
		else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			fractales[selectedFractal]->getShader()->setFloat("infiniteZoom", false);
			fractales[selectedFractal]->getShader()->setFloat("zoomFactor", 0.f);
			zoomFactor = 1.f;
		}
	});

	while (!glfwWindowShouldClose(window))
	{
		{
			// Resize window
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			fractales[selectedFractal]->getShader()->setInt("width", display_w);
			fractales[selectedFractal]->getShader()->setInt("height", display_h);
			glClearColor(0.0f, 0.0f, 0.0f, 1.f);
		}

		// Si on reste appuyé sur le bouton gauche de la souris, on récupère la position de la souris
		if (!ImGui::GetIO().WantCaptureMouse)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureMouse)
				break;

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				float x = (xpos / screenWidth) * 4 - 2;
				float y = (ypos / screenHeight) * 2 - 1;

				fractales[selectedFractal]->getShader()->setFloat("mouseX", x);
				fractales[selectedFractal]->getShader()->setFloat("mouseY", y);
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		app.addFPSCounter(window, glfwGetTime());

		app.NewFrame();
		fractales[selectedFractal]->Menu();
		colors.Menu();

		fractales[selectedFractal]->Render();

		app.Update();
		app.Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	for (auto fractale : fractales) {
		fractale->getShader()->unuse();
		delete fractale;
	}

	app.Destroy();

	return 0;
}