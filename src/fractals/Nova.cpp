#include "Nova.h"

#include <cmath>

#include <imgui.h>

Nova::Nova()
{
    m_id = 7;
    m_iterations = 60;
    loadShader("nova");
}

void Nova::render()
{
    shader().use();
    applyPalette();
    shader().setFloat("maxIter", static_cast<float>(m_iterations));
    shader().setBool("smooth_color", m_smooth);
    shader().setFloat("colorRange", m_colorRange);
    shader().setFloat("relaxation", m_relaxation);
}

void Nova::menu()
{
    Fractale::menu();
    ImGui::NewLine();
    ImGui::Text("Nova");
    ImGui::SliderFloat("Relaxation", &m_relaxation, 0.1f, 2.0f);
}

std::pair<double, double> Nova::coordsForZoom() const
{
    return std::make_pair(0.0, 0.0);
}

void Nova::animateParameters(float t)
{
    m_relaxation = 1.0f + 0.5f * std::sin(t * 2.0f * 3.14159265f);
}
