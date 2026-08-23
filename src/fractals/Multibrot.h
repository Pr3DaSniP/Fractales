#pragma once

#include "core/Fractale.h"

class Multibrot : public Fractale
{
public:
    Multibrot();

    void render() override;
    void menu() override;
    std::pair<double, double> coordsForZoom() const override;

    bool supportsParameterAnimation() const override { return true; }
    void animateParameters(float t) override;

private:
    int m_numberOfBrot = 4;
};
