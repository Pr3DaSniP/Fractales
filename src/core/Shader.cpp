#include "Shader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexPath);

    GLuint fragmentShader;
    try
    {
        fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentPath);
    }
    catch (...)
    {
        glDeleteShader(vertexShader);
        throw;
    }

    try
    {
        m_programID = linkProgram({vertexShader, fragmentShader});
    }
    catch (...)
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        throw;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    if (m_programID != 0)
    {
        glDeleteProgram(m_programID);
    }
}

Shader::Shader(Shader&& other) noexcept
    : m_programID(other.m_programID)
    , m_uniformCache(std::move(other.m_uniformCache))
    , m_attributeCache(std::move(other.m_attributeCache))
{
    other.m_programID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        if (m_programID != 0)
        {
            glDeleteProgram(m_programID);
        }

        m_programID = other.m_programID;
        m_uniformCache = std::move(other.m_uniformCache);
        m_attributeCache = std::move(other.m_attributeCache);

        other.m_programID = 0;
    }
    return *this;
}

void Shader::use() const
{
    glUseProgram(m_programID);
}

void Shader::unuse() const
{
    glUseProgram(0);
}

GLint Shader::uniformLocation(const std::string& name) const
{
    auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end())
    {
        return it->second;
    }

    GLint location = glGetUniformLocation(m_programID, name.c_str());
    m_uniformCache[name] = location;
    return location;
}

GLint Shader::attributeLocation(const std::string& name) const
{
    auto it = m_attributeCache.find(name);
    if (it != m_attributeCache.end())
    {
        return it->second;
    }

    GLint location = glGetAttribLocation(m_programID, name.c_str());
    m_attributeCache[name] = location;
    return location;
}

void Shader::setFloat(const std::string& name, float value)
{
    glUniform1f(uniformLocation(name), value);
}

void Shader::setInt(const std::string& name, int value)
{
    glUniform1i(uniformLocation(name), value);
}

void Shader::setBool(const std::string& name, bool value)
{
    glUniform1i(uniformLocation(name), static_cast<int>(value));
}

void Shader::setVec2(const std::string& name, float x, float y)
{
    glUniform2f(uniformLocation(name), x, y);
}

void Shader::setVec3(const std::string& name, float x, float y, float z)
{
    glUniform3f(uniformLocation(name), x, y, z);
}

void Shader::setVec3Array(const std::string& name, const std::vector<float>& values)
{
    GLsizei count = static_cast<GLsizei>(values.size() / 3);
    glUniform3fv(uniformLocation(name), count, values.data());
}

void Shader::setMat4(const std::string& name, const float* value)
{
    glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, value);
}

float Shader::getFloat(const std::string& name) const
{
    float value = 0.0f;
    glGetUniformfv(m_programID, uniformLocation(name), &value);
    return value;
}

void Shader::setDouble(const std::string& name, double value)
{
    glUniform1d(uniformLocation(name), value);
}

double Shader::getDouble(const std::string& name) const
{
    double value = 0.0;
    glGetUniformdv(m_programID, uniformLocation(name), &value);
    return value;
}

void Shader::setAttributePointer(const std::string& name, int size, int stride, int offset)
{
    GLint location = attributeLocation(name);
    glEnableVertexAttribArray(static_cast<GLuint>(location));
    glVertexAttribPointer(
        static_cast<GLuint>(location), size, GL_FLOAT, GL_FALSE,
        stride, reinterpret_cast<const void*>(static_cast<intptr_t>(offset)));
}

GLuint Shader::compileShader(GLenum type, const std::string& path)
{
    std::string source = readFile(path);
    const char* sourceCStr = source.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        glDeleteShader(shader);
        throw std::runtime_error(
            "Echec de compilation du shader '" + path + "': " + infoLog);
    }

    return shader;
}

GLuint Shader::linkProgram(const std::vector<GLuint>& shaders)
{
    GLuint program = glCreateProgram();
    for (GLuint shader : shaders)
    {
        glAttachShader(program, shader);
    }

    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        glDeleteProgram(program);
        throw std::runtime_error(std::string("Echec de link du programme: ") + infoLog);
    }

    for (GLuint shader : shaders)
    {
        glDetachShader(program, shader);
    }

    return program;
}

std::string Shader::readFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Impossible d'ouvrir le fichier shader: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
