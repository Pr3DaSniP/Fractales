#pragma once

#include "core/Fractale.h"

class Newton : public Fractale
{
public:
    Newton();

    void render() override;
    void menu() override;
    std::pair<double, double> coordsForZoom() const override;
};
