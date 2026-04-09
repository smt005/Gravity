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

void Draw::BindTexture(unsigned int textureId) {
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Draw::Render(unsigned int vao, int count)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, count);
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

// TODO:
void Draw::DrawToScreen()
{
	/*static const float vertices[] = {
		// Позиции        // Текстурные координаты
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f
	};*/

	static const float vertices[] = {
		-1.f, -1.f, 0.0f,  0.0f, 0.0f,
		1.f, -1.f, 0.0f,  1.0f, 0.0f,
		1.f,  1.f, 0.0f,  1.0f, 1.0f,
		-1.f,  1.f, 0.0f,  0.0f, 1.0f
	};

	static const unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	static GLuint VBO;
	static GLuint VAO;
	static GLuint EBO;
	static bool inited = false;

	if (!inited) {
		// Настройка VAO/VBO/EBO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Позиции вершин: 3 float
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Текстурные координаты: 2 float
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	
		inited = true;
	}

	// Draw ...
	if (inited) {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
