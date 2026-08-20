#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#include "fractals/Mandelbrot.h"

// Définition de l'extern déclaré dans core/Fractale.h.
int selectedFractal = 0;

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Erreur: impossible d'initialiser GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Fractales - squelette CMake", nullptr, nullptr);
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

    std::cout << "OpenGL charge : " << glGetString(GL_VERSION) << "\n";

    auto fractale = std::make_unique<Mandelbrot>();

    float quadVertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f,
    };

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        fractale->render();
        fractale->shader().setInt("width", width);
        fractale->shader().setInt("height", height);
        fractale->shader().setFloat("zoom", 0.0f);
        fractale->shader().setFloat("mouseX", 0.0f);
        fractale->shader().setFloat("mouseY", 0.0f);
        fractale->shader().setBool("infiniteZoom", false);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
