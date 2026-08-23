#pragma once

#include "core/Fractale.h"

class LambdaFractal : public Fractale
{
public:
    LambdaFractal();

    void render() override;
    void menu() override;
    std::pair<double, double> coordsForZoom() const override;
};
