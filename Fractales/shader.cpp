#include "shader.h"

Shader::Shader(string vertexFilename, string fragmentFilename)
{
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexFilename.c_str());
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFilename.c_str());
    GLuint program = createProgram({vertexShader, fragmentShader});
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    m_programID = program;
}

Shader::~Shader()
{
    glDeleteProgram(m_programID);
}

void Shader::use()
{
    glUseProgram(m_programID);
}

void Shader::unuse()
{
    glUseProgram(0);
}

GLuint Shader::getProgramID()
{
    return m_programID;
}

GLint Shader::getUniform(string name)
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = glGetUniformLocation(m_programID, name.c_str());
    }
    return m_uniforms[name];
}

GLint Shader::getAttribut(string name)
{
    if (m_attributs.find(name) == m_attributs.end())
    {
        m_attributs[name] = glGetAttribLocation(m_programID, name.c_str());
    }
    return m_attributs[name];
}

void Shader::setAttributePointer(string name, int size, int stride, int offset)
{
    glEnableVertexAttribArray(getAttribut(name));
    glVertexAttribPointer(getAttribut(name), size, GL_FLOAT, GL_FALSE, stride, (void *)offset);
}

void Shader::setUniformMVP(string name, mat4x4 value)
{
    glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, (const GLfloat *)value);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
};

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
};

void Shader::setBool(const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(m_programID, name.c_str()), (int)value);
};

GLuint Shader::createProgram(const vector<GLuint> &shaders)
{
    // On crée le programme
    GLuint program = glCreateProgram();
    // On attache les shaders
    for (auto shader : shaders)
    {
        glAttachShader(program, shader);
    }
    // On link le programme
    glLinkProgram(program);
    // On vérifie si le programme a bien été linké
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
        throw exception(infoLog);
    }
    // On detache les shaders
    for (auto shader : shaders)
    {
        glDetachShader(program, shader);
    }
    return program;
}

GLuint Shader::loadShader(GLenum type, const char *filename)
{
    // On charge le code source du shader
    string source = loadSource(filename);
    // On crée le shader
    GLuint shader = glCreateShader(type);
    const char *c_str = source.c_str();
    glShaderSource(shader, 1, &c_str, NULL);
    glCompileShader(shader);
    // On vérifie si le shader a bien été compilé
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
        throw exception(infoLog);
    }
    return shader;
}

string Shader::loadSource(const char *filename)
{
    // On ouvre le fichier (c++)
    ifstream file(filename);
    if (!file.is_open())
    {
        throw exception("Impossible d'ouvrir le fichier");
    }
    // On récupère ligne par ligne le contenu du fichier
    string line;
    string source;
    while (getline(file, line))
    {
        source += line + "\n";
    }
    // On retourne le contenu du fichier
    return source;
}

void Shader::setVec2(const std::string& name, float x, float y) {
    glUniform2f(glGetUniformLocation(m_programID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(m_programID, name.c_str()), x, y, z);
}

float Shader::getFloat(const std::string& name) {
	float value;
	glGetUniformfv(m_programID, glGetUniformLocation(m_programID, name.c_str()), &value);
	return value;
};