// ◦ Xyz ◦
#include "Shader.h"
#include <string>
#include <exception>
#include <FileManager/FileManager.h>
#include <glad/gl.h>

using namespace Engine;

Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
    if (!Load(vertexFilePath, fragmentFilePath)) {
        throw std::invalid_argument(TO_STRING("[Shader] Fail load shader '{}', '{}'.", vertexFilePath, fragmentFilePath));
    }
}

Shader::~Shader()
{
    glDeleteProgram(_program);
}

bool Shader::Load(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
    GLint params;
    glDeleteProgram(_program);

    const auto& fm = FileManager::Get("base");
    std::string vertexShaderSourceStr = fm.ReadTextFile(vertexFilePath);
    const char* vertexShaderSource = vertexShaderSourceStr.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &params);
    if (!params) {
        int infoLogLength, charactersWritten;
        std::string infoLog(infoLogLength + 1, '\0');
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        glGetShaderInfoLog(vertexShader, infoLogLength, &charactersWritten, infoLog.data());
        LOG("[Shader::Load] Shader compiled fragment ERROR: {}", infoLog);
        return false;
    }


    std::string fragmentShaderSourceStr = fm.ReadTextFile(fragmentFilePath);
    const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &params);
    if (!params) {
        int infoLogLength, charactersWritten;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::string infoLog(infoLogLength + 1, '\0');
        glGetShaderInfoLog(fragmentShader, infoLogLength, &charactersWritten, infoLog.data());
        glDeleteShader(vertexShader);

        LOG("[Shader::Load] Shader compiled fragment ERROR: {}", infoLog);
        return false;
    }

    _program = glCreateProgram();
    glAttachShader(_program, vertexShader);
    glAttachShader(_program, fragmentShader);
    glLinkProgram(_program);

    glGetProgramiv(_program, GL_LINK_STATUS, &params);
    if (!params) {
        int infoLogLength, charactersWritten;
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::string infoLog(infoLogLength + 1, '\0');
        glGetProgramInfoLog(_program, infoLogLength, &charactersWritten, infoLog.data());
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        LOG("[Shader::Load] Shader linked ERROR: {}", infoLog);
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void Shader::UseProgram()
{
    glUseProgram(_program);
}

void Shader::UseProgram(const std::string& name)
{
    try {
        auto& shader = shaders.at(name);
        if (!shader) {
            throw std::invalid_argument(TO_STRING("[Shader::UseProgram]: fail UseProgram {}", name));
        }

        shader->UseProgram();
    }
    catch (const std::invalid_argument& exc) {
        throw exc;
    }
    catch (...) {
        throw std::invalid_argument(TO_STRING("[Shader::UseProgram]: fail UseProgram {}", name));
    }

}
