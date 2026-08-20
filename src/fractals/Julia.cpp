#include "Julia.h"

#include <imgui.h>

Julia::Julia()
{
    m_id = 1;
    m_iterations = 150;
    loadPaletteShaders("julias", "julia");
}

void Julia::render()
{
    shader().setFloat("maxIter", static_cast<float>(m_iterations));
    shader().setBool("smooth_color", m_smooth);
    shader().setFloat("colorRange", m_colorRange);
    shader().setFloat("v1", m_v1);
    shader().setFloat("v2", m_v2);
    shader().use();
}

void Julia::menu()
{
    Fractale::menu();
    ImGui::NewLine();
    ImGui::Text("Julia");
    ImGui::SliderFloat("v1", &m_v1, -2.f, 2.f);
    ImGui::SliderFloat("v2", &m_v2, -2.f, 2.f);
}

std::pair<float, float> Julia::coordsForZoom() const
{
    return std::make_pair(-0.0175f, -0.453333f);
}
