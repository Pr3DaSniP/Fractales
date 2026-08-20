#pragma once

#include <glad/gl.h>

#include <string>
#include <unordered_map>
#include <vector>

// Représente un programme shader OpenGL (vertex + fragment) lié.
//
// RAII : le constructeur compile et link le programme, le destructeur le
// libère. La classe est move-only (pas de copie possible) pour garantir
// qu'un seul objet C++ ne soit jamais responsable de la libération d'un
// m_programID donné -> pas de double free / double glDeleteProgram.
class Shader
{
public:
    // Compile et link un programme à partir de deux fichiers sources.
    // Lance std::runtime_error en cas d'échec de compilation ou de link.
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    ~Shader();

    // Non copyable : un GLuint de programme ne doit avoir qu'un seul
    // propriétaire C++ à la fois.
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // Move-only : transfère la propriété du programme OpenGL.
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

    // Envoie un tableau de vec3 contigu (ex: une palette de couleurs) en un
    // seul appel. "values" doit contenir 3 floats par couleur (r,g,b,r,g,b,...).
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
