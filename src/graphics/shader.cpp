#include "shader.h"
#include <spdlog/spdlog.h>

Shader::Shader()
{}

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
    generate(vertexShaderPath, fragmentShaderPath);
}

void Shader::generate(const char *vertexShaderPath, const char *fragmentShaderPath)
{
    int success;
    char infoLog[1024];

    GLuint vertexShader   = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 1024, nullptr, infoLog);
        std::cout << "Linking error: \n"
                  << infoLog << std::endl;
        throw std::runtime_error("Program Linking Error");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::activate()
{
    glUseProgram(id);
}

// utility function
std::string Shader::loadShaderSrc(const char *filepath)
{
    std::ifstream file;
    std::stringstream buf;

    std::string ret = "";

    file.open(filepath);

    if (file.is_open())
    {
        buf << file.rdbuf();
        ret = buf.str();
    }
    else
    {
        spdlog::error("Could not open {}", filepath);
        throw std::runtime_error("Can't open shader file");
    }

    file.close();
    return ret;
}

GLuint Shader::compileShader(const char *filepath, GLenum type)
{
    int success;
    char infoLog[1024];

    GLuint shader            = glCreateShader(type);
    std::string strShaderSrc = loadShaderSrc(filepath);

    const GLchar *shaderSrc = strShaderSrc.c_str();
    glShaderSource(shader, 1, &shaderSrc, nullptr);
    glCompileShader(shader);

    // catch error
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        spdlog::info("Error compiling {} shader", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"));
        std::cout << "Error with shader compilation: " << std::endl
                  << infoLog << std::endl;
        throw std::runtime_error("shader compile error");
    }
    else
    {
        spdlog::info("Success compiling {} shader", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"));
    }

    return shader;
}

void Shader::setBool(const std::string &name, bool value)
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int) value);
}

void Shader::setInt(const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set3Float(const std::string &name, float v1, float v2, float v3)
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), v1, v2, v3);
}

void Shader::set3Float(const std::string &name, glm::vec3 v)
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z);
}

void Shader::set4Float(const std::string &name, float v1, float v2, float v3, float v4)
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), v1, v2, v3, v4);
}

// void Shader::set4Float(const std::string &name, aiColor4D color)
// {
//     glUniform4f(glGetUniformLocation(id, name.c_str()), color.r, color.g, color.b, color.a);
// }

void Shader::set4Float(const std::string &name, glm::vec4 v)
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z, v.w);
}

void Shader::setMat3(const std::string &name, glm::mat3 val)
{
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

// uniform functions
void Shader::setMat4(const std::string &name, const glm::mat4 &val)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}