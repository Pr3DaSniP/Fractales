#pragma once

#include "core/Fractale.h"

class Multibrot : public Fractale
{
public:
    Multibrot();

    void render() override;
    void menu() override;
    std::pair<float, float> coordsForZoom() const override;

private:
    int m_numberOfBrot = 4;
};
