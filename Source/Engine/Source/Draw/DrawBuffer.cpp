// ◦ Xyz ◦
#include "DrawBuffer.h"
#include <glad/gl.h>
#include <Draw/Shader.h>
#include "../../../Program/Gravity/Shaders/GravityShader.h"

using namespace Engine;

GLuint quadVAO = 0;

unsigned int DrawBuffer::QuadVAO()
{
    if (quadVAO == 0) {
        float quad[] = {
            -1, -1,
             1, -1,
             1,  1,
            -1,  1
        };

        GLuint VBO;
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    return quadVAO;
}

void DrawBuffer::Draw(const FrameBuffer& buffer)
{
    Draw(buffer.GetTexture());
}

void DrawBuffer::Draw(unsigned int idTexture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    shaders::DisplayShaderSingle::Instance().GetLocation();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, idTexture);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
