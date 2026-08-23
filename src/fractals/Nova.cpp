#include "Nova.h"

#include <imgui.h>

Nova::Nova()
{
    m_id = 7;
    m_iterations = 60;
    loadShader("nova");
}

void Nova::render()
{
    applyPalette();
    shader().setFloat("maxIter", static_cast<float>(m_iterations));
    shader().setBool("smooth_color", m_smooth);
    shader().setFloat("colorRange", m_colorRange);
    shader().setFloat("relaxation", m_relaxation);
    shader().use();
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
