#include "Newton.h"

Newton::Newton()
{
    m_id = 9;
    m_iterations = 40;
    loadShader("newton");
}

void Newton::render()
{
    shader().use();
    applyPalette();
    shader().setFloat("maxIter", static_cast<float>(m_iterations));
}

void Newton::menu()
{
    Fractale::menu();
}

std::pair<double, double> Newton::coordsForZoom() const
{
    return std::make_pair(0.0, 0.0);
}
