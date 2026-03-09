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

// BaseShader
bool BaseShader::UseProgram() const
{
	if (!_program) {
		return false;
	}

	glUseProgram(_program);
	glUniformMatrix4fv(uMatProjectionView, 1, GL_FALSE, Engine::Camera::GetLink().ProjectViewFloat());

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	return true;
}

bool BaseShader::GetLocation()
{
	if (!_program) {
		return false;
	}

	uMatProjectionView = glGetUniformLocation(_program, "uMatProjectionView");
	uMatViewModel = glGetUniformLocation(_program, "uMatViewModel");
	uColor = glGetUniformLocation(_program, "uColor");

	return true;
}

void BaseShader::SetColor(const float* const color) const
{
	if (color) {
		glUniform4fv(uColor, 1, color);
	}
}

void BaseShader::SetModelPos(const float* const pos) const
{
	if (pos) {
		glm::mat4x4 mat(1.f);
		mat[3][0] = pos[0];
		mat[3][1] = pos[1];
		mat[3][2] = pos[2];
		glUniformMatrix4fv(uMatViewModel, 1, GL_FALSE, glm::value_ptr(mat));
	}
}

void BaseShader::SetModelMatrix(const float* const mat) const
{
	if (mat) {
		glUniformMatrix4fv(uMatViewModel, 1, GL_FALSE, mat);
	}
}

// LineShader
bool LineShader::UseProgram() const
{
	if (!_program) {
		return false;
	}

	glBindVertexArray(0);

	glUseProgram(_program);
	glUniformMatrix4fv(uMatProjectionView, 1, GL_FALSE, Engine::Camera::GetLink().ProjectViewFloat());

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);

	return true;
}

bool LineShader::GetLocation()
{
	if (!_program) {
		return false;
	}

	uMatProjectionView = glGetUniformLocation(_program, "uMatProjectionView");
	uColor = glGetUniformLocation(_program, "uColor");

	return true;
}

void LineShader::SetColor(const float* const color) const
{
	if (color) {
		glUniform4fv(uColor, 1, color);
	}
}

// InitShaders
void shaders::InitShaders()
{
	{
		auto& shader = shaders::BaseShader::Instance();
		shader.LoadByName("Texture");
		shader.UseProgram();

		glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
		shader.SetColor(glm::value_ptr(color));

		glm::mat4x4 mat{ 1.f };
		shader.SetModelMatrix(glm::value_ptr(mat));
	}

	{
		auto& shader = shaders::LineShader::Instance();
		shader.LoadByName("Line");
		shader.UseProgram();

		glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
		shader.SetColor(glm::value_ptr(color));
	}
}
