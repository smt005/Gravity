// ◦ Xyz ◦
#include "Draw.h"
#include <glad/gl.h>
#include <Screen.h>

namespace Engine
{
    // ---------- quad ----------
    GLuint quadVAO;

    void createQuad() {
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

    // ---------- shader utils ----------
    GLuint compileShader(const char* vs, const char* fs) {
        GLuint v = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(v, 1, &vs, NULL);
        glCompileShader(v);

        GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(f, 1, &fs, NULL);
        glCompileShader(f);

        GLuint p = glCreateProgram();
        glAttachShader(p, v);
        glAttachShader(p, f);
        glLinkProgram(p);

        glDeleteShader(v);
        glDeleteShader(f);
        return p;
    }

    // ---------- shaders ----------
    const char* vsQuad = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
out vec2 vUV;
void main(){
    vUV = (aPos + 1.0) * 0.5;
    gl_Position = vec4(aPos,0,1);
}
)";

    const char* fsAccum = R"(
#version 330 core
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uPrev;
uniform sampler2D uCurrent;
uniform float uDecay;
uniform float uOffset;

void main(){
    vec4 prev = texture(uPrev, vUV) * uDecay - uOffset;
    vec4 curr = texture(uCurrent, vUV);
    FragColor = prev + curr;
}
)";

    const char* fsDisplay = R"(
#version 330 core
in vec2 vUV;
out vec4 FragColor;
uniform sampler2D uTex;
void main(){
    FragColor = texture(uTex, vUV);
}
)";

    // ---------- FBO ----------
    void createFBO(GLuint& fbo, GLuint& tex)
    {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Engine::ScreenParams::Width(), Engine::ScreenParams::Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    }

    // ---------- простые точки ----------
    GLuint pointFBO, pointTex;

    void createPointBuffer()
    {
        createFBO(pointFBO, pointTex);
    }

    // рисуем движущуюся точку
    void renderPoints(float t)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pointFBO);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

       /* float factor = cos(t/25) - 0.5f;
        float x = 0.5f + 0.4f * cos(t + factor);
        float y = 0.5f + 0.4f * sin(t - factor);

        x *= factor;
        x += 0.5f;
        y *= factor;
        y += 0.5f;

        glEnable(GL_SCISSOR_TEST);
        glScissor(x * Engine::ScreenParams::Width(), y * Engine::ScreenParams::Height(), 10, 10);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);*/
    }

    GLuint accumShader;
    GLuint displayShader;
    GLuint fboA, texA, fboB, texB;
    float time = 0.0f;

    void Draw::InitTraceBuffers()
    {
        createQuad();

        accumShader = compileShader(vsQuad, fsAccum);
        displayShader = compileShader(vsQuad, fsDisplay);

        createFBO(fboA, texA);
        createFBO(fboB, texB);

        createPointBuffer();
    }

    void Draw::RenderTrace(std::function<void()> fun)
    {
        time += 0.016f;

        // 1. точки
        renderPoints(time);
        fun();

        // 2. accumulation
        glBindFramebuffer(GL_FRAMEBUFFER, fboB);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(accumShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texA);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pointTex);

        glUniform1i(glGetUniformLocation(accumShader, "uPrev"), 0);
        glUniform1i(glGetUniformLocation(accumShader, "uCurrent"), 1);
        glUniform1f(glGetUniformLocation(accumShader, "uDecay"), 1.f);


        static float uOffset = 0.f;
        static float timerOffset = 0.f;
        if (timerOffset > 1.f) {
            timerOffset = 0.f;
            uOffset = 0.002f;
        }
        else {
            timerOffset += 0.5f;
            uOffset = 0.f;
        }

        glUniform1f(glGetUniformLocation(accumShader, "uOffset"), uOffset);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        std::swap(texA, texB);
        std::swap(fboA, fboB);

        // 3. вывод
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(displayShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texA);
        glUniform1i(glGetUniformLocation(displayShader, "uTex"), 0);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}