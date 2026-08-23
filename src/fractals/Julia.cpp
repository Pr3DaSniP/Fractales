#include "Julia.h"

#include <cmath>

#include <imgui.h>

Julia::Julia()
{
    m_id = 1;
    m_iterations = 150;
    loadShader("julia");
}

void Julia::render()
{
    applyPalette();
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

std::pair<double, double> Julia::coordsForZoom() const
{
    return std::make_pair(-0.0175f, -0.453333f);
}

void Julia::animateParameters(float t)
{
    float angle = t * 2.0f * 3.14159265f;
    float radius = 0.7885f;
    m_v1 = radius * std::cos(angle);
    m_v2 = radius * std::sin(angle);
}
