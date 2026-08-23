#pragma once

#include <memory>
#include <string>
#include <utility>

#include "core/Shader.h"

extern int selectedFractal;

class Fractale
{
public:
    virtual ~Fractale() = default;

    void setActivePalette(int id) { m_activePalette = id; }

    virtual void render() = 0;

    virtual void menu();

    Shader& shader() { return *m_shader; }

    virtual std::pair<double, double> coordsForZoom() const = 0;

    virtual bool supportsParameterAnimation() const { return false; }

    virtual void animateParameters(float /*t*/) {}

protected:
    static constexpr const char* kVertexShaderPath = "shaders/vertex.vs";

    void loadShader(const std::string& name);

    void applyPalette();

    std::unique_ptr<Shader> m_shader;
    int m_activePalette = 0;

    bool m_smooth = true;
    int m_iterations = 40;
    float m_colorRange = 0.017f;
    int m_id = 0;
};
