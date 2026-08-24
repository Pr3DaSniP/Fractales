#include "Tricorn.h"

Tricorn::Tricorn()
{
    m_id = 3;
    m_iterations = 300;
    loadShader("tricorn");
}

void Tricorn::render()
{
    shader().use();
    applyPalette();
    shader().setBool("smooth_color", m_smooth);
    shader().setFloat("colorRange", m_colorRange);
    shader().setFloat("maxIter", static_cast<float>(m_iterations));
}

void Tricorn::menu()
{
    Fractale::menu();
}

std::pair<double, double> Tricorn::coordsForZoom() const
{
    return std::make_pair(-1.762f, -0.02f);
}
