#pragma once

#include <glad/gl.h>

#include <string>
#include <unordered_map>
#include <vector>

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void use() const;
    void unuse() const;

    GLuint programID() const { return m_programID; }

    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    void setBool(const std::string& name, bool value);
    void setVec2(const std::string& name, float x, float y);
    void setVec3(const std::string& name, float x, float y, float z);

    void setDouble(const std::string& name, double value);
    double getDouble(const std::string& name) const;

    void setVec3Array(const std::string& name, const std::vector<float>& values);

    void setMat4(const std::string& name, const float* value);

    float getFloat(const std::string& name) const;

    void setAttributePointer(const std::string& name, int size, int stride, int offset);

private:
    GLuint m_programID = 0;
    mutable std::unordered_map<std::string, GLint> m_uniformCache;
    mutable std::unordered_map<std::string, GLint> m_attributeCache;

    GLint uniformLocation(const std::string& name) const;
    GLint attributeLocation(const std::string& name) const;

    static GLuint compileShader(GLenum type, const std::string& path);
    static GLuint linkProgram(const std::vector<GLuint>& shaders);
    static std::string readFile(const std::string& path);
};
