// ◦ Xyz ◦
#include "Draw.h"
#include <string>
#include <glad/gl.h>
#include <Screen.h>
#include <Files/FileManager.h>
#include <Object/Mesh.h>

using namespace Engine;

GLuint VBO, VAO;

void Draw::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Draw::ClearColor() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Draw::ClearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
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

void Draw::CullFace(CullFaceType type) {
	if (type == CullFaceType::NONE) {
		glDisable(GL_CULL_FACE);
		return;
	}

	glEnable(GL_CULL_FACE);

	if (type == CullFaceType::FRONT) {
		glCullFace(GL_FRONT);
	}
	else if (type == CullFaceType::BACK) {
		glCullFace(GL_BACK);
	}
	else if (type == CullFaceType::FRONT_AND_BACK) {
		glCullFace(GL_FRONT_AND_BACK);
	}
	else if (type == CullFaceType::FRONT_LEFT) {
		glCullFace(GL_FRONT_LEFT);
	}
	else if (type == CullFaceType::FRONT_RIGHT) {
		glCullFace(GL_FRONT_RIGHT);
	}
	else {
		glDisable(GL_CULL_FACE);
	}
}

void Draw::Viewport() {
	int widthScreen = Engine::ScreenParams::Width();
	int heightScreen = Engine::ScreenParams::Height();
	glViewport(0, 0, widthScreen, heightScreen);
}

void Draw::SetPointSize(const float sizePoint)
{
	if (sizePoint >= 0.f) {
		glPointSize(sizePoint);
	}
	else {
		glEnable(GL_PROGRAM_POINT_SIZE);
	}
}

void Draw::BindTexture(Texture& texture)
{
	BindTexture(texture.Id());
}

void Draw::BindTexture(unsigned int textureId) {
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Draw::Render(unsigned int vao, int count)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, count);
}

void Draw::RenderTriangleFun(unsigned int vao, int count)
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, count);
}

void Draw::RenderLines(const float* vertexes, unsigned int count)
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertexes);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_LOOP, 0, count);
}

void Draw::RenderPoints(const float* vertexes, unsigned int count)
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertexes);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_POINTS, 0, count);
}

void Draw::ActiveTexture(unsigned int texture)
{
	switch (texture) {
		case 0: glActiveTexture(GL_TEXTURE0); break;
		case 1: glActiveTexture(GL_TEXTURE1); break;
		case 2: glActiveTexture(GL_TEXTURE2); break;
		default: {
			throw "ERROR";
		};
	};
}
