#pragma once

#include "core/Fractale.h"

class CelticMandelbrot : public Fractale
{
public:
    CelticMandelbrot();

    void render() override;
    void menu() override;
    std::pair<double, double> coordsForZoom() const override;
};
