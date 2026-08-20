#pragma once

#include "core/Fractale.h"

class Julia : public Fractale
{
public:
    Julia();

    void render() override;
    void menu() override;
    std::pair<float, float> coordsForZoom() const override;

private:
    float m_v1 = 0.311f;
    float m_v2 = -0.026f;
};
