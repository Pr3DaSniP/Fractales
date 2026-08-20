#pragma once

#include <array>
#include <string>
#include <vector>

// Une palette de couleurs pour la coloration des fractales. "colors" est un
// vecteur de floats (r,g,b,r,g,b,...) prêt à être envoyé tel quel à
// Shader::setVec3Array.
struct Palette
{
    std::string name;
    std::vector<float> colors;

    int colorCount() const { return static_cast<int>(colors.size() / 3); }
};

// Retourne les 6 palettes disponibles, dans l'ordre attendu par le combo
// ImGui ("Original\0Fire\0Electric\0Gold\0Verdoyante\0Perle").
// Valeurs reprises telles quelles des anciens fichiers shaders/*_<Palette>.fs
// (aucune couleur n'a été modifiée pendant la migration).
const std::vector<Palette>& allPalettes();
