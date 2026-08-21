#pragma once

#include <array>
#include <string>
#include <vector>

struct Palette
{
    std::string name;
    std::vector<float> colors;

    int colorCount() const { return static_cast<int>(colors.size() / 3); }
};

const std::vector<Palette>& allPalettes();
