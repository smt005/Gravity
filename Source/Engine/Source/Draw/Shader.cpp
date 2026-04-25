// ◦ Xyz ◦

#include "Shader.h"
#include <exception>
#include <glad/gl.h>
#include <Files/FileManager.h>
#include <Logs.h>

using namespace Engine;

Shader::~Shader()
{
    glDeleteProgram(_program);
}

bool Shader::GetLocation()
{
    return true;
}

bool Shader::UseProgram() const
{
    if (!_program) {
        return false;
    }

    glUseProgram(_program);
    return true;
}

void Shader::UnuseProgram() const
{
    glUseProgram(0);
}

void Shader::Load(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
    glDeleteProgram(_program);
 
    GLint params;
    GLuint program = 0;
    const auto& fm = FileManager::Get("base");

    std::string vertexShaderSourceStr = fm.ReadTextFile(vertexFilePath);
    const char* vertexShaderSource = vertexShaderSourceStr.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &params);
    if (!params) {
        int infoLogLength, charactersWritten;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::string infoLog(infoLogLength + 1, '\0');
        glGetShaderInfoLog(vertexShader, infoLogLength, &charactersWritten, infoLog.data());

        throw std::invalid_argument(LOG_BREAK("[Shader::Load] Shader compiled vertex ERROR: {} file: '{}'", infoLog, vertexFilePath));
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

        throw std::invalid_argument(LOG_BREAK("[Shader::Load] Shader compiled fragment ERROR: {} file: '{}'", infoLog, fragmentFilePath));
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &params);
    if (!params) {
        int infoLogLength, charactersWritten;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::string infoLog(infoLogLength + 1, '\0');
        glGetProgramInfoLog(program, infoLogLength, &charactersWritten, infoLog.data());
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        __debugbreak();
        throw std::invalid_argument(LOG_BREAK("[Shader::Load] Shader linked ERROR: {} file: '{}', '{}'", infoLog));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    _program = program;
    GetLocation();
}

void Shader::LoadByName(const std::string& name)
{
    try {
        const std::string vertexFilePath = TO_STRING("Shaders/{}.vert", name);
        const std::string fragmentFilePath = TO_STRING("Shaders/{}.frag", name);
        Load(vertexFilePath, fragmentFilePath);
    }
    catch (const std::exception& exc) {
        LOG("EXCEPTION: {}", exc.what());
    }
}
