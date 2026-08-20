#pragma once

#include "core/Fractale.h"

class Tricorn : public Fractale
{
public:
    Tricorn();

    void render() override;
    void menu() override;
    std::pair<float, float> coordsForZoom() const override;
};
