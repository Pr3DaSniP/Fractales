#include "Multibrot.h"

#include <imgui.h>

Multibrot::Multibrot()
{
    m_id = 4;
    m_iterations = 30;
    loadShader("multibrot");
}

void Multibrot::render()
{
    shader().use();
    applyPalette();
    shader().setBool("smooth_color", m_smooth);
    shader().setFloat("colorRange", m_colorRange);
    shader().setFloat("maxIter", static_cast<float>(m_iterations));
    shader().setInt("numberOfBrot", m_numberOfBrot);
}

void Multibrot::menu()
{
    Fractale::menu();
    ImGui::NewLine();
    ImGui::Text("Multibrot");
    ImGui::SliderInt("Number of brot", &m_numberOfBrot, 1, 10);
}

std::pair<double, double> Multibrot::coordsForZoom() const
{
    return std::make_pair(0.0f, 0.0f);
}

void Multibrot::animateParameters(float t)
{
    float phase = t < 0.5f ? t * 2.0f : (1.0f - t) * 2.0f;
    m_numberOfBrot = 2 + static_cast<int>(phase * 6.0f);
}
