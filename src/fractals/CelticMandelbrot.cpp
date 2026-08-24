#include "CelticMandelbrot.h"

CelticMandelbrot::CelticMandelbrot()
{
    m_id = 5;
    loadShader("celtic");
}

void CelticMandelbrot::render()
{
    shader().use();
    applyPalette();
    shader().setFloat("maxIter", static_cast<float>(m_iterations));
    shader().setBool("smooth_color", m_smooth);
    shader().setFloat("colorRange", m_colorRange);
}

void CelticMandelbrot::menu()
{
    Fractale::menu();
}

std::pair<double, double> CelticMandelbrot::coordsForZoom() const
{
    return std::make_pair(-1.0, 0.15);
}
