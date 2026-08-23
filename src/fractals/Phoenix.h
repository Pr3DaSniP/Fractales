#pragma once

#include "core/Fractale.h"

class Phoenix : public Fractale
{
public:
    Phoenix();

    void render() override;
    void menu() override;
    std::pair<double, double> coordsForZoom() const override;

private:
    float m_v1 = 0.5667f;
    float m_v2 = 0.0f;
    float m_p = -0.5f;
};
