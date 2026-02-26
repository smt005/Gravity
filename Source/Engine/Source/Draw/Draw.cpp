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

void Draw::Viewport() {
    int widthScreen = ScreenParams::Width();
    int heightScreen = ScreenParams::Height();
    glViewport(0, 0, widthScreen, heightScreen);
}

void Draw::Init()
{
    LOG("Draw::Init");

    /*float vertices[] = {
     0.0f,  0.5f, 0.0f,  // верх
    -0.5f, -0.5f, 0.0f,  // левый низ
     0.5f, -0.5f, 0.0f   // правый низ
    };
    GLsizei countVertex = 3;
    */

    float vertices[] = {
        // Передняя грань
        -0.5f,  0.5f,  0.1f,
        -0.5f, -0.5f,  0.1f,
         0.5f, -0.5f,  0.1f,

        -0.5f,  0.5f,  0.1f,
         0.5f, -0.5f,  0.1f,
         0.5f,  0.5f,  0.1f,

         // Задняя грань
         -0.5f,  0.5f, -0.1f,
         -0.5f, -0.5f, -0.1f,
          0.5f, -0.5f, -0.1f,

         -0.5f,  0.5f, -0.1f,
          0.5f, -0.5f, -0.1f,
          0.5f,  0.5f, -0.1f,

          // Боковые грани (каждый набор — два треугольника)
          // Левая боковая грань
          -0.5f,  0.5f,  0.1f,
          -0.5f, -0.5f,  0.1f,
          -0.5f, -0.5f, -0.1f,

          -0.5f,  0.5f,  0.1f,
          -0.5f, -0.5f, -0.1f,
          -0.5f,  0.5f, -0.1f,

          // Правая боковая грань
           0.5f,  0.5f,  0.1f,
           0.5f, -0.5f,  0.1f,
           0.5f, -0.5f, -0.1f,

           0.5f,  0.5f,  0.1f,
           0.5f, -0.5f, -0.1f,
           0.5f,  0.5f, -0.1f,

           // Верхняя боковая грань
           -0.5f,  0.5f,  0.1f,
            0.5f,  0.5f,  0.1f,
            0.5f,  0.5f, -0.1f,

           -0.5f,  0.5f,  0.1f,
            0.5f,  0.5f, -0.1f,
           -0.5f,  0.5f, -0.1f,

           // Нижняя боковая грань
           -0.5f, -0.5f,  0.1f,
            0.5f, -0.5f,  0.1f,
            0.5f, -0.5f, -0.1f,

           -0.5f, -0.5f,  0.1f,
            0.5f, -0.5f, -0.1f,
           -0.5f, -0.5f, -0.1f
    };
    GLsizei countVertex = 36;

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
}

void Draw::Render()
{
    glBindVertexArray(VAO);

    GLsizei countVertex = 36;
    glDrawArrays(GL_TRIANGLES, 0, countVertex);
}
