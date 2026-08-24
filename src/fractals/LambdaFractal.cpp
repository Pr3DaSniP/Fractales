#include "LambdaFractal.h"

LambdaFractal::LambdaFractal()
{
    m_id = 8;
    loadShader("lambda");
}

void LambdaFractal::render()
{
    shader().use();
    applyPalette();
    shader().setFloat("maxIter", static_cast<float>(m_iterations));
    shader().setBool("smooth_color", m_smooth);
    shader().setFloat("colorRange", m_colorRange);
}

void LambdaFractal::menu()
{
    Fractale::menu();
}

std::pair<double, double> LambdaFractal::coordsForZoom() const
{
    return std::make_pair(0.0, 0.0);
}
