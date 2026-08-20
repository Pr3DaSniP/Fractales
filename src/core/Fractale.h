#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/Shader.h"

// Index de la fractale sélectionnée dans le menu ImGui (0 = Mandelbrot,
// 1 = Julia, etc.). Partagé entre Fractale::menu() (boutons radio) et
// la boucle principale (quelle fractale rendre).
extern int selectedFractal;

// Classe de base pour toutes les fractales.
class Fractale
{
public:
    virtual ~Fractale() = default;

    void setActiveShader(int id) { m_activeShader = id; }

    virtual void render() = 0;

    virtual void menu();

    Shader& shader() { return *m_shaders.at(m_activeShader); }

    virtual std::pair<float, float> coordsForZoom() const = 0;

protected:
    static constexpr const char* kVertexShaderPath = "shaders/vertex.vs";

    void loadPaletteShaders(const std::string& shaderFolder, const std::string& namePrefix);

    std::vector<std::unique_ptr<Shader>> m_shaders;
    int m_activeShader = 0;

    bool m_smooth = true;
    int m_iterations = 40;
    float m_colorRange = 0.017f;
    int m_id = 0;
};
