#include "Multibrot.h"

#include <imgui.h>

Multibrot::Multibrot()
{
    m_id = 4;
    m_iterations = 30;
    loadPaletteShaders("multibrots", "multibrot");
}

void Multibrot::render()
{
    shader().setBool("smooth_color", m_smooth);
    shader().setFloat("colorRange", m_colorRange);
    shader().setFloat("maxIter", static_cast<float>(m_iterations));
    shader().setInt("numberOfBrot", m_numberOfBrot);
    shader().use();
}

void Multibrot::menu()
{
    Fractale::menu();
    ImGui::NewLine();
    ImGui::Text("Multibrot");
    ImGui::SliderInt("Number of brot", &m_numberOfBrot, 1, 10);
}

std::pair<float, float> Multibrot::coordsForZoom() const
{
    return std::make_pair(0.0f, 0.0f);
}
