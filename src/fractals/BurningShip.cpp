#include "BurningShip.h"

BurningShip::BurningShip()
{
    m_id = 2;
    m_iterations = 300;
    loadShader("burningship");
}

void BurningShip::render()
{
    shader().use();
    applyPalette();
    shader().setBool("smooth_color", m_smooth);
    shader().setFloat("colorRange", m_colorRange);
    shader().setFloat("maxIter", static_cast<float>(m_iterations));
}

void BurningShip::menu()
{
    Fractale::menu();
}

std::pair<double, double> BurningShip::coordsForZoom() const
{
    return std::make_pair(-1.762f, -0.02f);
}
