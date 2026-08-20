#include "Fractale.h"

#include <array>
#include <imgui.h>

void Fractale::loadPaletteShaders(const std::string& shaderFolder, const std::string& namePrefix)
{
    static constexpr std::array<const char*, 6> kPaletteNames = {
        "Original", "Fire", "Electric", "Gold", "Verdoyante", "Perle"
    };

    m_shaders.reserve(kPaletteNames.size());
    for (const char* palette : kPaletteNames)
    {
        std::string fragmentPath =
            "shaders/" + shaderFolder + "/" + namePrefix + "_" + palette + ".fs";
        m_shaders.push_back(std::make_unique<Shader>(kVertexShaderPath, fragmentPath));
    }
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
