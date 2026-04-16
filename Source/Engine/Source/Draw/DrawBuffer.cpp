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

void CreateQuad()
{
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

/*class AccumShader : public Engine::Shader
{
public:
    bool GetLocation() override
    {
        uPrev = glGetUniformLocation(_program, "uPrev");
        uCurrent = glGetUniformLocation(_program, "uCurrent");
        uDecay = glGetUniformLocation(_program, "uDecay");
        uOffset = glGetUniformLocation(_program, "uOffset");
        return true;
    }
    bool UseProgram() const {
        if (Shader::UseProgram()) {
            glUniform1i(uPrev, 0);
            glUniform1i(uCurrent, 1);
            glUniform1f(uDecay, 1.f);
            glUniform1f(uOffset, 0.002f);
            return true;
        }
        return false;
    }

    unsigned int uPrev = 0;
    unsigned int uCurrent = 0;
    unsigned int uDecay = 0;
    unsigned int uOffset = 0;
};*/

FrameBuffer2 pointBuffer;
FrameBuffer2 bufferA;
FrameBuffer2 bufferB;
//AccumShader accumShader;
//Engine::Shader displayShader;

void DrawBuffer::InitPostDraw()
{
    CreateQuad();

    pointBuffer.Create();
    bufferA.Create();
    bufferB.Create();

    //accumShader.LoadByName("Post/Accumulate");
    //displayShader.LoadByName("Post/Display");
}

void DrawBuffer::PostDraw(std::function<void()> drawFun)
{
    //pointBuffer.Bind();
    //drawFun();
/*
    bufferB.Bind();
    glClear(GL_COLOR_BUFFER_BIT);

    //accumShader.UseProgram();
    shaders::AccumShaderSingle::Instance().UseProgram();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bufferA.GetTexture());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pointBuffer.GetTexture());

    //accumShader.GetLocation();
    shaders::AccumShaderSingle::Instance().GetLocation();

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    std::swap(bufferA, bufferB);

    Draw(bufferA);
*/
    /*glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    displayShader.UseProgram();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bufferA.GetTexture());

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);*/
}

void DrawBuffer::PostDraw(unsigned int idTexture, FrameBuffer2& _bufferA, FrameBuffer2& _bufferB)
{
/*
    _bufferB.Bind();
    glClear(GL_COLOR_BUFFER_BIT);

    //accumShader.UseProgram();
    shaders::AccumShaderSingle::Instance().UseProgram();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bufferA.GetTexture());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, idTexture);

    //accumShader.GetLocation();
    shaders::AccumShaderSingle::Instance().GetLocation();

    //glBindVertexArray(quadVAO);
    glBindVertexArray(QuadVAO());
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    std::swap(_bufferA, _bufferB);

    Draw(_bufferA);
*/
}

void DrawBuffer::Draw(const FrameBuffer2& buffer)
{
    Draw(buffer.GetTexture());
}

void DrawBuffer::Draw(unsigned int idTexture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    //displayShader.UseProgram();
    shaders::DisplayShaderSingle::Instance().GetLocation();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, idTexture);

//glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
