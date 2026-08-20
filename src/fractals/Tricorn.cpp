#include "Tricorn.h"

Tricorn::Tricorn()
{
    m_id = 3;
    m_iterations = 300;
    loadPaletteShaders("tricorns", "tricorn");
}

void Tricorn::render()
{
    shader().setBool("smooth_color", m_smooth);
    shader().setFloat("colorRange", m_colorRange);
    shader().setFloat("maxIter", static_cast<float>(m_iterations));
    shader().use();
}

void Tricorn::menu()
{
    Fractale::menu();
}

std::pair<float, float> Tricorn::coordsForZoom() const
{
    return std::make_pair(-1.762f, -0.02f);
}
