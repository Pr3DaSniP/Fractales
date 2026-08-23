#include "Mandelbrot.h"

Mandelbrot::Mandelbrot()
{
    m_id = 0;
    loadShader("mandelbrot");
}

void Mandelbrot::render()
{
    applyPalette();
    shader().setFloat("maxIter", static_cast<float>(m_iterations));
    shader().setBool("smooth_color", m_smooth);
    shader().setFloat("colorRange", m_colorRange);
    shader().use();
}

void Mandelbrot::menu()
{
    Fractale::menu();
}

std::pair<double, double> Mandelbrot::coordsForZoom() const
{
    return std::make_pair(0.4f, 0.35f);
}
