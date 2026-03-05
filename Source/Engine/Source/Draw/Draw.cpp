// ◦ Xyz ◦
#include "Draw.h"
#include <string>
#include <glad/gl.h>
#include <Screen.h>
#include <FileManager/FileManager.h>
#include "Mesh.h"

using namespace Engine;

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

void Draw::Render(unsigned int vao, int count)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, count);
}

void Draw::BindTexture(unsigned int textureId) {
    glBindTexture(GL_TEXTURE_2D, textureId);
}
