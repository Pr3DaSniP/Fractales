#pragma once

#include "core/Fractale.h"

class Mandelbrot : public Fractale
{
public:
    Mandelbrot();

    void render() override;
    void menu() override;
    std::pair<double, double> coordsForZoom() const override;
};
