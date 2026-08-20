#include "Fractale.h"

#include <imgui.h>

#include "core/Palette.h"

void Fractale::loadShader(const std::string& name)
{
    std::string fragmentPath = "shaders/" + name + ".fs";
    m_shader = std::make_unique<Shader>(kVertexShaderPath, fragmentPath);
}

void Fractale::applyPalette()
{
    const Palette& palette = allPalettes().at(m_activePalette);
    m_shader->setVec3Array("palette", palette.colors);
    m_shader->setInt("paletteSize", palette.colorCount());
}

void Fractale::menu()
{
    ImGui::Columns(3, "Fractals");
    ImGui::RadioButton("Mandelbrot", &selectedFractal, 0);
    ImGui::NextColumn();
    ImGui::RadioButton("Julia", &selectedFractal, 1);
    ImGui::NextColumn();
    ImGui::RadioButton("Burning Ship", &selectedFractal, 2);
    ImGui::NextColumn();
    ImGui::RadioButton("Tricorn", &selectedFractal, 3);
    ImGui::NextColumn();
    ImGui::RadioButton("Multibrot", &selectedFractal, 4);
    ImGui::Columns(1);

    ImGui::NewLine();
    ImGui::SliderInt("Iterations", &m_iterations, 20, 5000);

    ImGui::NewLine();
    ImGui::SliderFloat("Color Range", &m_colorRange, 0.01f, 10.0f);

    ImGui::Separator();
    ImGui::Checkbox("Smooth", &m_smooth);
}
