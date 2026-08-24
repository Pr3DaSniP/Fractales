#include "Phoenix.h"

#include <cmath>

#include <imgui.h>

Phoenix::Phoenix()
{
    m_id = 6;
    m_iterations = 150;
    loadShader("phoenix");
}

void Phoenix::render()
{
    shader().use();
    applyPalette();
    shader().setFloat("maxIter", static_cast<float>(m_iterations));
    shader().setBool("smooth_color", m_smooth);
    shader().setFloat("colorRange", m_colorRange);
    shader().setFloat("v1", m_v1);
    shader().setFloat("v2", m_v2);
    shader().setFloat("phoenixP", m_p);
}

void Phoenix::menu()
{
    Fractale::menu();
    ImGui::NewLine();
    ImGui::Text("Phoenix");
    ImGui::SliderFloat("v1", &m_v1, -2.f, 2.f);
    ImGui::SliderFloat("v2", &m_v2, -2.f, 2.f);
    ImGui::SliderFloat("p", &m_p, -1.f, 1.f);
}

std::pair<double, double> Phoenix::coordsForZoom() const
{
    return std::make_pair(0.0, 0.0);
}

void Phoenix::animateParameters(float t)
{
    float angle = t * 2.0f * 3.14159265f;
    m_v1 = 0.5667f * std::cos(angle);
    m_v2 = 0.5667f * std::sin(angle);
    m_p = -0.5f + 0.3f * std::sin(angle);
}
