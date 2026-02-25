// ◦ Xyz ◦
#include "Draw.h"
#include <string>
#include <glad/gl.h>
#include <Screen.h>
#include <FileManager/FileManager.h>

using namespace Engine;

GLuint shaderProgram = 0;
GLuint VBO, VAO;

void Draw::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Draw::ClearColor() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Draw::ClearDepth() {
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Draw::DepthTest(bool enable) {
    if (enable) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
}


void Draw::Init()
{
    LOG("Draw::Init");

    float vertices[] = {
     0.0f,  0.5f, 0.0f,  // верх
    -0.5f, -0.5f, 0.0f,  // левый низ
     0.5f, -0.5f, 0.0f   // правый низ
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                  // layout location
        3,                  // vec3
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //...
    const auto& fm = FileManager::Get("base");
    std::string vertexShaderSourceStr = fm.ReadFile<std::string>("test.vert");
    const char* vertexShaderSource = vertexShaderSourceStr.c_str();

    std::string fragmentShaderSourceStr = fm.ReadFile<std::string>("test.frag");
    const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Draw::Render()
{
    glViewport(0, 0, ScreenParams::width, ScreenParams::height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
