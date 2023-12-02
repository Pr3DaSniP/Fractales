#pragma once

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>

#include "linmath.h"

using std::exception;
using std::map;
using std::string;
using std::vector;
using std::ifstream;

class Shader
{
private:
    GLuint m_programID;
    map<string, GLuint> m_uniforms;
    map<string, GLuint> m_attributs;

    GLuint createProgram(const vector<GLuint> &shaders);

    GLuint loadShader(GLenum type, const char *filename);

    string loadSource(const char *filename);

public:
    Shader(string vertexFilename, string fragmentFilename);

    ~Shader();

    void use();

    void unuse();

    GLuint getProgramID();

    GLint getUniform(string name);

    GLint getAttribut(string name);

    float getFloat(const std::string& name);

    void setAttributePointer(string name, int size, int stride, int offset);

    void setUniformMVP(string name, mat4x4 value);

    void setFloat(const std::string& name, float value);

    void setInt(const std::string& name, int value);

    void setBool(const std::string& name, bool value);

    void setVec2(const std::string& name, float x, float y);

    void setVec3(const std::string& name, float x, float y, float z);
};