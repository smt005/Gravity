// ◦ Xyz ◦

#include "GravityShader.h"
#include <Draw/Shader.h>
#include <Draw/Camera.h>
#include <glad/gl.h>
#include <glm/ext/matrix_transform.hpp>

using namespace shaders;

void ColorShader::UseProgram()
{
	if (!_program) {
		return;
	}

	glUseProgram(_program);
	//glUniformMatrix4fv(uMatProjectionView, 1, GL_FALSE, Engine::Camera::GetLink().ProjectViewFloat());

	static float fov = 45.f;
	static glm::mat4x4 matProject(1.f);
	static glm::mat4x4 matView(1.f);

	static glm::vec3 pos = { 0.f, 0.f, 1.f };
	static glm::vec3 direct = { 0.f, 0.f, -1.f };
	static glm::vec3 up = { 0.f, 1.f, 0.f };

	matProject = glm::perspective(fov, 0.5f, 100000.f, 1.f);
	matView = glm::lookAt(pos, pos + direct, up);
	
	glm::mat4x4 mat =  matProject * matView;

	glUniformMatrix4fv(uMatProjectionView, 1, GL_FALSE, glm::value_ptr(mat));
	//mat = glm::rotate(mat, 0.01f, {0.f, 0.f, 1.f});


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ColorShader::GetLocation()
{
	if (!_program) {
		return;
	}

	uMatProjectionView = glGetUniformLocation(_program, "uMatProjectionView");
	//uMatViewModel = glGetUniformLocation(_program, "uMatViewModel");
	uColor = glGetUniformLocation(_program, "uColor");
}

void ColorShader::SetColor(const float* const color)
{
	if (color) {
		glUniform4fv(uColor, 1, color);
	}
}

void shaders::InitShaders()
{
	auto& colorShader = shaders::ColorShader::Instance();
	colorShader.LoadByName("Color");
	colorShader.UseProgram();
	float color[] = { 0.f, 0.f, 1.f, 1.f };
	colorShader.SetColor(color);
}
