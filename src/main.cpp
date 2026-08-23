#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/Application.h"
#include "core/Fractale.h"
#include "core/Screenshot.h"
#include "fractals/BurningShip.h"
#include "fractals/CelticMandelbrot.h"
#include "fractals/Julia.h"
#include "fractals/LambdaFractal.h"
#include "fractals/Mandelbrot.h"
#include "fractals/Multibrot.h"
#include "fractals/Newton.h"
#include "fractals/Nova.h"
#include "fractals/Phoenix.h"
#include "fractals/Tricorn.h"

int selectedFractal = 0;

namespace
{
    std::vector<std::unique_ptr<Fractale>> g_fractales;
    int g_screenWidth = 1600;
    int g_screenHeight = 900;
    double g_zoomFactor = 1.0;

    bool g_dragging = false;
    double g_lastCursorX = 0.0;
    double g_lastCursorY = 0.0;

    void resetRenderingShader()
    {
        g_fractales[selectedFractal]->setActivePalette(0);
        Shader& shader = g_fractales[selectedFractal]->shader();

        shader.setInt("maxIter", 40);
        shader.setDouble("zoom", 0.0);
        shader.setDouble("mouseX", 0.0);
        shader.setDouble("mouseY", 0.0);
        shader.setDouble("centerX", 0.0);
        shader.setDouble("centerY", 0.0);
        shader.setBool("infiniteZoom", false);
        shader.setDouble("zoomFactor", 1.0);
        shader.setInt("width", g_screenWidth);
        shader.setInt("height", g_screenHeight);

        g_zoomFactor = 1.0;
    }

    struct PaletteMenu
    {
        int selectedPalette = 0;

        void menu()
        {
            ImGui::NewLine();
            ImGui::Text("Colors:");
            ImGui::Combo(
                "##color_palette", &selectedPalette,
                "Original\0Fire\0Electric\0Gold\0Verdoyante\0Perle", 6);

            g_fractales[selectedFractal]->setActivePalette(selectedPalette);
        }
    };
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Erreur: impossible d'initialiser GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(g_screenWidth, g_screenHeight, "Fractales", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Erreur: impossible de creer la fenetre GLFW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Erreur: impossible de charger les fonctions OpenGL (glad)\n";
        return -1;
    }

    glViewport(0, 0, g_screenWidth, g_screenHeight);

    {
        float vertices[] = {
            //  x      y     z
            -1.0f, -1.0f, 0.0f,  // Bottom Left
             1.0f,  1.0f, 0.0f,  // Top Right
            -1.0f,  1.0f, 0.0f,  // Top Left
             1.0f, -1.0f, 0.0f,  // Bottom Right
        };
        unsigned int indices[] = { 0, 1, 2, 0, 3, 1 };

        GLuint vao, vbo, ebo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        g_fractales.push_back(std::make_unique<Mandelbrot>());
        g_fractales.push_back(std::make_unique<Julia>());
        g_fractales.push_back(std::make_unique<BurningShip>());
        g_fractales.push_back(std::make_unique<Tricorn>());
        g_fractales.push_back(std::make_unique<Multibrot>());
        g_fractales.push_back(std::make_unique<CelticMandelbrot>());
        g_fractales.push_back(std::make_unique<Phoenix>());
        g_fractales.push_back(std::make_unique<Nova>());
        g_fractales.push_back(std::make_unique<LambdaFractal>());
        g_fractales.push_back(std::make_unique<Newton>());

        Application app(window, "#version 410", "Fractales");
        PaletteMenu paletteMenu;
        std::string lastExportPath;

        glfwSetScrollCallback(window, [](GLFWwindow* w, double, double yoffset)
        {
            if (ImGui::GetIO().WantCaptureMouse)
                return;

            Shader& shader = g_fractales[selectedFractal]->shader();

            double oldZoom = shader.getDouble("zoom");
            double newZoom = oldZoom + yoffset * 0.1;

            int width, height;
            glfwGetFramebufferSize(w, &width, &height);

            double xpos, ypos;
            glfwGetCursorPos(w, &xpos, &ypos);

            double k = 2.5 / static_cast<double>(height);
            double baseX = k * (xpos - width / 2.0);
            double baseY = k * (height / 2.0 - ypos);

            double factor = 1.0 / (1.0 + oldZoom) - 1.0 / (1.0 + newZoom);
            double mouseX = shader.getDouble("mouseX") - baseX * factor;
            double mouseY = shader.getDouble("mouseY") + baseY * factor;

            shader.setDouble("zoom", newZoom);
            shader.setDouble("mouseX", mouseX);
            shader.setDouble("mouseY", mouseY);
        });

        glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int /*scancode*/, int action, int /*mods*/)
        {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(w, true);

            if (key == GLFW_KEY_R && action == GLFW_PRESS)
                resetRenderingShader();

            if (key == GLFW_KEY_W && action == GLFW_REPEAT)
            {
                std::pair<double, double> coords = g_fractales[selectedFractal]->coordsForZoom();
                Shader& shader = g_fractales[selectedFractal]->shader();

                shader.setBool("infiniteZoom", true);
                shader.setDouble("centerX", coords.first);
                shader.setDouble("centerY", coords.second);
                shader.setDouble("zoomFactor", g_zoomFactor);

                g_zoomFactor += 10.0;
            }
            else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
            {
                Shader& shader = g_fractales[selectedFractal]->shader();
                shader.setBool("infiniteZoom", false);
                shader.setDouble("zoomFactor", 0.0);
                g_zoomFactor = 1.0;
            }
        });

        while (!glfwWindowShouldClose(window))
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);

            Shader& activeShader = g_fractales[selectedFractal]->shader();
            activeShader.setInt("width", width);
            activeShader.setInt("height", height);

            if (!ImGui::GetIO().WantCaptureMouse &&
                glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);

                if (g_dragging)
                {
                    double deltaX = xpos - g_lastCursorX;
                    double deltaY = ypos - g_lastCursorY;
                    double scale = 2.5 / static_cast<double>(height) / (1.0 + activeShader.getDouble("zoom"));

                    activeShader.setDouble("mouseX", activeShader.getDouble("mouseX") + deltaX * scale);
                    activeShader.setDouble("mouseY", activeShader.getDouble("mouseY") + deltaY * scale);
                }

                g_lastCursorX = xpos;
                g_lastCursorY = ypos;
                g_dragging = true;
            }
            else
            {
                g_dragging = false;
            }

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);

            app.updateFPSCounter(glfwGetTime());

            app.beginFrame();
            g_fractales[selectedFractal]->menu();
            paletteMenu.menu();

            ImGui::NewLine();
            ImGui::Separator();
            if (ImGui::Button("Exporter en PNG (x4 resolution)"))
            {
                lastExportPath = exportFractalToPNG(
                    *g_fractales[selectedFractal], vao, width * 4, height * 4);
            }
            if (!lastExportPath.empty())
            {
                ImGui::Text("Exporte : %s", lastExportPath.c_str());
            }

            g_fractales[selectedFractal]->render();

            app.endFrame();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);

        g_fractales.clear();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
