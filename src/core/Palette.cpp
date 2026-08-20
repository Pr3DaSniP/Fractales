#include "Palette.h"

namespace
{
    constexpr float kInv255 = 1.0f / 255.0f;

    std::vector<float> makeColors(std::initializer_list<float> rgb255)
    {
        std::vector<float> result;
        result.reserve(rgb255.size());
        for (float channel : rgb255)
        {
            result.push_back(channel * kInv255);
        }
        return result;
    }
}

const std::vector<Palette>& allPalettes()
{
    static const std::vector<Palette> palettes = {
        {
            "Original",
            makeColors({
                0.f, 7.f, 100.f,
                32.f, 107.f, 203.f,
                237.f, 255.f, 255.f,
                255.f, 170.f, 0.f,
                0.f, 2.f, 0.f,
                0.f, 7.f, 100.f,
            }),
        },
        {
            "Fire",
            makeColors({
                20.f, 0.f, 0.f,
                255.f, 20.f, 0.f,
                255.f, 200.f, 0.f,
                255.f, 20.f, 0.f,
                20.f, 0.f, 0.f,
            }),
        },
        {
            "Electric",
            makeColors({
                0.f, 0.f, 0.f,
                0.f, 0.f, 200.f,
                255.f, 255.f, 255.f,
                0.f, 0.f, 200.f,
                0.f, 0.f, 0.f,
            }),
        },
        {
            "Gold",
            makeColors({
                85.f, 47.f, 0.f,
                255.f, 171.f, 12.f,
                255.f, 247.f, 127.f,
                255.f, 171.f, 12.f,
                85.f, 47.f, 0.f,
            }),
        },
        {
            "Verdoyante",
            makeColors({
                0.f, 0.f, 0.f,
                124.f, 254.f, 240.f,
                107.f, 255.f, 184.f,
                44.f, 234.f, 163.f,
                40.f, 150.f, 90.f,
            }),
        },
        {
            "Perle",
            makeColors({
                10.f, 9.f, 12.f,
                240.f, 237.f, 238.f,
                7.f, 57.f, 60.f,
                44.f, 102.f, 110.f,
                144.f, 221.f, 240.f,
                10.f, 9.f, 12.f,
            }),
        },
    };
    return palettes;
}
