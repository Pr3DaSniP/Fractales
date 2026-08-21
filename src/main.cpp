#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "core/Application.h"
#include "core/Fractale.h"
#include "fractals/BurningShip.h"
#include "fractals/Julia.h"
#include "fractals/Mandelbrot.h"
#include "fractals/Multibrot.h"
#include "fractals/Tricorn.h"

int selectedFractal = 0;

namespace
{
    std::vector<std::unique_ptr<Fractale>> g_fractales;
    int g_screenWidth = 1600;
    int g_screenHeight = 900;
    double g_zoomFactor = 1.0;

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

        Application app(window, "#version 410", "Fractales");
        PaletteMenu paletteMenu;

        glfwSetScrollCallback(window, [](GLFWwindow*, double, double yoffset)
        {
            Shader& shader = g_fractales[selectedFractal]->shader();
            shader.setDouble("zoom", shader.getDouble("zoom") + yoffset * 0.1);
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

                double x = (xpos / width) * 4.0 - 2.0;
                double y = (ypos / height) * 2.0 - 1.0;

                activeShader.setDouble("mouseX", x);
                activeShader.setDouble("mouseY", y);
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
