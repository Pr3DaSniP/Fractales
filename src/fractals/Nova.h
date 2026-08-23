#pragma once

#include "core/Fractale.h"

class Nova : public Fractale
{
public:
    Nova();

    void render() override;
    void menu() override;
    std::pair<double, double> coordsForZoom() const override;

    bool supportsParameterAnimation() const override { return true; }
    void animateParameters(float t) override;

private:
    float m_relaxation = 1.0f;
};
