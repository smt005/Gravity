// ◦ Xyz ◦

#include "GravityShader.h"
#include <Draw/Shader.h>
#include <Draw/Camera.h>
#include <glad/gl.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Temp/LogSpecification.h>
#include <Log.h>

using namespace shaders;

void ColorShader::UseProgram()
{
	if (!_program) {
		return;
	}

	glUseProgram(_program);
	glUniformMatrix4fv(uMatProjectionView, 1, GL_FALSE, Engine::Camera::GetLink().ProjectViewFloat());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ColorShader::GetLocation()
{
	if (!_program) {
		return;
	}

	uMatProjectionView = glGetUniformLocation(_program, "uMatProjectionView");
	uMatViewModel = glGetUniformLocation(_program, "uMatViewModel");
	uColor = glGetUniformLocation(_program, "uColor");
}

void ColorShader::SetColor(const float* const color) const
{
	if (color) {
		glUniform4fv(uColor, 1, color);
	}
}

void ColorShader::SetModelPos(const float* const pos) const
{
	if (pos) {
		glm::mat4x4 mat(1.f);
		mat[3][0] = pos[0];
		mat[3][1] = pos[1];
		mat[3][2] = pos[2];
		glUniformMatrix4fv(uMatViewModel, 1, GL_FALSE, glm::value_ptr(mat));
	}
}

void ColorShader::SetModelMatrix(const float* const mat) const
{
	if (mat) {
		glUniformMatrix4fv(uMatViewModel, 1, GL_FALSE, mat);
	}
}

void shaders::InitShaders()
{
	auto& colorShader = shaders::ColorShader::Instance();
	colorShader.LoadByName("Color");
	colorShader.UseProgram();

	glm::vec4 color{ 1.f, 0.6f, 0.f, 1.f };
	colorShader.SetColor(glm::value_ptr(color));

	glm::mat4x4 mat{1.f};
	colorShader.SetModelMatrix(glm::value_ptr(mat));
}
