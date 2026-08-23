#pragma once

#include "core/Fractale.h"

class BurningShip : public Fractale
{
public:
    BurningShip();

    void render() override;
    void menu() override;
    std::pair<double, double> coordsForZoom() const override;
};
